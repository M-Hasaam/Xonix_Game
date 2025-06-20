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

struct Player {
    int x, y;
    int dx, dy;
    int score;
    int moves;
    int trailType;
};

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

void Initial_Start_display(RenderWindow &window)
{
    Texture start_T;
    if (!start_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load start texture!" << endl;
        exit(-1);
    }

    Sprite start_S;
    start_S.setTexture(start_T);
    start_S.setScale(1.f, 1.f);
    start_S.setPosition(((N * ts) - start_S.getGlobalBounds().width) / 2.0,
                        ((M * ts + high) - start_S.getGlobalBounds().height) / 2.0);

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Text text;
    text.setFont(font);
    text.setString("START");
    text.setStyle(Text::Bold);
    text.setCharacterSize(start_S.getGlobalBounds().height * 0.7f);
    text.setFillColor(Color::White);

    FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0, textBounds.top + textBounds.height / 2.0);
    text.setPosition(start_S.getPosition().x + start_S.getGlobalBounds().width / 2.0,
                     start_S.getPosition().y + start_S.getGlobalBounds().height / 2.0);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                FloatRect range = start_S.getGlobalBounds();
                if (range.contains(e.mouseButton.x, e.mouseButton.y))
                {
                    cout << " START Seleceted\n";
                    return;
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(start_S);
        window.draw(text);
        window.display();
    }
}

int DisplayDifficultyMenu(RenderWindow &window)
{
    const string labels[4] = {"EASY", "MEDIUM", "HARD", "CONTINUES"};
    const int buttonCount = 4;
    const float spacing = 20.f;

    Texture button_T;
    if (!button_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load button texture!" << endl;
        exit(-1);
    }

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Sprite buttons[buttonCount];
    Text texts[buttonCount];

    Vector2f scale(2.f, 1.f);
    float buttonWidth = button_T.getSize().x * scale.x;
    float buttonHeight = button_T.getSize().y * scale.y;
    float totalHeight = buttonCount * buttonHeight + (buttonCount - 1) * spacing;

    float startX = ((N * ts) - buttonWidth) / 2.0f;
    float startY = ((M * ts) - totalHeight) / 2.0f;

    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i].setTexture(button_T);
        buttons[i].setScale(scale);
        buttons[i].setPosition(startX, startY + i * (buttonHeight + spacing) + high / 2.0);

        texts[i].setFont(font);
        texts[i].setString(labels[i]);
        texts[i].setStyle(Text::Bold);
        texts[i].setCharacterSize(buttonHeight * 0.5f);
        texts[i].setFillColor(Color::White);

        FloatRect textBounds = texts[i].getLocalBounds();
        texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        texts[i].setPosition(buttons[i].getPosition().x + buttonWidth / 2.0f,
                             buttons[i].getPosition().y + buttonHeight / 2.0f);
    }

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                exit(0);
            }

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                    {
                        if (labels[i] == "EASY")
                        {
                            cout << "Easy Selected\n";

                            return 1;
                        }

                        if (labels[i] == "MEDIUM")
                        {
                            cout << "Medium Selected\n";
                            return 2;
                        }
                        if (labels[i] == "HARD")
                        {
                            cout << "Hard Selected\n";

                            return 3;
                        }
                        if (labels[i] == "CONTINUES")
                        {
                            cout << "Continues Selected\n";

                            return 4;
                        }
                    }
                }
            }
        }

        window.clear(Color::Black);

        window.draw(background);
        for (int i = 0; i < buttonCount; ++i)
        {
            window.draw(buttons[i]);
            window.draw(texts[i]);
        }
        window.display();
    }

    return -1;
}

int DisplayExitMenu(RenderWindow &window, int score1, int score2, int time)
{
    const string labels[2] = {"EXIT", "MAIN MENU"};
    const int buttonCount = 2;
    const float spacing = 20.f;

    Texture button_T;
    if (!button_T.loadFromFile("images/button.png"))
    {
        cout << "Error: Could not load button texture!" << endl;
        exit(-1);
    }

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B6.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

    Font font;
    font.loadFromFile("Arial.ttf");

    Sprite buttons[buttonCount];
    Text texts[buttonCount];
    Text scoreText, winnerText, timeText;

    Vector2f scale(1.5f, 1.f);
    float buttonWidth = button_T.getSize().x * scale.x;
    float buttonHeight = button_T.getSize().y * scale.y;
    float totalHeight = buttonCount * buttonHeight + (buttonCount - 1) * spacing;

    float startX = ((N * ts) - buttonWidth) / 2.0f;
    float startY = ((M * ts) - totalHeight) / 2.0f + high / 2.0f;

    for (int i = 0; i < buttonCount; ++i)
    {
        buttons[i].setTexture(button_T);
        buttons[i].setScale(scale);
        buttons[i].setPosition(startX, startY + i * (buttonHeight + spacing) + high / 2.0f);

        texts[i].setFont(font);
        texts[i].setString(labels[i]);
        texts[i].setStyle(Text::Bold);
        texts[i].setCharacterSize(buttonHeight * 0.5f);

        FloatRect textBounds = texts[i].getLocalBounds();
        texts[i].setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        texts[i].setPosition(buttons[i].getPosition().x + buttonWidth / 2.0f,
                             buttons[i].getPosition().y + buttonHeight / 2.0f);
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setStyle(Text::Bold);
    scoreText.setString("Player 1: " + to_string(score1) + "    Player 2: " + to_string(score2));
    FloatRect sb = scoreText.getLocalBounds();
    scoreText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
    scoreText.setPosition(N * ts / 2.0f, 100.f + high / 2.0f);

    int minutes = time / 60;
    int seconds = time % 60;

    timeText.setFont(font);
    timeText.setCharacterSize(28);
    timeText.setFillColor(Color::Cyan);
    timeText.setStyle(Text::Bold);
    timeText.setString("Time Played: " + to_string(minutes) + " min " + to_string(seconds) + " sec");
    FloatRect tb = timeText.getLocalBounds();
    timeText.setOrigin(tb.left + tb.width / 2.0f, tb.top + tb.height / 2.0f);
    timeText.setPosition(N * ts / 2.0f, 140.f + high / 2.0f);

    winnerText.setFont(font);
    winnerText.setCharacterSize(40);
    winnerText.setStyle(Text::Bold);

    if (score1 > score2)
    {
        winnerText.setString("PLAYER 1 WINS!");
        winnerText.setFillColor(Color::Green);
    }
    else if (score2 > score1)
    {
        winnerText.setString("PLAYER 2 WINS!");
        winnerText.setFillColor(Color::Blue);
    }
    else
    {
        winnerText.setString("IT'S A DRAW!");
        winnerText.setFillColor(Color::Yellow);
    }

    FloatRect wb = winnerText.getLocalBounds();
    winnerText.setOrigin(wb.left + wb.width / 2.0f, wb.top + wb.height / 2.0f);
    winnerText.setPosition(N * ts / 2.0f, 40.f + high / 2.0f);

    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                exit(0);

            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for (int i = 0; i < buttonCount; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(e.mouseButton.x, e.mouseButton.y))
                    {
                        if (labels[i] == "EXIT")
                            return 0;
                        if (labels[i] == "MAIN MENU")
                            return 1;
                    }
                }
            }
        }

        window.clear(Color::Black);
        window.draw(background);
        window.draw(winnerText);
        window.draw(scoreText);
        window.draw(timeText);
        for (int i = 0; i < buttonCount; ++i)
        {
            window.draw(buttons[i]);
            window.draw(texts[i]);
        }
        window.display();
    }

    return -1;
}

const int MAX_ENTRIES = 5;

void UpdateScoreboard(int playerNumber, int score, int timeSeconds)
{
    int scores[MAX_ENTRIES + 1] = {0};
    int times[MAX_ENTRIES + 1] = {0};
    string players[MAX_ENTRIES + 1];
    int count = 0;

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

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts + high), "Xonix Game!");
    window.setFramerateLimit(60);

    // DisplayExitMenu(window, 1000, 1000, 100);

    Initial_Start_display(window);

MainMenu:
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = 0;

    int Level = DisplayDifficultyMenu(window);

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Texture backgroundTexture;
    !backgroundTexture.loadFromFile("images/B7.jpg");

    Sprite background(backgroundTexture);
    background.setScale(
        float(window.getSize().x) / background.getTexture()->getSize().x,
        float(window.getSize().y) / background.getTexture()->getSize().y);

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

    Font font;
    font.loadFromFile("Arial.ttf");

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
    int x1 = 0, y1 = 0, dx1 = 0, dy1 = 0;
    int x2 = N - 1, y2 = M - 1, dx2 = 0, dy2 = 0;
    float timer = 0, delay = 0.07, enemyIncreaseTimer = 0;
    Clock clock;

    Clock gameClock;
    int gameTime = 0;

    int player1Score = 0, player2Score = 0;
    int movesCount1 = 0, movesCount2 = 0;
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

                    x1 = 10;
                    y1 = 0;
                    Game = true;
                }
        }

        gameTime = gameClock.getElapsedTime().asSeconds();

        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            dx1 = -1;
            dy1 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            dx1 = 1;
            dy1 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            dx1 = 0;
            dy1 = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            dx1 = 0;
            dy1 = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            dx2 = -1;
            dy2 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            dx2 = 1;
            dy2 = 0;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            dx2 = 0;
            dy2 = -1;
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            dx2 = 0;
            dy2 = 1;
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
            x1 += dx1;
            y1 += dy1;
            x2 += dx2;
            y2 += dy2;

            if (x1 < 0)
                x1 = 0;
            if (x1 > N - 1)
                x1 = N - 1;
            if (y1 < 0)
                y1 = 0;
            if (y1 > M - 1)
                y1 = M - 1;
            if (x2 < 0)
                x2 = 0;
            if (x2 > N - 1)
                x2 = N - 1;
            if (y2 < 0)
                y2 = 0;
            if (y2 > M - 1)
                y2 = M - 1;

            if (grid[y1][x1] == 2)
                Game = false;
            if (grid[y1][x1] == 0)
            {
                grid[y1][x1] = 2;
                player1Score++;
                movesCount1++;
            }

            if (grid[y2][x2] == 3)
                Game = false;
            if (grid[y2][x2] == 0)
            {
                grid[y2][x2] = 3;
                player2Score++;
                movesCount2++;
            }

            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++)
            a[i].move();

        if (grid[y2][x2] == 1)
        {
            dx2 = dy2 = 0;
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
                    player2Score += 4 * count;
                }
                if (occurrences2 > 3)
                {
                    player2Score += 2 * count;
                    threshold2 = 5;
                }
                else
                {
                    player2Score += 2 * count;
                }

                occurrences2++;
            }
            if (count > 1)
                E2.play();
        }

        if (grid[y1][x1] == 1)
        {
            dx1 = dy1 = 0;
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
                    player1Score += 4 * count;
                }
                if (occurrences1 > 3)
                {
                    player1Score += 2 * count;
                    threshold1 = 5;
                }
                else
                {
                    player1Score += 2 * count;
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
        sTile.setPosition(x1 * ts, y1 * ts + high);
        window.draw(sTile);

        sTile.setTextureRect(IntRect(6 * ts, 0, ts, ts));
        sTile.setPosition(x2 * ts, y2 * ts + high);
        window.draw(sTile);

        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].x, a[i].y + high);
            window.draw(sEnemy);
        }

        {
            scoreText.setString("Player 1: " + to_string(player1Score) + " - Player 2: " + to_string(player2Score));
            FloatRect sb = scoreText.getLocalBounds();
            scoreText.setOrigin(sb.left + sb.width / 2.0f, sb.top + sb.height / 2.0f);
            scoreText.setPosition(N * ts / 2.0f, 20.f);

            moveText.setString("P1 Moves: " + to_string(movesCount1) + " - P2 Moves: " + to_string(movesCount2));
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

    UpdateScoreboard(1, player1Score, gameTime);
    UpdateScoreboard(2, player2Score, gameTime);

    if (DisplayExitMenu(window, player1Score, player2Score, gameTime) == 1)
        goto MainMenu;

    delete[] a;
    return 0;
}
