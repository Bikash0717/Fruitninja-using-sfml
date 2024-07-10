#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Fruit.h"
#include "Utility.h"
#include "Button.h"
#include "GameStats.h"

using namespace sf;

int main() {
    srand(time(0));

    GameStats stats;
    loadStats(stats, "game_stats.txt");

    RenderWindow app(VideoMode(800, 600), "Fruit Ninja");
    app.setFramerateLimit(60);

    Texture tBackground, tWatermelon, tOrange, tApple, tBanana, tMagic, tBomb, tBlade, tSlicedWatermelon, tSlicedOrange, tSlicedApple, tSlicedBanana, tSlicedMagic ,tSlicedBomb;
    tBackground.loadFromFile("images/background.jpeg");
    tWatermelon.loadFromFile("images/watermelon.png");
    tOrange.loadFromFile("images/orange.png");
    tApple.loadFromFile("images/apple.png");
    tBanana.loadFromFile("images/banana.png");
    tMagic.loadFromFile("images/magic.png");
    tBomb.loadFromFile("images/bomb.png");
    tBlade.loadFromFile("images/blade.png");
    tSlicedWatermelon.loadFromFile("images/watermelon_sliced.png");
    tSlicedOrange.loadFromFile("images/orange_sliced.png");
    tSlicedApple.loadFromFile("images/apple_sliced.png");
    tSlicedBanana.loadFromFile("images/banana_sliced.png");
    tSlicedMagic.loadFromFile("images/magic_sliced.png");
    tSlicedBomb.loadFromFile("images/bomb_sliced.png");

    SoundBuffer sliceBuffer, bombBuffer, missBuffer;
    if (!sliceBuffer.loadFromFile("audios/sliced.mp3") ||
        !bombBuffer.loadFromFile("audios/boom.mp3") ||
        !missBuffer.loadFromFile("audios/missed.mp3")) {
        return -1; // Error loading sounds
    }
    Sound sliceSound, bombSound, missSound;
    sliceSound.setBuffer(sliceBuffer);
    bombSound.setBuffer(bombBuffer);
    missSound.setBuffer(missBuffer);

    Sprite sBackground(tBackground);
    Sprite sBlade(tBlade);

    std::vector<Fruit> fruits;
    float spawnTimer = 0;
    int score = 0;
    int lives = 3;

    Font font;
    font.loadFromFile("arial.ttf");
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    Text livesText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::Red);
    livesText.setPosition(700, 10);

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition(200, 250);

    Button playAgainButton(300, 350, 200, 50, "Play Again", font, 24);
    Button viewStatsButton(300, 420, 200, 50, "View Stats", font, 24);

    Text statsText;
    statsText.setFont(font);
    statsText.setCharacterSize(24);
    statsText.setFillColor(Color::White);
    statsText.setPosition(50, 100);

    Vector2f previousMousePos;
    bool bladeVisible = false;
    bool gameOver = false;
    bool showStats = false;

    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();
            if (e.type == Event::MouseButtonPressed) {
                if (e.mouseButton.button == Mouse::Left) {
                    bladeVisible = true;
                    Vector2f mousePos(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y));

                    if (gameOver) {
                        if (playAgainButton.isMouseOver(mousePos)) {
                            gameOver = false;
                            showStats = false;
                            lives = 3;
                            score = 0;
                            fruits.clear();
                        } else if (viewStatsButton.isMouseOver(mousePos)) {
                            showStats = true;
                            statsText.setString("Games Played: " + std::to_string(stats.gamesPlayed) +
                                                "\nTotal Points: " + std::to_string(stats.totalPoints) +
                                                "\nHigh Score: " + std::to_string(stats.highScore));
                        }
                    }
                }
            }
            if (e.type == Event::MouseButtonReleased) {
                if (e.mouseButton.button == Mouse::Left) {
                    bladeVisible = false;
                }
            }
        }

        Vector2i mousePos = Mouse::getPosition(app);
        Vector2f currentMousePos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (!gameOver && !showStats) {
            if (spawnTimer > 30) {
                float x = static_cast<float>(rand() % (app.getSize().x - 50));
                float y = static_cast<float>(app.getSize().y);
                Vector2f position(x, y);

                // Randomly select a fruit type
                int fruitType = rand() % 100; // Use a larger range to adjust rarity of magic fruit
                Texture* fruitTexture;
                Texture* slicedFruitTexture;
                std::string type;
                float scale = 1.0f;
                if (fruitType < 30) {
                    fruitTexture = &tWatermelon;
                    slicedFruitTexture = &tSlicedWatermelon;
                    type = "Watermelon";
                    scale = 0.8f;
                } else if (fruitType < 50) {
                    fruitTexture = &tOrange;
                    slicedFruitTexture = &tSlicedOrange;
                    type = "Orange";
                    scale = 1.2f;
                } else if (fruitType < 70) {
                    fruitTexture = &tApple;
                    slicedFruitTexture = &tSlicedApple;
                    type = "Apple";
                    scale = 1.2f;
                } else if (fruitType < 90) {
                    fruitTexture = &tBanana;
                    slicedFruitTexture = &tSlicedBanana;
                    type = "Banana";
                    scale = 1.1f;
                } else if (fruitType < 95) {
                    fruitTexture = &tBomb;
                    slicedFruitTexture = &tSlicedBomb; 
                    type = "Bomb";
                    scale = 1.0f;
                } else {
                    fruitTexture = &tMagic;
                    slicedFruitTexture = &tSlicedMagic;
                    type = "Magic";
                    scale = 0.7f;
                }

                // Increase velocity based on score
                float baseVelocity = -2;
                float velocityMultiplier = 1 + (score / 10) * 0.1f; // Increase velocity by 10% for every 10 points
                Vector2f velocity(0, baseVelocity * velocityMultiplier);

                fruits.emplace_back(*fruitTexture, position, velocity, scale, type);
                spawnTimer = 0;
            }
            spawnTimer++;

            for (auto it = fruits.begin(); it != fruits.end(); ) {
                it->update();
                if (it->isSliced) {
                    if (it->type == "Bomb") {
                        
                        if (it->slicedClock.getElapsedTime().asSeconds() >= 0.5f) {
                           it = fruits.erase(it);
                            continue;
                        }
                         it->velocity = Vector2f(0, 0.2);
                    }
                    it->velocity = Vector2f(0, 5);
                    if (it->slicedTimer > 60) {
                        it = fruits.erase(it);
                        continue;
                    }
                } else if (bladeVisible && isLineIntersectingSprite(previousMousePos, currentMousePos, it->sprite)) {
                    it->isSliced = true;
                    if (it->type == "Watermelon") {
                        it->sprite.setTexture(tSlicedWatermelon);
                        sliceSound.play();
                        score++;
                    } else if (it->type == "Orange") {
                        it->sprite.setTexture(tSlicedOrange);
                        sliceSound.play();
                        score++;
                    } else if (it->type == "Apple") {
                        it->sprite.setTexture(tSlicedApple);
                        sliceSound.play();
                        score++;
                    } else if (it->type == "Banana") {
                        it->sprite.setTexture(tSlicedBanana);
                        sliceSound.play();
                        score++;
                    } else if (it->type == "Bomb") {
                        it->sprite.setTexture(tSlicedBomb);
                        it->velocity = Vector2f(0, 0.2);
                        bombSound.play();
                       
                        it->slicedClock.restart(); // Start the clock for the bomb
                        lives--;
                    } else if (it->type == "Magic") {
                        it->sprite.setTexture(tSlicedMagic);
                        sliceSound.play();
                        score += 5; // Increase score by 5 times
                    }
                } else if (it->sprite.getPosition().y < 0) {
                    if (!it->isSliced && it->type != "Bomb" && it->type != "Magic") {
                        missSound.play();
                        lives--;
                    }
                    
                    it = fruits.erase(it);
                    if (lives <= 0) {
                        gameOver = true;
                        stats.gamesPlayed++;
                        stats.totalPoints += score;
                        if (score > stats.highScore) {
                            stats.highScore = score;
                        }
                        saveStats(stats, "game_stats.txt");
                    }
                    continue;
                }
                ++it;
            }

            previousMousePos = currentMousePos;
        }

        app.clear();
        app.draw(sBackground);
        if (!showStats) {
            for (auto& fruit : fruits) {
                app.draw(fruit.sprite);
            }

            if (bladeVisible) {
                sBlade.setPosition(currentMousePos);
                app.draw(sBlade);
            }

            scoreText.setString("Score: " + std::to_string(score));
            app.draw(scoreText);

            livesText.setString("Lives: " + std::to_string(lives));
            app.draw(livesText);

            if (gameOver) {
                gameOverText.setString("Game Over!\nFinal Score: " + std::to_string(score));
                app.draw(gameOverText);

                app.draw(playAgainButton.shape);
                app.draw(playAgainButton.text);
                app.draw(viewStatsButton.shape);
                app.draw(viewStatsButton.text);
            }
        } else {
            app.draw(statsText);
        }

        app.display();
    }

    return 0;
}
