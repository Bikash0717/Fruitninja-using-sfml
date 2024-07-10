#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Fruit {
public:
    Sprite sprite;
    Vector2f velocity;
    bool isSliced;
    float slicedTimer;
    std::string type;
    Clock slicedClock; // Add a clock to track time since the fruit was sliced

    Fruit(Texture& texture, Vector2f position, Vector2f velocity, float scale, const std::string& type);

    void update();
};
