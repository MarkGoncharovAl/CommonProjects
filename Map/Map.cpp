#include "Map.hpp"

static const sfC::Picture_t BackGround("Tiles/BackGround4.jpg", sf::IntRect(0, 0, sfV::WINDOW_SIZE_X, sfV::WINDOW_SIZE_Y));

namespace sfC {
    void Map::draw() const {
        Window_.draw(BackGround);
    }
}