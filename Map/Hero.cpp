#include "Hero.hpp"
#include <array>

static const std::string Posing = "Tiles/Posing.png";
static std::array<sfC::Picture_t , 3> Way = {
    sfC::Picture_t (Posing, sf::IntRect (4, 33, 17, 31),   sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f)),
    sfC::Picture_t (Posing, sf::IntRect (28, 33, 17, 31),  sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f)),
    sfC::Picture_t (Posing, sf::IntRect (51, 33, 17, 31),  sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f))
};
static std::array<sfC::Picture_t , 3> Run = {
    sfC::Picture_t (Posing, sf::IntRect (76, 33, 17, 31),  sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f)),
    sfC::Picture_t (Posing, sf::IntRect (99, 33, 17, 31),  sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f)),
    sfC::Picture_t (Posing, sf::IntRect (124, 33, 17, 31), sf::Vector2f (500.f, 500.f), sf::Color (), sf::Vector2f (3.f, 3.f))
};

const sfC::Picture_t &sfC::Hero::Action::draw(int MilSec) const {
    switch (Type_)
    {
    case Type::Nothing:
        return Way[(MilSec / 100) % 3];
    case Type::Right:
        return Run[(MilSec / 100) % 3];
    }
    return Run[(MilSec / 100) % 3];
}

sfC::Hero::Type sfC::Hero::Speed::change(sfC::Hero::Type type) {
    return type;
}

sfC::Hero::Action sfC::Hero::Location::change (sfC::Hero::Action Act) {
    switch (Act.get()) {
        case Type::Right:
            return Speed_.change (Type::Right);
        case Type::Left:
            return Speed_.change (Type::Left);
        case Type::Up:
        default:
            std::cout << "Action wasn't declared!\n";
    }
    std::abort ();
}

sfC::Hero::Hero (sfC::Window window , sf::Clock* clock) :
    Action_ () ,
    Window_ (window) ,
    Loc_ (clock) {}

sfC::Hero::Action sfC::Hero::change (sf::Event::EventType EventType) {
    switch (EventType) {
        case sf::Keyboard::Right:
            Action_ = Loc_.change (Action{Type::Right});
            break;
        case sf::Keyboard::Left:
            Action_ = Loc_.change (Action{Type::Left});
            break;
        case sf::Keyboard::Up:
            Action_ = Loc_.change (Action{Type::Up});
            break;
        default:
            break;
    }
}

void sfC::Hero::draw (int time) {
    Window_.draw (Action_.draw(time));
}