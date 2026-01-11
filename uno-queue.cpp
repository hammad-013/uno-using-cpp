#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <raylib.h>
using namespace std;

enum GameState {
    PLAYER_TURN,
    WILD_COLOR_CHOICE,
    CHALLENGE_PENDING,
    ROUND_OVER,
    GAME_OVER
};

// Scoring constants
const int WINNING_SCORE = 500;
const int WILD_SCORE = 50;
const int ACTION_SCORE = 20;

template <typename T>
class queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val)
        {

         data=value;
         next=nullptr; }
    };

    Node* frontPtr;
    Node* rearPtr;
    int queueSize;

public:

    queue()
    {

     frontPtr=nullptr;
     rearPtr=nullptr;
    queueSize=0;
    }
    ~queue() {
        clear();
    }


    queue(const queue& other) : frontPtr(nullptr), rearPtr(nullptr), queueSize(0) {
        Node* current = other.frontPtr;
        while (current != nullptr) {
            push(current->data);
            current = current->next;
        }
    }


    queue& operator=(const queue& other) {
        if (this != &other) {
            clear();
            Node* current = other.frontPtr;
            while (current != nullptr) {
                push(current->data);
                current = current->next;
            }
        }
        return *this;
    }


    void push(T value) {
        Node* newNode = new Node(value);

        if (empty()) {
            frontPtr = rearPtr = newNode;
        } else {
            rearPtr->next = newNode;
            rearPtr = newNode;
        }
        queueSize++;
    }


    void pop() {
        if (empty()) return;

        Node* temp = frontPtr;
        frontPtr = frontPtr->next;

        if (frontPtr == nullptr) {
            rearPtr = nullptr;
        }

        delete temp;
        queueSize--;
    }


    T front() const {
        if (empty()) {
            return T();
        }
        return frontPtr->data;
    }

    T back() const {
        if (empty()) {
            return T();
        }
        return rearPtr->data;
    }


    bool empty() const {
        return frontPtr == nullptr;
    }


    bool isEmpty() const {
        return empty();
    }


    int size() const {
        return queueSize;
    }


    void clear() {
        while (!empty()) {
            pop();
        }
    }


    T getAt(int index) const {
        if (index < 0 || index >= queueSize) {
            return T();
        }

        Node* current = frontPtr;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }


    T removeAt(int index) {
        if (index < 0 || index >= queueSize || empty()) {
            return T();
        }

        T removedValue;

        if (index == 0) {

            removedValue = frontPtr->data;
            pop();
        } else {

            Node* prev = frontPtr;
            for (int i = 0; i < index - 1; i++) {
                prev = prev->next;
            }

            Node* toRemove = prev->next;
            removedValue = toRemove->data;

            prev->next = toRemove->next;


            if (toRemove == rearPtr) {
                rearPtr = prev;
            }

            delete toRemove;
            queueSize--;
        }

        return removedValue;
    }


    T* copyToArray() const {
        if (empty()) return nullptr;

        T* arr = new T[queueSize];
        Node* current = frontPtr;

        for (int i = 0; i < queueSize; i++) {
            arr[i] = current->data;
            current = current->next;
        }

        return arr;
    }


    void loadFromArray(T* arr, int arrSize) {
        clear();

        for (int i = 0; i < arrSize; i++) {
            push(arr[i]);
        }
    }


    bool hasColor(const string& color) const {
        Node* current = frontPtr;

        while (current != nullptr) {
            string cardName = current->data;
             pos = cardName.find('_');
            if (pos != string::npos) {
                string cardColor = cardName.substr(0, pos);
                if (cardColor == color) {
                    return true;
                }
            }
            current = current->next;
        }
        return false;
    }
};
class Card {
public:
    string name;

    Card() { name = ""; }
    Card(string n) { name = n; }

    string getColor() const {
        if (name == "WILD" || name == "WILD_DRAW4") return "WILD";

        int pos = 0;
        while (pos < name.length() && name[pos] != '_') pos++;
        if (pos >= name.length()) return "";

        string color = "";
        for (int i = 0; i < pos; i++) {
            color += name[i];
        }
        return color;
    }
 string getValue() const {
        if (name == "WILD") return "WILD";
        if (name == "WILD_DRAW_FOUR") return "DRAW_FOUR";

        int pos = 0;
        while (pos < name.length() && name[pos] != '_') pos++;
        if (pos >= name.length()) return name;

        pos++;
        string value = "";
        for (int i = pos; i < name.length(); i++) {
            value += name[i];
        }
        return value;
    }
bool isWild() const {
        if (name == "WILD") return true;
        if (name == "WILD_DRAW_FOUR") return true;
        return false;
    }
bool canPlayOn(const Card& top, const string& forcedColor) const {
        if (isWild()) return true;

        string myColor = getColor();
        string topColor = top.getColor();

        if (forcedColor != "") {
            if (myColor == forcedColor) return true;
            return false;
        }

        if (myColor == topColor) return true;
        if (getValue() == top.getValue()) return true;

        return false;
    }
class Player {
public:
    int id;
    queue<string> hand;

    Player() : id(0) {}
    Player(int pid) : id(pid) {}

    void addCard(string cardName) {
        hand.push(cardName);
    }

    string playCard(int index) {
        if (index < 0 || index >= (int)hand.size()) return "";

        queue<string> temp;
        string chosen = "";
        int count = 0;

        while (!hand.empty()) {
            string c = hand.front();
            hand.pop();
            if (count == index) {
                chosen = c;
            } else {
                temp.push(c);
            }
            count++;
        }
        hand = temp;
        return chosen;
    }

    string getCardAt(int index) const {
        queue<string> temp = hand;
        string result = "";
        int count = 0;

        while (!temp.empty()) {
            if (count == index) {
                result = temp.front();
                break;
            }
            temp.pop();
            count++;
        }
        return result;
    }

    int getHandSize() const {
        return hand.size();
    }
};

class Deck
{
private:
    queue<string> drawPile;
    queue<string> discardPile;

    void moveDiscardToDraw()
     {
        if (discardPile.size() <= 1) return;


        string topCard = "";
        queue<string> temp;

        while (!discardPile.empty())
        {
            temp.push(discardPile.front());
            discardPile.pop();
        }
        topCard = temp.back();
        temp.pop();


        while (!temp.empty())
        {
            drawPile.push(temp.front());
            temp.pop();
        }


        string cards[108];
        int count = 0;
        while (!drawPile.empty())
        {
            cards[count] = drawPile.front();
            drawPile.pop();
            count++;
        }

        srand(time(0));
        for (int i = count - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);
            string swapTemp = cards[i];
            cards[i] = cards[j];
            cards[j] = swapTemp;
        }

        for (int i = 0; i < count; i++)
        {
            drawPile.push(cards[i]);
        }


        discardPile.push(topCard);
    }

public:
    void createDeck() {
        queue<string> allCards;
        string colors[4] = {"RED", "GREEN", "BLUE", "YELLOW"};

        for (int c = 0; c < 4; c++) {
            string color = colors[c];
            allCards.push(color + "_0");

            for (int n = 1; n <= 9; n++) {
                string num = to_string(n);
                allCards.push(color + "_" + num);
                allCards.push(color + "_" + num);
            }

            allCards.push(color + "_SKIP");
            allCards.push(color + "_SKIP");
            allCards.push(color + "_REVERSE");
            allCards.push(color + "_REVERSE");
            allCards.push(color + "_DRAW_TWO");
            allCards.push(color + "_DRAW_TWO");

            allCards.push("WILD");
            allCards.push("WILD_DRAW_FOUR");
        }


        string cardArray[108];
        int idx = 0;
        while (!allCards.empty()) {
            cardArray[idx] = allCards.front();
            allCards.pop();
            idx++;
        }

        srand(time(0));
        for (int i = 107; i > 0; i--) {
            int j = rand() % (i + 1);
            string temp = cardArray[i];
            cardArray[i] = cardArray[j];
            cardArray[j] = temp;
        }

        drawPile = queue<string>();
        for (int i = 0; i < 108; i++) {
            drawPile.push(cardArray[i]);
        }
        discardPile = queue<string>();
    }

    string draw() {
        if (drawPile.empty()) {
            moveDiscardToDraw();
        }
        if (drawPile.empty()) return "";
        string card = drawPile.front();
        drawPile.pop();
        return card;
    }

    void discard(string card) {
        discardPile.push(card);
    }

    string topDiscard() const {
        if (discardPile.empty()) return "";
        queue<string> temp = discardPile;
        string top = "";
        while (!temp.empty()) {
            top = temp.front();
            temp.pop();
        }
        return top;
    }

    int drawPileSize() const {
        return drawPile.size();
    }
};
class Game {
private:
    Player players[4];
    Deck deck;
    int currentPlayer;
    bool clockwise;
    string activeColor;
    GameState state;


    int challengedPlayer;
    string colorBeforeWild;
    bool challengePending;


    bool roundOver;
    string roundMessage;
    float roundMessageTimer;
    int roundWinner;

    bool gameOver;


    string lastPlayedWildCard;

public:
    Game() : players{
        Player(0, "Player 1"),
        Player(1, "Player 2"),
        Player(2, "Player 3"),
        Player(3, "Player 4")
    } {
        currentPlayer = 0;
        clockwise = true;
        activeColor = "";
        state = PLAYER_TURN;
        challengedPlayer = -1;
        colorBeforeWild = "";
        challengePending = false;
        roundOver = false;
        roundMessage = "";
        roundMessageTimer = 0;
        roundWinner = -1;
        gameOver = false;
        lastPlayedWildCard = "";
    }

    void start() {
        startNewRound();
    }

    bool playWildCard(int playerIdx, int cardIndex, string& msg) {
        if (playerIdx != currentPlayer) {
            msg = "Not your turn!";
            return false;
        }

        Player& p = players[playerIdx];
        string playedName = p.getCardAt(cardIndex);
        if (playedName == "") {
            msg = "Invalid card!";
            return false;
        }

        Card played(playedName);
        Card top(deck.topDiscard());

        if (!played.canPlayOn(top, activeColor)) {
            msg = "Cannot play " + playedName + " on " + activeColor;
            return false;
        }


        playedName = p.playCard(cardIndex);
        deck.discard(playedName);
        lastPlayedWildCard = playedName;


        if (p.getHandSize() == 0) {
            roundWinner = playerIdx;
            calculateRoundScores();
            return true;
        }


        string val = played.getValue();

        if (val == "DRAW_FOUR") {
            advanceTurn();
            if (!challengePending) {
                int next = currentPlayer;
                for (int i = 0; i < 4; i++) {
                    players[next].addCard(deck.draw());
                }
                advanceTurn();
            }
            msg = "Played: " + playedName;
        }
        else if (val == "WILD") {
            advanceTurn();
            msg = "Played: " + playedName;
        }

        return true;
    }

    bool play(int playerIdx, int cardIndex, string& msg) {
        if (playerIdx != currentPlayer) {
            msg = "Not your turn!";
            return false;
        }

        if (state != PLAYER_TURN) {
            msg = "Cannot play now!";
            return false;
        }

        Player& p = players[playerIdx];
        if (cardIndex < 0 || cardIndex >= p.getHandSize()) {
            msg = "Invalid card!";
            return false;
        }

        string playedName = p.getCardAt(cardIndex);
        if (playedName == "") {
            msg = "Invalid card!";
            return false;
        }

        Card played(playedName);
        Card top(deck.topDiscard());

        if (!played.canPlayOn(top, activeColor)) {
            msg = "Cannot play " + playedName + " on " + activeColor;
            return false;
        }


        if (played.getValue() == "DRAW_FOUR") {
            colorBeforeWild = activeColor;
            challengePending = true;
            challengedPlayer = (clockwise) ? (currentPlayer + 1) % 4 : (currentPlayer - 1 + 4) % 4;
        }


        playedName = p.playCard(cardIndex);
        deck.discard(playedName);

        if (!played.isWild()) {
            activeColor = played.getColor();
        }

        if (p.getHandSize() == 0) {
            roundWinner = playerIdx;
            calculateRoundScores();
            return true;
        }

        string val = played.getValue();

        if (val == "DRAW_FOUR") {
            state = WILD_COLOR_CHOICE;
            lastPlayedWildCard = playedName;
            msg = "Choose a color for Wild Draw Four";
            return true;
        }
        else if (val == "WILD") {
            state = WILD_COLOR_CHOICE;
            lastPlayedWildCard = playedName;
            msg = "Choose a color for Wild";
            return true;
        }
        else if (val == "DRAW_TWO") {
            advanceTurn();
            int next = currentPlayer;
            for (int i = 0; i < 2; i++) {
                players[next].addCard(deck.draw());
            }
            advanceTurn();
            msg = "Played: " + playedName + " - Next player draws 2";
        }
        else if (val == "SKIP") {
            advanceTurn();
            advanceTurn();
            msg = "Played: " + playedName + " - Next player skipped";
        }
        else if (val == "REVERSE") {
            clockwise = !clockwise;
            if (players[0].getHandSize() == 2) {
                advanceTurn();
            } else {
                advanceTurn();
            }
            msg = "Played: " + playedName + " - Direction reversed";
        }
        else {
            advanceTurn();
            msg = "Played: " + playedName;
        }

        return true;
    }

    void advanceTurn() {
        if (clockwise) {
            currentPlayer = (currentPlayer + 1) % 4;
        } else {
            currentPlayer = (currentPlayer - 1 + 4) % 4;
        }
    }

    void drawForCurrent() {
        players[currentPlayer].addCard(deck.draw());
        advanceTurn();
    }

    void chooseColor(string color) {
        if (state != WILD_COLOR_CHOICE) return;

        activeColor = color;

        if (challengePending) {
            state = CHALLENGE_PENDING;
        } else {
            if (lastPlayedWildCard == "WILD_DRAW_FOUR") {
                advanceTurn();
                int next = currentPlayer;
                for (int i = 0; i < 4; i++) {
                    players[next].addCard(deck.draw());
                }
                advanceTurn();
            } else {
                advanceTurn();
            }
            state = PLAYER_TURN;
        }
    }

    void handleChallenge(bool acceptChallenge) {
        if (state != CHALLENGE_PENDING) return;

        Player& challenger = players[challengedPlayer];
        int wildPlayerIdx = (challengedPlayer == 0) ? 3 : challengedPlayer - 1;
        if (wildPlayerIdx < 0) wildPlayerIdx = 0;
        if (wildPlayerIdx >= 4) wildPlayerIdx = 3;

        Player& wildPlayer = players[wildPlayerIdx];

        if (!acceptChallenge) {
            for (int i = 0; i < 4; i++) {
                challenger.addCard(deck.draw());
            }
            advanceTurn();
            state = PLAYER_TURN;
            challengePending = false;
            return;
        }

        bool hadMatchingColor = wildPlayer.hasColor(colorBeforeWild);

        if (hadMatchingColor) {
            for (int i = 0; i < 4; i++) {
                wildPlayer.addCard(deck.draw());
            }
            currentPlayer = challengedPlayer;
        } else {
            for (int i = 0; i < 6; i++) {
                challenger.addCard(deck.draw());
            }
            advanceTurn();
        }

        state = PLAYER_TURN;
        challengePending = false;
    }

    void setColor(string col) {
        activeColor = col;
    }

    int getCurrent() const { return currentPlayer; }
    string getColor() const { return activeColor; }
    string getTop() const { return deck.topDiscard(); }
    Player getPlayer(int i) const {
        if (i >= 0 && i < 4) return players[i];
        return Player();
    }
    bool isOver() const { return gameOver; }
    bool isRoundOver() const { return state == ROUND_OVER; }
    bool isChallengePending() const { return state == CHALLENGE_PENDING; }
    bool isChoosingColor() const { return state == WILD_COLOR_CHOICE; }
    int drawSize() const { return deck.drawPileSize(); }
    string getRoundMessage() const { return roundMessage; }
    float getRoundMessageTimer() const { return roundMessageTimer; }
    GameState getState() const { return state; }
    int getChallengedPlayer() const { return challengedPlayer; }

    bool canPlayWildDrawFour(int playerIdx) const {
        if (playerIdx != currentPlayer) return false;

        const Player& p = players[playerIdx];

        for (int i = 0; i < p.getHandSize(); i++) {
            string cardName = p.getCardAt(i);
            Card c(cardName);
            if (c.getValue() == "DRAW_FOUR") {
                return true;
            }
        }
        return false;
    }

    void update(float deltaTime) {
        if (state == ROUND_OVER) {
            roundMessageTimer -= deltaTime;
            if (roundMessageTimer <= 0) {
                if (gameOver) {
                    state = GAME_OVER;
                } else {
                    startNewRound();
                }
            }
        }
    }

    void reset() {
        for (int i = 0; i < 4; i++) {
            players[i].totalScore = 0;
            players[i].resetHand();
        }
        gameOver = false;
        start();
    }

private:
    void calculateRoundScores() {
        if (roundWinner < 0 || roundWinner >= 4) return;

        Player& winner = players[roundWinner];
        int roundPoints = 0;

        for (int i = 0; i < 4; i++) {
            if (i != roundWinner) {
                roundPoints += players[i].getHandScore();
            }
        }

        winner.totalScore += roundPoints;

        roundMessage = winner.name + " wins the round!\n";
        roundMessage += "Points earned: " + to_string(roundPoints) + "\n";
        roundMessage += "Total score: " + to_string(winner.totalScore);

        if (winner.hasWon()) {
            roundMessage += "\n\n" + winner.name + " REACHED 500 POINTS!";
            gameOver = true;
            state = GAME_OVER;
        } else {
            roundMessageTimer = 3.0f;
            state = ROUND_OVER;
        }
    }

    void startNewRound() {
        for (int i = 0; i < 4; i++) {
            players[i].resetHand();
        }

        deck.createDeck();

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                string card = deck.draw();
                if (!card.empty()) {
                    players[i].addCard(card);
                }
            }
        }

        string firstCard = deck.draw();
        while (firstCard.empty()) {
            deck.createDeck();
            firstCard = deck.draw();
        }

        Card c(firstCard);
        while (c.isWild()) {
            deck.discard(firstCard);
            firstCard = deck.draw();
            if (firstCard.empty()) break;
            c = Card(firstCard);
        }

        if (!firstCard.empty()) {
            deck.discard(firstCard);
            activeColor = c.getColor();
        } else {
            activeColor = "RED";
        }

        if (roundWinner >= 0 && roundWinner < 4) {
            currentPlayer = roundWinner;
        } else {
            currentPlayer = 0;
        }

        clockwise = true;
        roundOver = false;
        roundMessageTimer = 0;
        challengePending = false;
        lastPlayedWildCard = "";
        state = PLAYER_TURN;
    }
};

struct Button {
    float x, y, w, h;
    string text;
    Button(float px, float py, float pw, float ph, string t) : x(px), y(py), w(pw), h(ph), text(t) {}

    bool hovered() {
        Vector2 m = GetMousePosition();
        return (m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h);
    }

    bool clicked() {
        return hovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void draw() {
        Color col = hovered() ? YELLOW : GREEN;
        DrawRectangle(x, y, w, h, col);
        DrawRectangleLines(x, y, w, h, BLACK);
        int tw = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), x + w/2 - tw/2, y + h/2 - 10, 20, BLACK);
    }

    void drawWithColor(Color color) {
        Color col = hovered() ? Fade(color, 0.7f) : color;
        DrawRectangleRounded({x, y, w, h}, 0.3f, 8, col);
        DrawRectangleRoundedLines({x, y, w, h}, 0.3f, 8, WHITE);
        int tw = MeasureText(text.c_str(), 20);
        DrawText(text.c_str(), x + w/2 - tw/2, y + h/2 - 10, 20, BLACK);
    }
};

class UNOGUI {
private:
    Game* game;
    Texture2D cardBack;
    vector<pair<string, Texture2D>> cardTextures;
    int selected = -1;
    int hovered = -1;
    string message = "";
    float msgTime = 0;
    bool choosingColor = false;
    int selectedCardIndex = -1;

    Texture2D findTexture(const string& name) {
        for (const auto& pair : cardTextures) {
            if (pair.first == name) {
                return pair.second;
            }
        }

        string upperName = name;
        for (char& c : upperName) c = toupper(c);

        for (const auto& pair : cardTextures) {
            string upperPair = pair.first;
            for (char& c : upperPair) c = toupper(c);
            if (upperPair == upperName) {
                return pair.second;
            }
        }

        if (name.find("DRAW_TWO") != string::npos) {
            for (const auto& pair : cardTextures) {
                if (pair.first.find("DRAW") != string::npos || pair.first.find("Draw") != string::npos) {
                    return pair.second;
                }
            }
        }

        return cardBack;
    }

    void loadImages() {
        string path = "assets/Cards/";
        string colors[4] = {"RED", "GREEN", "BLUE", "YELLOW"};
        string values[13] = {"0","1","2","3","4","5","6","7","8","9",
                            "SKIP","REVERSE","DRAW_TWO"};

        for (int c = 0; c < 4; c++) {
            for (int v = 0; v < 13; v++) {
                string cardName = colors[c] + "_" + values[v];

                vector<string> filesToTry;
                filesToTry.push_back(path + cardName + ".jpg");
                filesToTry.push_back(path + cardName + ".png");

                string capColor = colors[c];
                capColor[0] = toupper(capColor[0]);
                for (size_t i = 1; i < capColor.size(); i++) {
                    capColor[i] = tolower(capColor[i]);
                }

                string capValue = values[v];
                if (values[v] == "SKIP" || values[v] == "REVERSE" || values[v] == "DRAW_TWO") {
                    capValue[0] = toupper(capValue[0]);
                    for (size_t i = 1; i < capValue.size(); i++) {
                        capValue[i] = tolower(capValue[i]);
                    }
                }

                if (values[v] == "DRAW_TWO") {
                    filesToTry.push_back(path + capColor + "_Draw_2.jpg");
                    filesToTry.push_back(path + capColor + "_Draw_2.png");
                }

                filesToTry.push_back(path + capColor + "_" + capValue + ".jpg");
                filesToTry.push_back(path + capColor + "_" + capValue + ".png");

                bool loaded = false;
                for (const string& file : filesToTry) {
                    if (FileExists(file.c_str())) {
                        Texture2D tex = LoadTexture(file.c_str());
                        cardTextures.push_back({cardName, tex});
                        loaded = true;
                        break;
                    }
                }


                if (!loaded) {
                    Color cardColor;
                    if (colors[c] == "RED") cardColor = RED;
                    else if (colors[c] == "GREEN") cardColor = GREEN;
                    else if (colors[c] == "BLUE") cardColor = BLUE;
                    else cardColor = YELLOW;

                    Image img = GenImageColor(120, 180, cardColor);
                    Texture2D tex = LoadTextureFromImage(img);
                    cardTextures.push_back({cardName, tex});
                    UnloadImage(img);
                }
            }
        }


        bool wildLoaded = false;
        vector<string> wildFiles = {
            path + "WILD.jpg", path + "Wild.jpg",
            path + "WILD.png", path + "Wild.png"
        };

        for (const string& file : wildFiles) {
            if (FileExists(file.c_str())) {
                Texture2D t = LoadTexture(file.c_str());
                cardTextures.push_back({"WILD", t});
                wildLoaded = true;
                break;
            }
        }

        if (!wildLoaded) {
            Image img = GenImageColor(120, 180, BLACK);
            Texture2D tex = LoadTextureFromImage(img);
            cardTextures.push_back({"WILD", tex});
            UnloadImage(img);
        }


        bool wildFourLoaded = false;
        vector<string> wildFourFiles = {
            path + "WILD_DRAW_FOUR.jpg", path + "Wild_Draw_4.jpg",
            path + "WILD_DRAW_FOUR.png", path + "Wild_Draw_4.png"
        };

        for (const string& file : wildFourFiles) {
            if (FileExists(file.c_str())) {
                Texture2D t = LoadTexture(file.c_str());
                cardTextures.push_back({"WILD_DRAW_FOUR", t});
                wildFourLoaded = true;
                break;
            }
        }

        if (!wildFourLoaded) {
            Image img = GenImageColor(120, 180, DARKPURPLE);
            Texture2D tex = LoadTextureFromImage(img);
            cardTextures.push_back({"WILD_DRAW_FOUR", tex});
            UnloadImage(img);
        }


        vector<string> backFiles = {
            path + "back.jpg", path + "BACK.jpg",
            path + "back.png", path + "BACK.png"
        };

        bool backLoaded = false;
        for (const string& file : backFiles) {
            if (FileExists(file.c_str())) {
                cardBack = LoadTexture(file.c_str());
                backLoaded = true;
                break;
            }
        }

        if (!backLoaded) {
            Image img = GenImageColor(120, 180, DARKBLUE);
            cardBack = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    void drawCard(const string& name, float x, float y, float w, float h, bool highlight) {
        Texture2D tex = findTexture(name);
        if (tex.id == 0) tex = cardBack;

        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
        Rectangle dest = {x, y, w, h};
        DrawTexturePro(tex, src, dest, {0,0}, 0, WHITE);

        if (highlight) {
            DrawRectangleLines(x-3, y-3, w+6, h+6, YELLOW);
        }
    }

    void drawBack(float x, float y, float w, float h) {
        if (cardBack.id != 0) {
            Rectangle src = {0,0,(float)cardBack.width,(float)cardBack.height};
            Rectangle dest = {x,y,w,h};
            DrawTexturePro(cardBack, src, dest, {0,0}, 0, WHITE);
        } else {
            DrawRectangle(x, y, w, h, DARKBLUE);
            DrawText("UNO", x + 20, y + 60, 40, YELLOW);
        }
    }

    void drawRoundSummary() {
        DrawRectangle(0, 0, 1400, 900, {0, 0, 0, 200});

        Rectangle box = {400, 200, 600, 400};
        DrawRectangleRec(box, {30, 30, 30, 240});
        DrawRectangleLinesEx(box, 3, YELLOW);

        DrawText("ROUND SUMMARY", 500, 230, 40, YELLOW);

        string summary = game->getRoundMessage();
        vector<string> lines;
        size_t start = 0;
        size_t end = summary.find('\n');
        while (end != string::npos) {
            lines.push_back(summary.substr(start, end - start));
            start = end + 1;
            end = summary.find('\n', start);
        }
        lines.push_back(summary.substr(start));

        int lineY = 300;
        for (const string& line : lines) {
            int width = MeasureText(line.c_str(), 24);
            DrawText(line.c_str(), 700 - width/2, lineY, 24, WHITE);
            lineY += 35;
        }

        int countdown = (int)game->getRoundMessageTimer() + 1;
        string countText = "Next round in " + to_string(countdown) + "...";
        DrawText(countText.c_str(), 700 - MeasureText(countText.c_str(), 20)/2, 550, 20, LIGHTGRAY);
    }

    void drawChallengeScreen() {
        DrawRectangle(0, 0, 1400, 900, {0, 0, 0, 180});

        int challengedIdx = game->getChallengedPlayer();
        if (challengedIdx < 0 || challengedIdx >= 4) return;

        Player challenged = game->getPlayer(challengedIdx);

        string title = challenged.name + " - Wild Draw 4 Challenge!";
        DrawText(title.c_str(), 700 - MeasureText(title.c_str(), 40)/2, 250, 40, YELLOW);

        DrawText("You can challenge the previous play!", 700 - 180, 320, 20, WHITE);
        DrawText("If they had a matching color: they draw 4", 700 - 200, 360, 18, LIGHTGRAY);
        DrawText("If they didn't: you draw 6", 700 - 120, 390, 18, LIGHTGRAY);

        Button challengeBtn(500, 450, 180, 60, "CHALLENGE");
        challengeBtn.drawWithColor(RED);

        Button drawBtn(720, 450, 180, 60, "DRAW 4");
        drawBtn.drawWithColor(GREEN);

        if (challengeBtn.clicked()) {
            game->handleChallenge(true);
            message = "Challenge accepted!";
            msgTime = 2;
        }

        if (drawBtn.clicked()) {
            game->handleChallenge(false);
            message = "Drawing 4 cards...";
            msgTime = 2;
        }
    }

public:
    UNOGUI(Game* g) : game(g) {
        InitWindow(1400, 900, "UNO - 4 Players");
        SetTargetFPS(60);
        loadImages();
    }

    ~UNOGUI() {
        for (auto& pair : cardTextures) {
            UnloadTexture(pair.second);
        }
        if (cardBack.id != 0) UnloadTexture(cardBack);
        CloseWindow();
    }

    void run() {
        game->start();

        while (!WindowShouldClose()) {
            game->update(GetFrameTime());

            if (msgTime > 0) msgTime -= GetFrameTime();

            BeginDrawing();
            ClearBackground({20, 100, 40, 255});

            if (game->getState() == GAME_OVER) {
                drawGameOver();
            } else if (game->isRoundOver()) {
                drawRoundSummary();
            } else if (game->isChallengePending()) {
                drawChallengeScreen();
            } else if (game->isChoosingColor()) {
                drawColorChoice();
            } else {
                drawMain();
            }

            if (msgTime > 0) {
                DrawRectangle(100, 100, 500, 80, {0,0,0,200});
                DrawRectangleLines(100, 100, 500, 80, YELLOW);
                DrawText(message.c_str(), 120, 130, 24, YELLOW);
            }

            EndDrawing();
        }
    }

    void drawMain() {
        int cp = game->getCurrent();
        if (cp < 0 || cp >= 4) return;

        Player curr = game->getPlayer(cp);

        DrawRectangle(0, 0, 1400, 60, {0, 0, 0, 150});
        DrawText("SCORES:", 20, 20, 24, YELLOW);

        int scoreX = 150;
        for (int i = 0; i < 4; i++) {
            Player p = game->getPlayer(i);
            string scoreText = p.name + ": " + to_string(p.totalScore);
            Color scoreColor = (i == cp) ? YELLOW : WHITE;
            DrawText(scoreText.c_str(), scoreX, 20, 24, scoreColor);
            scoreX += 250;
        }

        DrawText(TextFormat("Player %d's Turn", cp + 1), 600, 70, 30, WHITE);
        DrawText(TextFormat("Active Color: %s", game->getColor().c_str()), 600, 110, 20, WHITE);

        drawBack(400, 300, 120, 180);
        DrawText(TextFormat("Draw: %d", game->drawSize()), 420, 500, 20, WHITE);

        string top = game->getTop();
        if (top != "") {
            drawCard(top, 600, 300, 120, 180, false);
            DrawText("Discard", 620, 500, 20, WHITE);
        }

        int size = curr.getHandSize();
        if (size == 0) return;

        int cardW = 100;
        int cardH = 150;
        int spacing = 90;
        if (size > 12) {
            cardW = 80;
            cardH = 120;
            spacing = 70;
        }

        int startX = max(50, (1400 - (size * spacing)) / 2);
        int startY = 900 - cardH - 50;

        hovered = -1;

        for (int i = 0; i < size; i++) {
            string cardName = curr.getCardAt(i);
            if (cardName.empty()) continue;

            int x = startX + i * spacing;
            int y = startY;
            bool isHover = false;

            Vector2 mouse = GetMousePosition();
            if (mouse.x >= x && mouse.x <= x + cardW && mouse.y >= y && mouse.y <= y + cardH) {
                isHover = true;
                hovered = i;
                y -= 20;
            }

            bool isSelected = (i == selected);
            drawCard(cardName, x, y, cardW, cardH, isHover || isSelected);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHover) {
                if (selected == i) {
                    selected = -1;
                } else {
                    selected = i;
                }
            }
        }

        Button drawBtn(1200, 700, 150, 50, "DRAW CARD");
        drawBtn.draw();
        if (drawBtn.clicked()) {
            game->drawForCurrent();
            message = "Drew a card";
            msgTime = 2;
            selected = -1;
        }

        if (selected >= 0) {
            Button playBtn(1200, 600, 150, 50, "PLAY");
            playBtn.draw();
            if (playBtn.clicked()) {
                string cardName = curr.getCardAt(selected);
                if (cardName.empty()) {
                    message = "Invalid card selection!";
                    msgTime = 2;
                    selected = -1;
                    return;
                }

                Card c(cardName);

                if (c.isWild()) {
                    string msg;
                    if (game->play(cp, selected, msg)) {
                        selected = -1;
                        selectedCardIndex = selected;
                        choosingColor = true;
                    }
                    message = msg;
                    msgTime = 2;
                } else {
                    string msg;
                    if (game->play(cp, selected, msg)) {
                        selected = -1;
                    }
                    message = msg;
                    msgTime = 2;
                }
            }
        }
    }

    void drawColorChoice() {
        DrawRectangle(0, 0, 1400, 900, {0,0,0,180});
        DrawText("Choose Color for Wild", 500, 200, 40, WHITE);

        Button red(400, 400, 150, 150, "RED");
        Button blue(600, 400, 150, 150, "BLUE");
        Button green(800, 400, 150, 150, "GREEN");
        Button yellow(1000, 400, 150, 150, "YELLOW");

        red.draw();    DrawRectangle(420, 420, 110, 110, RED);
        blue.draw();   DrawRectangle(620, 420, 110, 110, BLUE);
        green.draw();  DrawRectangle(820, 420, 110, 110, GREEN);
        yellow.draw(); DrawRectangle(1020, 420, 110, 110, YELLOW);

        if (red.clicked()) finishWild("RED");
        if (blue.clicked()) finishWild("BLUE");
        if (green.clicked()) finishWild("GREEN");
        if (yellow.clicked()) finishWild("YELLOW");
    }

    void finishWild(string color) {
        game->chooseColor(color);

        message = "Color set to " + color;
        msgTime = 2;

        choosingColor = false;
    }

    void drawGameOver() {
        DrawRectangle(0, 0, 1400, 900, {0, 0, 0, 200});

        int winnerIdx = -1;
        for (int i = 0; i < 4; i++) {
            if (game->getPlayer(i).hasWon()) {
                winnerIdx = i;
                break;
            }
        }

        if (winnerIdx == -1) winnerIdx = 0;

        Player winner = game->getPlayer(winnerIdx);

        DrawText("GAME OVER!", 500, 100, 60, YELLOW);
        DrawText(TextFormat("%s WINS THE GAME!", winner.name.c_str()),
                700 - MeasureText(TextFormat("%s WINS THE GAME!", winner.name.c_str()), 50)/2,
                200, 50, GREEN);

        DrawText(TextFormat("Final Score: %d points", winner.totalScore),
                700 - MeasureText(TextFormat("Final Score: %d points", winner.totalScore), 30)/2,
                270, 30, YELLOW);

        int yPos = 350;
        DrawText("FINAL SCORES:", 700 - MeasureText("FINAL SCORES:", 28)/2, yPos, 28, WHITE);
        yPos += 50;

        for (int i = 0; i < 4; i++) {
            Player p = game->getPlayer(i);
            string scoreText = p.name + ": " + to_string(p.totalScore) + " pts";
            Color textColor = (i == winnerIdx) ? GOLD : YELLOW;

            int width = MeasureText(scoreText.c_str(), 24);
            DrawText(scoreText.c_str(), 700 - width/2, yPos, 24, textColor);
            yPos += 40;
        }

        Button playAgain(500, 600, 200, 60, "PLAY AGAIN");
        Button mainMenu(700, 600, 200, 60, "MAIN MENU");
        Button exitGame(900, 600, 200, 60, "EXIT");

        playAgain.draw();
        mainMenu.draw();
        exitGame.draw();

        if (playAgain.clicked()) {
            game->reset();
        }
        if (mainMenu.clicked()) {
            game->reset();
        }
        if (exitGame.clicked()) {
            CloseWindow();
        }
    }
};

int main() {
    srand(time(0));
    Game game;
    UNOGUI gui(&game);
    gui.run();
    return 0;
}
