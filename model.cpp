#include <fstream>
#include <iostream>
#include <cassert>
#include "model.h"

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
        add_text_ = std::move(val);
    } else if (key == "add_text_second") {
        add_text_second_ = std::move(val);
    } else if (key == "visited") {
        visited_ = std::move(val);
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
    } else if (key == "hp") {
        hp_ = atoi(val.c_str());
    } else {
        assert(false);
    }
}

void initializeScreenMap(screenMap& map){
    std::ifstream ifs;

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
        std::string main_text = s.substr(off, end - off);
        off = end + 3;

        std::vector<choice> choices;
        while ((end = s.find("<<<", off)) != std::string::npos) {
            std::string tmp = s.substr(off, end - off);
            off = end + 3;
            choices.emplace_back(std::move(tmp));
        }

        map.emplace(std::piecewise_construct,
                    std::forward_as_tuple(std::move(label)),
                    std::forward_as_tuple(std::move(main_text),
                                          std::move(choices)));
    }
}
