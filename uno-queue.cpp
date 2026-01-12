#include <iostream>
#include <string>
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

const int WINNING_SCORE = 500;
const int WILD_SCORE = 50;
const int ACTION_SCORE = 20;


template <typename T>
class queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* frontPtr;
    Node* rearPtr;
    int queueSize;

public:

    queue() : frontPtr(nullptr), rearPtr(nullptr), queueSize(0) {}


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
            size_t pos = cardName.find('_');
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
    Card(string n) {

        if (n.find("Draw_2") != string::npos) {
            size_t pos = n.find("Draw_2");
            if (pos != string::npos) {
                n.replace(pos, 6, "DRAW_TWO");
            }
        }
        name = n;
    }

    string getColor() const {
        if (name == "WILD" || name == "WILD_DRAW_FOUR") return "WILD";

        size_t pos = name.find('_');
        if (pos == string::npos) return "";

        return name.substr(0, pos);
    }

    string getValue() const {
        if (name == "WILD") return "WILD";
        if (name == "WILD_DRAW_FOUR") return "DRAW_FOUR";

        size_t pos = name.find('_');
        if (pos == string::npos) return name;

        string value = name.substr(pos + 1);
        return value;
    }

    bool isWild() const {
        return name == "WILD" || name == "WILD_DRAW_FOUR";
    }

    bool isActionCard() const {
        string val = getValue();
        return val == "SKIP" || val == "REVERSE" || val == "DRAW_TWO";
    }

    bool isNumberCard() const {
        string val = getValue();
        return val >= "0" && val <= "9";
    }

    int getScore() const {
        if (isWild()) return WILD_SCORE;
        if (isActionCard()) return ACTION_SCORE;
        if (isNumberCard()) return stoi(getValue());
        return 0;
    }

    bool canPlayOn(const Card& top, const string& activeColor) const {
        if (isWild()) return true;

        string myColor = getColor();
        string myValue = getValue();
        string topValue = top.getValue();

        if (myColor == activeColor) return true;
        if (myValue == topValue) return true;

        return false;
    }
};

class Player {
public:
    int id;
    string name;
    int totalScore;
    queue<string> hand;
    bool unoDeclared;

    Player() : id(0), totalScore(0), name(""), unoDeclared(false) {}
    Player(int pid, string n) : id(pid), totalScore(0), name(n), unoDeclared(false) {}

    void addCard(string cardName) {
        hand.push(cardName);
        unoDeclared = false;
    }

    string playCard(int index) {
        if (index < 0 || index >= hand.size()) return "";

        string card = hand.removeAt(index);
        if (hand.size() == 1 && !unoDeclared) {
            unoDeclared = true;
        }
        return card;
    }

    string getCardAt(int index) const {
        return hand.getAt(index);
    }

    int getHandSize() const {
        return hand.size();
    }

    int getHandScore() const {
        int score = 0;
        for (int i = 0; i < hand.size(); i++) {
            string cardName = hand.getAt(i);
            if (!cardName.empty()) {
                Card c(cardName);
                score += c.getScore();
            }
        }
        return score;
    }

    bool hasWon() const {
        return totalScore >= WINNING_SCORE;
    }

    void resetHand() {
        hand.clear();
        unoDeclared = false;
    }

    bool hasColor(const string& color) const {
        for (int i = 0; i < hand.size(); i++) {
            string cardName = hand.getAt(i);
            if (!cardName.empty()) {
                Card c(cardName);
                if (c.getColor() == color) return true;
            }
        }
        return false;
    }

    bool shouldDeclareUno() const {
        return hand.size() == 1 && !unoDeclared;
    }

    void declareUno() {
        if (hand.size() == 1) {
            unoDeclared = true;
        }
    }

    bool hasDeclaredUno() const {
        return unoDeclared;
    }
};

class Deck {
private:
    queue<string> drawPile;
    queue<string> discardPile;

    void moveDiscardToDraw() {
        if (discardPile.size() <= 1) return;


        string topCard = "";


        queue<string> tempQueue;


        while (!discardPile.empty()) {
            string card = discardPile.front();
            discardPile.pop();

            if (discardPile.empty()) {

                topCard = card;
            } else {
                tempQueue.push(card);
            }
        }

        int tempSize = tempQueue.size();
        if (tempSize > 0) {
            string* cardsArray = tempQueue.copyToArray();


            for (int i = tempSize - 1; i > 0; i--) {
                int j = rand() % (i + 1);
                swap(cardsArray[i], cardsArray[j]);
            }


            drawPile.loadFromArray(cardsArray, tempSize);
            delete[] cardsArray;
        }


        if (!topCard.empty()) {
            discardPile.push(topCard);
        }
    }

public:
    void createDeck() {

        drawPile.clear();
        discardPile.clear();


        queue<string> tempCards;
        string colors[4] = {"RED", "GREEN", "BLUE", "YELLOW"};

        for (int c = 0; c < 4; c++) {
            string color = colors[c];
            tempCards.push(color + "_0");

            for (int n = 1; n <= 9; n++) {
                string num = to_string(n);
                tempCards.push(color + "_" + num);
                tempCards.push(color + "_" + num);
            }

            tempCards.push(color + "_SKIP");
            tempCards.push(color + "_SKIP");
            tempCards.push(color + "_REVERSE");
            tempCards.push(color + "_REVERSE");
            tempCards.push(color + "_DRAW_TWO");
            tempCards.push(color + "_DRAW_TWO");
        }

        for (int i = 0; i < 4; i++) {
            tempCards.push("WILD");
            tempCards.push("WILD_DRAW_FOUR");
        }


        int totalCards = tempCards.size();
        string* allCards = tempCards.copyToArray();


        for (int i = totalCards - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(allCards[i], allCards[j]);
        }


        drawPile.loadFromArray(allCards, totalCards);
        delete[] allCards;
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
        string lastCard = "";

        while (!temp.empty()) {
            lastCard = temp.front();
            temp.pop();
        }
        return lastCard;
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

    string roundMessage;
    float roundMessageTimer;
    int roundWinner;


    bool gameOver;


    string lastPlayedWildCard;

    bool justDrewCard;
    string lastDrawnCard;
    int lastDrawnCardIndex;


    float unoMessageTimer;
    string unoMessage;

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
        roundMessage = "";
        roundMessageTimer = 0;
        roundWinner = -1;
        gameOver = false;
        lastPlayedWildCard = "";
        justDrewCard = false;
        lastDrawnCard = "";
        lastDrawnCardIndex = -1;
        unoMessageTimer = 0;
        unoMessage = "";
    }

    void start() {
        startNewRound();
    }

    void handleUnoDeclaration() {
        Player& p = players[currentPlayer];
        if (p.shouldDeclareUno()) {
            p.declareUno();
            unoMessage = p.name + " says UNO!";
            unoMessageTimer = 2.0f;
        }
    }

    void handleUNOPenalty() {
        for (int i = 0; i < 4; i++) {
            if (i == currentPlayer) continue;

            Player& p = players[i];
            if (p.getHandSize() == 1 && !p.hasDeclaredUno()) {

                unoMessage = p.name + " didn't say UNO! Drawing 2 penalty cards.";
                unoMessageTimer = 3.0f;


                for (int j = 0; j < 2; j++) {
                    p.addCard(deck.draw());
                }


                p.unoDeclared = false;
                return;
            }
        }
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


        handleUnoDeclaration();

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


        handleUnoDeclaration();

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
            advanceTurn();
            msg = "Played: " + playedName + " - Direction reversed";
        }
        else {
            advanceTurn();
            msg = "Played: " + playedName;
        }


        handleUNOPenalty();

        justDrewCard = false;
        lastDrawnCardIndex = -1;
        return true;
    }

    void advanceTurn() {
        if (clockwise) {
            currentPlayer = (currentPlayer + 1) % 4;
        } else {
            currentPlayer = (currentPlayer - 1 + 4) % 4;
        }
        justDrewCard = false;
        lastDrawnCardIndex = -1;
    }

    string drawCardForCurrent() {
        Player& p = players[currentPlayer];
        string drawnCard = deck.draw();
        if (!drawnCard.empty()) {
            p.addCard(drawnCard);
            lastDrawnCard = drawnCard;
            lastDrawnCardIndex = p.getHandSize() - 1;
            justDrewCard = true;
        }
        return drawnCard;
    }

    void skipAfterDraw() {
        justDrewCard = false;
        lastDrawnCardIndex = -1;
        advanceTurn();
    }

    bool playDrawnCard() {
        if (!justDrewCard || lastDrawnCard.empty() || lastDrawnCardIndex == -1) return false;

        Player& p = players[currentPlayer];
        Card drawnCard(lastDrawnCard);
        Card top(deck.topDiscard());

        if (drawnCard.canPlayOn(top, activeColor)) {
            string msg;
            bool result = play(currentPlayer, lastDrawnCardIndex, msg);
            if (result) {

                handleUNOPenalty();
            }
            return result;
        }
        return false;
    }

    bool canPlayCard(int playerIdx, int cardIndex) const {
        if (playerIdx != currentPlayer) return false;

        if (justDrewCard) {
            return cardIndex == lastDrawnCardIndex;
        }

        return true;
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


            handleUNOPenalty();
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
    bool getJustDrewCard() const { return justDrewCard; }
    bool isClockwise() const { return clockwise; }

    float getUnoMessageTimer() const { return unoMessageTimer; }
    string getUnoMessage() const { return unoMessage; }

    bool canPlayCardIndex(int playerIdx, int cardIndex) const {
        return canPlayCard(playerIdx, cardIndex);
    }

    int getLastDrawnCardIndex() const { return lastDrawnCardIndex; }

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

        if (unoMessageTimer > 0) {
            unoMessageTimer -= deltaTime;
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
        roundMessageTimer = 0;
        challengePending = false;
        lastPlayedWildCard = "";
        justDrewCard = false;
        lastDrawnCard = "";
        lastDrawnCardIndex = -1;
        unoMessageTimer = 0;
        unoMessage = "";
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
    queue<pair<string, Texture2D>> cardTextures;
    int selected = -1;
    int hovered = -1;
    string message = "";
    float msgTime = 0;
    bool choosingColor = false;
    int selectedCardIndex = -1;

    Texture2D findTexture(const string& name) {
        for (int i = 0; i < cardTextures.size(); i++) {
            auto pair = cardTextures.getAt(i);
            if (pair.first == name) {
                return pair.second;
            }
        }

        string upperName = name;
        for (char& c : upperName) c = toupper(c);

        for (int i = 0; i < cardTextures.size(); i++) {
            auto pair = cardTextures.getAt(i);
            string upperPair = pair.first;
            for (char& c : upperPair) c = toupper(c);
            if (upperPair == upperName) {
                return pair.second;
            }
        }

        if (name.find("DRAW_TWO") != string::npos) {
            for (int i = 0; i < cardTextures.size(); i++) {
                auto pair = cardTextures.getAt(i);
                if (pair.first.find("DRAW") != string::npos || pair.first.find("Draw") != string::npos) {
                    return pair.second;
                }
            }
        }

        return cardBack;
    }

    void loadImages() {
        string path = "assets/Cards/";
        string colors[4] = {"Red", "Green", "Blue", "Yellow"};
        string values[13] = {"0","1","2","3","4","5","6","7","8","9",
                           "Skip","Reverse","Draw_2"};

        for (int c = 0; c < 4; c++) {
            for (int v = 0; v < 13; v++) {
                string internalCardName;
                string colorUpper = colors[c];
                for (char& ch : colorUpper) ch = toupper(ch);

                string valueUpper = values[v];
                if (valueUpper == "Draw_2") valueUpper = "DRAW_TWO";
                else if (valueUpper == "Skip") valueUpper = "SKIP";
                else if (valueUpper == "Reverse") valueUpper = "REVERSE";
                for (char& ch : valueUpper) ch = toupper(ch);

                internalCardName = colorUpper + "_" + valueUpper;
                string fileName = colors[c] + "_" + values[v] + ".jpg";
                string filePath = path + fileName;

                if (FileExists(filePath.c_str())) {
                    Texture2D tex = LoadTexture(filePath.c_str());
                    cardTextures.push({internalCardName, tex});
                } else {
                    Color cardColor;
                    if (colors[c] == "Red") cardColor = RED;
                    else if (colors[c] == "Green") cardColor = GREEN;
                    else if (colors[c] == "Blue") cardColor = BLUE;
                    else cardColor = YELLOW;

                    Image img = GenImageColor(120, 180, cardColor);
                    string displayValue = values[v];
                    if (displayValue == "Draw_2") displayValue = "+2";
                    else if (displayValue == "Skip") displayValue = "S";
                    else if (displayValue == "Reverse") displayValue = "R";

                    int fontSize = 40;
                    if (displayValue.length() > 2) fontSize = 30;

                    int textWidth = MeasureText(displayValue.c_str(), fontSize);
                    ImageDrawText(&img, displayValue.c_str(),
                                 60 - textWidth/2, 90 - 20,
                                 fontSize, WHITE);

                    Texture2D tex = LoadTextureFromImage(img);
                    cardTextures.push({internalCardName, tex});
                    UnloadImage(img);
                }
            }
        }


        string wildFiles[2] = {path + "Wild.jpg", path + "WILD.jpg"};
        bool wildLoaded = false;
        for (int i = 0; i < 2; i++) {
            if (FileExists(wildFiles[i].c_str())) {
                Texture2D tex = LoadTexture(wildFiles[i].c_str());
                cardTextures.push({"WILD", tex});
                wildLoaded = true;
                break;
            }
        }
        if (!wildLoaded) {
            Image img = GenImageColor(120, 180, BLACK);
            ImageDrawText(&img, "WILD", 25, 70, 40, WHITE);
            Texture2D tex = LoadTextureFromImage(img);
            cardTextures.push({"WILD", tex});
            UnloadImage(img);
        }


        string wildFourFiles[2] = {path + "Wild_Draw_4.jpg", path + "WILD_DRAW_FOUR.jpg"};
        bool wildFourLoaded = false;
        for (int i = 0; i < 2; i++) {
            if (FileExists(wildFourFiles[i].c_str())) {
                Texture2D tex = LoadTexture(wildFourFiles[i].c_str());
                cardTextures.push({"WILD_DRAW_FOUR", tex});
                wildFourLoaded = true;
                break;
            }
        }
        if (!wildFourLoaded) {
            Image img = GenImageColor(120, 180, DARKPURPLE);
            ImageDrawText(&img, "WILD", 25, 50, 40, WHITE);
            ImageDrawText(&img, "DRAW 4", 15, 100, 30, WHITE);
            Texture2D tex = LoadTextureFromImage(img);
            cardTextures.push({"WILD_DRAW_FOUR", tex});
            UnloadImage(img);
        }


        string backFiles[2] = {path + "back.jpg", path + "BACK.jpg"};
        bool backLoaded = false;
        for (int i = 0; i < 2; i++) {
            if (FileExists(backFiles[i].c_str())) {
                cardBack = LoadTexture(backFiles[i].c_str());
                backLoaded = true;
                break;
            }
        }
        if (!backLoaded) {
            Image img = GenImageColor(120, 180, Color{46, 125, 50, 255});
            for (int y = 0; y < 180; y += 20) {
                for (int x = 0; x < 120; x += 20) {
                    if ((x/20 + y/20) % 2 == 0) {
                        ImageDrawRectangle(&img, x, y, 20, 20, Color{255, 87, 34, 255});
                    } else {
                        ImageDrawRectangle(&img, x, y, 20, 20, Color{33, 150, 243, 255});
                    }
                }
            }
            ImageDrawText(&img, "UNO", 25, 70, 50, WHITE);
            cardBack = LoadTextureFromImage(img);
            UnloadImage(img);
        }
    }

    void drawCard(const string& name, float x, float y, float w, float h, bool highlight, bool disabled = false) {
        Texture2D tex = findTexture(name);
        if (tex.id == 0) {
            Card c(name);
            Color cardColor;
            string colorStr = c.getColor();

            if (colorStr == "RED") cardColor = RED;
            else if (colorStr == "GREEN") cardColor = GREEN;
            else if (colorStr == "BLUE") cardColor = BLUE;
            else if (colorStr == "YELLOW") cardColor = YELLOW;
            else if (c.isWild()) cardColor = BLACK;
            else cardColor = GRAY;

            if (disabled) {
                cardColor = Color{
                    (unsigned char)(cardColor.r * 0.5f),
                    (unsigned char)(cardColor.g * 0.5f),
                    (unsigned char)(cardColor.b * 0.5f),
                    cardColor.a
                };
            }

            DrawRectangle(x, y, w, h, cardColor);

            string val = c.getValue();
            if (val == "DRAW_TWO") val = "+2";
            else if (val == "DRAW_FOUR") val = "+4";

            int fontSize = 30;
            if (val.length() > 2) fontSize = 20;

            int textWidth = MeasureText(val.c_str(), fontSize);
            DrawText(val.c_str(), x + w/2 - textWidth/2, y + h/2 - 15, fontSize, disabled ? GRAY : WHITE);

            DrawRectangleLines(x, y, w, h, BLACK);
        } else {
            Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
            Rectangle dest = {x, y, w, h};
            Color tint = disabled ? GRAY : WHITE;
            DrawTexturePro(tex, src, dest, {0,0}, 0, tint);
        }

        if (highlight) {
            DrawRectangleLines(x-3, y-3, w+6, h+6, YELLOW);
        }

        if (disabled) {
            DrawLine(x, y, x + w, y + h, RED);
            DrawLine(x + w, y, x, y + h, RED);
        }
    }

    void drawBack(float x, float y, float w, float h) {
        if (cardBack.id != 0) {
            Rectangle src = {0,0,(float)cardBack.width,(float)cardBack.height};
            Rectangle dest = {x,y,w,h};
            DrawTexturePro(cardBack, src, dest, {0,0}, 0, WHITE);
        } else {
            DrawRectangle(x, y, w, h, Color{46, 125, 50, 255});
            DrawRectangleLines(x, y, w, h, WHITE);
            DrawText("UNO", x + 20, y + 60, 40, YELLOW);
        }
    }

public:
    UNOGUI(Game* g) : game(g) {
        InitWindow(1400, 900, "UNO - 4 Players");
        SetTargetFPS(60);
        loadImages();
    }

    ~UNOGUI() {
        for (int i = 0; i < cardTextures.size(); i++) {
            auto pair = cardTextures.getAt(i);
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
            ClearBackground(Color{48, 25, 52, 255});

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


        DrawRectangle(0, 0, 1400, 50, {0, 0, 0, 150});
        DrawText("SCORES:", 20, 15, 24, YELLOW);

        int scoreX = 150;
        for (int i = 0; i < 4; i++) {
            Player p = game->getPlayer(i);
            string scoreText = p.name + ": " + to_string(p.totalScore) + " pts";
            Color textColor = (i == cp) ? YELLOW : WHITE;
            DrawText(scoreText.c_str(), scoreX, 15, 20, textColor);
            scoreX += 300;
        }


        DrawRectangle(50, 100, 400, 350, {0, 0, 0, 180});
        DrawRectangleLines(50, 100, 400, 350, YELLOW);

        DrawText(TextFormat("Player %d's Turn", cp + 1), 70, 120, 30, WHITE);

        string activeColorText = "Active Color: " + game->getColor();
        Color activeColor = WHITE;
        string colorStr = game->getColor();
        if (colorStr == "RED") activeColor = RED;
        else if (colorStr == "BLUE") activeColor = BLUE;
        else if (colorStr == "GREEN") activeColor = GREEN;
        else if (colorStr == "YELLOW") activeColor = YELLOW;

        DrawText("Active Color:", 70, 160, 20, WHITE);
        DrawText(colorStr.c_str(), 200, 160, 20, activeColor);

        string directionText = "Direction: " + string(game->isClockwise() ? "Clockwise" : "Counter-Clockwise");
        DrawText(directionText.c_str(), 70, 190, 20, WHITE);

        DrawLine(70, 220, 420, 220, WHITE);

        DrawText("Player Cards:", 70, 240, 22, YELLOW);

        int playerInfoY = 270;
        for (int i = 0; i < 4; i++) {
            Player p = game->getPlayer(i);
            string cardText = p.name + ": " + to_string(p.getHandSize()) + " cards";
            Color textColor = (i == cp) ? YELLOW : WHITE;

            if (p.getHandSize() == 1) {
                DrawRectangle(70, playerInfoY - 5, 350, 30, {255, 0, 0, 100});
                if (p.hasDeclaredUno()) {
                    cardText += " - UNO!";
                } else {
                    cardText += " - SAY UNO!";
                }
            }

            DrawText(cardText.c_str(), 80, playerInfoY, 20, textColor);
            playerInfoY += 40;
        }


        if (game->getUnoMessageTimer() > 0) {
            string unoMsg = game->getUnoMessage();
            DrawRectangle(500, 200, 400, 60, {0, 0, 0, 200});
            DrawRectangleLines(500, 200, 400, 60, RED);
            DrawText(unoMsg.c_str(), 700 - MeasureText(unoMsg.c_str(), 30)/2, 215, 30, YELLOW);
        }


        drawBack(500, 300, 120, 180);
        DrawText("Draw Deck", 515, 500, 20, WHITE);


        string top = game->getTop();
        if (top != "") {
            drawCard(top, 650, 300, 120, 180, false);
            DrawText("Discard Pile", 660, 500, 20, WHITE);
        }

        int size = curr.getHandSize();
        if (size == 0) return;


        int cardW, cardH, spacing;
        if (size <= 8) {
            cardW = 100;
            cardH = 150;
            spacing = 110;
        } else if (size <= 12) {
            cardW = 90;
            cardH = 135;
            spacing = 95;
        } else if (size <= 16) {
            cardW = 80;
            cardH = 120;
            spacing = 85;
        } else {
            cardW = 70;
            cardH = 105;
            spacing = 75;
        }

        int startX = max(400, (1400 - (size * spacing)) / 2);
        int startY = 900 - cardH - 50;

        hovered = -1;

        for (int i = 0; i < size; i++) {
            string cardName = curr.getCardAt(i);
            if (cardName.empty()) continue;

            int x = startX + i * spacing;
            int y = startY;
            bool isHover = false;

            bool canPlay = game->canPlayCardIndex(cp, i);

            Vector2 mouse = GetMousePosition();
            if (mouse.x >= x && mouse.x <= x + cardW && mouse.y >= y && mouse.y <= y + cardH) {
                if (canPlay) {
                    isHover = true;
                    hovered = i;
                    y -= 20;
                }
            }

            bool isSelected = (i == selected);
            drawCard(cardName, x, y, cardW, cardH, isHover || isSelected, !canPlay);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHover && canPlay) {
                if (selected == i) {
                    selected = -1;
                } else {
                    selected = i;
                }
            }
        }

        bool showUNOButton = curr.shouldDeclareUno();

        if (showUNOButton) {
            Button unoBtn(1150, 100, 200, 50, "SAY UNO!");
            unoBtn.drawWithColor(RED);
            if (unoBtn.clicked()) {
                curr.declareUno();
                message = "UNO declared!";
                msgTime = 2;
            }
        } else {

            Button checkUNOBtn(1150, 100, 200, 50, "CHECK UNO");
            checkUNOBtn.drawWithColor(ORANGE);
            if (checkUNOBtn.clicked()) {

                message = "Checking for UNO violations...";
                msgTime = 1;
            }
        }

        if (game->getJustDrewCard()) {
            Button playDrawnBtn(1150, 170, 200, 50, "PLAY DRAWN CARD");
            playDrawnBtn.drawWithColor(Color{216, 191, 216, 255});

            Button skipBtn(1150, 240, 200, 50, "SKIP TURN");
            skipBtn.drawWithColor(ORANGE);

            if (playDrawnBtn.clicked()) {
                if (game->playDrawnCard()) {
                    message = "Played drawn card";
                    selected = -1;
                } else {
                    message = "Cannot play drawn card";
                }
                msgTime = 2;
            }

            if (skipBtn.clicked()) {
                game->skipAfterDraw();
                message = "Turn skipped";
                msgTime = 2;
            }
        } else {
            Button drawBtn(1150, 170, 200, 50, "DRAW CARD");
            drawBtn.drawWithColor(Color{216, 191, 216, 255});
            if (drawBtn.clicked()) {
                string drawnCard = game->drawCardForCurrent();
                if (!drawnCard.empty()) {
                    message = "Drew a card";
                    msgTime = 2;
                    selected = -1;

                    Card c(drawnCard);
                    Card top(game->getTop());
                    if (c.canPlayOn(top, game->getColor())) {
                        message += " (can be played)";
                    }
                }
            }
        }

        if (selected >= 0 && game->canPlayCardIndex(cp, selected)) {
            Button playBtn(1150, 310, 200, 50, "PLAY SELECTED");
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


        int drawCount = game->drawSize();
        string drawText = "Cards left: " + to_string(drawCount);
        DrawText(drawText.c_str(), 525, 550, 20, WHITE);
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

    void drawRoundSummary() {
        DrawRectangle(0, 0, 1400, 900, {0, 0, 0, 200});

        Rectangle box = {400, 200, 600, 400};
        DrawRectangleRec(box, {30, 30, 30, 240});
        DrawRectangleLinesEx(box, 3, YELLOW);

        DrawText("ROUND SUMMARY", 500, 230, 40, YELLOW);

        string summary = game->getRoundMessage();


        int lineCount = 0;
        for (char c : summary) {
            if (c == '\n') lineCount++;
        }
        lineCount++;

        string* lines = new string[lineCount];
        int currentLine = 0;
        string currentStr = "";

        for (char c : summary) {
            if (c == '\n') {
                lines[currentLine++] = currentStr;
                currentStr = "";
            } else {
                currentStr += c;
            }
        }
        if (!currentStr.empty()) {
            lines[currentLine] = currentStr;
        }

        int lineY = 300;
        for (int i = 0; i < lineCount; i++) {
            int width = MeasureText(lines[i].c_str(), 24);
            DrawText(lines[i].c_str(), 700 - width/2, lineY, 24, WHITE);
            lineY += 35;
        }

        delete[] lines;

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
