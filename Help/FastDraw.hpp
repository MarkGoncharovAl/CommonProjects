#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

/*
Compile with:
1) Help/Text/Text.cpp
2) Common_libs/Errors/Errors.cpp
3) sfml-graphics, sfml-window
4) -std=c++2a

Using:
sfC::Window{window}.Draw(item1, item2, ...}

Method:
1. if 'item' has function 'draw ()'
   -> will be called window.draw(item.for_draw())
2. if 'item' is array -> array will be checked
3. In another case will be called window.draw(item)
*/

namespace sfC
{
    struct Window final
    {
        Window (sf::RenderWindow *window) : window_ (window) {}

        template <typename ... Args>
        void draw (const Args& ... args) const
        {
            (Try<Args>{}.draw (window_ , args) && ...);
        }

        sf::RenderWindow* operator-> () { return window_; }

    private:
        sf::RenderWindow *window_;

        template <typename T , typename U = std::void_t<>>
        struct Try
        {
            bool draw (sf::RenderWindow *window , const T& elem) { window->draw (elem); return true; }
        };

        template <typename T>
        struct Try<T , std::void_t<decltype(std::declval<T>().draw ())>>
        {
            bool draw (sf::RenderWindow *window , const T& elem) { window->draw (elem.draw ()); return true; }
        };

        template <typename T>
        struct Try < T , std::void_t<decltype(std::declval<T> ().cbegin()) , decltype(std::declval<T>().cend ()) >>
        {
            bool draw (sf::RenderWindow *window , const T& array)
            {
                using value_type = typename T::value_type;

                std::for_each (array.cbegin () , array.cend () ,
                [window](const value_type& elem) {
                    Try<value_type>{}.draw (window , elem);
                });

                return true;
            }
        };
    };
}