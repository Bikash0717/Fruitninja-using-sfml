#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button {
public:
    RectangleShape shape;
    Text text;

    Button(float x, float y, float width, float height, const std::string& buttonText, Font& font, unsigned int textSize);

    bool isMouseOver(Vector2f mousePos);
};
