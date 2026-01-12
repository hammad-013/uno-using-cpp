#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <raylib.h>
using namespace std;

template <typename T>
class Stack
{
private:
    T* arr;
    int capacity;
    int top;

    void resize()
    {
        capacity *= 2;
        T* newArr = new T[capacity];
        for (int i = 0; i < top; i++)
        {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }

public:
    Stack() : capacity(10), top(0)
    {
        arr = new T[capacity];
    }

    Stack(const Stack& other) : capacity(other.capacity), top(other.top)
    {
        arr = new T[capacity];
        for (int i = 0; i < top; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    Stack& operator=(const Stack& other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            top = other.top;
            arr = new T[capacity];
            for (int i = 0; i < top; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }

    ~Stack()
    {
        delete[] arr;
    }

    void push(T value)
    {
        if (top >= capacity)
        {
            resize();
        }
        arr[top++] = value;
    }

    T pop()
    {
        if (isEmpty())
        {
            return T();
        }
        return arr[--top];
    }

    T peek() const
    {
        if (isEmpty())
        {
            return T();
        }
        return arr[top - 1];
    }

    bool isEmpty() const
    {
        return top == 0;
    }

    int sizee() const
    {
        return top;
    }

    void Clear()
    {
        top = 0;
    }

    void shuffle()
    {
        if (isEmpty() || sizee() == 1)
            return;

        int n = sizee();
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            T temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

   /* T getAtIndex(int index) const
    {
        if (index < 0 || index >= top)
            return T();
        return arr[index];
    }*/

    void setAtIndex(int index, T value)
    {
        if (index >= 0 && index < top)
        {
            arr[index] = value;
        }
    }

    bool removeAtIndex(int index)
    {
        if (index < 0 || index >= top)
            return false;

        for (int i = index; i < top - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        top--;
        return true;
    }
};

class Card
{
public:
    string fullName;

    Card() : fullName("") {}
    Card(string name) : fullName(name) {}

    string getColor()
    {
        if (fullName == "Wild" || fullName == "Wild_Draw_4")
        {
            return "Wild";
        }
        int underscorePos = -1;
        for (int i = 0; i < fullName.length(); i++)
        {
            if (fullName[i] == '_')
            {
                underscorePos = i;
                break;
            }
        }

        if (underscorePos == -1)
        {
            return "";
        }

        string color = "";
        for (int i = 0; i < underscorePos; i++)
        {
            color += fullName[i];
        }
        return color;
    }

    string getValue()
    {
        if (fullName == "Wild")
        {
            return "Wild";
        }
        if (fullName == "Wild_Draw_4")
        {
            return "Draw_4";
        }
        int underscorePos = -1;
        for (int i = 0; i < fullName.length(); i++)
        {
            if (fullName[i] == '_')
            {
                underscorePos = i;
                break;
            }
        }

        if (underscorePos == -1)
        {
            return fullName;
        }

        string value = "";
        for (int i = underscorePos + 1; i < fullName.length(); i++)
        {
            value += fullName[i];
        }
        return value;
    }

    bool isWild()
    {
        return (fullName == "Wild" || fullName == "Wild_Draw_4");
    }

    bool isSpecial()
    {
        string val = getValue();
        return (val == "Skip" || val == "Reverse" || val == "Draw_2" || val == "Draw_4");
    }

    bool canPlayOn(Card topCard, string currentColor)
    {
        if (isWild())
        {
            return true;
        }

        if (topCard.isWild())
        {
            return (getColor() == currentColor);
        }

        return (getColor() == topCard.getColor() || getValue() == topCard.getValue());
    }
};

class Hand
{
private:
    Stack<string> cards;

public:
    void addCard(string card)
    {
        cards.push(card);
    }

    int sizee()
    {
        return cards.sizee();
    }

    bool isEmpty()
    {
        return cards.isEmpty();
    }

    string getCardAt(int index)
    {
        if (index < 0 || index >= cards.sizee())
            return "";
        Card temp[cards.sizee()];
        int c = 0;
        while (!cards.isEmpty())
        {
            temp[c++] = cards.pop();
        }
        Card result = temp[index];

        for (int i = c - 1; i >= 0; i--)
        {
            cards.push(temp[i]);
        }

        return result;
    }

    bool removeCard(string cardToRemove)
    {
        for (int i = 0; i < cards.sizee(); i++)
        {
            if (cards.getAtIndex(i) == cardToRemove)
            {
                return cards.removeAtIndex(i);
            }
        }
        return false;
    }

    void clear()
    {
        cards.Clear();
    }
};

class Player
{
public:
    int id;
    Hand hand;
    int score;

    Player() : id(0), score(0) {}
    Player(int playerId) : id(playerId), score(0) {}

    void drawCard(string card)
    {
        hand.addCard(card);
    }

    string playCard(int index)
    {
        string card = hand.getCardAt(index);
        if (card != "")
        {
            hand.removeCard(card);
        }
        return card;
    }

    int getHandSize()
    {
        return hand.sizee();
    }

    void addScore(int points)
    {
        score += points;
    }

    int getScore() const
    {
        return score;
    }
};

class Deck
{
private:
    Stack<string> drawPile;
    Stack<string> discardPile;

public:
    void createDeck()
    {
        drawPile.Clear();
        discardPile.Clear();

        string colors[4] = {"Red","Blue","Green","Yellow"};
        for (int c = 0; c < 4; c++)
        {
            string color = colors[c];
            drawPile.push(color + "_0");
            for (int num = 1; num<=9; num++)
            {
                string numStr = to_string(num);
                drawPile.push(color + "_" + numStr);
                drawPile.push(color + "_" + numStr);
            }
            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Draw_2");
            drawPile.push(color + "_Draw_2");
            drawPile.push("Wild");
            drawPile.push("Wild_Draw_4");
        }
        drawPile.shuffle();
    }

    string drawCard()
    {
        if (drawPile.isEmpty())
        {
            reshuffleDiscard();
        }

        if (drawPile.isEmpty())
        {
            return "";
        }

        return drawPile.pop();
    }

    void discard(string card)
    {
        discardPile.push(card);
    }

    string getTopDiscard() const
    {
        if (discardPile.isEmpty())
        {
            return "";
        }
        return discardPile.peek();
    }

    void reshuffleDiscard()
    {
        if (discardPile.sizee() <= 1)
        {
            return;
        }
        string topCard = discardPile.pop();

        while (!discardPile.isEmpty())
        {
            drawPile.push(discardPile.pop());
        }

        drawPile.shuffle();
        discardPile.push(topCard);
    }

    int getDrawPileSize()
    {
        return drawPile.sizee();
    }

    int getDiscardPileSize()
    {
        return discardPile.sizee();
    }

    string flipStartCard()
    {
        string card = drawCard();
        if (card != "")
        {
            discard(card);
        }
        return card;
    }
};

class Game
{
private:
    Stack<Player> players;
    Deck deck;
    int currentPlayerIndex;
    int totalPlayers;
    bool clockwise;
    string currentColor;
    bool gameOver;
    string colorBeforeWild;
    bool challengeAvailable;
    int lastWildDraw4Player;
    string lastDrawnCard;
    bool drawnCardPlayable;
    int gameWinner;

    Player getPlayerAt(int index)
    {
        if (index < 0 || index >= totalPlayers)
            return Player();
        return players.getAtIndex(index);
    }

    void updatePlayerAt(int index, Player updatedPlayer)
    {
        if (index >= 0 && index < totalPlayers)
        {
            players.setAtIndex(index, updatedPlayer);
        }
    }

    int getNextPlayerIndex()
    {
        if (clockwise)
        {
            return (currentPlayerIndex + 1) % totalPlayers;
        }
        else
        {
            return (currentPlayerIndex - 1 + totalPlayers) % totalPlayers;
        }
    }

    int getCardPoints(string cardName)
    {
        return 1;
    }

    void calculateGameScore(int winnerIndex)
    {
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            if (i == winnerIndex)
            {
                p.score = 0;
            }
            else
            {
                p.score = -(p.getHandSize());
            }
            updatePlayerAt(i, p);
        }
    }

public:
    Game() : currentPlayerIndex(0), totalPlayers(4), clockwise(true),
     currentColor(""), gameOver(false), challengeAvailable(false),
     lastWildDraw4Player(-1), lastDrawnCard(""), drawnCardPlayable(false),
     gameWinner(-1)
    {
        for (int i = 0; i < 4; i++)
        {
            players.push(Player(i));
        }
    }

    int getTotalPlayers() const
    {
         return totalPlayers;
    }

    int getCurrentPlayerIndex() const
    {
         return currentPlayerIndex;
    }

    bool getClockwise() const
    {
         return clockwise;
    }

    string getCurrentColor() const
    {
         return currentColor;
    }

    string getTopDiscard() const
    {
         return deck.getTopDiscard();
    }

    Player getPlayerAtPublic(int index)
    {
         return getPlayerAt(index);
    }

    bool isGameOver() const
    {
         return gameOver;
    }

    int getDrawPileSize()
    {
         return deck.getDrawPileSize();
    }

    int getDiscardPileSize()
    {
         return deck.getDiscardPileSize();
    }

    bool canDrawFromDeck()
    {
        int totalAvailable = deck.getDrawPileSize();
        if (deck.getDiscardPileSize() > 1)
        {
            totalAvailable += deck.getDiscardPileSize() - 1;
        }
        return totalAvailable > 0;
    }

    bool currentPlayerHasPlayableCard()
    {
        Player currentPlayer = getPlayerAt(currentPlayerIndex);
        Card topCard(deck.getTopDiscard());

        for (int i = 0; i < currentPlayer.getHandSize(); i++)
        {
            string cardName = currentPlayer.hand.getCardAt(i);
            Card card(cardName);
            if (card.canPlayOn(topCard, currentColor))
            {
                return true;
            }
        }
        return false;
    }

    string getLastDrawnCard() const
    {
        return lastDrawnCard;
    }

    int getPlayerScore(int playerIndex)
    {
        if (playerIndex >= 0 && playerIndex < totalPlayers)
        {
            Player p = getPlayerAt(playerIndex);
            return p.getScore();
        }
        return 0;
    }

    int getGameWinner() const
    {
        return gameWinner;
    }

    void startGame()
    {
        deck.createDeck();
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            for (int j = 0; j < 7; j++)
            {
                p.drawCard(deck.drawCard());
            }
            updatePlayerAt(i, p);
        }

        string startCard = deck.flipStartCard();
        Card start(startCard);
        while (start.isWild())
        {
            deck.discard(deck.drawCard());
            startCard = deck.getTopDiscard();
            start = Card(startCard);
        }

        currentColor = start.getColor();

        if (start.getValue() == "Skip")
        {
            currentPlayerIndex = 1;
        }
        else if (start.getValue() == "Reverse")
        {
            clockwise = false;
        }
        else if (start.getValue() == "Draw_2")
        {
            Player p = getPlayerAt(0);
            p.drawCard(deck.drawCard());
            p.drawCard(deck.drawCard());
            updatePlayerAt(0, p);
            currentPlayerIndex = 1;
        }

        gameOver = false;
    }

    string drawCard()
    {
        return deck.drawCard();
    }

    bool playCard(int cardIndex, string& message)
    {
        Player currentPlayer = getPlayerAt(currentPlayerIndex);
        string playedCard = currentPlayer.playCard(cardIndex);

        if (playedCard == "")
        {
            message = "Invalid card!";
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return false;
        }

        Card played(playedCard);
        Card topCard(deck.getTopDiscard());

        if (!played.canPlayOn(topCard, currentColor))
        {
            message = "Cannot play this card!";
            currentPlayer.hand.addCard(playedCard);
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return false;
        }

        deck.discard(playedCard);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        message = "Player " + to_string(currentPlayer.id) + " played: " + playedCard;

        if (currentPlayer.getHandSize() == 0)
        {
            gameOver = true;
            gameWinner = currentPlayerIndex;
            calculateGameScore(currentPlayerIndex);

            message = "Player " + to_string(currentPlayer.id) + " WINS THE GAME!";
            return true;
        }

        Player updatedPlayer = getPlayerAt(currentPlayerIndex);
        if (updatedPlayer.getHandSize() == 1)
        {
            message = "Player " + to_string(currentPlayer.id) + " played: " + playedCard + " - UNO!";
        }

        if (!played.isWild())
        {
            currentColor = played.getColor();
        }

        handleCardEffect(played, currentPlayer);
        return true;
    }

    void handleCardEffect(Card played, Player& currentPlayer)
    {
        string value = played.getValue();

        if (value == "Draw_4")
        {
            Card topCard(deck.getTopDiscard());
            if (deck.getDiscardPileSize() >= 2)
            {
                colorBeforeWild = currentColor;
            }
            else
            {
                colorBeforeWild = "";
            }

            challengeAvailable = true;
            lastWildDraw4Player = currentPlayerIndex;

            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);

            for (int i = 0; i < 4; i++)
            {
                string card = deck.drawCard();
                if (!card.empty())
                {
                    nextPlayer.drawCard(card);
                }
            }
            updatePlayerAt(nextIndex, nextPlayer);
            currentPlayerIndex = getNextPlayerIndex();
        }
        else if (value == "Skip")
        {
            currentPlayerIndex = getNextPlayerIndex();
        }
        else if (value == "Reverse")
        {
            clockwise = !clockwise;
            if (totalPlayers == 2)
            {
                currentPlayerIndex = getNextPlayerIndex();
            }
        }
        else if (value == "Draw_2")
        {
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);

            for (int i = 0; i < 2; i++)
            {
                string card = deck.drawCard();
                if (!card.empty())
                {
                    nextPlayer.drawCard(card);
                }
            }
            updatePlayerAt(nextIndex, nextPlayer);
            currentPlayerIndex = getNextPlayerIndex();
        }
    }

    void nextTurn()
    {
        currentPlayerIndex = getNextPlayerIndex();
    }

    void setCurrentColor(string color)
    {
        currentColor = color;
    }

    void drawCardForCurrentPlayer()
    {
        Player currentPlayer = getPlayerAt(currentPlayerIndex);
        lastDrawnCard = deck.drawCard();

        if (lastDrawnCard.empty())
        {
            lastDrawnCard = "";
            drawnCardPlayable = false;
            return;
        }

        currentPlayer.drawCard(lastDrawnCard);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        Card drawnCard(lastDrawnCard);
        Card topCard(deck.getTopDiscard());
        drawnCardPlayable = drawnCard.canPlayOn(topCard, currentColor);
    }

    bool canPlayDrawnCard()
    {
        return drawnCardPlayable && !lastDrawnCard.empty();
    }

    void clearDrawnCard()
    {
        lastDrawnCard = "";
        drawnCardPlayable = false;
    }

    bool canChallenge()
    {
        return challengeAvailable;
    }

    void acceptWildDraw4()
    {
        challengeAvailable = false;
    }

    bool processChallenge(string& message)
    {
        if (!challengeAvailable)
        {
            message = "No challenge available!";
            return false;
        }

        challengeAvailable = false;

        Player challengedPlayer = getPlayerAt(lastWildDraw4Player);

        bool hadPlayableCard = false;

        for (int i = 0; i < challengedPlayer.getHandSize(); i++)
        {
            string cardName = challengedPlayer.hand.getCardAt(i);
            Card card(cardName);
            if (!card.isWild() && card.getColor() == colorBeforeWild)
            {
                hadPlayableCard = true;
                break;
            }
        }

        if (hadPlayableCard)
        {
            message = "Challenge successful! Player " + to_string(lastWildDraw4Player) + " draws 4 more as penalty!";

            for (int i = 0; i < 4; i++)
            {
                string card = deck.drawCard();
                if (!card.empty())
                {
                    challengedPlayer.drawCard(card);
                }
            }

            updatePlayerAt(lastWildDraw4Player, challengedPlayer);

            return true;
        }
        else
        {
            message = "Challenge failed! Player " + to_string(lastWildDraw4Player) + " played legally. You draw 2 more cards (6 total)!";

            Player currentPlayer = getPlayerAt(currentPlayerIndex);

            for (int i = 0; i < 2; i++)
            {
                string card = deck.drawCard();
                if (!card.empty())
                {
                    currentPlayer.drawCard(card);
                }
            }
            updatePlayerAt(currentPlayerIndex, currentPlayer);

            return false;
        }
    }
};

class Button
{
    float x, y, width, height;
    string text;
    Color normalColor;
    Color hoverColor;

public:
    Button() : x(0), y(0), width(0), height(0), text(""),
               normalColor(GREEN), hoverColor(YELLOW) {}

    Button(float px, float py, float w, float h, string t)
        : x(px), y(py), width(w), height(h), text(t),
          normalColor(GREEN), hoverColor(YELLOW) {}

    bool isHovered()
    {
        Vector2 mouse = GetMousePosition();
        return mouse.x >= x && mouse.x <= x + width &&
               mouse.y >= y && mouse.y <= y + height;
    }

    bool isClicked()
    {
        return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void draw()
    {
        Color col = isHovered() ? hoverColor : normalColor;
        DrawRectangle((int)x, (int)y, (int)width, (int)height, col);
        DrawRectangleLines((int)x, (int)y, (int)width, (int)height, BLACK);

        int textWidth = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), (int)(x + width/2 - textWidth/2),
                 (int)(y + height/2 - 10), 20, BLACK);
    }
};

class CardRect
{
    float x, y, width, height;
    string cardName;

public:
    CardRect() : x(0), y(0), width(0), height(0), cardName("") {}

    CardRect(float px, float py, float w, float h, string name)
        : x(px), y(py), width(w), height(h), cardName(name) {}

    bool contains(Vector2 point)
    {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }
};

class unoGUI
{
private:
    Game* game;
    int screenWidth;
    int screenHeight;
    bool playing;
    int currentScreen;
    Stack<string> textureNames;
    Stack<Texture2D> textures;

    Texture2D texLogo;
    Texture2D texPlayButton;
    Texture2D texCardBack;

    int selectedCardIndex;
    int hoveredCardIndex;
    string message;
    float messageTimer;
    bool waitingForColorChoice;
    string pendingWildCard;
    Rectangle playButton;

    Stack<CardRect> currentPlayerCardRects;

    void loadCardImages()
    {
        string colors[] = {"Red", "Blue", "Green", "Yellow"};
        string basePath = "E:/BSCS-Semester3/DSA/UNO/assets/cards/";
        for (int c = 0; c<4; c++)
        {
            for (int val = 0; val<=9; val++)
            {
                string key = colors[c] + "_" + to_string(val);
                string path = basePath + key + ".jpg";
                if (FileExists(path.c_str()))
                {
                    Texture2D tex = LoadTexture(path.c_str());
                    textures.push(tex);
                    textureNames.push(key);
                }
            }
        }
        string actions[] = {"Skip", "Reverse", "Draw_2"};
        for (int c = 0; c<4;c++)
        {
            for (int a = 0; a<3;a++)
            {
                string key = colors[c] + "_" + actions[a];
                string path = basePath + key + ".jpg";
                if (FileExists(path.c_str()))
                {
                    Texture2D tex = LoadTexture(path.c_str());
                    textures.push(tex);
                    textureNames.push(key);
                }
            }
        }

        string wildPath1 = basePath + "Wild.jpg";
        if (FileExists(wildPath1.c_str()))
        {
            Texture2D tex = LoadTexture(wildPath1.c_str());
            textures.push(tex);
            textureNames.push("Wild");
        }
        string wildPath2 = basePath + "Wild_Draw_4.jpg";
        if (FileExists(wildPath2.c_str()))
        {
            Texture2D tex = LoadTexture(wildPath2.c_str());
            textures.push(tex);
            textureNames.push("Wild_Draw_4");
        }
    }

    Texture2D getTexture(string name)
    {
        Stack<string> tempNames;
        Stack<Texture2D> tempTextures;
        Texture2D result = texCardBack;

        while (!textureNames.isEmpty())
        {
            string n = textureNames.pop();
            Texture2D t = textures.pop();

            if (n == name)
            {
                result = t;
            }

            tempNames.push(n);
            tempTextures.push(t);
        }

        while (!tempNames.isEmpty())
        {
            textureNames.push(tempNames.pop());
            textures.push(tempTextures.pop());
        }
        return result;
    }

    Color getColorFromString(string colorName)
    {
        if (colorName == "Red") return RED;
        if (colorName == "Blue") return BLUE;
        if (colorName == "Green") return GREEN;
        if (colorName == "Yellow") return YELLOW;
        return GRAY;
    }

    void drawCard(string cardName, float x, float y, float width, float height, bool highlight = false)
    {
        Texture2D cardTexture = getTexture(cardName);
        Rectangle source = {0, 0, (float)cardTexture.width, (float)cardTexture.height};
        Rectangle dest = {x, y, width, height};
        DrawTexturePro(cardTexture, source, dest, {0, 0}, 0, WHITE);

        if (highlight)
        {
            DrawRectangleLines((int)x - 2, (int)y - 2, (int)width + 4,
                             (int)height + 4, YELLOW);
        }
    }

    void drawCardBack(float x, float y, float width, float height)
    {
        if (texCardBack.id != 0)
        {
            Rectangle source = {0, 0, (float)texCardBack.width, (float)texCardBack.height};
            Rectangle dest = {x, y, width, height};
            DrawTexturePro(texCardBack, source, dest, {0, 0}, 0, WHITE);
        }
        else
        {
            DrawRectangle((int)x, (int)y, (int)width, (int)height, DARKBLUE);
            DrawRectangleLines((int)x, (int)y, (int)width, (int)height, BLACK);
            DrawText("UNO", (int)(x + width/2 - 20), (int)(y + height/2 - 10), 20, YELLOW);
        }
    }

public:
    unoGUI(Game* g, int width = 1400, int height = 900)
    {
        game = g;
        screenWidth = width;
        screenHeight = height;
        playing = true;
        currentScreen = 0;
        selectedCardIndex = -1;
        hoveredCardIndex = -1;
        messageTimer = 0.0f;
        message = "";
        waitingForColorChoice = false;
        pendingWildCard = "";

        InitWindow(width, height, "UNO - Raylib GUI");
        SetTargetFPS(60);
        texCardBack = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/back.jpg");
        texLogo = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/logo.png");
        texPlayButton = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/playbutton.png");

        loadCardImages();
    }

    ~unoGUI()
    {
        while (!textures.isEmpty())
        {
            UnloadTexture(textures.pop());
            textureNames.pop();
        }

        if (texCardBack.id != 0) UnloadTexture(texCardBack);
        CloseWindow();
    }

    void run()
    {
        while (playing && !WindowShouldClose())
        {
            update();
            draw();
        }
    }

    void update()
    {
        if (messageTimer > 0)
        {
            messageTimer -= GetFrameTime();
        }

        if (currentScreen == 0)
        {
            updateStartScreen();
        }
        else if (currentScreen == 1)
        {
            updateGameScreen();
        }
        else if (currentScreen == 2)
        {
            updateColorSelectionScreen();
        }
    }

    void draw()
    {
        BeginDrawing();
        ClearBackground((Color){10, 80, 30, 255});

        if (currentScreen == 0)
        {
            drawStartScreen();
        }
        else if (currentScreen == 1)
        {
            drawGameScreen();
        }
        else if (currentScreen == 2)
        {
            drawColorSelectionScreen();
        }
        else if (currentScreen == 3)
        {
            drawGameOverScreen();
        }

        EndDrawing();
    }

    void updateStartScreen()
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, playButton))
            {
                game->startGame();
                currentScreen = 1;
                message = "Game Started!";
                messageTimer = 2.0f;
            }
        }
    }

    void drawStartScreen()
    {
        ClearBackground((Color){20, 120, 40, 255});
        float scale = 0.3f;
        float logoW = texLogo.width * scale;
        float logoH = texLogo.height * scale;
        float logoX = screenWidth / 2.0f - logoW / 2.0f;
        float logoY = screenHeight * 0.15f;

        Rectangle src = {0, 0, (float)texLogo.width, (float)texLogo.height};
        Rectangle dest = {logoX, logoY, logoW, logoH};
        Vector2 origin = {0, 0};

        DrawTexturePro(texLogo, src, dest, origin, 0.0f, WHITE);

        float Btnscale = 0.5f;
        playButton.width = (float)texPlayButton.width * Btnscale;
        playButton.height = (float)texPlayButton.height * Btnscale;
        playButton.x = screenWidth / 2.0f - playButton.width / 2.0f + 20;
        playButton.y = screenHeight * 0.65f + 80;

        Color tint = WHITE;
        if (CheckCollisionPointRec(GetMousePosition(), playButton))
        {
            tint = YELLOW;
        }

        Rectangle btnsrc = {0, 0, (float)texPlayButton.width, (float)texPlayButton.height};
        Rectangle btndest = {playButton.x, playButton.y, playButton.width, playButton.height};
        Vector2 btnorigin = {0.0f, 0.0f};

        DrawTexturePro(texPlayButton, btnsrc, btndest, btnorigin, 0.0f, tint);
    }

    void updateGameScreen()
    {
        if (game->isGameOver())
        {
            currentScreen = 3;
            return;
        }

        if (waitingForColorChoice)
        {
            return;
        }

        Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());
        Vector2 mouse = GetMousePosition();
        bool hasDrawnCard = !game->getLastDrawnCard().empty();
        bool deckHasCards = game->canDrawFromDeck();

        if (!hasDrawnCard && !deckHasCards && !game->currentPlayerHasPlayableCard())
        {
            message = "Deck empty and no playable cards. Turn skipped.";
            messageTimer = 2.0f;
            game->nextTurn();
            selectedCardIndex = -1;
            return;
        }

        if (game->canChallenge())
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Rectangle challengeBtnRect = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 150), 200, 60};
                Rectangle acceptBtnRect = {(float)(screenWidth / 2 - 100), (float)(screenHeight / 2 - 50), 200, 60};

                if (CheckCollisionPointRec(mouse, challengeBtnRect))
                {
                    string msg;
                    bool success = game->processChallenge(msg);
                    message = msg;
                    messageTimer = 3.0f;
                }
                else if (CheckCollisionPointRec(mouse, acceptBtnRect))
                {
                    game->acceptWildDraw4();
                    message = "Wild Draw 4 accepted.";
                    messageTimer = 2.0f;
                }
            }
            return;
        }

        hoveredCardIndex = -1;
        int drawnCardIndex = hasDrawnCard ? currentPlayer.getHandSize() - 1 : -1;
        Stack<CardRect> tempRects = currentPlayerCardRects;
        int index = 0;

        while (!tempRects.isEmpty())
        {
            CardRect rect = tempRects.pop();
            int cardIdx = currentPlayer.getHandSize() - 1 - index;
            if (!hasDrawnCard || cardIdx == drawnCardIndex)
            {
                if (rect.contains(mouse))
                {
                    hoveredCardIndex = cardIdx;
                    break;
                }
            }
            index++;
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Stack<CardRect> tempRects2 = currentPlayerCardRects;
            int idx = 0;

            while (!tempRects2.isEmpty())
            {
                CardRect rect = tempRects2.pop();
                int cardIdx = currentPlayer.getHandSize() - 1 - idx;
                if (!hasDrawnCard || cardIdx == drawnCardIndex)
                {
                    if (rect.contains(mouse))
                    {
                        selectedCardIndex = cardIdx;
                        break;
                    }
                }
                idx++;
            }
        }

        if (!hasDrawnCard)
        {
            Button drawBtn(screenWidth - 150, screenHeight - 100, 120, 40, "DRAW");
            if (drawBtn.isClicked())
            {
                game->drawCardForCurrentPlayer();

                if (game->getLastDrawnCard().empty())
                {
                    message = "Deck is empty! Cannot draw. Turn skipped.";
                    messageTimer = 2.0f;
                    selectedCardIndex = -1;
                }
                else if (game->canPlayDrawnCard())
                {
                    message = "Card drawn and is playable!";
                    messageTimer = 2.0f;
                    selectedCardIndex = currentPlayer.getHandSize() - 1;
                }
                else
                {
                    message = "Card drawn but not playable. Turn skipped.";
                    messageTimer = 2.0f;
                    game->clearDrawnCard();
                    game->nextTurn();
                    selectedCardIndex = -1;
                }
            }

            if (selectedCardIndex >= 0)
            {
                Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
                if (playBtn.isClicked())
                {
                    string cardToPlay = currentPlayer.hand.getCardAt(selectedCardIndex);
                    Card card(cardToPlay);

                    if (card.isWild())
                    {
                        waitingForColorChoice = true;
                        pendingWildCard = cardToPlay;
                        currentScreen = 2;
                    }
                    else
                    {
                        string msg;
                        if (game->playCard(selectedCardIndex, msg))
                        {
                            message = msg;
                            messageTimer = 2.0f;
                            if (!game->isGameOver())
                            {
                                game->nextTurn();
                            }
                        }
                        else
                        {
                            message = msg;
                            messageTimer = 2.0f;
                        }
                        selectedCardIndex = -1;
                    }
                }
            }
        }
        else
        {
            Button skipBtn(screenWidth - 150, screenHeight - 100, 120, 40, "SKIP");
            if (skipBtn.isClicked())
            {
                game->clearDrawnCard();
                game->nextTurn();
                message = "Turn skipped. Next player's turn.";
                messageTimer = 2.0f;
                selectedCardIndex = -1;
            }

            if (selectedCardIndex == drawnCardIndex)
            {
                Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
                if (playBtn.isClicked())
                {
                    string cardToPlay = currentPlayer.hand.getCardAt(selectedCardIndex);
                    Card card(cardToPlay);

                    if (card.isWild())
                    {
                        waitingForColorChoice = true;
                        pendingWildCard = cardToPlay;
                        currentScreen = 2;
                    }
                    else
                    {
                        string msg;
                        if (game->playCard(selectedCardIndex, msg))
                        {
                            message = msg;
                            messageTimer = 2.0f;
                            game->clearDrawnCard();
                            if (!game->isGameOver())
                            {
                                game->nextTurn();
                            }
                        }
                        else
                        {
                            message = msg;
                            messageTimer = 2.0f;
                        }
                        selectedCardIndex = -1;
                    }
                }
            }
        }
    }

    void drawGameScreen()
    {
        DrawText("Color:", screenWidth - 300, 20, 20, WHITE);
        Color currentCol = getColorFromString(game->getCurrentColor());
        DrawRectangle(screenWidth - 200, 15, 40, 30, currentCol);
        DrawRectangleLines(screenWidth - 200, 15, 40, 30, BLACK);
        string dir = game->getClockwise() ? "Clockwise" : "Counter-CW";
        DrawText(dir.c_str(), screenWidth - 140, 20, 20, WHITE);

        Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());
        string current_player = "Current Player: " + to_string(currentPlayer.id);
        DrawText(current_player.c_str(), screenWidth - 600, 20, 20, WHITE);

        drawCenterArea();
        drawOtherPlayersHands();
        drawCurrentPlayerHand();

        if (game->canChallenge())
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 180});
            DrawText("Wild Draw 4 played!", screenWidth / 2 - 200, screenHeight / 2 - 300, 40, YELLOW);
            DrawText("Do you want to challenge?", screenWidth / 2 - 180, screenHeight / 2 - 250, 30, WHITE);
            DrawText("Challenge if you think they had a playable card", screenWidth / 2 - 250, screenHeight / 2 - 200, 20, LIGHTGRAY);

            Button challengeBtn(screenWidth / 2 - 100, screenHeight / 2 - 150, 200, 60, "CHALLENGE");
            challengeBtn.draw();

            Button acceptBtn(screenWidth / 2 - 100, screenHeight / 2 - 50, 200, 60, "ACCEPT");
            acceptBtn.draw();

            if (messageTimer > 0)
            {
                int msgWidth = 500;
                int msgHeight = 100;
                int msgX = 60;
                int msgY = 60;

                DrawRectangle(msgX, msgY, msgWidth, msgHeight, (Color){0, 0, 0, 200});
                DrawRectangleLines(msgX, msgY, msgWidth, msgHeight, YELLOW);
                DrawText(message.c_str(), msgX + 10, msgY + 20, 22, YELLOW);
            }
            return;
        }

        bool hasDrawnCard = !game->getLastDrawnCard().empty();
        bool deckHasCards = game->canDrawFromDeck();

        if (!hasDrawnCard)
        {
            if (deckHasCards)
            {
                Button drawBtn(screenWidth - 150, screenHeight - 100, 120, 40, "DRAW");
                drawBtn.draw();
            }
            else
            {
                DrawRectangle(screenWidth - 180, screenHeight - 110, 160, 60, (Color){200, 50, 50, 200});
                DrawRectangleLines(screenWidth - 180, screenHeight - 110, 160, 60, YELLOW);
                DrawText("Deck Empty!", screenWidth - 165, screenHeight - 100, 18, WHITE);
                DrawText("Play a card!", screenWidth - 165, screenHeight - 75, 16, YELLOW);
            }

            if (selectedCardIndex >= 0)
            {
                Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
                playBtn.draw();
            }
        }
        else
        {
            Button skipBtn(screenWidth - 150, screenHeight - 100, 120, 40, "SKIP");
            skipBtn.draw();

            int drawnCardIndex = currentPlayer.getHandSize() - 1;
            if (selectedCardIndex == drawnCardIndex)
            {
                Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
                playBtn.draw();
            }
        }

        if (messageTimer > 0)
        {
            int msgWidth = 500;
            int msgHeight = 100;
            int msgX = 60;
            int msgY = 60;

            DrawRectangle(msgX, msgY, msgWidth, msgHeight, (Color){0, 0, 0, 200});
            DrawRectangleLines(msgX, msgY, msgWidth, msgHeight, YELLOW);
            DrawText(message.c_str(), msgX + 10, msgY + 20, 24, YELLOW);
        }
    }

    void drawCenterArea()
    {
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        int deckX = centerX - 200;
        int deckY = centerY - 150;
        drawCardBack(deckX, deckY, 160, 220);

        int discardX = centerX + 100;
        int discardY = centerY - 150;
        string topCard = game->getTopDiscard();

        if (!topCard.empty())
        {
            drawCard(topCard, discardX, discardY, 160, 220, false);
        }
        else
        {
            DrawRectangle(discardX, discardY, 100, 140, GRAY);
            DrawRectangleLines(discardX, discardY, 100, 140, BLACK);
        }
    }

    void drawOtherPlayersHands()
    {
        int currentIdx = game->getCurrentPlayerIndex();
        int totalPlayers = game->getTotalPlayers();

        for (int i = 0; i < totalPlayers; i++)
        {
            if (i == currentIdx)
                continue;

            Player p = game->getPlayerAtPublic(i);
            int x, y;
            int relativePos = (i - currentIdx + totalPlayers) % totalPlayers;

            if (relativePos == 1)
            {
                x = screenWidth - 190;
                y = screenHeight / 2 - 90;
            }
            else if (relativePos == 2)
            {
                x = screenWidth / 2 - 70;
                y = 80;
            }
            else if (relativePos == 3)
            {
                x = 25;
                y = screenHeight / 2 - 90;
            }
            else
            {
                continue;
            }

            string label = "Player " + to_string(p.id);
            DrawText(label.c_str(), x, y - 25, 25, WHITE);
            string count = to_string(p.getHandSize()) + " cards";
            DrawText(count.c_str(), x, y, 20, YELLOW);

            int cardsToDraw = (p.getHandSize() > 5) ? 5 : p.getHandSize();
            for (int j = 0; j < cardsToDraw; j++)
            {
                drawCardBack(x + j * 20, y + 25, 88, 140);
            }
        }
    }

    void drawCurrentPlayerHand()
    {
        Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());
        currentPlayerCardRects.Clear();

        if (currentPlayer.getHandSize() == 0)
            return;

        bool hasDrawnCard = !game->getLastDrawnCard().empty();
        int drawnCardIndex = hasDrawnCard ? currentPlayer.getHandSize() - 1 : -1;

        int cardWidth, cardHeight, spacing;
        int handSize = currentPlayer.getHandSize();
        bool twoRows = false;

        if (handSize <= 7)
        {
            cardWidth = 160;
            cardHeight = 220;
            spacing = 120;
        }
        else if (handSize <= 10)
        {
            cardWidth = 120;
            cardHeight = 165;
            spacing = 90;
        }
        else if (handSize <= 15)
        {
            cardWidth = 90;
            cardHeight = 124;
            spacing = 70;
        }
        else if (handSize <= 20)
        {
            cardWidth = 70;
            cardHeight = 96;
            spacing = 55;
        }
        else
        {
            twoRows = true;
            cardWidth = 70;
            cardHeight = 96;
            spacing = 55;
        }

        string label = "Your Hand (Player " + to_string(currentPlayer.id) + ") - " +
                      to_string(currentPlayer.getHandSize()) + " cards";

        if (!twoRows)
        {
            int totalWidth = handSize * spacing;
            int startX = (screenWidth - totalWidth) / 2;
            int startY = screenHeight - cardHeight - 40;

            DrawText(label.c_str(), startX, startY - 30, 18, WHITE);

            for (int i = 0; i < currentPlayer.getHandSize(); i++)
            {
                string cardName = currentPlayer.hand.getCardAt(i);
                int cardX = startX + i * spacing;
                int cardY = startY;

                bool isDrawnCard = (i == drawnCardIndex);
                bool canInteract = !hasDrawnCard || isDrawnCard;
                bool isHighlighted = canInteract && (i == hoveredCardIndex || i == selectedCardIndex);

                if (isHighlighted)
                {
                    cardY -= 15;
                }

                drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

                if (hasDrawnCard && !isDrawnCard)
                {
                    DrawRectangle(cardX, cardY, cardWidth, cardHeight, (Color){0, 0, 0, 150});
                }

                if (isDrawnCard)
                {
                    DrawRectangleLines(cardX - 3, cardY - 3, cardWidth + 6, cardHeight + 6, GOLD);
                    DrawRectangleLines(cardX - 2, cardY - 2, cardWidth + 4, cardHeight + 4, GOLD);
                }

                currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

                string indexStr = to_string(i);
                DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
            }
        }
        else
        {
            int cardsPerRow = (handSize + 1) / 2;
            int bottomRowCards = handSize - cardsPerRow;
            int topRowWidth = cardsPerRow * spacing;
            int topStartX = (screenWidth - topRowWidth) / 2;
            int topStartY = screenHeight - (cardHeight * 2) - 60;
            int bottomRowWidth = bottomRowCards * spacing;
            int bottomStartX = (screenWidth - bottomRowWidth) / 2;
            int bottomStartY = screenHeight - cardHeight - 40;

            DrawText(label.c_str(), screenWidth / 2 - 150, topStartY - 30, 18, WHITE);

            for (int i = 0; i < cardsPerRow; i++)
            {
                string cardName = currentPlayer.hand.getCardAt(i);
                int cardX = topStartX + i * spacing;
                int cardY = topStartY;

                bool isDrawnCard = (i == drawnCardIndex);
                bool canInteract = !hasDrawnCard || isDrawnCard;
                bool isHighlighted = canInteract && (i == hoveredCardIndex || i == selectedCardIndex);

                if (isHighlighted)
                {
                    cardY -= 15;
                }

                drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

                if (hasDrawnCard && !isDrawnCard)
                {
                    DrawRectangle(cardX, cardY, cardWidth, cardHeight, (Color){0, 0, 0, 150});
                }

                if (isDrawnCard)
                {
                    DrawRectangleLines(cardX - 3, cardY - 3, cardWidth + 6, cardHeight + 6, GOLD);
                    DrawRectangleLines(cardX - 2, cardY - 2, cardWidth + 4, cardHeight + 4, GOLD);
                }

                currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

                string indexStr = to_string(i);
                DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
            }

            for (int i = 0; i < bottomRowCards; i++)
            {
                int actualIndex = cardsPerRow + i;
                string cardName = currentPlayer.hand.getCardAt(actualIndex);
                int cardX = bottomStartX + i * spacing;
                int cardY = bottomStartY;

                bool isDrawnCard = (actualIndex == drawnCardIndex);
                bool canInteract = !hasDrawnCard || isDrawnCard;
                bool isHighlighted = canInteract && (actualIndex == hoveredCardIndex || actualIndex == selectedCardIndex);

                if (isHighlighted)
                {
                    cardY -= 15;
                }

                drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

                if (hasDrawnCard && !isDrawnCard)
                {
                    DrawRectangle(cardX, cardY, cardWidth, cardHeight, (Color){0, 0, 0, 150});
                }

                if (isDrawnCard)
                {
                    DrawRectangleLines(cardX - 3, cardY - 3, cardWidth + 6, cardHeight + 6, GOLD);
                    DrawRectangleLines(cardX - 2, cardY - 2, cardWidth + 4, cardHeight + 4, GOLD);
                }

                currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

                string indexStr = to_string(actualIndex);
                DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
            }
        }
    }

    void updateColorSelectionScreen()
    {
        Button redBtn(screenWidth/2 - 250, screenHeight/2 - 50, 100, 100, "RED");
        Button blueBtn(screenWidth/2 - 100, screenHeight/2 - 50, 100, 100, "BLUE");
        Button greenBtn(screenWidth/2 + 50, screenHeight/2 - 50, 100, 100, "GREEN");
        Button yellowBtn(screenWidth/2 + 200, screenHeight/2 - 50, 100, 100, "YELLOW");

        if (redBtn.isClicked())
        {
            chooseColor("Red");
        }
        else if (blueBtn.isClicked())
        {
            chooseColor("Blue");
        }
        else if (greenBtn.isClicked())
        {
            chooseColor("Green");
        }
        else if (yellowBtn.isClicked())
        {
            chooseColor("Yellow");
        }
    }

    void drawColorSelectionScreen()
    {
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});
        DrawText("Choose a Color for Wild Card", screenWidth/2 - 200, screenHeight/2 - 150, 30, WHITE);

        int btnY = screenHeight/2 - 50;
        int btnSize = 100;

        DrawRectangle(screenWidth/2 - 250, btnY, btnSize, btnSize, RED);
        DrawRectangleLines(screenWidth/2 - 250, btnY, btnSize, btnSize, BLACK);
        DrawText("RED", screenWidth/2 - 230, btnY + 40, 20, WHITE);

        DrawRectangle(screenWidth/2 - 100, btnY, btnSize, btnSize, BLUE);
        DrawRectangleLines(screenWidth/2 - 100, btnY, btnSize, btnSize, BLACK);
        DrawText("BLUE", screenWidth/2 - 85, btnY + 40, 20, WHITE);

        DrawRectangle(screenWidth/2 + 50, btnY, btnSize, btnSize, GREEN);
        DrawRectangleLines(screenWidth/2 + 50, btnY, btnSize, btnSize, BLACK);
        DrawText("GREEN", screenWidth/2 + 60, btnY + 40, 20, WHITE);

        DrawRectangle(screenWidth/2 + 200, btnY, btnSize, btnSize, YELLOW);
        DrawRectangleLines(screenWidth/2 + 200, btnY, btnSize, btnSize, BLACK);
        DrawText("YELLOW", screenWidth/2 + 205, btnY + 40, 20, BLACK);
    }

    void chooseColor(string color)
    {
        game->setCurrentColor(color);
        string msg;
        if (game->playCard(selectedCardIndex, msg))
        {
            message = "Wild card played! Color set to " + color;
            messageTimer = 2.0f;
            if (!game->isGameOver())
            {
                game->nextTurn();
            }
        }

        waitingForColorChoice = false;
        pendingWildCard = "";
        selectedCardIndex = -1;
        currentScreen = 1;
    }

    void drawGameOverScreen()
    {
        DrawText("GAME OVER!", screenWidth/2 - 150, screenHeight/3, 50, YELLOW);

        int winner = game->getGameWinner();
        string winnerText = "Player " + to_string(winner) + " WINS THE GAME!";
        DrawText(winnerText.c_str(), screenWidth/2 - 200, screenHeight/2 - 100, 35, GREEN);

        DrawText("Final Scores:", screenWidth/2 - 80, screenHeight/2 - 30, 22, WHITE);
        for (int i = 0; i < game->getTotalPlayers(); i++)
        {
            Player p = game->getPlayerAtPublic(i);
            string score = "Player " + to_string(i) + ": " + to_string(p.getScore()) + " cards";
            Color color = (i == winner) ? GREEN : RED;
            DrawText(score.c_str(), screenWidth/2 - 100, screenHeight/2 + i * 30, 20, color);
        }
    }
};

int main()
{
    srand(time(0));
    Game game;
    unoGUI gui(&game, 1400, 900);
    gui.run();
    return 0;
}
