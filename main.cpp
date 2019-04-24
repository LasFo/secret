#include "SFML/Graphics.hpp"
#include <ctime>
#include <sstream>
// only for intermediate testing remove later
#include <iostream>
#include <cassert>


size_t w_width = 1600;
size_t w_height = 900;

void drawChoices(size_t number, size_t current_choice, sf::RenderWindow& window/*, string text*/){
    size_t text_box_size = w_height/4;
    size_t choice_box_size = w_height/9;
    assert(number > current_choice);

    // draw the main text box
    sf::RectangleShape displayed_text;
    displayed_text.setSize(sf::Vector2f(w_width - 10, text_box_size - 10));
    displayed_text.setOutlineThickness(5);
    displayed_text.setOutlineColor(sf::Color::Blue);
    displayed_text.setFillColor(sf::Color(50, 0, 0));
    displayed_text.setPosition(5, w_height - text_box_size - number * choice_box_size);
    window.draw(displayed_text);

    // TODO: make one surrounded by golden to indicate acive choice
    while (number--) {
        sf::RectangleShape choice;
        choice.setSize(sf::Vector2f(w_width - 10, choice_box_size - 10));
        choice.setOutlineThickness(5);
        sf::Color col = current_choice == number ?
                            sf::Color(255,220,0) :
                            sf::Color(169,169,169);
        choice.setOutlineColor(col);
        choice.setFillColor(sf::Color(0, 50, 0));
        choice.setPosition(5, w_height - (number+1) * choice_box_size);
        window.draw(choice);
    }
}

int main(int argc, const char** argv) {
    
    if (argc == 3) {
        w_width = atoi(argv[1]);
        w_height = atoi(argv[2]);
    }
    sf::RenderWindow window(sf::VideoMode(w_width, w_height), "SFML works!");
    // TODO: get some background picture
    std::time_t old_time = std::time(nullptr);
    std::time_t new_time;
    uint32_t fps_counter = 0;
    uint32_t last_fps = 0;
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf")) {
        return 1;
    }
    sf::Text fps_text;
    fps_text.setFont(font);
    fps_text.setCharacterSize(20);
    fps_text.setPosition(w_width - 50, 10);
    fps_text.setFillColor(sf::Color::White);

    int current_choice = 0;
    size_t num_of_choices = 3;
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
                if (event.key.code == sf::Keyboard::Down)
                        current_choice = --current_choice < 0 ? num_of_choices - 1 : current_choice;
                if (event.key.code == sf::Keyboard::Up)
                        current_choice = ++current_choice % num_of_choices;
                if (event.key.code == sf::Keyboard::Enter)
                        std::cout << current_choice << std::endl;
            }
        }

        window.clear();
        drawChoices(num_of_choices, current_choice, window);

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

        window.display();
    }
    
return 0;
}
