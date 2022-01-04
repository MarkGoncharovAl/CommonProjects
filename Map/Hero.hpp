#pragma once

#include "../Help/Picture.hpp"
#include "../Help/FastDraw.hpp"

namespace sfV {
    inline constexpr float ACC_JUMP = 1.f;
    inline constexpr float ACC_RUN = 1.f;
}

namespace sfC {
    class Hero
    {
        enum class Type { Nothing , Left , Right , Up };

        class Action
        {
            public:
            Action(Type type = Type::Nothing) : Type_(type) {}
            Action& operator = (const Action& Act) { Type_ = Act.get(); return *this; }
            Type get () const noexcept { return Type_; };
            const sfC::Picture_t &draw(int MilSec) const;
            
            private:
            Type Type_;
        };

        class Speed
        {
            float Speed_ = 0.f;
            float Acc_ = sfV::ACC_RUN;

            public:

            Type change (Type Act);
        };

        class Jump
        {
            float Speed_ = 0.f;
            float Acc_ = 0.f;

            public:
        };

        class Location
        {
            private:
            sf::Vector2f Loc_ { 300.f, 300.f };
            Speed Speed_;
            Jump Jump_;

            public:
            Location (sf::Clock* Clock) {}

            const sfC::Picture_t& applyChanges ();
            Action change (Action Act);
        };

        sf::Clock* Clock_ = nullptr;
        sfC::Window Window_;
        Location Loc_;
        Action Action_;

        public:
        Hero (sfC::Window window , sf::Clock* clock);

        Action change (sf::Event::EventType EventType);
        void draw (int time);
    };
}