#pragma once

#include "../Help/FastDraw.hpp"
#include "../Help/Picture.hpp"

namespace sfC {
    class Map {
        private:
            sfC::Window Window_;

        public:
            Map(sfC::Window Window) :
                Window_(Window)
            {}

            void draw() const;
    };
}

namespace sfV {
    constexpr size_t WINDOW_SIZE_X = 1920;
    constexpr size_t WINDOW_SIZE_Y = 1920;
}