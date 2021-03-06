#include <fstream>
#include <iostream>
#include <cassert>
#include "model.h"

size_t w_width_ = 1600;

std::string& insertLineBreaks(std::string& s) {
     size_t th = w_width_ / 10;
     while (th < s.size()) {
        s.insert(th, 1, '\n');
        th += w_width_ / 10;
    }
    return s;
}

choice::choice(std::string&& s) {
    size_t off = 0, end = 0;
    end = s.find(">>");
    assert(end != std::string::npos);
    text_ = s.substr(0, end);
    off = end + 2;

    while ((end = s.find(">>", off)) != std::string::npos) {
        std::string kv = s.substr(off, end - off);
        size_t colon = 0;
        colon = kv.find(':');
        assert(colon != std::string::npos);
        std::string key = kv.substr(0, colon);
        std::string val = kv.substr(colon+1);
        set(key, std::move(val));
        off = end+2;
    }
}

void choice::set(const std::string& key, std::string&& val) {
    if(key == "target") {
        target_ = std::move(val);
    } else if (key == "target_second") {
        target_second_ = std::move(val);
    } else if (key == "add_text") {
        add_text_ = std::move(insertLineBreaks(val));
    } else if (key == "add_text_second") {
        add_text_second_ = std::move(insertLineBreaks(val));
    } else if (key == "visited") {
        visited_ = std::move(val);
    } else if (key == "condition") {
        condition_ = std::move(val);
    } else if (key == "set_cond") {
        size_t col = val.find(':');
        assert(col != std::string::npos);
        set_cond_.first = val.substr(0, col);
        if (val.substr(col+1) == "true") {
            set_cond_.second = true;
        } else if (val.substr(col+1) == "false") {
            set_cond_.second = false;
        } else {
            assert(false);
        }
    } else if (key == "set_cond_second") {
        size_t col = val.find(':');
        assert(col != std::string::npos);
        set_cond_second_.first = val.substr(0, col);
        if (val.substr(col+1) == "true") {
            set_cond_second_.second = true;
        } else if (val.substr(col+1) == "false") {
            set_cond_second_.second = false;
        } else {
            assert(false);
        }
    } else if (key == "hp") {
        hp_ = atoi(val.c_str());
    } else {
        assert(false);
    }
}

void initializeScreenMap(screenMap& map, size_t w_width) {
    std::ifstream ifs;
    w_width_ = w_width;
    ifs.open("resources/text_data", std::ifstream::in);
    std::string s;
    while (getline(ifs,s)) {
        size_t off = 0, end = 0;
        end = s.find("<<<");
        assert(end != std::string::npos);
        std::string label = s.substr(0, end);
        off = end + 3;

        end = s.find("<<<", off);
        assert(end != std::string::npos);
        // image is optional so assume it is not there
        // and adjust if it is there
        std::string main_text = s.substr(off, end - off);
        std::string image;
        off = end + 3;
        if (main_text.find("image:") == 0u) {
            image = main_text.substr(6);
            end = s.find("<<<", off);
            assert(end != std::string::npos);
            main_text = s.substr(off, end - off);
            off = end + 3;
        }
        insertLineBreaks(main_text);

        std::vector<choice> choices;
        while ((end = s.find("<<<", off)) != std::string::npos) {
            std::string tmp = s.substr(off, end - off);
            off = end + 3;
            choices.emplace_back(std::move(tmp));
        }

        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple(std::move(label)),
                    std::forward_as_tuple(std::move(main_text),
                                          std::move(image),
                                          std::move(choices)));
    }
}
