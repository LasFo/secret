#include "SFML/Graphics.hpp"

int main(int argc, const char** argv) {
    
    size_t width = 1600, height = 900;
    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    // TODO: get some background picture

    // draw the main text box
    size_t text_box_size = height/4;
    size_t choice_box_size = height/9;
    sf::RectangleShape displayed_text;
    displayed_text.setSize(sf::Vector2f(width - 10, text_box_size - 10));
    displayed_text.setOutlineThickness(5);
    displayed_text.setOutlineColor(sf::Color::Blue);
    displayed_text.setFillColor(sf::Color(50, 0, 0));
    displayed_text.setPosition(5, height - text_box_size - 2 * choice_box_size);
    
    // draw choice boxes TODO: make this variable not 2
    // TODO: make one surrounded by golden to indicate acive choice
    sf::RectangleShape choice1;
    choice1.setSize(sf::Vector2f(width - 10, choice_box_size - 10));
    choice1.setOutlineThickness(5);
    choice1.setOutlineColor(sf::Color(169,169,169));
    choice1.setFillColor(sf::Color(0, 50, 0));
    choice1.setPosition(5, height - 2 * choice_box_size);

    sf::RectangleShape choice2;
    choice2.setSize(sf::Vector2f(width - 10, choice_box_size - 10));
    choice2.setOutlineThickness(5);
    choice2.setOutlineColor(sf::Color(169,169,169));
    choice2.setFillColor(sf::Color(0, 50, 0));
    choice2.setPosition(5, height - choice_box_size);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || 
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
        window.draw(choice1);
        window.draw(choice2);
        window.draw(displayed_text);
        window.display();
    }
    
return 0;
}
