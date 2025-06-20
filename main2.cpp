#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

using namespace sf;
using namespace std;

const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 25;
const int TILE_SIZE = 18;
const int TOP_PANEL = 70;

enum GameState { MAIN_MENU, PLAYING, GAME_OVER };
enum TileType { EMPTY, WALL, PLAYER1_TRAIL, PLAYER2_TRAIL };

struct Player {
    int x, y;
    int dx, dy;
    int score;
    int moves;
    Color color;
};

struct Enemy {
    float x, y;
    float dx, dy;
};

class GameManager {
private:
    RenderWindow window;
    GameState currentState;
    
    vector<vector<TileType>> grid;
    vector<Enemy> enemies;
    
    Player player1, player2;
    int gameTime;
    Clock gameClock;
    
    // Resources
    Texture tilesTexture, enemyTexture, backgroundTexture;
    Font font;
    Music bgMusic;
    SoundBuffer soundBuffer;
    Sound captureSound;
    
    // Game settings
    int difficulty;
    bool twoPlayers;
    float enemySpeed;
    float gameSpeed;
    
public:
    GameManager() : window(VideoMode(GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE + TOP_PANEL), "Xonix Clone") {
        window.setFramerateLimit(60);
        currentState = MAIN_MENU;
        loadResources();
        initGrid();
    }
    
    void loadResources() {
        tilesTexture.loadFromFile("images/tiles.png");
        enemyTexture.loadFromFile("images/enemy.png");
        backgroundTexture.loadFromFile("images/B6.jpg");
        font.loadFromFile("Arial.ttf");
        
        bgMusic.openFromFile("Music.ogg");
        bgMusic.setLoop(true);
        
        soundBuffer.loadFromFile("Effect.wav");
        captureSound.setBuffer(soundBuffer);
    }
    
    void initGrid() {
        grid.resize(GRID_HEIGHT, vector<TileType>(GRID_WIDTH, EMPTY));
        
        // Create borders
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (y == 0 || y == GRID_HEIGHT-1 || x == 0 || x == GRID_WIDTH-1) {
                    grid[y][x] = WALL;
                }
            }
        }
    }
    
    void resetGame() {
        initGrid();
        enemies.clear();
        
        player1 = {10, 0, 0, 0, 0, 0, Color::Green};
        player2 = {GRID_WIDTH-10, GRID_HEIGHT-1, 0, 0, 0, 0, Color::Blue};
        
        gameTime = 0;
        gameClock.restart();
        
        // Create enemies based on difficulty
        int enemyCount = difficulty * 2;
        for (int i = 0; i < enemyCount; i++) {
            addEnemy();
        }
        
        currentState = PLAYING;
        bgMusic.play();
    }
    
    void addEnemy() {
        int x, y;
        do {
            x = 2 + rand() % (GRID_WIDTH - 4);
            y = 2 + rand() % (GRID_HEIGHT - 4);
        } while (grid[y][x] != EMPTY);
        
        Enemy e;
        e.x = x * TILE_SIZE + TILE_SIZE/2;
        e.y = y * TILE_SIZE + TILE_SIZE/2;
        e.dx = (rand() % 3 - 1) * enemySpeed;
        e.dy = (rand() % 3 - 1) * enemySpeed;
        
        enemies.push_back(e);
    }
    
    void floodFill(int y, int x, TileType target, TileType replacement) {
        if (y < 0 || y >= GRID_HEIGHT || x < 0 || x >= GRID_WIDTH) return;
        if (grid[y][x] != target) return;
        
        grid[y][x] = replacement;
        
        floodFill(y-1, x, target, replacement);
        floodFill(y+1, x, target, replacement);
        floodFill(y, x-1, target, replacement);
        floodFill(y, x+1, target, replacement);
    }
    
    void captureArea(Player& player) {
        // Mark all empty areas
        for (auto& e : enemies) {
            floodFill(e.y/TILE_SIZE, e.x/TILE_SIZE, EMPTY, TileType(-1));
        }
        
        // Count captured tiles and convert to walls
        int count = 0;
        TileType playerTrail = (&player == &player1) ? PLAYER1_TRAIL : PLAYER2_TRAIL;
        
        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                if (grid[y][x] == EMPTY || grid[y][x] == playerTrail) {
                    grid[y][x] = WALL;
                    count++;
                } else if (grid[y][x] == TileType(-1)) {
                    grid[y][x] = EMPTY;
                }
            }
        }
        
        // Score calculation
        if (count > 0) {
            player.score += count * 2;
            if (count > 5) player.score += count; // Bonus for large areas
            captureSound.play();
        }
    }
    
    void updateEnemies() {
        for (auto& e : enemies) {
            e.x += e.dx;
            e.y += e.dy;
            
            // Bounce off walls
            if (grid[int(e.y)/TILE_SIZE][int(e.x)/TILE_SIZE] == WALL) {
                e.dx = -e.dx;
                e.x += e.dx;
                e.dy = -e.dy;
                e.y += e.dy;
            }
        }
    }
    
    void updatePlayers(float deltaTime) {
        auto updatePlayer = [&](Player& p) {
            p.x += p.dx;
            p.y += p.dy;
            
            // Boundary check
            p.x = max(0, min(GRID_WIDTH-1, p.x));
            p.y = max(0, min(GRID_HEIGHT-1, p.y));
            
            // Check for collision with trails
            TileType opponentTrail = (&p == &player1) ? PLAYER2_TRAIL : PLAYER1_TRAIL;
            if (grid[p.y][p.x] == opponentTrail) {
                currentState = GAME_OVER;
            }
            
            // Leave trail if on empty space
            if (grid[p.y][p.x] == EMPTY) {
                grid[p.y][p.x] = (&p == &player1) ? PLAYER1_TRAIL : PLAYER2_TRAIL;
                p.score++;
                p.moves++;
            }
            
            // Check for wall hit to capture area
            if (grid[p.y][p.x] == WALL) {
                p.dx = p.dy = 0;
                captureArea(p);
            }
        };
        
        updatePlayer(player1);
        if (twoPlayers) updatePlayer(player2);
    }
    
    void checkEnemyCollisions() {
        for (const auto& e : enemies) {
            int gridX = e.x / TILE_SIZE;
            int gridY = e.y / TILE_SIZE;
            
            if (grid[gridY][gridX] == PLAYER1_TRAIL || grid[gridY][gridX] == PLAYER2_TRAIL) {
                currentState = GAME_OVER;
                break;
            }
        }
    }
    
    void handleInput() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            
            if (currentState == PLAYING) {
                if (event.type == Event::KeyPressed) {
                    // Player 1 controls (WASD)
                    if (event.key.code == Keyboard::A) { player1.dx = -1; player1.dy = 0; }
                    if (event.key.code == Keyboard::D) { player1.dx = 1; player1.dy = 0; }
                    if (event.key.code == Keyboard::W) { player1.dx = 0; player1.dy = -1; }
                    if (event.key.code == Keyboard::S) { player1.dx = 0; player1.dy = 1; }
                    
                    // Player 2 controls (Arrow keys)
                    if (twoPlayers) {
                        if (event.key.code == Keyboard::Left) { player2.dx = -1; player2.dy = 0; }
                        if (event.key.code == Keyboard::Right) { player2.dx = 1; player2.dy = 0; }
                        if (event.key.code == Keyboard::Up) { player2.dx = 0; player2.dy = -1; }
                        if (event.key.code == Keyboard::Down) { player2.dx = 0; player2.dy = 1; }
                    }
                    
                    if (event.key.code == Keyboard::Escape) {
                        resetGame();
                    }
                }
            }
        }
    }
    
    void update(float deltaTime) {
        if (currentState != PLAYING) return;
        
        gameTime = gameClock.getElapsedTime().asSeconds();
        
        updateEnemies();
        updatePlayers(deltaTime);
        checkEnemyCollisions();
        
        // Increase difficulty over time
        static float difficultyTimer = 0;
        difficultyTimer += deltaTime;
        if (difficultyTimer > 20.0f) {
            enemySpeed *= 1.1f;
            if (difficulty == 4) { // Continues mode
                addEnemy();
            }
            difficultyTimer = 0;
        }
    }
    
    void render() {
        window.clear();
        
        // Draw background
        Sprite background(backgroundTexture);
        background.setScale(
            float(window.getSize().x) / background.getTexture()->getSize().x,
            float(window.getSize().y) / background.getTexture()->getSize().y
        );
        window.draw(background);
        
        if (currentState == MAIN_MENU) {
            renderMainMenu();
        } 
        else if (currentState == PLAYING || currentState == GAME_OVER) {
            // Draw grid
            Sprite tile(tilesTexture);
            for (int y = 0; y < GRID_HEIGHT; y++) {
                for (int x = 0; x < GRID_WIDTH; x++) {
                    if (grid[y][x] == WALL) {
                        tile.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE));
                    } 
                    else if (grid[y][x] == PLAYER1_TRAIL) {
                        tile.setTextureRect(IntRect(3*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    }
                    else if (grid[y][x] == PLAYER2_TRAIL) {
                        tile.setTextureRect(IntRect(4*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                    }
                    else {
                        continue;
                    }
                    
                    tile.setPosition(x * TILE_SIZE, y * TILE_SIZE + TOP_PANEL);
                    window.draw(tile);
                }
            }
            
            // Draw players
            tile.setTextureRect(IntRect(2*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            tile.setPosition(player1.x * TILE_SIZE, player1.y * TILE_SIZE + TOP_PANEL);
            window.draw(tile);
            
            if (twoPlayers) {
                tile.setTextureRect(IntRect(6*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
                tile.setPosition(player2.x * TILE_SIZE, player2.y * TILE_SIZE + TOP_PANEL);
                window.draw(tile);
            }
            
            // Draw enemies
            Sprite enemySprite(enemyTexture);
            enemySprite.setOrigin(TILE_SIZE/2, TILE_SIZE/2);
            for (const auto& e : enemies) {
                enemySprite.setPosition(e.x, e.y + TOP_PANEL);
                enemySprite.rotate(5);
                window.draw(enemySprite);
            }
            
            // Draw UI
            renderGameUI();
            
            if (currentState == GAME_OVER) {
                renderGameOver();
            }
        }
        
        window.display();
    }
    
    void renderMainMenu() {
        Text title("XONIX CLONE", font, 50);
        title.setFillColor(Color::Cyan);
        title.setStyle(Text::Bold);
        title.setPosition(window.getSize().x/2 - title.getGlobalBounds().width/2, 50);
        
        vector<string> options = {"1 PLAYER", "2 PLAYERS", "EXIT"};
        vector<RectangleShape> buttons(options.size());
        
        for (size_t i = 0; i < options.size(); i++) {
            buttons[i].setSize(Vector2f(200, 50));
            buttons[i].setFillColor(Color(70, 70, 70, 200));
            buttons[i].setOutlineThickness(2);
            buttons[i].setOutlineColor(Color::White);
            buttons[i].setPosition(window.getSize().x/2 - 100, 150 + i*70);
            
            Text option(options[i], font, 30);
            option.setFillColor(Color::White);
            option.setPosition(
                buttons[i].getPosition().x + buttons[i].getSize().x/2 - option.getGlobalBounds().width/2,
                buttons[i].getPosition().y + buttons[i].getSize().y/2 - option.getGlobalBounds().height
            );
            
            window.draw(buttons[i]);
            window.draw(option);
            
            // Check for clicks
            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (i == 0) {
                        twoPlayers = false;
                        difficulty = 1; // Easy by default
                        resetGame();
                    } else if (i == 1) {
                        twoPlayers = true;
                        difficulty = 1; // Easy by default
                        resetGame();
                    } else if (i == 2) {
                        window.close();
                    }
                }
            }
        }
        
        window.draw(title);
    }
    
    void renderGameUI() {
        // Score display
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::White);
        
        string scoreStr = "Player 1: " + to_string(player1.score);
        if (twoPlayers) {
            scoreStr += "   Player 2: " + to_string(player2.score);
        }
        scoreText.setString(scoreStr);
        scoreText.setPosition(10, 10);
        window.draw(scoreText);
        
        // Time display
        int minutes = gameTime / 60;
        int seconds = int(gameTime) % 60;
        Text timeText("Time: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds), 
                     font, 24);
        timeText.setPosition(window.getSize().x - timeText.getGlobalBounds().width - 10, 10);
        window.draw(timeText);
    }
    
    void renderGameOver() {
        RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(Color(0, 0, 0, 150));
        window.draw(overlay);
        
        Text gameOverText("GAME OVER", font, 50);
        gameOverText.setFillColor(Color::Red);
        gameOverText.setStyle(Text::Bold);
        gameOverText.setPosition(
            window.getSize().x/2 - gameOverText.getGlobalBounds().width/2,
            window.getSize().y/2 - 100
        );
        
        string winnerText;
        if (!twoPlayers) {
            winnerText = "Your score: " + to_string(player1.score);
        } else {
            if (player1.score > player2.score) {
                winnerText = "Player 1 wins!";
            } else if (player2.score > player1.score) {
                winnerText = "Player 2 wins!";
            } else {
                winnerText = "It's a tie!";
            }
        }
        
        Text winnerDisplay(winnerText, font, 30);
        winnerDisplay.setFillColor(Color::Yellow);
        winnerDisplay.setPosition(
            window.getSize().x/2 - winnerDisplay.getGlobalBounds().width/2,
            window.getSize().y/2
        );
        
        Text restartPrompt("Press SPACE to restart or ESC for menu", font, 24);
        restartPrompt.setFillColor(Color::White);
        restartPrompt.setPosition(
            window.getSize().x/2 - restartPrompt.getGlobalBounds().width/2,
            window.getSize().y/2 + 100
        );
        
        window.draw(gameOverText);
        window.draw(winnerDisplay);
        window.draw(restartPrompt);
        
        // Check for restart input
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            resetGame();
        } else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            currentState = MAIN_MENU;
        }
    }
    
    void run() {
        Clock clock;
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            handleInput();
            update(deltaTime);
            render();
        }
    }
};

int main() {
    srand(time(nullptr));
    
    GameManager game;
    game.run();
    
    return 0;
}