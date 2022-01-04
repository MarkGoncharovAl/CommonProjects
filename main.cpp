#include <iostream>
#include "Map/Map.hpp"
#include "Help/FastDraw.hpp"
#include "Map/Hero.hpp"

#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow RawWindow(sf::VideoMode(sfV::WINDOW_SIZE_X, sfV::WINDOW_SIZE_Y), "SFML works!", sf::Style::Fullscreen);
    sfC::Window Window(&RawWindow);
    sfC::Map Map(Window);
    sf::Clock Clock;
    sfC::Hero Hero(Window, &Clock);

    while (Window->isOpen())
    {
        sf::Event event;
        while (Window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                Window->close();
        }

        Window->clear();
        int time = static_cast<int>(Clock.getElapsedTime ().asMilliseconds());
        Map.draw();
        Hero.draw(time);
        Window->display();
    }

    return 0;
}