#include "Fruit.h"

Fruit::Fruit(Texture& texture, Vector2f position, Vector2f velocity, float scale, const std::string& type)
    : velocity(velocity), isSliced(false), slicedTimer(0), type(type) {
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(scale, scale);
}

void Fruit::update() {
    sprite.move(velocity);
    if (isSliced) {
        slicedTimer++;
    }
}
