#include <SFML/Graphics.hpp>
#include "graphismes.hh"


void Grille_MAPF::new_wall(const unsigned int x, const unsigned int y){
    _graphe.new_wall(x,y);
}


void Grille_MAPF::run(){
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Grille de MAPF");
    window.setFramerateLimit(144);
    //sf::Clock clock; // starts the clock

    sf::Vector2u size = window.getSize();
    auto [width, height] = size;

    while (window.isOpen())
    {
        auto c = sf::Color(0,0,0); 

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(c);

        for(int i=0;i<_graphe.get_width();i++){
            for(int j=0;j<_graphe.get_height();j++){
                sf::RectangleShape box({(float)(width/_graphe.get_width()-1), (float)(height/_graphe.get_height()-1)});
                if(_graphe.is_empty(Position(i,j)))
                    box.setFillColor(sf::Color(200,200,200)); 
                else 
                    box.setFillColor(sf::Color(0,0,0)); 
                box.setPosition({(float)(1+i*width/_graphe.get_width()),(float)(1+j*height/_graphe.get_height())});
                window.draw(box);
            }
        }
        

        window.display();
    }
}