#pragma once
#include "Error.hpp"
#include <SFML/Graphics.hpp>

namespace sfC
{
    class Picture_t final
    {
    private:
        sf::Image image_;
        sf::Texture texture_;
        sf::Sprite sprite_;

    public:
        Picture_t (const std::string& file , const sf::IntRect& tex_rect , const sf::Vector2f& coord = sf::Vector2f{0.f, 0.f} , const sf::Color& mask = sf::Color () , const sf::Vector2f& scale = sf::Vector2f (1.f , 1.f))
            : image_ () ,
            texture_ () ,
            sprite_ ()
        {
            Err::assert(image_.loadFromFile (file), "Can't load file: " + file);

            image_.createMaskFromColor (mask);
            texture_.loadFromImage (image_);

            sprite_.setTexture (texture_);
            sprite_.setTextureRect (tex_rect);
            sprite_.setPosition (coord);

            sprite_.setScale (scale);
        }
        /////////////////////////////////////////////////////
        // Fast function, used for drawing
        // Application:
        // sf::RenderWindow::draw (Picture_::for_draw());
        /////////////////////////////////////////////////////
        const sf::Sprite& draw () const { return sprite_; }

        /////////////////////////////////////////////////////
        // Getting sf::Sprite::getTextureRect() of object
        /////////////////////////////////////////////////////
        const sf::IntRect& getRect () const { return sprite_.getTextureRect (); }

        /////////////////////////////////////////////////////
        // Returns rectangle of an object
        /////////////////////////////////////////////////////
        sf::FloatRect getLocalBounds () const { return sprite_.getLocalBounds (); }
        sf::FloatRect getGlobalBounds () const { return sprite_.getGlobalBounds (); }

        /////////////////////////////////////////////////////
        // Settings
        /////////////////////////////////////////////////////
        void setTexture  (const sf::IntRect& tex) { sprite_.setTextureRect (tex); }
        void setOrigin   (float x , float y)      { sprite_.setOrigin (x , y); }
        void setScale    (float x , float y)      { sprite_.scale (x , y); }
        void setPosition (float x , float y)      { sprite_.setPosition (x , y); }
        void move        (float x , float y)      { sprite_.move (x , y); }
        void rotate      (float x)                { sprite_.rotate (x); }
        void setRotation (float x)                { sprite_.setRotation (x); }

        /////////////////////////////////////////////////////
        // Checking contains of coordinate in object
        /////////////////////////////////////////////////////
        bool contains (const sf::Vector2f& coords) const { return sprite_.getGlobalBounds ().contains (coords); }
        bool contains (float x , float y)          const { return sprite_.getGlobalBounds ().contains (x , y); }
        bool contains (int x , int y)              const { return sprite_.getGlobalBounds ().contains (static_cast<float>(x) , static_cast<float>(y)); }
    };

} // namespace sfC