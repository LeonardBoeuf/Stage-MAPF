#include "graph.hh"
#include <random>

Position::Position(unsigned int x, unsigned int y) noexcept{
    x_=x;
    y_=y;
}

unsigned int Position::get_x() const noexcept{
    return x_;
}

unsigned int Position::get_y() const noexcept{
    return y_;
}

Graph::Graph(unsigned int width, unsigned int height){
    width_=width;
    height_=height;
}

unsigned int Graph::get_height() const noexcept {
    return height_;
}

unsigned int Graph::get_width() const noexcept {
    return width_;
}

bool Graph::is_empty(const Position &pos) const{
    return ((pos.get_x()+pos.get_y())%6!=0);
}