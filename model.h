#include <string>
#include <vector>
#include <utility>
#include <map>

struct choice {
    choice(std::string&& text);

    void set(const std::string& key, std::string&& val);

    std::string text_;
    std::string target_;
    std::string target_second_;
    std::string add_text_;
    std::string add_text_second_;
    std::string visited_;
    std::pair<std::string, bool> set_cond_;
    int hp_;
};

struct screen {
    screen(std::string&& main_text, std::vector<choice>&& choices) :
        main_text_(std::move(main_text)), choices_(std::move(choices)) {}

    std::string main_text_;
    std::vector<choice> choices_;
};

typedef std::map<std::string, screen> screenMap;

void initializeScreenMap(screenMap& map);
