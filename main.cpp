#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "model.h"
#include <ctime>
#include <sstream>
#include <cassert>
#include <cstring>
#include <unordered_set>
#include <filesystem>
// only for intermediate testing remove later
#include <iostream>

size_t w_width = 1600;
size_t w_height = 900;
size_t text_box_size = w_height/6;
size_t choice_box_size = w_height/9;
sf::Font font;
std::filesystem::path imgp;

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
        choice_box.setFillColor(sf::Color(0, 50, 0, 200));
        choice_box.setPosition(5, w_height - (i+1) * choice_box_size + 5);
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
    displayed_text.setFillColor(sf::Color(50, 0, 0, 150));
    displayed_text.setPosition(5, w_height - text_box_size - screen.choices_.size() * choice_box_size + 5);
    window.draw(displayed_text);

    sf::Text main_text = getDefaultText(displayed_text.getPosition() + sf::Vector2f(20, 20),
                                        screen.main_text_);
    window.draw(main_text);

    drawChoices(window, screen.choices_, current_choice, cond_map);
}

void displayAddText(sf::RenderWindow& window, const std::string& add_text, sf::Sprite sp) {
    sf::RectangleShape add_text_box;
    add_text_box.setSize(sf::Vector2f(w_width - 10, text_box_size - 10));
    add_text_box.setOutlineThickness(5);
    add_text_box.setOutlineColor(sf::Color::Blue);
    add_text_box.setFillColor(sf::Color(50, 0, 0, 150));
    add_text_box.setPosition(5, w_height - text_box_size - 3 * choice_box_size);

    sf::Text text = getDefaultText(add_text_box.getPosition() + sf::Vector2f(20, 20), add_text);

    while (window.isOpen()) {
        window.clear();
        window.draw(sp);
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
        sf::Sprite& sp,
        int& hp) {
    hp += c.hp_;
    // check if this is a multi visit location
    std::string next_tag;
    std::string add_text;
    if (c.visited_.empty() || !cond_map.count(c.visited_)) {
        // single visit location
        next_tag = c.target_;
        add_text = c.add_text_;
    } else {
        next_tag = c.target_second_;
        add_text = c.add_text_second_;
    }
    if (!add_text.empty()) {
        displayAddText(window, add_text, sp);
    }
    // update conditions if necessary
    if (!c.set_cond_.first.empty()) {
        if (c.set_cond_.second) {
            cond_map.insert(c.set_cond_.first);
        } else {
            cond_map.erase(c.set_cond_.first);
        }
    }
    if (!c.set_cond_second_.first.empty()) {
        if (c.set_cond_second_.second) {
            cond_map.insert(c.set_cond_second_.first);
        } else {
            cond_map.erase(c.set_cond_second_.first);
        }
    }
    return next_tag;
}

bool choiceAvailable(
        size_t cur_choice,
        std::unordered_set<std::string> cond_map,
        screenMap::iterator it) {
    choice& c = it->second.choices_[cur_choice];
    return c.condition_.empty() || cond_map.count(c.condition_);
}

void checkHP(sf::RenderWindow& window, sf::Sprite sprite, int& hp) {
    if (hp < 70) {
        std::stringstream ss;
        ss << "Pass mal lieber auf du hast nur noch " << hp << " HP";
        displayAddText(window, ss.str(), sprite);
        ss.str("");
        hp = 75;
        ss << "Naja ich heil dich mal ein bisschen. Jetzt hast du wieder " << hp << " HP";
        displayAddText(window, ss.str(), sprite);
    }
}

int main(int argc, const char** argv) {
    std::unordered_set<std::string> cond_map;
    screenMap smap;
    bool enable_fps = false;
    if (argc >= 2) {
        enable_fps = strcmp(argv[1], "-f") == 0;
    }
    if (argc >= 4) {
        w_width = atoi(argv[2]);
        w_height = atoi(argv[3]);
    }
    initializeScreenMap(smap, w_width);
    sf::RenderWindow window(sf::VideoMode(w_width, w_height), "Awesome Game!");
    std::time_t old_time = std::time(nullptr);
    std::time_t new_time;
    uint32_t fps_counter = 0;
    uint32_t last_fps = 0;
    std::filesystem::path fontp = "resources";
    if (!font.loadFromFile(fontp / "sansation.ttf")) {
        return 1;
    }
    sf::Text fps_text = getDefaultText(sf::Vector2f(w_width - 50, 10), "0");
    sf::Text hp_text = getDefaultText(sf::Vector2f(10, 10), "100");
    hp_text.setOutlineThickness(11);
    hp_text.setFillColor(sf::Color::Red);

    int current_choice = 0;
    size_t num_of_choices = 3;
    auto it = smap.find("begin");
    int hp = 100;
    sf::Texture tx;
    
    // back ground picture
    imgp = "resources";
    imgp /= "bgp";
    if (!tx.loadFromFile(imgp / "fire.jpg"))
    {
        return 2;
    }
    sf::Sprite sprite(tx);

    // back ground music
    sf::Music music;
    std::filesystem::path musicp = "resources";
    if (!music.openFromFile(musicp / "chapter1.ogg")) {
        return 3;
    }
    music.play();
    music.setLoop(true);

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
                        do {
                            current_choice = --current_choice < 0 ? num_of_choices - 1 : current_choice;
                        } while(!choiceAvailable(current_choice, cond_map, it));
                }
                if (event.key.code == sf::Keyboard::Up) {
                        do {
                            ++current_choice;
                            current_choice = current_choice % num_of_choices;
                        } while(!choiceAvailable(current_choice, cond_map, it));
                }
                if (event.key.code == sf::Keyboard::Enter) {
                    // look up in current screen to get next screen
                    assert(it->second.choices_.size() > static_cast<size_t>(current_choice));
                    choice& chosen = it->second.choices_[current_choice];
                    std::string new_label = evaluateChoice(window, chosen, cond_map, sprite, hp);
                    if (new_label == "game_over") {
                        cond_map.clear();
                        hp = 100;
                        sf::Sprite sp;
                        displayAddText(window, "You lost! Try Again!", sp);
                        it = smap.find("begin");
                        music.openFromFile(musicp / "chapter1.ogg");
                        music.setLoop(true);
                        music.play();
                    } else {
                        it = smap.find(new_label);
                    }
                    current_choice = 0;
                    if (new_label == "chapter2_begin") {
                        music.openFromFile(musicp / "chapter2.ogg");
                        music.setLoop(true);
                        music.play();
                    }
                    if (cond_map.count("west") &&
                        cond_map.count("east") &&
                        cond_map.count("south")) {
                        // make sure it triggers only once
                        cond_map.erase("east");
                        current_choice = 2;
                        it = smap.find("final");
                    }
                    
                    checkHP(window, sprite, hp);
                    assert(it != smap.end());
                    if (!it->second.image_.empty()) {
                        tx.loadFromFile(imgp / it->second.image_);
                    }
                    num_of_choices = it->second.choices_.size();
                }
            }
        }

        window.clear();
        window.draw(sprite);
        drawScreen(window, it->second, current_choice, cond_map);

        std::stringstream ss;
        ss << hp;
        hp_text.setString(ss.str());
        window.draw(hp_text);

        if (enable_fps) {
            // fps
            ++fps_counter;
            std::time(&new_time);
            if (new_time != old_time) {
                old_time = new_time;
                last_fps = fps_counter;
                fps_counter = 0;
            }
            ss.str("");
            ss << last_fps;
            fps_text.setString(ss.str());
            window.draw(fps_text);
        }

        window.display();
    }
    
    return 0;
}
