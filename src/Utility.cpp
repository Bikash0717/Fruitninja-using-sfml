#include "Utility.h"

void loadStats(GameStats& stats, const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> stats.gamesPlayed >> stats.totalPoints >> stats.highScore;
        file.close();
    }
}

void saveStats(const GameStats& stats, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << stats.gamesPlayed << std::endl;
        file << stats.totalPoints << std::endl;
        file << stats.highScore << std::endl;
        file.close();
    }
}

bool isLineIntersectingSprite(const Vector2f& start, const Vector2f& end, const Sprite& sprite) {
    FloatRect bounds = sprite.getGlobalBounds();

    if (bounds.contains(start) || bounds.contains(end))
        return true;

    Vector2f points[4] = {
        {bounds.left, bounds.top},
        {bounds.left + bounds.width, bounds.top},
        {bounds.left, bounds.top + bounds.height},
        {bounds.left + bounds.width, bounds.top + bounds.height}
    };

    for (int i = 0; i < 4; i++) {
        Vector2f a = points[i];
        Vector2f b = points[(i + 1) % 4];
        float denominator = ((end.x - start.x) * (b.y - a.y)) - ((end.y - start.y) * (b.x - a.x));
        float numerator1 = ((start.y - a.y) * (b.x - a.x)) - ((start.x - a.x) * (b.y - a.y));
        float numerator2 = ((start.y - a.y) * (end.x - start.x)) - ((start.x - a.x) * (end.y - start.y));

        if (denominator == 0)
            continue;

        float r = numerator1 / denominator;
        float s = numerator2 / denominator;

        if ((r >= 0 && r <= 1) && (s >= 0 && s <= 1))
            return true;
    }

    return false;
}
