#include <SFML/Graphics.hpp>
#include "graphismes.hh"


void Grille_MAPF::run(){
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(144);
    //sf::Clock clock; // starts the clock

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    while (window.isOpen())
    {
        auto c = sf::Color(200,200,200); 

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(c);

        for(int i=0;i<=_n;i++){
            sf::RectangleShape line({(float)width, 1.f});
            line.setFillColor(sf::Color(0,0,0)); 
            line.setPosition({0,i*height/_n});
            window.draw(line);
        }
        for(int i=0;i<=_n;i++){
            sf::RectangleShape line({(float)height, 1.f});
            line.setFillColor(sf::Color(0,0,0));
            line.setPosition({i*width/_n,0});
            line.rotate(sf::degrees(90));
            window.draw(line);
        }
        

        window.display();
    }
}