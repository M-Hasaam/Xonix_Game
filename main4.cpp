#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace sf;
using namespace std;

const int M = 25;
const int N = 40;

int grid[M][N] = {0};
int ts = 18; // tile size
int high = 70;

struct Enemy
{
    float x, y, dx, dy;

    Enemy()
    {
        int gx, gy;
        do
        {
            gx = 2 + rand() % (N - 4);
            gy = 2 + rand() % (M - 4);
        } while (grid[gy][gx] != 0);

        x = gx * ts;
        y = gy * ts;

        dx = rand() % 3 - 1.f;
        dy = rand() % 3 - 1.f;
    }

    void move()
    {
        x += dx;
        if (grid[int(y) / ts][int(x) / ts] == 1)
        {
            dx = -dx;
            x += dx;
        }
        y += dy;
        if (grid[int(y) / ts][int(x) / ts] == 1)
        {
            dy = -dy;
            y += dy;
        }
    }
};

// void centerTextInButton(Text &text, const RectangleShape &button)
// {
//     FloatRect textBounds = text.getLocalBounds();
//     text.setOrigin(textBounds.left + textBounds.width / 2.0f,
//                    textBounds.top + textBounds.height / 2.0f);

//     Vector2f buttonPos = button.getPosition();
//     Vector2f buttonSize = button.getSize();

//     text.setPosition(
//         buttonPos.x + buttonSize.x / 2.0f,
//         buttonPos.y + buttonSize.y / 2.0f);
// }
// void centerSpriteInBackground(Sprite &sprite, const Sprite &background)
// {
//     FloatRect spriteBounds = sprite.getLocalBounds();
//     sprite.setOrigin(spriteBounds.left + spriteBounds.width / 2.0f,
//                      spriteBounds.top + spriteBounds.height / 2.0f);

//     FloatRect bgBounds = background.getGlobalBounds();

//     sprite.setPosition(
//         bgBounds.left + bgBounds.width / 2.0f,
//         bgBounds.top + bgBounds.height / 2.0f);
// }
// void centerRectangleInBackground(RectangleShape &rect, const Sprite &background) {
//     FloatRect rectBounds = rect.getLocalBounds();
//     rect.setOrigin(rectBounds.left + rectBounds.width / 2.0f,
//                    rectBounds.top + rectBounds.height / 2.0f);

//     FloatRect bgBounds = background.getGlobalBounds();

//     rect.setPosition(
//         bgBounds.left + bgBounds.width / 2.0f,
//         bgBounds.top + bgBounds.height / 2.0f);
// }

void drop(int y, int x)
{
    if (grid[y][x] == 0)
        grid[y][x] = -1;
    if (grid[y - 1][x] == 0)
        drop(y - 1, x);
    if (grid[y + 1][x] == 0)
        drop(y + 1, x);
    if (grid[y][x - 1] == 0)
        drop(y, x - 1);
    if (grid[y][x + 1] == 0)
        drop(y, x + 1);
}

// void centerTextInButton(Text &text, const RectangleShape &button) {
//     FloatRect textBounds = text.getLocalBounds();
//     text.setOrigin(textBounds.left + textBounds.width / 2.0f,
//                    textBounds.top + textBounds.height / 2.0f);
//     Vector2f buttonPos = button.getPosition();
//     Vector2f buttonSize = button.getSize();
//     text.setPosition(buttonPos.x + buttonSize.x / 2.0f,
//                      buttonPos.y + buttonSize.y / 2.0f);
// }
// void centerTextInButton(Text &text, const RectangleShape &button)
// {
//     FloatRect textBounds = text.getLocalBounds();
//     text.setOrigin(textBounds.left + textBounds.width / 2.0f,
//                    textBounds.top + textBounds.height / 2.0f);

//     Vector2f buttonPos = button.getPosition();
//     Vector2f buttonSize = button.getSize();

//     // Adjust Y manually for pixel-perfect baseline alignment
//     text.setPosition(
//         buttonPos.x + buttonSize.x / 2.0f,
//         buttonPos.y + buttonSize.y / 2.0f - 4.0f  // ← tweak this offset if needed
//     );
// }
// void centerTextInButton(Text &text, const RectangleShape &button)
// {
//     FloatRect bounds = text.getLocalBounds();

//     text.setOrigin(
//         bounds.left + bounds.width / 2.0f,
//         bounds.top + bounds.height / 2.0f
//     );

//     Vector2f buttonCenter = button.getPosition() + button.getSize() / 2.0f;

//     // Optional vertical adjustment (based on font size)
//     float verticalAdjustment = text.getCharacterSize() * 0.1f;

//     text.setPosition(
//         buttonCenter.x,
//         buttonCenter.y - verticalAdjustment
//     );
// }
void centerTextInButton(Text &text, const RectangleShape &button)
{
    FloatRect bounds = text.getLocalBounds();

    // Fix origin: includes left/top for accurate alignment
    text.setOrigin(
        bounds.left + bounds.width / 2.0f,
        bounds.top + bounds.height / 2.0f);

    Vector2f buttonCenter = button.getPosition() + button.getSize() / 2.0f;

    // Vertical offset compensates for baseline shift
    float verticalAdjust = text.getCharacterSize() * 0.1f;

    text.setPosition(buttonCenter.x, buttonCenter.y - verticalAdjust);
}

void centerRectangleInBackground(RectangleShape &rect, const Sprite &background)
{
    FloatRect rectBounds = rect.getLocalBounds();
    rect.setOrigin(rectBounds.left + rectBounds.width / 2.0f,
                   rectBounds.top + rectBounds.height / 2.0f);
    FloatRect bgBounds = background.getGlobalBounds();
    rect.setPosition(bgBounds.left + bgBounds.width / 2.0f,
                     bgBounds.top + bgBounds.height / 2.0f);
}

void Strat_Screen(RenderWindow &window, Sprite &BackGround, Font &font, int &player_1_2)
{
    RectangleShape Start_button(Vector2f(200, 60));
    centerRectangleInBackground(Start_button, BackGround);
    Start_button.setFillColor(Color::Blue);

    RectangleShape Player1_button(Vector2f(200, 60));
    RectangleShape Player2_button(Vector2f(200, 60));
    RectangleShape Easy_button(Vector2f(200, 60));
    RectangleShape Medium_button(Vector2f(200, 60));
    RectangleShape Hard_button(Vector2f(200, 60));
    RectangleShape Continue_button(Vector2f(200, 60));

    float centerX = BackGround.getGlobalBounds().left + BackGround.getGlobalBounds().width / 2.0f;
    float centerY = BackGround.getGlobalBounds().top + BackGround.getGlobalBounds().height / 2.0f;

    Player1_button.setPosition(centerX - 100, centerY - 40 - 30);
    Player2_button.setPosition(centerX - 100, centerY + 40 + 30);

    Player1_button.setFillColor(Color::Blue);
    Player2_button.setFillColor(Color::Blue);

    Text buttonText("", font, 30);
    buttonText.setFillColor(Color::White);
    buttonText.setStyle(Text::Bold);

    int Start_1_Player_2_Difficulty_3 = 1;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(BackGround);

        if (Start_1_Player_2_Difficulty_3 == 1)
        {
            window.draw(Start_button);
            buttonText.setString("Start");
            buttonText.setPosition(centerX - 30, centerY - 20);
            window.draw(buttonText);

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(Mouse::getPosition(window));
                if (Start_button.getGlobalBounds().contains(mousePos))
                    Start_1_Player_2_Difficulty_3 = 2;
            }
        }
        else if (Start_1_Player_2_Difficulty_3 == 2)
        {
            window.draw(Player1_button);
            buttonText.setString("Player 1");
            centerTextInButton(buttonText, Player1_button);
            window.draw(buttonText);

            window.draw(Player2_button);
            buttonText.setString("Player 2");
            centerTextInButton(buttonText, Player2_button);
            window.draw(buttonText);

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(Mouse::getPosition(window));
                if (Player1_button.getGlobalBounds().contains(mousePos))
                {
                    player_1_2 = 1;
                    return;
                }
                else if (Player2_button.getGlobalBounds().contains(mousePos))
                {
                    player_1_2 = 2;
                    return;
                }
            }
        }

        window.display();
    }
}

void Difficulty_Button(RenderWindow &window, Sprite &BackGround, Font &font, int &level)
{
    RectangleShape Easy_button(Vector2f(200, 60));
    RectangleShape Medium_button(Vector2f(200, 60));
    RectangleShape Hard_button(Vector2f(200, 60));
    RectangleShape Continue_button(Vector2f(200, 60));

    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    Easy_button.setPosition(centerX - 100, centerY - 180);
    Medium_button.setPosition(centerX - 100, centerY - 80);
    Hard_button.setPosition(centerX - 100, centerY + 20);
    Continue_button.setPosition(centerX - 100, centerY + 120);

    Easy_button.setFillColor(Color::Blue);
    Medium_button.setFillColor(Color::Blue);
    Hard_button.setFillColor(Color::Blue);
    Continue_button.setFillColor(Color::Blue);

    Text buttonText("", font, 25);
    buttonText.setFillColor(Color::White);
    buttonText.setStyle(Text::Bold);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(Mouse::getPosition(window));

                if (Easy_button.getGlobalBounds().contains(mousePos))
                {
                    level = 1;
                    return;
                }
                else if (Medium_button.getGlobalBounds().contains(mousePos))
                {
                    level = 2;
                    return;
                }
                else if (Hard_button.getGlobalBounds().contains(mousePos))
                {
                    level = 3;
                    return;
                }
                else if (Continue_button.getGlobalBounds().contains(mousePos))
                {
                    level = 4;
                    return;
                }
            }
        }

        window.clear(Color::Black);

        window.draw(BackGround);

        window.draw(Easy_button);
        buttonText.setString("Easy");
        centerTextInButton(buttonText, Easy_button);
        window.draw(buttonText);

        window.draw(Medium_button);
        buttonText.setString("Medium");
        centerTextInButton(buttonText, Medium_button);
        window.draw(buttonText);

        window.draw(Hard_button);
        buttonText.setString("Hard");
        centerTextInButton(buttonText, Hard_button);
        window.draw(buttonText);

        window.draw(Continue_button);
        buttonText.setString("Continues");
        centerTextInButton(buttonText, Continue_button);
        window.draw(buttonText);

        window.display();
    }
}

void DisplayExitMenu(RenderWindow &window, int score1, int score2, int time, Sprite &BackGround, Font &font, bool &replay_the_game)
{
    // Menu configuration
    const float spacing = 20.f;

    // Button style configuration - simplified with fixed color
    const float buttonWidth = 200.f;
    const float buttonHeight = 60.f;
    const float outlineThickness = 2.f;

    // Initialize UI elements
    RectangleShape exitButton, mainMenuButton;
    Text text; // Single text object that will be reconfigured for different uses

    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    // Setup EXIT button
    exitButton.setSize(Vector2f(200, 60));
    mainMenuButton.setSize(Vector2f(200, 60));

    exitButton.setFillColor(Color::Blue);
    mainMenuButton.setFillColor(Color::Blue);

    exitButton.setPosition(centerX-100, centerY-40);
    mainMenuButton.setPosition(centerX-100, centerY+50);

    text.setFont(font);
    text.setStyle(Text::Bold);
    // text.setColor(Color::White);

    // Main menu loop
    while (window.isOpen())
    {
        // Handle events
        Event e;
        while (window.pollEvent(e))
        {
            // Exit on close button
            if (e.type == Event::Closed)
                exit(0);

            // Handle button clicks
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                Vector2f mousePos(e.mouseButton.x, e.mouseButton.y);

                // Check EXIT button
                if (exitButton.getGlobalBounds().contains(mousePos))
                {
                    exit(0); // Exit the game
                }

                // Check MAIN MENU button
                if (mainMenuButton.getGlobalBounds().contains(mousePos))
                {
                    // Go back to main menu
                    replay_the_game = true;
                    return;
                }
            }
        }

        // Draw everything
        window.clear(Color::Black);
        window.draw(BackGround);

        // Draw winner announcement
        text.setCharacterSize(40);
        string winnerMessage;
        if (score1 > score2)
            winnerMessage = "PLAYER 1 WINS!";
        else if (score2 > score1)
            winnerMessage = "PLAYER 2 WINS!";
        else
            winnerMessage = "IT'S A DRAW!";
        text.setString(winnerMessage);
        text.setFillColor(Color::Yellow);

        text.setPosition(N * ts / 2.0f - 200, high / 2.0f);
        window.draw(text);

        // Draw score display
        text.setCharacterSize(30);
        text.setFillColor(Color::White);

        text.setString("Player 1: " + to_string(score1));
        text.setPosition(N * ts / 2.0f - 200, 60.f + high / 2.0f);
        window.draw(text);

        text.setString("Player 2: " + to_string(score2));
        text.setPosition(N * ts / 2.0f - 200, 100.f + high / 2.0f);
        window.draw(text);

        // Draw time display
        int minutes = time / 60;
        int seconds = time % 60;
        text.setCharacterSize(28);
        text.setString("Time Played: " + to_string(minutes) + " min " + to_string(seconds) + " sec");
        text.setPosition(N * ts / 2.0f - 200, 140.f + high / 2.0f);
        window.draw(text);

        window.draw(exitButton);
        window.draw(mainMenuButton);

        text.setString("EXIT");
        centerTextInButton(text, exitButton);
        window.draw(text);

        // Draw MAIN MENU button text
        text.setString("MAIN MENU");
        centerTextInButton(text, mainMenuButton);
        window.draw(text);

        window.display();
    }
}


const int MAX_ENTRIES = 5;
void UpdateScoreboard(int playerNumber, int score, int timeSeconds)
{
    int scores[MAX_ENTRIES + 1] = {0};
    int times[MAX_ENTRIES + 1] = {0};
    int count = 0;
    string players[MAX_ENTRIES + 1];

    ifstream infile("scoreboard.txt");
    if (infile.is_open())
    {
        string label;
        infile >> label >> label >> label;
        while (infile >> players[count] >> label >> scores[count] >> label >> times[count])
        {
            count++;
            if (count == MAX_ENTRIES)
                break;
        }
        infile.close();
    }

    scores[count] = score;
    times[count] = timeSeconds;
    players[count] = "Player[" + to_string(playerNumber) + "]";
    count++;

    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (scores[j] > scores[i] || (scores[j] == scores[i] && times[j] < times[i]))
            {
                swap(scores[i], scores[j]);
                swap(times[i], times[j]);
                swap(players[i], players[j]);
            }
        }
    }

    if (count > MAX_ENTRIES)
        count = MAX_ENTRIES;

    ofstream outfile("scoreboard.txt");
    outfile << "=== SCOREBOARD ===\n";
    for (int i = 0; i < count; i++)
        outfile << players[i] << " Score: " << scores[i] << " Time: " << times[i] << "\n";
}

struct Player
{
    int x, y;
    int dx, dy;
    int previous_x, previous_y;
    int score;
    int moves;
};

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts + high), "Xonix Game!");
    window.setFramerateLimit(60);

    Font font;
    font.loadFromFile("Arial.ttf");

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B7.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    int player_1_2 = 0;


MainMenu:
    Strat_Screen(window, background, font, player_1_2);
    
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;

    int Level = 0;

    Difficulty_Button(window, background, font, Level);

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Music music;
    if (!music.openFromFile("Music.ogg"))
        return -1;
    music.setLoop(true);
    music.play();

    SoundBuffer SB;
    if (!SB.loadFromFile("Effect.wav"))
        return -1;

    Sound E1, E2;
    E1.setBuffer(SB);
    E2.setBuffer(SB);

    Text moveText;
    moveText.setFont(font);
    moveText.setCharacterSize(25);
    moveText.setFillColor(Color::White);
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(25);
    scoreText.setFillColor(Color::White);
    scoreText.setStyle(Text::Bold);

    ////////

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    Enemy *a = nullptr;
    int enemyCount = Level * 2;
    if (Level == 4)
        enemyCount = 2;
    a = new Enemy[enemyCount];

    bool Game = true;

    // Replace individual player variables with Player structs
    Player player1 = {0, 0, 0, 0, 0, 0, 0, 0};
    Player player2 = {N - 1, M - 1, 0, 0, N - 1, M - 1, 0, 0};

    float timer = 0, delay = 0.07, enemyIncreaseTimer = 0;
    Clock clock;

    Clock gameClock;
    int gameTime = 0;

    int threshold1 = 10, threshold2 = 10;
    int occurrences1 = 0, occurrences2 = 0;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        enemyIncreaseTimer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    player1.x = 10;
                    player1.y = 0;
                    Game = true;
                }
        }

        gameTime = gameClock.getElapsedTime().asSeconds();

        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            player1.dx = -1;
            player1.dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            player1.dx = 1;
            player1.dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            player1.dx = 0;
            player1.dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            player1.dx = 0;
            player1.dy = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            player2.dx = -1;
            player2.dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            player2.dx = 1;
            player2.dy = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            player2.dx = 0;
            player2.dy = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            player2.dx = 0;
            player2.dy = 1;
        }

        if (!Game)
            continue;

        if (enemyIncreaseTimer >= 20.f)
        {
            if (Level == 4)
            {
                Enemy *temp = new Enemy[enemyCount + 1];
                for (int i = 0; i < enemyCount; i++)
                    temp[i] = a[i];
                delete[] a;
                a = temp;
                enemyCount++;
            }

            const float speedFactor = 1.2f;

            static int CC = 0;

            cout << " C = " << CC++ << " dx = " << a[0].dx << endl;
            for (int i = 0; i < enemyCount; i++)
            {
                a[i].dx *= speedFactor;
                a[i].dy *= speedFactor;
            }
            enemyIncreaseTimer = 0;
        }

        if (timer > delay)
        {
            // Store previous positions
            player1.previous_x = player1.x;
            player1.previous_y = player1.y;
            player2.previous_x = player2.x;
            player2.previous_y = player2.y;

            // Update positions
            player1.x += player1.dx;
            player1.y += player1.dy;
            player2.x += player2.dx;
            player2.y += player2.dy;

            // Boundary checks for player1
            if (player1.x < 0)
                player1.x = 0;
            if (player1.x > N - 1)
                player1.x = N - 1;
            if (player1.y < 0)
                player1.y = 0;
            if (player1.y > M - 1)
                player1.y = M - 1;

            // Boundary checks for player2
            if (player2.x < 0)
                player2.x = 0;
            if (player2.x > N - 1)
                player2.x = N - 1;
            if (player2.y < 0)
                player2.y = 0;
            if (player2.y > M - 1)
                player2.y = M - 1;

            // Game logic for player1
            if (grid[player1.y][player1.x] == 2)
                Game = false;
            if (grid[player1.y][player1.x] == 0)
            {
                grid[player1.y][player1.x] = 2;
                player1.score++;
                player1.moves++;
            }

            // Game logic for player2
            if (grid[player2.y][player2.x] == 3)
                Game = false;
            if (grid[player2.y][player2.x] == 0)
            {
                grid[player2.y][player2.x] = 3;
                player2.score++;
                player2.moves++;
            }

            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++)
            a[i].move();

        if (grid[player2.y][player2.x] == 1)
        {
            player2.dx = player2.dy = 0;
            int count = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1)
                        grid[i][j] = 0;
                    else if (grid[i][j] == 0 || grid[i][j] == 3)
                    {
                        grid[i][j] = 1;
                        count++;
                    }

            if (count >= threshold2)
            {
                if (occurrences2 > 5)
                {
                    player2.score += 4 * count;
                }
                if (occurrences2 > 3)
                {
                    player2.score += 2 * count;
                    threshold2 = 5;
                }
                else
                {
                    player2.score += 2 * count;
                }

                occurrences2++;
            }
            if (count > 1)
                E2.play();
        }

        if (grid[player1.y][player1.x] == 1)
        {
            player1.dx = player1.dy = 0;
            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            int count = 0;

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1)
                        grid[i][j] = 0;
                    else if (grid[i][j] == 0 || grid[i][j] == 2)
                    {
                        grid[i][j] = 1;
                        count++;
                    }

            if (count >= threshold1)
            {
                if (occurrences1 > 5)
                {
                    player1.score += 4 * count;
                }
                if (occurrences1 > 3)
                {
                    player1.score += 2 * count;
                    threshold1 = 5;
                }
                else
                {
                    player1.score += 2 * count;
                }

                occurrences1++;
            }

            if (count > 1)
                E1.play();
        }

        for (int i = 0; i < enemyCount; i++)
            if (grid[int(a[i].y) / ts][int(a[i].x) / ts] == 2 || grid[int(a[i].y) / ts][int(a[i].x) / ts] == 3)
                Game = false;

        window.clear();
        window.draw(background);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0)
                    continue;
                if (grid[i][j] == 1)
                    sTile.setTextureRect(IntRect(0, 0, ts, ts));
                if (grid[i][j] == 2)
                    sTile.setTextureRect(IntRect(3 * ts, 0, ts, ts));
                if (grid[i][j] == 3)
                    sTile.setTextureRect(IntRect(4 * ts, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts + high);
                window.draw(sTile);
            }

        sTile.setTextureRect(IntRect(2 * ts, 0, ts, ts));
        sTile.setPosition(player1.x * ts, player1.y * ts + high);
        window.draw(sTile);

        sTile.setTextureRect(IntRect(6 * ts, 0, ts, ts));
        sTile.setPosition(player2.x * ts, player2.y * ts + high);
        window.draw(sTile);

        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y + high);
            window.draw(sEnemy);
        }

        {
            scoreText.setString("Player 1: " + to_string(player1.score) + " - Player 2: " + to_string(player2.score));
            FloatRect sb = scoreText.getLocalBounds();
            scoreText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
            scoreText.setPosition(N * ts / 2.0f, 20.f);

            moveText.setString("P1 Moves: " + to_string(player1.moves) + " - P2 Moves: " + to_string(player2.moves));
            sb = moveText.getLocalBounds();
            moveText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
            moveText.setPosition(N * ts / 2.0f, 50.f);
        }

        window.draw(scoreText);
        window.draw(moveText);

        if (!Game)
            break;

        window.display();
    }

    UpdateScoreboard(1, player1.score, gameTime);
    UpdateScoreboard(2, player2.score, gameTime);

    bool goback;

    DisplayExitMenu(window, player1.score, player2.score, gameTime, background, font, goback);

    if (goback)
        goto MainMenu;

    delete[] a;
    return 0;
}
