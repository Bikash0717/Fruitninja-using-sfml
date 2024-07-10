#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "GameStats.h"

using namespace sf;

void loadStats(GameStats& stats, const std::string& filename);
void saveStats(const GameStats& stats, const std::string& filename);
bool isLineIntersectingSprite(const Vector2f& start, const Vector2f& end, const Sprite& sprite);
