#include "Button.h"

Button::Button(float x, float y, float width, float height, const std::string& buttonText, Font& font, unsigned int textSize) {
    shape.setPosition(x, y);
    shape.setSize(Vector2f(width, height));
    shape.setFillColor(Color::Blue);

    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(textSize);
    text.setFillColor(Color::White);
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);
}

bool Button::isMouseOver(Vector2f mousePos) {
    return shape.getGlobalBounds().contains(mousePos);
}
