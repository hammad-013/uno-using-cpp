#include <iostream>
#include <string>
#include <cstdlib>
<<<<<<< HEAD
=======
#include <vector>
#include <cassert>
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
#include <cstring>
#include <ctime>
#include <raylib.h>
using namespace std;

template <typename T>
class Stack
{
private:
    struct Node //change this
    {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}

    Stack(const Stack& other) : topNode(nullptr), count(0)
    {
        if (other.topNode == nullptr) return;

        T* tempArray = new T[other.count];
        Node* current = other.topNode;
        int index = 0;
        while (current != nullptr)
        {
            tempArray[index++] = current->data;
            current = current->next;
        }
        for (int i = index - 1; i >= 0; i--)
        {
            push(tempArray[i]);
        }

        delete[] tempArray;
    }

    Stack& operator=(const Stack& other)
    {
        if (this != &other)
        {
            clear();
            if (other.topNode != nullptr)
            {
                T* tempArray = new T[other.count];

                Node* current = other.topNode;
                int index = 0;
                while (current != nullptr)
                {
                    tempArray[index++] = current->data;
                    current = current->next;
                }

                for (int i = index - 1; i >= 0; i--)
                {
                    push(tempArray[i]);
                }

                delete[] tempArray;
            }
        }
        return *this;
    }

    ~Stack()
    {
        clear();
    }

    void push(T value)
    {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    T pop()
    {
        if (isEmpty())
        {
            return T();
        }
        Node* temp = topNode;
        T value = topNode->data;
        topNode = topNode->next;
        delete temp;
        count--;
        return value;
    }

    T peek() const
    {
        if (isEmpty())
        {
            return T();
        }
        return topNode->data;
    }

    bool isEmpty() const
    {
        return topNode == nullptr;
    }

    int size() const
    {
        return count;
    }

    void clear()
    {
        while (!isEmpty())
        {
            pop();
        }
    }

    void shuffle()
    {
        if (isEmpty() || size() == 1)
            return;

        int n = size();
        Stack<T> temp1;
        while (!isEmpty())
        {
            temp1.push(pop());
        }
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);

            Stack<T> temp2;
            T selectedItem;
            for (int k = 0; k <= i; k++)
            {
                T item = temp1.pop();
                if (k == j)
                {
                    selectedItem = item;
                }
                else
                {
                    temp2.push(item);
                }
            }

            push(selectedItem);
            while (!temp2.isEmpty())
            {
                temp1.push(temp2.pop());
            }
        }
        if (!temp1.isEmpty())
        {
            push(temp1.pop());
        }
    }
};



// Card rectangle structure
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
    int currentScreen;  // 0 = start, 1 = game, 2 = wild color selection, 3 = game over

    // Texture management using stacks
    Stack<string> textureNames;
    Stack<Texture2D> textures;

    Texture2D texLogo;
    Texture2D texPlayButton;
    Texture2D texCardBack;

<<<<<<< HEAD
    // Game state
    int selectedCardIndex;
    int hoveredCardIndex;
    string message;
    float messageTimer;
    bool waitingForColorChoice;
    string pendingWildCard;
    Rectangle playButton;

    // Card positions using stack
    Stack<CardRect> currentPlayerCardRects;

     void loadCardImages()
      {
    string colors[] = {"Red", "Blue", "Green", "Yellow"};
    string basePath = "E:/BSCS-Semester3/DSA/UNO/assets/cards/";

    // Number cards
    for (int c = 0; c < 4; c++) {
        for (int val = 0; val <= 9; val++) {
            string key = colors[c] + "_" + to_string(val);
            string path = basePath + key + ".jpg";
            if (FileExists(path.c_str()))
            {
                Texture2D tex = LoadTexture(path.c_str());
                textures.push(tex);
                textureNames.push(key);  // ADD THIS LINE
            }
        }
    }

    // Action cards
    string actions[] = {"Skip", "Reverse", "Draw_2"};
    for (int c = 0; c < 4; c++) {
        for (int a = 0; a < 3; a++) {
            string key = colors[c] + "_" + actions[a];
            string path = basePath + key + ".jpg";
            if (FileExists(path.c_str()))
            {
                Texture2D tex = LoadTexture(path.c_str());
                textures.push(tex);
                textureNames.push(key);  // ADD THIS LINE
            }
        }
    }

    // Wild cards
    string wildPath1 = basePath + "Wild.jpg";
    if (FileExists(wildPath1.c_str())) {
        Texture2D tex = LoadTexture(wildPath1.c_str());
        textures.push(tex);
        textureNames.push("Wild");  // ADD THIS LINE
    }

    string wildPath2 = basePath + "Wild_Draw_4.jpg";
    if (FileExists(wildPath2.c_str())) {
        Texture2D tex = LoadTexture(wildPath2.c_str());
        textures.push(tex);
        textureNames.push("Wild_Draw_4");  // ADD THIS LINE
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
=======
    bool isEmpty()
    {
        return cards.isEmpty();
    }
    string getCardAt(int index)
    {
        if (index < 0 || index >= size())
            return "";

        Stack<string> temp;
        string result = "";

        int n = size();
        int targetFromTop = n-1-index;

        int currentFromTop = 0;
        while (!cards.isEmpty())
        {
            string card = cards.pop();

            if (currentFromTop == targetFromTop)
                result = card;

            temp.push(card);
            currentFromTop++;
        }

        while (!temp.isEmpty())
            cards.push(temp.pop());
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2

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
        // Get the texture for this card using the helper function
        Texture2D cardTexture = getTexture(cardName);

        // Draw the card texture
        Rectangle source = {0, 0, (float)cardTexture.width, (float)cardTexture.height};
        Rectangle dest = {x, y, width, height};
        DrawTexturePro(cardTexture, source, dest, {0, 0}, 0, WHITE);

        // Draw highlight border if needed
        if (highlight)
        {
            DrawRectangleLines((int)x - 2, (int)y - 2, (int)width + 4,
                             (int)height + 4, YELLOW);
        }
    }

    void drawCardBack(float x, float y, float width, float height)
    {
        // Draw the card back texture if loaded
        if (texCardBack.id != 0)  // Check if texture is loaded
        {
            Rectangle source = {0, 0, (float)texCardBack.width, (float)texCardBack.height};
            Rectangle dest = {x, y, width, height};
            DrawTexturePro(texCardBack, source, dest, {0, 0}, 0, WHITE);
        }
        else
        {
            // Fallback to drawing a rectangle if texture not loaded
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

        // Load basic textures (if they exist, otherwise use colored rectangles)
        string cardBackPath = "E:/BSCS-Semester3/DSA/UNO/assets/cards/back.jpg";
        texCardBack = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/back.jpg");
        loadCardImages();
    }

    ~unoGUI()
    {
        // Unload all textures
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
        else if (currentScreen == 3)
        {
            updateGameOverScreen();
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
        texLogo = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/logo.png");
        texPlayButton = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/playbutton.png");


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
        playButton.x = screenWidth / 2.0f - playButton.width / 2.0f +20 ;
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
            return;  // Don't process game input while choosing color
        }

        Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());
        Vector2 mouse = GetMousePosition();
        // Update hovered card
        hoveredCardIndex = -1;
        Stack<CardRect> tempRects = currentPlayerCardRects;
        int index = 0;

        while (!tempRects.isEmpty())
        {
            CardRect rect = tempRects.pop();
            if (rect.contains(mouse))
            {
                hoveredCardIndex = currentPlayer.getHandSize() - 1 - index;
                break;
            }
            index++;
        }

        // Handle card selection
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Stack<CardRect> tempRects2 = currentPlayerCardRects;
            int idx = 0;

            while (!tempRects2.isEmpty())
            {
<<<<<<< HEAD
                CardRect rect = tempRects2.pop();
                if (rect.contains(mouse))
=======
                canPlay = true;
            }
            temp.push(cardStr);
        }
        while (!temp.isEmpty())
        {
            cards.push(temp.pop());
        }

        return canPlay;
    }
};

class Player
{
public:
    int id;
    Hand hand;

    Player() : id(0) {}

    Player(int playerId) : id(playerId) {}

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
        return hand.size();
    }

    void showHand()
    {
        cout << id << "'s hand:" << endl;
        hand.display();
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
        drawPile.clear();
        discardPile.clear();

        string colors[4] = {"Red", "Blue", "Green", "Yellow"};

        for (int c = 0; c < 4; c++)
        {
            string color = colors[c];
            drawPile.push(color + "_0");

            for (int num = 1; num <= 9; num++)
            {
                string numStr = "";
                int temp = num;
                while (temp > 0)
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
                {
                    selectedCardIndex = currentPlayer.getHandSize() - 1 - idx;
                    break;
                }
                idx++;
            }
        }


        Card topCard(Card(game->getTopDiscard()));
        Button drawBtn(screenWidth - 150, screenHeight - 100, 120, 40, "DRAW");
        if (drawBtn.isClicked()) {
            game->drawCardForCurrentPlayer();
            game->nextTurn();
            message = "Drew a card. Next player's turn.";
            messageTimer = 2.0f;
            selectedCardIndex = -1;
        }
        // Play button
        Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
        if (playBtn.isClicked() && selectedCardIndex >= 0)
        {
            string cardToPlay = currentPlayer.hand.getCardAt(selectedCardIndex);
            Card card(cardToPlay);

<<<<<<< HEAD
            if (card.isWild())
=======
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
        if (discardPile.size() <= 1)
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
        return drawPile.size();
    }

    int getDiscardPileSize()
    {
        return discardPile.size();
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
    bool calledUno;
    int skipNext;

    Player getPlayerAt(int index)
    {
        Stack<Player> temp;
        Player result;
        int count = 0;

        while (!players.isEmpty())
        {
            Player p = players.pop();
            if (count == index)
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
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

    void drawGameScreen()
    {


        // Draw current color indicator
        DrawText("Color:", screenWidth - 300, 20, 20, WHITE);
        Color currentCol = getColorFromString(game->getCurrentColor());
        DrawRectangle(screenWidth - 200, 15, 40, 30, currentCol);
        DrawRectangleLines(screenWidth - 200, 15, 40, 30, BLACK);

        // Draw direction
        string dir = game->getClockwise() ? "Clockwise" : "Counter-CW";
        DrawText(dir.c_str(), screenWidth - 140, 20, 20, WHITE);
        Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());
        string current_player = "Current Player: " + to_string(currentPlayer.id);
        DrawText(current_player.c_str(), screenWidth - 600, 20, 20, WHITE);
        // Draw center area (deck and discard pile)
        drawCenterArea();

        // Draw other players' hands
        drawOtherPlayersHands();

        // Draw current player's hand
        drawCurrentPlayerHand();

        // Draw buttons
        Button drawBtn(screenWidth - 150, screenHeight - 100, 120, 40, "DRAW");
        drawBtn.draw();

        if (selectedCardIndex >= 0)
        {
            Button playBtn(screenWidth - 150, screenHeight - 160, 120, 40, "PLAY");
            playBtn.draw();
        }
        if (messageTimer > 0)
        {
            int msgWidth = 400;
            int msgHeight = 60;
            int msgX = 100;
            int msgY = 100;

            DrawRectangle(msgX, msgY, msgWidth, msgHeight, (Color){0, 0, 0, 200});
            DrawRectangleLines(msgX, msgY, msgWidth, msgHeight, YELLOW);
            DrawText(message.c_str(), msgX + 10, msgY + 20, 18, YELLOW);
        }
    }

    void drawCenterArea()
    {
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;

        // Draw deck (draw pile)
        int deckX = centerX - 200;
        int deckY = centerY - 150;
        drawCardBack(deckX, deckY, 160, 220);

        string deckCount = to_string(game->getDrawPileSize());
        DrawText(deckCount.c_str(), deckX + 40, deckY + 150, 20, WHITE);
        // Draw discard pile
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

<<<<<<< HEAD

    void drawOtherPlayersHands()
    {
        int currentIdx = game->getCurrentPlayerIndex();
        int totalPlayers = game->getTotalPlayers();

        // Draw other 3 players' hands around the table
=======
public:
    Game() : currentPlayerIndex(0), totalPlayers(4), clockwise(true),
             currentColor(""), gameOver(false), calledUno(false), skipNext(0) {}
    int getTotalPlayers() const { return totalPlayers; }
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    bool getClockwise() const { return clockwise; }
    string getCurrentColor() const { return currentColor; }
    string getTopDiscard() const { return deck.getTopDiscard(); }

    Player getPlayerAtPublic(int index) { return getPlayerAt(index); }

    bool isGameOver() const { return gameOver; }

    void startGame()
    {
        deck.createDeck();
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
        for (int i = 0; i < totalPlayers; i++)
        {
            if (i == currentIdx)
                continue;  // Skip current player

            Player p = game->getPlayerAtPublic(i);

            int x, y;

            // Calculate relative position from current player
            int relativePos = (i - currentIdx + totalPlayers) % totalPlayers;

            if (relativePos == 1)  // Next player (Right)
            {
                x = screenWidth - 190;
                y = screenHeight / 2 - 90;
            }
<<<<<<< HEAD
            else if (relativePos == 2)  // Opposite player (Top)
=======
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

        cout << "\nStart card: " << startCard << endl;
        cout << "Current color: " << currentColor << endl;


        if (start.getValue() == "Skip")
        {
            cout << "Start card is Skip! First player skipped!" << endl;
            currentPlayerIndex = 1;
        }
        else if (start.getValue() == "Reverse")
        {
            cout << "Start card is Reverse! Playing counterclockwise!" << endl;
            clockwise = false;
        }
        else if (start.getValue() == "Draw_2")
        {
            cout << "Start card is Draw 2! First player draws 2 cards!" << endl;
            Player p = getPlayerAt(0);
            p.drawCard(deck.drawCard());
            p.drawCard(deck.drawCard());
            updatePlayerAt(0, p);
            currentPlayerIndex = 1;
        }

        gameOver = false;
    }

    void playTurn()
    {
        if (gameOver)
        {
            return;
        }

        Player currentPlayer = getPlayerAt(currentPlayerIndex);

        cout << "\n========================================" << endl;
        cout << "Direction: " << (clockwise ? "Clockwise" : "Counterclockwise") << endl;
        cout << "Top card: " << deck.getTopDiscard() << endl;
        cout << "Current color: " << currentColor << endl;
        cout << "Cards in hand: " << currentPlayer.getHandSize() << endl;
        cout << "========================================" << endl;

        currentPlayer.showHand();

        Card topCard(deck.getTopDiscard());
        bool hasPlayable = currentPlayer.hand.hasPlayableCard(topCard, currentColor);

        if (!hasPlayable)
        {
            cout << "\nNo playable cards! Drawing a card..." << endl;
            string drawnCard = deck.drawCard();
            currentPlayer.drawCard(drawnCard);
            cout << "Drew: " << drawnCard << endl;

            Card drawn(drawnCard);
            if (drawn.canPlayOn(topCard, currentColor))
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
            {
                x = screenWidth / 2 - 70;
                y = 80;
            }
            else if (relativePos == 3)  // Previous player (Left)
            {
                x = 25;
                y = screenHeight / 2 - 90;
            }
            else
            {
                continue;  // Safety check
            }

            // Draw player label
            string label = "Player " + to_string(p.id);
            DrawText(label.c_str(), x, y - 25, 25, WHITE);

            // Draw card count
            string count = to_string(p.getHandSize()) + " cards";
            DrawText(count.c_str(), x, y, 20, YELLOW);

<<<<<<< HEAD
            // Draw card backs
            int cardsToDraw = (p.getHandSize() > 5) ? 5 : p.getHandSize();
            for (int j = 0; j < cardsToDraw; j++)
            {
                drawCardBack(x + j * 20, y + 25,88, 140);
            }
=======
        if (choice == -1)
        {
            string drawnCard = deck.drawCard();
            currentPlayer.drawCard(drawnCard);
            cout << "Drew: " << drawnCard << endl;
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            nextTurn();
            return;
        }

        string playedCard = currentPlayer.playCard(choice);

        if (playedCard == "")
        {
            cout << "Invalid card index!" << endl;
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return;
        }

        Card played(playedCard);

        if (!played.canPlayOn(topCard, currentColor))
        {
            cout << "Cannot play this card! It doesn't match color or value!" << endl;
            currentPlayer.hand.addCard(playedCard);
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return;
        }
        deck.discard(playedCard);
        cout << "\n" << currentPlayer.id << " played: " << playedCard << endl;
        if (currentPlayer.getHandSize() == 1)
        {
            cout << "\n*** " << currentPlayer.id << " has UNO! (1 card left) ***" << endl;
        }

        handleCardEffect(played, currentPlayer);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        checkWin(currentPlayer);
    }

    void handleCardEffect(Card played, Player& currentPlayer)
    {
        string value = played.getValue();

        if (played.isWild())
        {
            cout << "\nChoose color for Wild card:" << endl;
            cout << "1. Red" << endl;
            cout << "2. Blue" << endl;
            cout << "3. Green" << endl;
            cout << "4. Yellow" << endl;
            cout << "Choice: ";
            int colorChoice;
            cin >> colorChoice;

            if (colorChoice == 1) currentColor = "Red";
            else if (colorChoice == 2) currentColor = "Blue";
            else if (colorChoice == 3) currentColor = "Green";
            else if (colorChoice == 4) currentColor = "Yellow";
            else currentColor = "Red";

            cout << "New color: " << currentColor << endl;

            if (value == "Draw_4")
            {
                int nextIndex = getNextPlayerIndex();
                Player nextPlayer = getPlayerAt(nextIndex);
                cout << nextPlayer.id << " draws 4 cards and loses turn!" << endl;
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2

            // Highlight if it's their turn (though current player is already highlighted elsewhere)
            if (i == currentIdx)
            {
                DrawRectangleLines(x - 5, y - 30, 180, 180, YELLOW);
            }
        }
    }
    void drawCurrentPlayerHand()
{
    Player currentPlayer = game->getPlayerAtPublic(game->getCurrentPlayerIndex());

    // Clear previous card rectangles
    currentPlayerCardRects.clear();

    if (currentPlayer.getHandSize() == 0)
        return;

    // Dynamic card sizing based on hand size
    int cardWidth, cardHeight, spacing;
    int handSize = currentPlayer.getHandSize();
    bool twoRows = false;

    if (handSize <= 7)
    {
        // Normal size for 7 or fewer cards
        cardWidth = 160;
        cardHeight = 220;
        spacing = 120;
    }
    else if (handSize <= 10)
    {
        // Slightly smaller for 8-10 cards
        cardWidth = 120;
        cardHeight = 165;
        spacing = 90;
    }
    else if (handSize <= 15)
    {
        // Smaller for 11-15 cards
        cardWidth = 90;
        cardHeight = 124;
        spacing = 70;
    }
    else if (handSize <= 20)
    {
        // Very small for 16-20 cards
        cardWidth = 70;
        cardHeight = 96;
        spacing = 55;
    }
    else
    {
        // Two rows for 21+ cards
        twoRows = true;
        cardWidth = 70;
        cardHeight = 96;
        spacing = 55;
    }

    // Draw label
    string label = "Your Hand (Player " + to_string(currentPlayer.id) + ") - " +
                  to_string(currentPlayer.getHandSize()) + " cards";

    if (!twoRows)
    {
        // Single row display
        int totalWidth = handSize * spacing;
        int startX = (screenWidth - totalWidth) / 2;
        int startY = screenHeight - cardHeight - 40;

        DrawText(label.c_str(), startX, startY - 30, 18, WHITE);

        // Draw cards
        for (int i = 0; i < currentPlayer.getHandSize(); i++)
        {
<<<<<<< HEAD
            string cardName = currentPlayer.hand.getCardAt(i);
            int cardX = startX + i * spacing;
            int cardY = startY;

            // Highlight if hovered or selected
            bool isHighlighted = (i == hoveredCardIndex || i == selectedCardIndex);
            if (isHighlighted)
=======
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);
            cout << nextPlayer.id << " is skipped!" << endl;
            currentPlayerIndex = getNextPlayerIndex();
            currentColor = played.getColor();
        }
        else if (value == "Reverse")
        {
            clockwise = !clockwise;
            cout << "Direction reversed! Now playing " << (clockwise ? "clockwise" : "counterclockwise") << endl;
            currentColor = played.getColor();
            if (totalPlayers == 2)
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
            {
                cardY -= 15;
            }

            drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

            // Store card rectangle for click detection
            currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

            // Draw card index
            string indexStr = to_string(i);
            DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
        }
    }
    else
    {
        // Two rows display
        int cardsPerRow = (handSize + 1) / 2;  // Split evenly, top row gets extra if odd
        int bottomRowCards = handSize - cardsPerRow;

        // Top row
        int topRowWidth = cardsPerRow * spacing;
        int topStartX = (screenWidth - topRowWidth) / 2;
        int topStartY = screenHeight - (cardHeight * 2) - 60;

        // Bottom row
        int bottomRowWidth = bottomRowCards * spacing;
        int bottomStartX = (screenWidth - bottomRowWidth) / 2;
        int bottomStartY = screenHeight - cardHeight - 40;

        // Draw label above both rows
        DrawText(label.c_str(), screenWidth / 2 - 150, topStartY - 30, 18, WHITE);

        // Draw top row cards
        for (int i = 0; i < cardsPerRow; i++)
        {
            string cardName = currentPlayer.hand.getCardAt(i);
            int cardX = topStartX + i * spacing;
            int cardY = topStartY;

            // Highlight if hovered or selected
            bool isHighlighted = (i == hoveredCardIndex || i == selectedCardIndex);
            if (isHighlighted)
            {
                cardY -= 15;
            }

            drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

            // Store card rectangle for click detection
            currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

            // Draw card index
            string indexStr = to_string(i);
            DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
        }

        // Draw bottom row cards
        for (int i = 0; i < bottomRowCards; i++)
        {
            int actualIndex = cardsPerRow + i;
            string cardName = currentPlayer.hand.getCardAt(actualIndex);
            int cardX = bottomStartX + i * spacing;
            int cardY = bottomStartY;

            // Highlight if hovered or selected
            bool isHighlighted = (actualIndex == hoveredCardIndex || actualIndex == selectedCardIndex);
            if (isHighlighted)
            {
                cardY -= 15;
            }

            drawCard(cardName, cardX, cardY, cardWidth, cardHeight, isHighlighted);

            // Store card rectangle for click detection
            currentPlayerCardRects.push(CardRect(cardX, cardY, cardWidth, cardHeight, cardName));

            // Draw card index
            string indexStr = to_string(actualIndex);
            DrawText(indexStr.c_str(), cardX + 5, cardY + cardHeight + 5, 14, WHITE);
        }
    }
}
    void updateColorSelectionScreen()
    {
        // Color selection buttons
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
        // Semi-transparent overlay
        DrawRectangle(0, 0, screenWidth, screenHeight, (Color){0, 0, 0, 150});

        DrawText("Choose a Color for Wild Card", screenWidth/2 - 200, screenHeight/2 - 150, 30, WHITE);

        // Color buttons with actual colors
        int btnY = screenHeight/2 - 50;
        int btnSize = 100;

        // Red button
        DrawRectangle(screenWidth/2 - 250, btnY, btnSize, btnSize, RED);
        DrawRectangleLines(screenWidth/2 - 250, btnY, btnSize, btnSize, BLACK);
        DrawText("RED", screenWidth/2 - 230, btnY + 40, 20, WHITE);

        // Blue button
        DrawRectangle(screenWidth/2 - 100, btnY, btnSize, btnSize, BLUE);
        DrawRectangleLines(screenWidth/2 - 100, btnY, btnSize, btnSize, BLACK);
        DrawText("BLUE", screenWidth/2 - 85, btnY + 40, 20, WHITE);

        // Green button
        DrawRectangle(screenWidth/2 + 50, btnY, btnSize, btnSize, GREEN);
        DrawRectangleLines(screenWidth/2 + 50, btnY, btnSize, btnSize, BLACK);
        DrawText("GREEN", screenWidth/2 + 60, btnY + 40, 20, WHITE);

        // Yellow button
        DrawRectangle(screenWidth/2 + 200, btnY, btnSize, btnSize, YELLOW);
        DrawRectangleLines(screenWidth/2 + 200, btnY, btnSize, btnSize, BLACK);
        DrawText("YELLOW", screenWidth/2 + 205, btnY + 40, 20, BLACK);
    }

    void chooseColor(string color)
    {
        game->setCurrentColor(color);

        // Now play the wild card
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
<<<<<<< HEAD
=======
        else if (value == "Draw_2")
        {
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);
            cout << nextPlayer.id << " draws 2 cards and loses turn!" << endl;
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2

        waitingForColorChoice = false;
        pendingWildCard = "";
        selectedCardIndex = -1;
        currentScreen = 1;
    }

    void updateGameOverScreen()
    {
        Button restartBtn(screenWidth/2 - 100, screenHeight/2 + 100, 200, 60, "RESTART");

        if (restartBtn.isClicked())
        {
            game->startGame();
            currentScreen = 1;
            selectedCardIndex = -1;
            hoveredCardIndex = -1;
            message = "New Game Started!";
            messageTimer = 2.0f;
        }
    }

    void drawGameOverScreen()
    {
        DrawText("GAME OVER!", screenWidth/2 - 150, screenHeight/3, 50, YELLOW);

        // Find winner
        int winner = -1;
        for (int i = 0; i < game->getTotalPlayers(); i++)
        {
            Player p = game->getPlayerAtPublic(i);
            if (p.getHandSize() == 0)
            {
                winner = p.id;
                break;
            }
        }

        string winnerText = "Player " + to_string(winner) + " WINS!";
        DrawText(winnerText.c_str(), screenWidth/2 - 100, screenHeight/2 - 50, 30, GREEN);

<<<<<<< HEAD
        Button restartBtn(screenWidth/2 - 100, screenHeight/2 + 100, 200, 60, "RESTART");
        restartBtn.draw();
=======
    void checkWin(Player p)
    {
        if (p.getHandSize() == 0)
        {
            cout << "\n\n****************************************" << endl;
            cout << "*** " << p.id << " WINS THE GAME! ***" << endl;
            cout << "****************************************\n" << endl;
            gameOver = true;
        }
    }

    bool isGameOver()
    {
        return gameOver;
    }

    void showGameState()
    {
        cout << "\n--- Current Game State ---" << endl;
        cout << "Top card: " << deck.getTopDiscard() << endl;
        cout << "Current color: " << currentColor << endl;
        cout << "Direction: " << (clockwise ? "Clockwise" : "Counterclockwise") << endl;
        cout << "Draw pile: " << deck.getDrawPileSize() << " cards" << endl;

        cout << "\nPlayers:" << endl;
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            string marker = (i == currentPlayerIndex) ? " <-- CURRENT" : "";
            cout << "  " << p.id << ": " << p.getHandSize() << " cards" << marker << endl;
        }
        cout << endl;
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2
    }
};

class unoGUI
{
   Game* game;
   int screenWidth;
   int screenHeight;
   bool playing;
   int current_screen;
   Texture2D texlogo;
   Texture2D texPlayButton;
   Texture2D texCardBack;
   static const int max_cards = 120;
   Texture2D card[max_cards];
   string cardNames[max_cards];
   int card_count;
   Rectangle playButton;

<<<<<<< HEAD
    Game game;
    unoGUI gui(&game, 1400, 900);
    gui.run();
=======
   void LoadBasicImages()
   {
       texlogo = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/logo.png");
       texPlayButton = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/playbutton.png");
       texCardBack = LoadTexture("E:/BSCS-Semester3/DSA/UNO/assets/cards/back.jpg");
   }
   void UnloadImages()
   {
        for (int i = 0; i < card_count; i++)
        {
            UnloadTexture(card[i]);
        }

        UnloadTexture(texlogo);
        UnloadTexture(texPlayButton);
        UnloadTexture(texCardBack);
    }
    Texture2D getCardTexture(string cardName) {
        // 1) Check if already loaded
        for (int i = 0; i < card_count; i++) {
            if (cardNames[i] == cardName) {
                return card[i];
            }
        }
        if (card_count >= max_cards) {
            return texCardBack;
        }

        string path = "E:/BSCS-Semester3/DSA/UNO/assets/cards/" + cardName + ".jpg";
        Texture2D tex = LoadTexture(path.c_str());

        if (tex.id == 0) {
            return texCardBack;
        }

        cardNames[card_count]   = cardName;
        card[card_count] = tex;
        card_count++;

        return tex;
    }
    void Update()
    {
        if (current_screen == 0)
        {
            UpdateStartScreen();
        }
        else if (current_screen == 1)
        {
            UpdateGameScreen();
        }
    }
    void Draw()
    {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (current_screen == 0)
        {
            DrawStartScreen();
        }
         else if (current_screen == 1)
        {
            DrawGameScreen();
        }
        EndDrawing();
    }
    void UpdateStartScreen()
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, playButton))
             {
                current_screen = 1;
             }
        }
    }
   void DrawStartScreen() {
        ClearBackground((Color){20, 120, 40, 255});

        float scale = 0.3f;

        float logoW = texlogo.width  * scale;
        float logoH = texlogo.height * scale;

        float logoX = screenWidth  / 2.0f - logoW / 2.6f;
        float logoY = screenHeight * 0.15f;

        // Draw scaled logo
        Rectangle src  = { 0, 0, (float)texlogo.width, (float)texlogo.height };
        Rectangle dest = { logoX, logoY, logoW, logoH };
        Vector2 origin = { 0, 0 };

        DrawTexturePro(texlogo, src, dest, origin, 0.0f, WHITE);

        float Btnscale = 0.5f;
        playButton.width  = (float)texPlayButton.width* Btnscale;
        playButton.height = (float)texPlayButton.height * Btnscale;
        playButton.x = screenWidth / 2.0f - playButton.width  / 2.0f+70;
        playButton.y = screenHeight * 0.65f+140;
        Color tint = WHITE;
        if (CheckCollisionPointRec(GetMousePosition(), playButton)) {
            tint = YELLOW;
        }
        Rectangle btnsrc  = { 0, 0, (float)texPlayButton.width, (float)texPlayButton.height };
        Rectangle btndest = { playButton.x, playButton.y, playButton.width, playButton.height };
        Vector2 btnorigin = { 0.0f, 0.0f };
        DrawTexturePro(texPlayButton, btnsrc, btndest, btnorigin, 0.0f, tint);


    }
    void UpdateGameScreen()
    {
        if (game->isGameOver())
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                playing = false;
            }
        }
    }
    void DrawGameScreen()
    {
        ClearBackground((Color){10, 80, 30, 255});
        DrawGameHeader();
        DrawDeckAndDiscard();
        DrawPlayers();
    }
    void DrawGameHeader()
    {
        DrawRectangle(0, 0, screenWidth, 60, (Color){20, 120, 40, 255});
        DrawText("UNO", 20, 18, 28, RAYWHITE);

        string color = game->getCurrentColor();
        Color c = RED;
        if (color == "Blue")   c = BLUE;
        if (color == "Green")  c = GREEN;
        if (color == "Yellow") c = YELLOW;
        if (color == "Red")    c = RED;

        DrawText("Current Color:", 200, 18, 20, RAYWHITE);
        DrawRectangle(340, 15, 40, 30, c);
        DrawRectangleLines(340, 15, 40, 30, BLACK);

        string dirText = "Direction: ";
        if (game->getClockwise()) dirText += "Clockwise";
        else                      dirText += "Counterclockwise";
        DrawText(dirText.c_str(), 420, 18, 20, RAYWHITE);

        int currentIndex = game->getCurrentPlayerIndex();
        if (currentIndex >= 0 && currentIndex < game->getTotalPlayers()) {
            Player p = game->getPlayerAtPublic(currentIndex);
            string label = "Current Player: " + p.id;
            DrawText(label.c_str(), 750, 18, 20, YELLOW);
        }
    }
    void DrawDeckAndDiscard() {
        int centerY = screenHeight / 2;

        // Deck (draw pile) - just card back
        int deckX = screenWidth / 2 - 200;
        int deckY = centerY - texCardBack.height / 2;
        DrawTexture(texCardBack, deckX, deckY, WHITE);
        DrawText("Deck", deckX + 15, deckY + texCardBack.height + 5, 18, RAYWHITE);

        // Discard pile - top card face
        string topCard = game->getTopDiscard();
        int discardX = screenWidth / 2 + 80;
        int discardY = centerY - texCardBack.height / 2;

        if (!topCard.empty()) {
            Texture2D tex = getCardTexture(topCard);
            DrawTexture(tex, discardX, discardY, WHITE);
        } else {
            DrawRectangle(discardX, discardY, texCardBack.width, texCardBack.height, GRAY);
            DrawRectangleLines(discardX, discardY, texCardBack.width, texCardBack.height, BLACK);
        }
        DrawText("Discard", discardX + 5, discardY + texCardBack.height + 5, 18, RAYWHITE);
    }

    void DrawPlayers() {
        int total = game->getTotalPlayers();
        if (total <= 0) return;

        int marginTop = 80;
        int rowHeight = texCardBack.height + 40;

        for (int i = 0; i < total; i++) {
            Player p = game->getPlayerAtPublic(i);
            bool isCurrent = (i == game->getCurrentPlayerIndex());

            int rowY = marginTop + i * rowHeight;

            // Player label
            string info = p.id + " (" + to_string(p.getHandSize()) + " cards)";
            if (isCurrent) info += "  <--";
            DrawText(info.c_str(), 20, rowY, 20, isCurrent ? YELLOW : RAYWHITE);

            // Draw their hand as card backs (we don't have direct access to card names)
            int cardCount = p.getHandSize();
            int startX = 250;
            int spacing = 40;

            for (int k = 0; k < cardCount; k++) {
                int x = startX + k * spacing;
                int y = rowY + 25;
                DrawTexture(texCardBack, x, y, WHITE);
                DrawRectangleLines(x, y, texCardBack.width, texCardBack.height, BLACK);
            }
        }
    }
   public:
       unoGUI(Game* g, int width = 1400, int height = 900)
       {
           game = g;
           screenHeight = height;
           screenWidth = width;
           playing = true;
           current_screen =0;
           InitWindow(1400, 900, "UNO - Raylib GUI");
           SetTargetFPS(60);
           card_count = 0;
           LoadBasicImages();
       }
       ~unoGUI()
       {
           UnloadImages();
           CloseWindow();
       }
       void Run()
       {
           while (playing && !WindowShouldClose())
           {
               Update();
               Draw();
           }
       }
};

int main() {
    srand(time(0));

    Game game;
    game.startGame();

    unoGUI gui(&game, 1280, 720);
    gui.Run();
>>>>>>> ac3b289a659be540cc6a3d90ccc2e6235f1d7ad2

    return 0;
}
