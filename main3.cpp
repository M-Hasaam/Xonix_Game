// #include <SFML/Graphics.hpp>
// #include <SFML/Audio.hpp>
// #include <time.h>
// #include <cmath>
// #include <cstdlib>
// #include <string>
// #include <cstring>
// #include <fstream>
// #include <iostream>

// using namespace sf;
// using namespace std;

// const int M = 25;
// const int N = 40;

// int grid[M][N] = {0};
// int ts = 18; // tile size
// int high = 70;

// struct Player {
//     int x, y;
//     int dx, dy;
//     int score;
//     int moves;
//     Color color;
//     int trailType;
//     Keyboard::Key controls[4];
// };

// struct Enemy {
//     float x, y, dx, dy;

//     Enemy() {
//         int gx, gy;
//         do {
//             gx = 2 + rand() % (N - 4);
//             gy = 2 + rand() % (M - 4);
//         } while (grid[gy][gx] != 0);

//         x = gx * ts;
//         y = gy * ts;

//         dx = rand() % 3 - 1.f;
//         dy = rand() % 3 - 1.f;
//     }

//     void move() {
//         x += dx;
//         if (grid[int(y) / ts][int(x) / ts] == 1) {
//             dx = -dx;
//             x += dx;
//         }
//         y += dy;
//         if (grid[int(y) / ts][int(x) / ts] == 1) {
//             dy = -dy;
//             y += dy;
//         }
//     }
// };

// void drop(int y, int x) {
//     if (grid[y][x] == 0) grid[y][x] = -1;
//     if (grid[y - 1][x] == 0) drop(y - 1, x);
//     if (grid[y + 1][x] == 0) drop(y + 1, x);
//     if (grid[y][x - 1] == 0) drop(y, x - 1);
//     if (grid[y][x + 1] == 0) drop(y, x + 1);
// }

// void Initial_Start_display(RenderWindow &window) {
//     Texture backgroundTexture;
//     if (!backgroundTexture.loadFromFile("images/B6.jpg")) {
//         cout << "Error: Could not load background texture!" << endl;
//         exit(-1);
//     }

//     Sprite background(backgroundTexture);
//     background.setScale(
//         float(window.getSize().x) / background.getTexture()->getSize().x,
//         float(window.getSize().y) / background.getTexture()->getSize().y);

//     Font font;
//     if (!font.loadFromFile("Arial.ttf")) {
//         cout << "Error: Could not load font!" << endl;
//         exit(-1);
//     }

//     Text title("XONIX GAME", font, 50);
//     title.setFillColor(Color::Cyan);
//     title.setStyle(Text::Bold);
//     title.setPosition(N * ts / 2 - title.getGlobalBounds().width / 2, 50);

//     RectangleShape buttons[3];
//     Text buttonTexts[3];
//     string labels[3] = {"1 PLAYER", "2 PLAYERS", "EXIT"};

//     for (int i = 0; i < 3; i++) {
//         buttons[i].setSize(Vector2f(200, 50));
//         buttons[i].setFillColor(Color(70, 70, 70, 200));
//         buttons[i].setOutlineThickness(2);
//         buttons[i].setOutlineColor(Color::White);
//         buttons[i].setPosition(N * ts / 2 - 100, 150 + i * 70);

//         buttonTexts[i].setFont(font);
//         buttonTexts[i].setString(labels[i]);
//         buttonTexts[i].setCharacterSize(30);
//         buttonTexts[i].setFillColor(Color::White);
//         buttonTexts[i].setPosition(
//             buttons[i].getPosition().x + buttons[i].getSize().x / 2 - buttonTexts[i].getGlobalBounds().width / 2,
//             buttons[i].getPosition().y + buttons[i].getSize().y / 2 - buttonTexts[i].getGlobalBounds().height
//         );
//     }

//     while (window.isOpen()) {
//         Event e;
//         while (window.pollEvent(e)) {
//             if (e.type == Event::Closed) {
//                 exit(0);
//             }

//             if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
//                 Vector2i mousePos = Mouse::getPosition(window);
//                 for (int i = 0; i < 3; i++) {
//                     if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                         if (i == 0) return; // 1 Player
//                         if (i == 1) return; // 2 Players (same return for now)
//                         if (i == 2) exit(0);
//                     }
//                 }
//             }
//         }

//         window.clear();
//         window.draw(background);
//         window.draw(title);
//         for (int i = 0; i < 3; i++) {
//             window.draw(buttons[i]);
//             window.draw(buttonTexts[i]);
//         }
//         window.display();
//     }
// }

// int DisplayDifficultyMenu(RenderWindow &window) {
//     Texture backgroundTexture;
//     if (!backgroundTexture.loadFromFile("images/B6.jpg")) {
//         cout << "Error: Could not load background texture!" << endl;
//         exit(-1);
//     }

//     Sprite background(backgroundTexture);
//     background.setScale(
//         float(window.getSize().x) / background.getTexture()->getSize().x,
//         float(window.getSize().y) / background.getTexture()->getSize().y);

//     Font font;
//     if (!font.loadFromFile("Arial.ttf")) {
//         cout << "Error: Could not load font!" << endl;
//         exit(-1);
//     }

//     Text title("SELECT DIFFICULTY", font, 40);
//     title.setFillColor(Color::Cyan);
//     title.setStyle(Text::Bold);
//     title.setPosition(N * ts / 2 - title.getGlobalBounds().width / 2, 50);

//     RectangleShape buttons[4];
//     Text buttonTexts[4];
//     string labels[4] = {"EASY", "MEDIUM", "HARD", "ENDLESS"};

//     for (int i = 0; i < 4; i++) {
//         buttons[i].setSize(Vector2f(200, 50));
//         buttons[i].setFillColor(Color(70, 70, 70, 200));
//         buttons[i].setOutlineThickness(2);
//         buttons[i].setOutlineColor(Color::White);
//         buttons[i].setPosition(N * ts / 2 - 100, 150 + i * 70);

//         buttonTexts[i].setFont(font);
//         buttonTexts[i].setString(labels[i]);
//         buttonTexts[i].setCharacterSize(30);
//         buttonTexts[i].setFillColor(Color::White);
//         buttonTexts[i].setPosition(
//             buttons[i].getPosition().x + buttons[i].getSize().x / 2 - buttonTexts[i].getGlobalBounds().width / 2,
//             buttons[i].getPosition().y + buttons[i].getSize().y / 2 - buttonTexts[i].getGlobalBounds().height
//         );
//     }

//     while (window.isOpen()) {
//         Event e;
//         while (window.pollEvent(e)) {
//             if (e.type == Event::Closed) {
//                 exit(0);
//             }

//             if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
//                 Vector2i mousePos = Mouse::getPosition(window);
//                 for (int i = 0; i < 4; i++) {
//                     if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                         return i + 1; // 1-4 for difficulty levels
//                     }
//                 }
//             }
//         }

//         window.clear();
//         window.draw(background);
//         window.draw(title);
//         for (int i = 0; i < 4; i++) {
//             window.draw(buttons[i]);
//             window.draw(buttonTexts[i]);
//         }
//         window.display();
//     }
//     return -1;
// }

// int DisplayExitMenu(RenderWindow &window, int score1, int score2, int time) {
//     Texture backgroundTexture;
//     if (!backgroundTexture.loadFromFile("images/B6.jpg")) {
//         cout << "Error: Could not load background texture!" << endl;
//         exit(-1);
//     }

//     Sprite background(backgroundTexture);
//     background.setScale(
//         float(window.getSize().x) / background.getTexture()->getSize().x,
//         float(window.getSize().y) / background.getTexture()->getSize().y);

//     Font font;
//     if (!font.loadFromFile("Arial.ttf")) {
//         cout << "Error: Could not load font!" << endl;
//         exit(-1);
//     }

//     Text gameOverText("GAME OVER", font, 50);
//     gameOverText.setFillColor(Color::Red);
//     gameOverText.setStyle(Text::Bold);
//     gameOverText.setPosition(N * ts / 2 - gameOverText.getGlobalBounds().width / 2, 50);

//     string winnerStr;
//     if (score1 > score2) winnerStr = "PLAYER 1 WINS!";
//     else if (score2 > score1) winnerStr = "PLAYER 2 WINS!";
//     else winnerStr = "IT'S A DRAW!";

//     Text winnerText(winnerStr, font, 40);
//     winnerText.setFillColor(score1 > score2 ? Color::Green : Color::Blue);
//     winnerText.setPosition(N * ts / 2 - winnerText.getGlobalBounds().width / 2, 120);

//     Text scoreText("P1: " + to_string(score1) + " - P2: " + to_string(score2), font, 30);
//     scoreText.setFillColor(Color::White);
//     scoreText.setPosition(N * ts / 2 - scoreText.getGlobalBounds().width / 2, 180);

//     int minutes = time / 60;
//     int seconds = time % 60;
//     Text timeText("Time: " + to_string(minutes) + "m " + to_string(seconds) + "s", font, 25);
//     timeText.setFillColor(Color::Yellow);
//     timeText.setPosition(N * ts / 2 - timeText.getGlobalBounds().width / 2, 220);

//     RectangleShape buttons[2];
//     Text buttonTexts[2];
//     string labels[2] = {"MAIN MENU", "EXIT"};

//     for (int i = 0; i < 2; i++) {
//         buttons[i].setSize(Vector2f(200, 50));
//         buttons[i].setFillColor(Color(70, 70, 70, 200));
//         buttons[i].setOutlineThickness(2);
//         buttons[i].setOutlineColor(Color::White);
//         buttons[i].setPosition(N * ts / 2 - 100, 280 + i * 70);

//         buttonTexts[i].setFont(font);
//         buttonTexts[i].setString(labels[i]);
//         buttonTexts[i].setCharacterSize(30);
//         buttonTexts[i].setFillColor(Color::White);
//         buttonTexts[i].setPosition(
//             buttons[i].getPosition().x + buttons[i].getSize().x / 2 - buttonTexts[i].getGlobalBounds().width / 2,
//             buttons[i].getPosition().y + buttons[i].getSize().y / 2 - buttonTexts[i].getGlobalBounds().height
//         );
//     }

//     while (window.isOpen()) {
//         Event e;
//         while (window.pollEvent(e)) {
//             if (e.type == Event::Closed) {
//                 exit(0);
//             }

//             if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
//                 Vector2i mousePos = Mouse::getPosition(window);
//                 for (int i = 0; i < 2; i++) {
//                     if (buttons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
//                         return i; // 0 for main menu, 1 for exit
//                     }
//                 }
//             }
//         }

//         window.clear();
//         window.draw(background);
//         window.draw(gameOverText);
//         window.draw(winnerText);
//         window.draw(scoreText);
//         window.draw(timeText);
//         for (int i = 0; i < 2; i++) {
//             window.draw(buttons[i]);
//             window.draw(buttonTexts[i]);
//         }
//         window.display();
//     }
//     return -1;
// }

// void UpdateScoreboard(int playerNumber, int score, int timeSeconds) {
//     const int MAX_ENTRIES = 5;
//     int scores[MAX_ENTRIES + 1] = {0};
//     int times[MAX_ENTRIES + 1] = {0};
//     string players[MAX_ENTRIES + 1];
//     int count = 0;

//     ifstream infile("scoreboard.txt");
//     if (infile.is_open()) {
//         string label;
//         infile >> label >> label >> label;
//         while (infile >> players[count] >> label >> scores[count] >> label >> times[count]) {
//             count++;
//             if (count == MAX_ENTRIES) break;
//         }
//         infile.close();
//     }

//     scores[count] = score;
//     times[count] = timeSeconds;
//     players[count] = "Player[" + to_string(playerNumber) + "]";
//     count++;

//     for (int i = 0; i < count - 1; i++) {
//         for (int j = i + 1; j < count; j++) {
//             if (scores[j] > scores[i] || (scores[j] == scores[i] && times[j] < times[i])) {
//                 swap(scores[i], scores[j]);
//                 swap(times[i], times[j]);
//                 swap(players[i], players[j]);
//             }
//         }
//     }

//     if (count > MAX_ENTRIES) count = MAX_ENTRIES;

//     ofstream outfile("scoreboard.txt");
//     outfile << "=== SCOREBOARD ===\n";
//     for (int i = 0; i < count; i++)
//         outfile << players[i] << " Score: " << scores[i] << " Time: " << times[i] << "\n";
// }

// int main() {
//     srand(time(0));

//     RenderWindow window(VideoMode(N * ts, M * ts + high), "Xonix Game Clone!");
//     window.setFramerateLimit(60);

//     Initial_Start_display(window);

// MainMenu:
//     for (int i = 0; i < M; i++)
//         for (int j = 0; j < N; j++)
//             grid[i][j] = 0;

//     int Level = DisplayDifficultyMenu(window);

//     Texture t1, t2, t3;
//     if (!t1.loadFromFile("images/tiles.png") || 
//         !t2.loadFromFile("images/gameover.png") || 
//         !t3.loadFromFile("images/enemy.png")) {
//         cout << "Error loading textures!" << endl;
//         return -1;
//     }

//     Texture backgroundTexture;
//     if (!backgroundTexture.loadFromFile("images/B7.jpg")) {
//         cout << "Error loading background!" << endl;
//         return -1;
//     }
//     Sprite background(backgroundTexture);
//     background.setScale(
//         float(window.getSize().x) / background.getTexture()->getSize().x,
//         float(window.getSize().y) / background.getTexture()->getSize().y);

//     Music music;
//     if (!music.openFromFile("Music.ogg")) {
//         cout << "Error loading music!" << endl;
//         return -1;
//     }
//     music.setLoop(true);
//     music.play();

//     SoundBuffer SB;
//     if (!SB.loadFromFile("Effect.wav")) {
//         cout << "Error loading sound effect!" << endl;
//         return -1;
//     }
//     Sound E1, E2;
//     E1.setBuffer(SB);
//     E2.setBuffer(SB);

//     Font font;
//     if (!font.loadFromFile("Arial.ttf")) {
//         cout << "Error loading font!" << endl;
//         return -1;
//     }

//     Text moveText, scoreText;
//     moveText.setFont(font);
//     moveText.setCharacterSize(25);
//     moveText.setFillColor(Color::White);
//     scoreText.setFont(font);
//     scoreText.setCharacterSize(25);
//     scoreText.setFillColor(Color::White);
//     scoreText.setStyle(Text::Bold);

//     Sprite sTile(t1), sGameover(t2), sEnemy(t3);
//     sGameover.setPosition(100, 100);
//     sEnemy.setOrigin(20, 20);

//     Enemy* enemies = nullptr;
//     int enemyCount = Level * 2;
//     if (Level == 4) enemyCount = 2;
//     enemies = new Enemy[enemyCount];

//     bool Game = true;
//     Player player1 = {10, 0, 0, 0, 0, 0, Color::Green, 2, {Keyboard::A, Keyboard::D, Keyboard::W, Keyboard::S}};
//     Player player2 = {N-10, M-1, 0, 0, 0, 0, Color::Blue, 3, {Keyboard::Left, Keyboard::Right, Keyboard::Up, Keyboard::Down}};

//     float timer = 0, delay = 0.07, enemyIncreaseTimer = 0;
//     Clock clock;
//     Clock gameClock;
//     int gameTime = 0;

//     for (int i = 0; i < M; i++)
//         for (int j = 0; j < N; j++)
//             if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
//                 grid[i][j] = 1;

//     while (window.isOpen()) {
//         float time = clock.getElapsedTime().asSeconds();
//         clock.restart();
//         timer += time;
//         enemyIncreaseTimer += time;

//         Event e;
//         while (window.pollEvent(e)) {
//             if (e.type == Event::Closed) {
//                 window.close();
//             }

//             if (e.type == Event::KeyPressed) {
//                 if (e.key.code == Keyboard::Escape) {
//                     for (int i = 1; i < M - 1; i++)
//                         for (int j = 1; j < N - 1; j++)
//                             grid[i][j] = 0;
//                     player1 = {10, 0, 0, 0, player1.score, player1.moves, Color::Green, 2, {Keyboard::A, Keyboard::D, Keyboard::W, Keyboard::S}};
//                     Game = true;
//                 }
//             }
//         }

//         gameTime = gameClock.getElapsedTime().asSeconds();

//         auto handlePlayerInput = [](Player& p) {
//             if (Keyboard::isKeyPressed(p.controls[0])) { p.dx = -1; p.dy = 0; }
//             if (Keyboard::isKeyPressed(p.controls[1])) { p.dx = 1; p.dy = 0; }
//             if (Keyboard::isKeyPressed(p.controls[2])) { p.dx = 0; p.dy = -1; }
//             if (Keyboard::isKeyPressed(p.controls[3])) { p.dx = 0; p.dy = 1; }
//         };

//         handlePlayerInput(player1);
//         handlePlayerInput(player2);

//         if (!Game) continue;

//         if (enemyIncreaseTimer >= 20.f) {
//             if (Level == 4) {
//                 Enemy* temp = new Enemy[enemyCount + 1];
//                 for (int i = 0; i < enemyCount; i++)
//                     temp[i] = enemies[i];
//                 delete[] enemies;
//                 enemies = temp;
//                 enemyCount++;
//             }

//             for (int i = 0; i < enemyCount; i++) {
//                 enemies[i].dx *= 1.2f;
//                 enemies[i].dy *= 1.2f;
//             }
//             enemyIncreaseTimer = 0;
//         }

//         if (timer > delay) {
//             auto updatePlayer = [](Player& p) {
//                 p.x += p.dx;
//                 p.y += p.dy;

//                 p.x = max(0, min(N - 1, p.x));
//                 p.y = max(0, min(M - 1, p.y));

//                 if (grid[p.y][p.x] == p.trailType) Game = false;
//                 if (grid[p.y][p.x] == 0) {
//                     grid[p.y][p.x] = p.trailType;
//                     p.score++;
//                     p.moves++;
//                 }

//                 if (grid[p.y][p.x] == 1) {
//                     p.dx = p.dy = 0;
//                     int count = 0;

//                     for (int i = 0; i < enemyCount; i++)
//                         drop(enemies[i].y / ts, enemies[i].x / ts);

//                     for (int i = 0; i < M; i++) {
//                         for (int j = 0; j < N; j++) {
//                             if (grid[i][j] == -1) grid[i][j] = 0;
//                             else if (grid[i][j] == 0 || grid[i][j] == p.trailType) {
//                                 grid[i][j] = 1;
//                                 count++;
//                             }
//                         }
//                     }

//                     if (count > 0) {
//                         p.score += count * 2;
//                         if (count > 5) p.score += count;
//                         E1.play();
//                     }
//                 }
//             };

//             updatePlayer(player1);
//             updatePlayer(player2);
//             timer = 0;
//         }

//         for (int i = 0; i < enemyCount; i++) {
//             enemies[i].move();
//             if (grid[int(enemies[i].y) / ts][int(enemies[i].x) / ts] == player1.trailType || 
//                 grid[int(enemies[i].y) / ts][int(enemies[i].x) / ts] == player2.trailType) {
//                 Game = false;
//             }
//         }

//         window.clear();
//         window.draw(background);

//         for (int i = 0; i < M; i++) {
//             for (int j = 0; j < N; j++) {
//                 if (grid[i][j] == 0) continue;
//                 if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
//                 if (grid[i][j] == 2) sTile.setTextureRect(IntRect(3 * ts, 0, ts, ts));
//                 if (grid[i][j] == 3) sTile.setTextureRect(IntRect(4 * ts, 0, ts, ts));
//                 sTile.setPosition(j * ts, i * ts + high);
//                 window.draw(sTile);
//             }
//         }

//         sTile.setTextureRect(IntRect(2 * ts, 0, ts, ts));
//         sTile.setPosition(player1.x * ts, player1.y * ts + high);
//         window.draw(sTile);

//         sTile.setTextureRect(IntRect(6 * ts, 0, ts, ts));
//         sTile.setPosition(player2.x * ts, player2.y * ts + high);
//         window.draw(sTile);

//         sEnemy.rotate(10);
//         for (int i = 0; i < enemyCount; i++) {
//             sEnemy.setPosition(enemies[i].x, enemies[i].y + high);
//             window.draw(sEnemy);
//         }

//         scoreText.setString("P1: " + to_string(player1.score) + " - P2: " + to_string(player2.score));
//         scoreText.setPosition(N * ts / 2 - scoreText.getGlobalBounds().width / 2, 20);
//         window.draw(scoreText);

//         moveText.setString("Moves: " + to_string(player1.moves) + " - " + to_string(player2.moves));
//         moveText.setPosition(N * ts / 2 - moveText.getGlobalBounds().width / 2, 50);
//         window.draw(moveText);

//         if (!Game) break;

//         window.display();
//     }

//     UpdateScoreboard(1, player1.score, gameTime);
//     UpdateScoreboard(2, player2.score, gameTime);

//     if (DisplayExitMenu(window, player1.score, player2.score, gameTime) == 0) {
//         delete[] enemies;
//         goto MainMenu;
//     }

//     delete[] enemies;
//     return 0;
// }