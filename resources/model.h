#include <string>
#include <vector>
#include <utility>

struct screen {
    screen(std::vector<choice>&& choices, std::string&& main_text) :
        choices(std::move(choices)), main_text_(std::move(main_text)) {}

    std::vector<choice> choices_;
    std::string main_text_;
};

struct choice {
    choice(std::string&& text) :
        text_(std::move(text)),
        hp(0) {}

    std::string text_;
    std::string target_;
    std::string target_second_;
    std::string add_text_;
    std::string add_text_second_;
    std::string visited_;
    std::pair<std::string, bool> set_cond_;
    int hp;
};
