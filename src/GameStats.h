#pragma once

struct GameStats {
    int gamesPlayed;
    int totalPoints;
    int highScore;

    GameStats() : gamesPlayed(0), totalPoints(0), highScore(0) {}
};
