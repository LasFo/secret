#include "SFML/Graphics.hpp"
#include "model.h"
#include <ctime>
#include <sstream>
#include <cassert>
#include <unordered_set>
// only for intermediate testing remove later
#include <iostream>

size_t w_width = 1600;
size_t w_height = 900;
size_t text_box_size = w_height/4;
size_t choice_box_size = w_height/9;
sf::Font font;

sf::Text getDefaultText(sf::Vector2f position, std::string txt) {
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setPosition(position);
    text.setFillColor(sf::Color::White);
    text.setString(txt);
    return text;
}

void drawChoices(
            sf::RenderWindow& window,
            std::vector<choice>& choices,
            size_t current_choice,
            std::unordered_set<std::string>& cond_map) {

    for (size_t i = 0; i < choices.size(); ++i) {
        choice c = choices[i];
        if (!c.condition_.empty() && !cond_map.count(c.condition_)) continue; // choice not available
        sf::RectangleShape choice_box;
        choice_box.setSize(sf::Vector2f(w_width - 10, choice_box_size - 10));
        choice_box.setOutlineThickness(5);
        sf::Color col = current_choice == i ?
                            sf::Color(255,220,0) :
                            sf::Color(169,169,169);
        choice_box.setOutlineColor(col);
        choice_box.setFillColor(sf::Color(0, 50, 0));
        choice_box.setPosition(5, w_height - (i+1) * choice_box_size);
        window.draw(choice_box);

        sf::Text choice_text = getDefaultText(choice_box.getPosition() + sf::Vector2f(20, 20),
                                              c.text_);
        window.draw(choice_text);
    }
}

void drawScreen(
        sf::RenderWindow& window,
        screen& screen,
        size_t current_choice,
        std::unordered_set<std::string>& cond_map) {
    // draw the main text box
    sf::RectangleShape displayed_text;
    displayed_text.setSize(sf::Vector2f(w_width - 10, text_box_size - 10));
    displayed_text.setOutlineThickness(5);
    displayed_text.setOutlineColor(sf::Color::Blue);
    displayed_text.setFillColor(sf::Color(50, 0, 0));
    displayed_text.setPosition(5, w_height - text_box_size - screen.choices_.size() * choice_box_size);
    window.draw(displayed_text);

    sf::Text main_text = getDefaultText(displayed_text.getPosition() + sf::Vector2f(20, 20),
                                        screen.main_text_);
    window.draw(main_text);

    drawChoices(window, screen.choices_, current_choice, cond_map);
}

void displayAddText(sf::RenderWindow& window, const std::string& add_text) {
    sf::RectangleShape add_text_box;
    add_text_box.setSize(sf::Vector2f(w_width - 10, text_box_size - 10));
    add_text_box.setOutlineThickness(5);
    add_text_box.setOutlineColor(sf::Color::Blue);
    add_text_box.setFillColor(sf::Color(50, 0, 0));
    add_text_box.setPosition(5, w_height - text_box_size - 3 * choice_box_size);

    sf::Text text = getDefaultText(add_text_box.getPosition() + sf::Vector2f(20, 20), add_text);

    while (window.isOpen()) {
        window.clear();
        window.draw(add_text_box);
        window.draw(text);
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                    window.close();
                    return;
                }

            if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    return;
                }
            }
        }
    }
}

std::string evaluateChoice(
        sf::RenderWindow& window,
        choice& c,
        std::unordered_set<std::string>& cond_map,
        int& hp) {
    hp += c.hp_;
    // check if this is a multi visit location
    if (!c.set_cond_.first.empty()) {
        if (c.set_cond_.second) {
            cond_map.insert(c.set_cond_.first);
        } else {
            cond_map.erase(c.set_cond_.first);
        }
    }
    std::string next_tag;
    std::string add_text;
    if (c.visited_.empty() || cond_map.insert(c.visited_).second) {
        // single visit location
        next_tag = c.target_;
        add_text = c.add_text_;
    } else {
        next_tag = c.target_second_;
        add_text = c.add_text_second_;
    }
    if (!add_text.empty()) {
        displayAddText(window, add_text);
    }
    return next_tag;
}

int main(int argc, const char** argv) {
    std::unordered_set<std::string> cond_map;
    screenMap smap;
    if (argc == 3) {
        w_width = atoi(argv[1]);
        w_height = atoi(argv[2]);
    }
    initializeScreenMap(smap, w_width);
    sf::RenderWindow window(sf::VideoMode(w_width, w_height), "SFML works!");
    // TODO: get some background picture
    std::time_t old_time = std::time(nullptr);
    std::time_t new_time;
    uint32_t fps_counter = 0;
    uint32_t last_fps = 0;
    if (!font.loadFromFile("resources/sansation.ttf")) {
        return 1;
    }
    sf::Text fps_text = getDefaultText(sf::Vector2f(w_width - 50, 10), "0");
    sf::Text hp_text = getDefaultText(sf::Vector2f(10, 10), "100");
    hp_text.setFillColor(sf::Color::Red);

    int current_choice = 0;
    size_t num_of_choices = 3;
    auto it = smap.find("begin");
    int hp = 100;

    while (window.isOpen())
    {
        // event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if(event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                        current_choice = --current_choice < 0 ? num_of_choices - 1 : current_choice;
                }
                if (event.key.code == sf::Keyboard::Up) {
                        ++current_choice;
                        current_choice = current_choice % num_of_choices;
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    // look up in current screen to get next screen
                    assert(it->second.choices_.size() > static_cast<size_t>(current_choice));
                    choice& chosen = it->second.choices_[current_choice];
                    std::string new_label = evaluateChoice(window, chosen, cond_map, hp);
                    if (new_label == "game_over") {
                        cond_map.clear();
                        hp = 100;
                        displayAddText(window, "You lost! Try Again!");
                        it = smap.find("begin");
                    } else {
                        it = smap.find(new_label);
                    }
                    if (cond_map.count("west") &&
                        cond_map.count("east") &&
                        cond_map.count("south")) {
                        // you won
                        it = smap.find("final");
                    }
                    assert(it != smap.end());
                    num_of_choices = it->second.choices_.size();
                    current_choice = 0;
                }
            }
        }

        if (hp < 70) {
            std::stringstream ss;
            ss << "Pass mal lieber auf du hast nur noch " << hp << " HP";
            displayAddText(window, ss.str());
            ss.str("");
            hp = 75;
            ss << "Naja ich heil dich mal ein bisschen. Jetzt hast du wieder " << hp << " HP";
            displayAddText(window, ss.str());
        }

        window.clear();
        drawScreen(window, it->second, current_choice, cond_map);

        // fps
        ++fps_counter;
        std::time(&new_time);
        if (new_time != old_time) {
            old_time = new_time;
            last_fps = fps_counter;
            fps_counter = 0;
        }
        std::stringstream ss;
        ss << last_fps;
        fps_text.setString(ss.str());
        window.draw(fps_text);

        ss.str("");
        ss << hp;
        hp_text.setString(ss.str());
        window.draw(hp_text);

        window.display();
    }
    
    return 0;
}
