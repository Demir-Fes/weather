weather
=======

for learning purpose .. get xml then parse and display .. sfml irrxml


#include <thew.h>
#include <SFML/Window.hpp>

int main()
{
    sf::RenderWindow window;
    window.create(sf::VideoMode(800, 600), "Weather Forecast");

    sf::Font font;
    font.loadFromFile("Font.ttf");

    TheWeather thew(font);
    thew.setPosition(300, 100);
    thew.refresh();
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color(255, 255, 255));

        window.draw(thew);
        window.display();
    }
}
