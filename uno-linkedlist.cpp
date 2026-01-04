#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>

using namespace std;

mt19937 rng(random_device{}());

int randomValue(int a, int b) {
    uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

const int CARD_WIDTH = 110;
const int CARD_HEIGHT = 160;

template <typename T>
class Node {
    T data;
    Node<T>* next;
    Node<T>* prev;

public:
    Node(T d) : data(d), next(nullptr), prev(nullptr) {}

    T& getData() { return data; }
    void setData(T data) { this->data = data; }
    Node<T>* getNext() { return next; }
    Node<T>* getPrev() { return prev; }
    void setNext(Node<T>* next) { this->next = next; }
    void setPrev(Node<T>* prev) { this->prev = prev; }
};

template <typename T>
class LinkedList {
    Node<T>* head;
    Node<T>* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
        if (other.isEmpty()) return;
        Node<T>* current = other.head;
        do {
            insertEnd(current->getData());
            current = current->getNext();
        } while (current != other.head);
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this == &other) return *this;
        clear();
        if (!other.isEmpty()) {
            Node<T>* current = other.head;
            do {
                insertEnd(current->getData());
                current = current->getNext();
            } while (current != other.head);
        }
        return *this;
    }

    bool isEmpty() const { return head == nullptr; }

    void insertFront(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (isEmpty()) {
            head = tail = newNode;
            head->setNext(head);
            head->setPrev(head);
            return;
        }
        newNode->setNext(head);
        newNode->setPrev(tail);
        head->setPrev(newNode);
        tail->setNext(newNode);
        head = newNode;
    }

    void insertEnd(T val) {
        Node<T>* newNode = new Node<T>(val);
        if (isEmpty()) {
            head = tail = newNode;
            head->setNext(head);
            head->setPrev(head);
            return;
        }
        newNode->setPrev(tail);
        newNode->setNext(head);
        tail->setNext(newNode);
        head->setPrev(newNode);
        tail = newNode;
    }

    void deleteFront() {
        if (isEmpty()) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        Node<T>* temp = head;
        head = head->getNext();
        head->setPrev(tail);
        tail->setNext(head);
        delete temp;
    }

    void deleteEnd() {
        if (isEmpty()) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        Node<T>* temp = tail;
        tail = tail->getPrev();
        tail->setNext(head);
        head->setPrev(tail);
        delete temp;
    }

    T deleteAt(int pos) {
        if (head == nullptr) {
            cout << "List is empty" << endl;
            return T();
        }
        if (pos < 0) {
            cout << "Invalid Position" << endl;
            return T();
        }
        if (pos == 0) {
            T toReturn = head->getData();
            deleteFront();
            return toReturn;
        }

        Node<T>* temp = head;
        int count = 0;
        while (count < pos - 1) {
            temp = temp->getNext();
            count++;
            if (temp == head) {
                cout << "Position out of range" << endl;
                return T();
            }
        }

        Node<T>* toDelete = temp->getNext();
        if (toDelete == head) {
            cout << "Position out of range" << endl;
            return T();
        }

        T toReturn = toDelete->getData();
        temp->setNext(toDelete->getNext());
        if (toDelete->getNext() != nullptr) {
            toDelete->getNext()->setPrev(temp);
        }
        if (toDelete == tail) {
            tail = temp;
            if (head != nullptr) {
                head->setPrev(tail);
            }
        }
        if (toDelete->getNext() == toDelete) {
            head = tail = nullptr;
        }

        toDelete->setNext(nullptr);
        toDelete->setPrev(nullptr);
        delete toDelete;
        return toReturn;
    }

    Node<T>* getHead() const { return head; }
    Node<T>* getTail() const { return tail; }

    ~LinkedList() { clear(); }

    void clear() {
        if (!head) return;
        tail->setNext(nullptr);
        head->setPrev(nullptr);
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* next = current->getNext();
            delete current;
            current = next;
        }
        head = nullptr;
        tail = nullptr;
    }
};

enum CardColor { CARD_RED, CARD_GREEN, CARD_BLUE, CARD_YELLOW, WILD };
enum CardType { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD_COLOR, WILD_DRAW_FOUR };
enum TurnState { WAITING_FOR_INPUT, WAITING_FOR_WILD_COLOR, TURN_FINISHED, GAME_OVER };
enum Screen { SCREEN_MAIN_MENU, SCREEN_GAMEPLAY, SCREEN_EXIT };

class Card {
public:
    CardColor color;
    CardType type;
    int number;
    int score;
    Card() : color(WILD), type(WILD_COLOR), number(-1), score(0) {}
    Card(CardColor c, CardType t, int s = 0, int n = -1) : color(c), type(t), number(n), score(s) {}

    string getColorString() const {
        switch (color) {
            case CARD_RED:    return "Red";
            case CARD_GREEN:  return "Green";
            case CARD_BLUE:   return "Blue";
            case CARD_YELLOW: return "Yellow";
            case WILD:        return "Wild";
            default:          return "Unknown";
        }
    }

    string toString() const {
        string colorStr = getColorString();
        string typeStr;
        switch (type) {
            case NUMBER:         typeStr = to_string(number); break;
            case SKIP:           typeStr = "Skip"; break;
            case REVERSE:        typeStr = "Reverse"; break;
            case DRAW_TWO:       typeStr = "Draw Two"; break;
            case WILD_COLOR:     typeStr = "Wild Color"; break;
            case WILD_DRAW_FOUR: typeStr = "Wild Draw Four"; break;
        }
        return colorStr + " " + typeStr;
    }
};

string GetCardTexturePath(Card card) {
    if (card.type == WILD_COLOR)     return "assets/Wild.jpg";
    if (card.type == WILD_DRAW_FOUR) return "assets/Wild_Draw_4.jpg";

    string color;
    switch (card.color) {
        case CARD_RED:    color = "Red";    break;
        case CARD_BLUE:   color = "Blue";   break;
        case CARD_GREEN:  color = "Green";  break;
        case CARD_YELLOW: color = "Yellow"; break;
        default: break;
    }

    string value;
    switch (card.type) {
        case NUMBER:   value = to_string(card.number); break;
        case SKIP:     value = "Skip";     break;
        case REVERSE:  value = "Reverse";  break;
        case DRAW_TWO: value = "Draw_2";   break;
        default: break;
    }

    return "assets/" + color + "_" + value + ".jpg";
}

class Deck {
    LinkedList<Card> cards;

public:
    Deck() {
        CardColor colors[] = {CARD_RED, CARD_GREEN, CARD_BLUE, CARD_YELLOW};
        for (CardColor color : colors) {
            cards.insertEnd(Card(color, NUMBER, 0, 0));
            for (int i = 1; i <= 9; ++i) {
                cards.insertEnd(Card(color, NUMBER, i, i));
                cards.insertEnd(Card(color, NUMBER, i, i));
            }
            cards.insertEnd(Card(color, SKIP, 20));
            cards.insertEnd(Card(color, SKIP, 20));
            cards.insertEnd(Card(color, REVERSE, 20));
            cards.insertEnd(Card(color, REVERSE, 20));
            cards.insertEnd(Card(color, DRAW_TWO, 20));
            cards.insertEnd(Card(color, DRAW_TWO, 20));
        }
        for (int i = 0; i < 4; ++i) {
            cards.insertEnd(Card(WILD, WILD_COLOR, 50));
            cards.insertEnd(Card(WILD, WILD_DRAW_FOUR, 50));
        }
    }

    Card drawCard() {
        Node<Card>* headNode = cards.getHead();
        if (headNode == nullptr) {
            cout << "Deck is empty" << endl;
            return Card();
        }
        Card drawnCard = headNode->getData();
        cards.deleteFront();
        return drawnCard;
    }

    void insertCardToBottom(Card c) { cards.insertEnd(c); }

    bool isEmpty() { return cards.getHead() == nullptr; }

    void shuffle() {
        if (cards.getHead() == nullptr || cards.getHead()->getNext() == cards.getHead())
            return;

        vector<Card> tempCards;
        Node<Card>* current = cards.getHead();
        do {
            tempCards.push_back(current->getData());
            current = current->getNext();
        } while (current != cards.getHead());

        for (size_t i = tempCards.size() - 1; i > 0; --i) {
            size_t j = randomValue(0, (int)i);
            swap(tempCards[i], tempCards[j]);
        }

        current = cards.getHead();
        for (const Card& c : tempCards) {
            current->setData(c);
            current = current->getNext();
        }
    }
};

class DiscardPile {
    LinkedList<Card> pile;

public:
    void addCard(Card c) { pile.insertEnd(c); }

    Card getTopCard() const {
        Node<Card>* tailNode = pile.getTail();
        if (tailNode == nullptr) {
            cout << "Discard pile is empty" << endl;
            return Card();
        }
        return tailNode->getData();
    }

    void resetIntoDeck(Deck& deck) {
        if (pile.isEmpty()) return;

        Card topCard = getTopCard();
        pile.deleteEnd();

        while (!pile.isEmpty()) {
            Card c = pile.getHead()->getData();
            deck.insertCardToBottom(c);
            pile.deleteFront();
        }

        pile.insertEnd(topCard);
    }

    bool isEmpty() { return pile.isEmpty(); }
};

class Player {
    LinkedList<Card> hand;
    string name;
    int id;
    int score;

public:
    Player() : name(""), id(-1), score(0) {}
    Player(string n, int i) : name(n), id(i), score(0) {}

    string getName() const { return name; }
    int getId() const { return id; }
    int getScore() const { return score; }

    int getHandSize() const {
        if (!hand.getHead()) return 0;
        int count = 0;
        Node<Card>* temp = hand.getHead();
        do {
            count++;
            temp = temp->getNext();
        } while (temp != hand.getHead());
        return count;
    }

    bool hasColor(CardColor color) const {
        Node<Card>* temp = hand.getHead();
        if (!temp) return false;
        do {
            if (temp->getData().color == color) return true;
            temp = temp->getNext();
        } while (temp != hand.getHead());
        return false;
    }

    void addToHand(Card c) { 
        hand.insertEnd(c);
        score += c.score; 
    }

    Card playCard(int index) { 
        Card c =  hand.deleteAt(index); 
        score -= c.score;
        return c;
    }

    Card peekCard(int index) const {
        Node<Card>* temp = hand.getHead();
        int count = 0;
        while (temp != nullptr && count < index) {
            temp = temp->getNext();
            count++;
        }
        if (temp == nullptr) {
            cout << "Invalid Card Index" << endl;
            return Card();
        }
        return temp->getData();
    }
};

class Game {
    bool isGameOver;
    CardColor currentColor;
    Deck deck;
    DiscardPile discardPile;
    LinkedList<Player> players;
    int totalPlayers;
    Node<Player>* currentPlayer;
    int direction;
    TurnState turnState = WAITING_FOR_INPUT;
    CardType pendingWildType;
    bool hasDrawnCard = false;
    Card drawnCard;
    bool canPlayDrawnCard = false;
    int playerScores[4] = {0, 0, 0, 0};

public:
    Game() : isGameOver(false), totalPlayers(4), direction(1) {
        for (int i = 0; i < totalPlayers; i++) {
            players.insertEnd(Player("Player " + to_string(i + 1), i));
        }
        currentPlayer = players.getHead();
    }

    LinkedList<Player>& getPlayers() {
        return players;
    }

    bool getHasDrawnCard() const {
        return hasDrawnCard;
    }

    bool getCanPlayDrawnCard() const {
        return canPlayDrawnCard;
    }

    void start() {
        deck.shuffle();
        for (int i = 0; i < 7; i++) {
            Node<Player>* temp = players.getHead();
            for (int j = 0; j < totalPlayers; j++) {
                temp->getData().addToHand(deck.drawCard());
                temp = temp->getNext();
            }
        }

        Card firstCard = deck.drawCard();
        while (firstCard.color == WILD || firstCard.type == REVERSE ||
               firstCard.type == SKIP || firstCard.type == DRAW_TWO) {
            deck.insertCardToBottom(firstCard);
            deck.shuffle();
            firstCard = deck.drawCard();
        }

        discardPile.addCard(firstCard);
        currentColor = firstCard.color;
    }

    Player& getCurrentPlayer() { return currentPlayer->getData(); }
    Card getTopCard() const { return discardPile.getTopCard(); }
    CardColor getCurrentColor() const { return currentColor; }

    bool isCardValid(Card selectedCard) {
        Card top = discardPile.getTopCard();
        if (selectedCard.color == WILD ||
            selectedCard.type == WILD_COLOR ||
            selectedCard.type == WILD_DRAW_FOUR ||
            selectedCard.color == currentColor ||
            (selectedCard.type == NUMBER && top.type == NUMBER && selectedCard.number == top.number) ||
            (selectedCard.type != NUMBER && selectedCard.type == top.type)) {
            return true;
        }
        return false;
    }

    Card safeDraw() {
        if (deck.isEmpty()) {
            discardPile.resetIntoDeck(deck);
            deck.shuffle();
        }
        return deck.drawCard();
    }

    bool playSpecialCard(Card playedCard) {
    if (playedCard.color != WILD) {
        currentColor = playedCard.color;
    }

    if (playedCard.type == REVERSE) {
        direction *= -1;
        return false;
    }
    
    if (playedCard.type == SKIP) {
        moveToNextPlayer(); 
        moveToNextPlayer();
        return true;
    }
    
    if (playedCard.type == DRAW_TWO) {
        Node<Player>* target = (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();
        target->getData().addToHand(safeDraw());
        target->getData().addToHand(safeDraw());
        moveToNextPlayer();
        moveToNextPlayer(); 
        return true; 
    }
    
    if (playedCard.type == WILD_COLOR || playedCard.type == WILD_DRAW_FOUR) {
        turnState = WAITING_FOR_WILD_COLOR;
        pendingWildType = playedCard.type;
        return false; 
    }
    
    return false; 
}

    bool isWaitingForWildColor() const { return turnState == WAITING_FOR_WILD_COLOR; }

    void chooseWildColor(CardColor color) {
        if (turnState != WAITING_FOR_WILD_COLOR) return;
        currentColor = color;

        if (pendingWildType == WILD_DRAW_FOUR) {
            Node<Player>* target = (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();
            for (int i = 0; i < 4; i++) {
                target->getData().addToHand(safeDraw());
            }
            moveToNextPlayer();
            moveToNextPlayer();
        } else {
            moveToNextPlayer();
        }

        turnState = TURN_FINISHED;
        pendingWildType = NUMBER;
    }

    void beginTurn() {
    turnState = WAITING_FOR_INPUT;
    hasDrawnCard = false;
    canPlayDrawnCard = false;
    drawnCard = Card();
}


    bool isWaitingForInput() const { return turnState == WAITING_FOR_INPUT; }

    void handleDraw() {
    if (hasDrawnCard) return;
    
    Player& player = currentPlayer->getData();
    drawnCard = safeDraw();
    player.addToHand(drawnCard);
    hasDrawnCard = true;
    
    if (isCardValid(drawnCard)) {
        canPlayDrawnCard = true;
    } else {
        canPlayDrawnCard = false;
        moveToNextPlayer();
        turnState = TURN_FINISHED;
    }
}
void passTurn() {
    if (!hasDrawnCard || !canPlayDrawnCard) return;
    
    canPlayDrawnCard = false;
    moveToNextPlayer();
    turnState = TURN_FINISHED;
}

    void advanceTurnIfNeeded() {
        if (turnState == TURN_FINISHED && !isGameOver) {
            beginTurn();
        }
    }


    bool handlePlay(int cardIndex) {
    if (turnState != WAITING_FOR_INPUT || isWaitingForWildColor()) return false;

    Player& player = currentPlayer->getData();

    if (hasDrawnCard) {
        int drawnCardIndex = player.getHandSize() - 1;
        if (cardIndex != drawnCardIndex) {
            return false; 
        }
        if (!canPlayDrawnCard) {
            return false; 
        }
    }

    Card selected = player.peekCard(cardIndex);

    if (!isCardValid(selected)) return false;
    if (selected.type == WILD_DRAW_FOUR && player.hasColor(currentColor)) return false;

    Card playedCard = player.playCard(cardIndex);
    
    discardPile.addCard(playedCard);

    hasDrawnCard = false;
    canPlayDrawnCard = false;

    if (player.getHandSize() == 0) {
        isGameOver = true;
        turnState = GAME_OVER;
        return true;
    }

    bool skipHandled = playSpecialCard(playedCard);
    

    if (!skipHandled && !isWaitingForWildColor()) {
        moveToNextPlayer();
    }


    if (!isWaitingForWildColor()) {
        turnState = TURN_FINISHED;
    }
    
    return true;
}

    bool gameOver() const { return isGameOver; }

    void moveToNextPlayer() {
        currentPlayer = (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();
    }

    void reset() {
        isGameOver = false;
        currentColor = CARD_RED;
        turnState = WAITING_FOR_INPUT;
        direction = 1;
        deck = Deck();
        discardPile = DiscardPile();
        players.clear();

        for (int i = 0; i < totalPlayers; i++) {
            players.insertEnd(Player("Player " + to_string(i + 1), i));
        }
        currentPlayer = players.getHead();

        start();
        beginTurn();
    }
};

// struct CardTextures {
//     unordered_map<string, Texture2D> textures;
//     Texture2D back;
// };

struct CardTextures {
    struct textureEntry {
        char name[50];
        Texture2D tex;
    };
    
    textureEntry cards[60];
    int cardCount;
    Texture2D back;
    
    CardTextures() : cardCount(0) {}
};

class UnoGUI {
    Screen currentScreen;
    Game& game;
    CardTextures cardTextures;
    int hoveredCardIndex = -1;
    Rectangle drawButton;
    Rectangle resetButton;
    Rectangle colorButtons[4];
    const char* colorNames[4] = {"Red", "Green", "Blue", "Yellow"};
    Color colorValues[4] = {RED, GREEN, BLUE, YELLOW};
    Rectangle passButton;
public:
    int screenWidth = 1440;
    int screenHeight = 900;

    UnoGUI(Game& g) : game(g) {
        currentScreen = SCREEN_MAIN_MENU;
        drawButton = {0, 0, 150, 50};
        passButton = {0, 0, 150, 50};

        int btnWidth = 140;
        int btnHeight = 60;
        int centerX = screenWidth / 2 - (btnWidth * 2 + 20) / 2;
        int centerY = screenHeight / 2 - btnHeight / 2;

        colorButtons[0] = {(float)centerX, (float)(centerY - 80), (float)btnWidth, (float)btnHeight};
        colorButtons[1] = {(float)(centerX + btnWidth + 20), (float)(centerY - 80), (float)btnWidth, (float)btnHeight};
        colorButtons[2] = {(float)centerX, (float)(centerY + 20), (float)btnWidth, (float)btnHeight};
        colorButtons[3] = {(float)(centerX + btnWidth + 20), (float)(centerY + 20), (float)btnWidth, (float)btnHeight};
    }

    void loadCardTextures() {
        cardTextures.back = LoadTexture("assets/back.jpg");
        cardTextures.cardCount = 0;

        vector<string> files = {
            "Blue_0.jpg", "Red_Reverse.jpg", "Green_Draw_2.jpg",
            "Wild.jpg", "Wild_Draw_4.jpg", "Red_0.jpg",
            "Red_1.jpg", "Red_2.jpg", "Red_3.jpg", "Red_4.jpg",
            "Red_5.jpg", "Red_6.jpg", "Red_7.jpg", "Red_8.jpg", "Red_9.jpg",
            "Red_Skip.jpg", "Red_Draw_2.jpg",
            "Green_0.jpg", "Green_1.jpg", "Green_2.jpg", "Green_3.jpg", "Green_4.jpg",
            "Green_5.jpg", "Green_6.jpg", "Green_7.jpg", "Green_8.jpg", "Green_9.jpg",
            "Green_Skip.jpg", "Green_Reverse.jpg", "Green_Draw_2.jpg",
            "Blue_1.jpg", "Blue_2.jpg", "Blue_3.jpg", "Blue_4.jpg",
            "Blue_5.jpg", "Blue_6.jpg", "Blue_7.jpg", "Blue_8.jpg", "Blue_9.jpg",
            "Blue_Skip.jpg", "Blue_Reverse.jpg", "Blue_Draw_2.jpg",
            "Yellow_0.jpg", "Yellow_1.jpg", "Yellow_2.jpg", "Yellow_3.jpg", "Yellow_4.jpg",
            "Yellow_5.jpg", "Yellow_6.jpg", "Yellow_7.jpg", "Yellow_8.jpg", "Yellow_9.jpg",
            "Yellow_Skip.jpg", "Yellow_Reverse.jpg", "Yellow_Draw_2.jpg"
        };

        for (auto& f : files) {
        if (cardTextures.cardCount < 60) {
            strcpy(cardTextures.cards[cardTextures.cardCount].name, f.c_str());
            cardTextures.cards[cardTextures.cardCount].tex = LoadTexture(("assets/" + f).c_str());
            cardTextures.cardCount++;
        }
    }

        // for (auto& f : files) {
        //     cardTextures.textures[f] = LoadTexture(("assets/" + f).c_str());
        // }
    }

    void drawCardTexture(const Card& card, int x, int y, bool faceDown = false) {
        Texture2D tex;
        if (faceDown) {
            tex = cardTextures.back;
        } else {
            string path = GetCardTexturePath(card);
            string filename = path.substr(path.find_last_of("/") + 1);
            for (int i = 0; i < cardTextures.cardCount; i++) {
            if (strcmp(cardTextures.cards[i].name, filename.c_str()) == 0) {
                tex = cardTextures.cards[i].tex;
                break;
            }
        }
            // tex = cardTextures.textures[filename];
        }

        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
        Rectangle dst = {(float)x, (float)y, CARD_WIDTH, CARD_HEIGHT};
        Vector2 origin = {0, 0};
        DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
    }

    void init() {
        InitWindow(screenWidth, screenHeight, "UNO - LinkedList Implementation");
        SetTargetFPS(60);
        loadCardTextures();
    }

    void shutdown() { CloseWindow(); }

    void beginFrame() {
        BeginDrawing();
        ClearBackground({200, 30, 30, 255});
    }

    void endFrame() { EndDrawing(); }

    bool runFrame() {
        beginFrame();
        switch (currentScreen) {
            case SCREEN_MAIN_MENU:
                drawMainMenu();
                break;
            case SCREEN_GAMEPLAY:
                drawGameplay();
                break;
            case SCREEN_EXIT:
                endFrame();
                return false;
        }
        endFrame();
        return true;
    }

    void drawMainMenu() {
    static float time = 0.0f;
    time += GetFrameTime();
    
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                           Color{200, 30, 30, 255}, Color{180, 20, 20, 255});
    
    struct FloatingCard {
        float x, y, rotation, speed, scale;
        Card card;
    };
    
    static FloatingCard cards[] = {
        {120, 150, -15, 0.8f, 1.0f, Card(CARD_RED, NUMBER, 7, 7)},
        {280, 480, 10, 1.2f, 0.9f, Card(CARD_GREEN, NUMBER, 2, 2)},
        {900, 180, 20, 0.6f, 0.85f, Card(CARD_RED, NUMBER, 9, 9)},
        {1100, 250, -10, 1.0f, 1.0f, Card(CARD_BLUE, NUMBER, 0, 0)},
        {1150, 380, 15, 0.9f, 0.95f, Card(CARD_RED, DRAW_TWO)},
        {1050, 600, -20, 0.7f, 1.1f, Card(CARD_YELLOW, NUMBER, 5, 5)},
        {800, 650, 25, 1.1f, 0.8f, Card(CARD_YELLOW, DRAW_TWO)},
        {50, 750, 12, 0.85f, 0.9f, Card(CARD_GREEN, SKIP)}
    };
    
    for (int i = 0; i < 8; i++) {
        float floatval = sin(time * cards[i].speed + i) * 10.0f;
        float cardX = cards[i].x;
        float cardY = cards[i].y + floatval;
        
        string path = GetCardTexturePath(cards[i].card);
        string filename = path.substr(path.find_last_of("/") + 1);
        Texture2D texture;
        for (int i = 0; i < cardTextures.cardCount; i++) {
    if (strcmp(cardTextures.cards[i].name, filename.c_str()) == 0) {
        texture = cardTextures.cards[i].tex;
        break;
    }
}
        // Texture2D texture = cardTextures.textures[filename];
        
        if (texture.id == 0) continue;
        
        float cardWidth = 100 * cards[i].scale;
        float cardHeight = 150 * cards[i].scale;
        
        Vector2 origin = {cardWidth / 2, cardHeight / 2};
        
        Rectangle src = {0, 0, (float)texture.width, (float)texture.height};
        Rectangle dst = {cardX, cardY, cardWidth, cardHeight};
        DrawTexturePro(texture, src, dst, origin, cards[i].rotation, WHITE);
    }
    
    const char* title = "UNO";
    int titleSize = 120;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (GetScreenWidth() - titleWidth) / 2;
    int titleY = 250;
    
    DrawText(title, titleX, titleY, titleSize, WHITE);
    
    
    int centerX = GetScreenWidth() / 2;
    int buttonWidth = 320;
    int buttonHeight = 60;
    int buttonY = 500;
    int buttonSpacing = 75;
    
    struct MenuButton {
        const char* text;
        Color color;
        Color hoverColor;
        int yOffset;
    };
    
    MenuButton buttons[] = {
        {"START GAME", YELLOW, Color{255, 230, 0, 255}, 0},
        {"HOW TO PLAY", BLUE, Color{50, 100, 180, 255}, buttonSpacing},
        {"SETTINGS", GREEN, Color{0, 180, 80, 255}, buttonSpacing * 2}
    };
    
    Vector2 mousePos = GetMousePosition();
    
    for (int i = 0; i < 3; i++) {
    int by = buttonY + buttons[i].yOffset;
    Rectangle buttonRect = {
        (float)(centerX - buttonWidth/2), 
        (float)by, 
        (float)buttonWidth, 
        (float)buttonHeight
    };
    
    bool isHovered = CheckCollisionPointRec(mousePos, buttonRect);
    Color buttonColor = isHovered ? buttons[i].hoverColor : buttons[i].color;

    
    DrawRectangleRounded(buttonRect, 0.15f, 10, buttonColor);
    
    int textWidth = MeasureText(buttons[i].text, 28);
    DrawText(buttons[i].text, centerX - textWidth/2, by + 16, 28, BLACK);
    
    if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (i == 0) { 
            game.start();
            game.beginTurn();
            currentScreen = SCREEN_GAMEPLAY;
        }
    }
}
    
    DrawText("Press ESC to Exit", 10, GetScreenHeight() - 30, 20, BLACK);
    
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = SCREEN_EXIT;
    }
}

    void updateHoveredCard(const Player& player, int startX, int y, int cardWidth, int cardHeight, int spacing) {
        hoveredCardIndex = -1;
        if (!game.isWaitingForInput()) return;

        Vector2 mousePos = GetMousePosition();
        int handSize = player.getHandSize();

        for (int i = 0; i < handSize; i++) {
            int x = startX + i * (cardWidth + spacing);
            Rectangle normalRect = {(float)x, (float)y, (float)cardWidth, (float)cardHeight};
            Rectangle liftedRect = {(float)x, (float)y - 25, (float)cardWidth, (float)cardHeight};

            if (CheckCollisionPointRec(mousePos, normalRect) ||
                CheckCollisionPointRec(mousePos, liftedRect)) {
                hoveredCardIndex = i;
                return;
            }
        }
    }

    void handleInput() {
        if (game.gameOver()) return;

        if (game.isWaitingForWildColor()) {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 4; i++) {
                if (CheckCollisionPointRec(mouse, colorButtons[i])) {
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        CardColor chosen;
                        switch (i) {
                            case 0: chosen = CARD_RED;    break;
                            case 1: chosen = CARD_GREEN;  break;
                            case 2: chosen = CARD_BLUE;   break;
                            case 3: chosen = CARD_YELLOW; break;
                        }
                        game.chooseWildColor(chosen);
                        return;
                    }
                }
            }

            if (IsKeyPressed(KEY_R)) game.chooseWildColor(CARD_RED);
            if (IsKeyPressed(KEY_G)) game.chooseWildColor(CARD_GREEN);
            if (IsKeyPressed(KEY_B)) game.chooseWildColor(CARD_BLUE);
            if (IsKeyPressed(KEY_Y)) game.chooseWildColor(CARD_YELLOW);
            return;
        }

        if (!game.isWaitingForInput()) return;
        Player& player = game.getCurrentPlayer();
    
    if (game.getHasDrawnCard() && game.getCanPlayDrawnCard()) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), passButton)) {
                game.passTurn();
                return;
            }
        }
        if (IsKeyPressed(KEY_P)) {
            game.passTurn();
            return;
        }
    }

        int handSize = player.getHandSize();

        for (int i = KEY_ZERO; i <= KEY_NINE; i++) {
            if (IsKeyPressed(i)) {
                int index = i - KEY_ZERO;
                if (index < handSize) {
                    game.handlePlay(index);
                }
            }
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), drawButton)) {
                game.handleDraw();
                return;
            }
            if (hoveredCardIndex != -1 && hoveredCardIndex < handSize) {
                Card selected = player.peekCard(hoveredCardIndex);
                if (game.isCardValid(selected)) {
                    if (selected.type == WILD_DRAW_FOUR && player.hasColor(game.getCurrentColor())) {
                        return;
                    }
                    game.handlePlay(hoveredCardIndex);
                }
            }
        }

        if (IsKeyPressed(KEY_D)) game.handleDraw();
        if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_EXIT;
    }

    void drawGameplay() {
        // DrawText("UNO Game", 20, 20, 30, WHITE);

        if (!game.gameOver()) {
            Player& currentPlayer = game.getCurrentPlayer();
            string playerName = currentPlayer.getName();
            DrawText(("Current Player: " + playerName).c_str(), 20, 60, 20, WHITE);


            if (game.getHasDrawnCard() && game.getCanPlayDrawnCard()) {
        DrawRectangle(screenWidth / 2 - 200, 90, 400, 80, Color{50, 50, 50, 200});
        DrawText("You drew a playable card!", screenWidth / 2 - 150, 100, 20, YELLOW);
        DrawText("Click it to play or click PASS", screenWidth / 2 - 140, 130, 18, WHITE);
        
        passButton.x = screenWidth / 2 - 75;
        passButton.y = 170;
        bool mouseOverPass = CheckCollisionPointRec(GetMousePosition(), passButton);
        DrawRectangleRounded(passButton, 0.3f, 8, mouseOverPass ? YELLOW : DARKGREEN);
        DrawText("PASS", passButton.x + 45, passButton.y + 15, 20, BLACK);
    }

            int centerY = (screenHeight - CARD_HEIGHT) / 2 - 50;
            drawCardTexture(Card(), screenWidth / 2 - CARD_WIDTH - 20, centerY, true);
            drawCardTexture(game.getTopCard(), screenWidth / 2 + 20, centerY);

            DrawText("DECK",    screenWidth / 2 - CARD_WIDTH - 20, centerY + CARD_HEIGHT + 10, 15, WHITE);
            DrawText("DISCARD", screenWidth / 2 + 20,             centerY + CARD_HEIGHT + 10, 15, WHITE);

            int cardWidth = CARD_WIDTH;
            int cardHeight = CARD_HEIGHT;
            int handSize = currentPlayer.getHandSize();

            if (handSize == 0) {
                DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight - 50, 20, LIGHTGRAY);
                handleInput();
                game.advanceTurnIfNeeded();
                return;
            }

            int handAreaWidth = screenWidth - 120;
            int desiredSpacing = 30;
            int spacing = desiredSpacing;
            int totalWidth = handSize * cardWidth + (handSize - 1) * spacing;

            if (totalWidth > handAreaWidth && handSize > 1) {
                spacing = (handAreaWidth - handSize * cardWidth) / (handSize - 1);
                totalWidth = handSize * cardWidth + (handSize - 1) * spacing;
            }

            int minVisible = 35;
            int minSpacing = minVisible - cardWidth;
            if (spacing < minSpacing) spacing = minSpacing;

            int startX = (screenWidth - totalWidth) / 2;
            int baseY = screenHeight - cardHeight - 80;

            updateHoveredCard(currentPlayer, startX, baseY, cardWidth, cardHeight, spacing);

            for (int i = 0; i < handSize; i++) {
                if (i == hoveredCardIndex) continue;
                Card card = currentPlayer.peekCard(i);
                int x = startX + i * (cardWidth + spacing);

                if (!game.isCardValid(card)) {
                    string path = GetCardTexturePath(card);
                    string filename = path.substr(path.find_last_of("/") + 1);
                    Texture2D tex;
                    for (int i = 0; i < cardTextures.cardCount; i++) {
    if (strcmp(cardTextures.cards[i].name, filename.c_str()) == 0) {
        tex = cardTextures.cards[i].tex;
        break;
    }
}
                    // Texture2D tex = cardTextures.textures[filename];
                    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
                    Rectangle dst = {(float)x, (float)baseY, CARD_WIDTH, CARD_HEIGHT};
                    DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, Color{255, 255, 255, 204});
                } else {
                    drawCardTexture(card, x, baseY);
                }
            }

            if (hoveredCardIndex != -1) {
                Card card = currentPlayer.peekCard(hoveredCardIndex);
                int x = startX + hoveredCardIndex * (cardWidth + spacing);
                int liftedY = baseY - 25;
                bool isPlayable = game.isCardValid(card);

                if (card.type == WILD_DRAW_FOUR && currentPlayer.hasColor(game.getCurrentColor())) {
                    isPlayable = false;
                }

                if (isPlayable) {
                    DrawRectangle(x - 3, liftedY - 3, cardWidth + 6, cardHeight + 6, Color{255, 255, 200, 80});
                    drawCardTexture(card, x, liftedY);
                    DrawText("Click to Play", x + 5, liftedY - 20, 12, GREEN);
                } else {
                    string path = GetCardTexturePath(card);
                    string filename = path.substr(path.find_last_of("/") + 1);
                    Texture2D tex;
                    for (int j = 0; j < cardTextures.cardCount; j++) {
        if (strcmp(cardTextures.cards[j].name, filename.c_str()) == 0) {
            tex = cardTextures.cards[j].tex;
            break;
        }
    }
                    // Texture2D tex = cardTextures.textures[filename];
                    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
                    Rectangle dst = {(float)x, (float)liftedY, CARD_WIDTH, CARD_HEIGHT};
                    DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, Color{255, 255, 255, 153});
                    DrawText("Cannot Play", x + 10, liftedY - 20, 12, YELLOW);
                }
            }

            drawButton.x = screenWidth / 2 - drawButton.width / 2;
            drawButton.y = screenHeight - 40;

            bool mouseOverDraw = CheckCollisionPointRec(GetMousePosition(), drawButton);
            Color buttonColor = mouseOverDraw ? YELLOW : DARKGREEN;

            DrawRectangleRounded(drawButton, 0.3f, 8, buttonColor);
            DrawRectangleRoundedLines(drawButton, 0.3f, 8, DARKGRAY);
            DrawText("DRAW CARD",
                     drawButton.x + drawButton.width / 2 - MeasureText("DRAW CARD", 20) / 2,
                     drawButton.y + drawButton.height / 2 - 10, 20, BLACK);

            if (game.isWaitingForWildColor()) {
                DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 160});
                DrawText("Choose a color:",
                         screenWidth / 2 - MeasureText("Choose a color:", 40) / 2,
                         screenHeight / 2 - 160, 40, WHITE);

                for (int i = 0; i < 4; i++) {
                    bool hovered = CheckCollisionPointRec(GetMousePosition(), colorButtons[i]);
                    Color col = hovered ? Fade(colorValues[i], 0.7f) : colorValues[i];
                    DrawRectangleRounded(colorButtons[i], 0.3f, 12, col);
                    DrawRectangleRoundedLines(colorButtons[i], 0.3f, 12, WHITE);

                    int tx = colorButtons[i].x + colorButtons[i].width / 2 - MeasureText(colorNames[i], 30) / 2;
                    int ty = colorButtons[i].y + colorButtons[i].height / 2 - 15;
                    DrawText(colorNames[i], tx, ty, 30, BLACK);
                }

                DrawText("(or press R / G / B / Y)",
                         screenWidth / 2 - MeasureText("(or press R / G / B / Y)", 20) / 2,
                         screenHeight / 2 + 140, 20, LIGHTGRAY);
            }

            handleInput();
            game.advanceTurnIfNeeded();
        } else {
            drawGameOver();
        }
    }

    void drawGameOver() {
    static float time = 0.0f;
    time += GetFrameTime();
    
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), 
                           Color{200, 30, 30, 255}, Color{180, 20, 20, 255});
    
    struct FloatingCard {
        float x, y, rotation, speed, scale;
        Card card;
    };
    
    static FloatingCard cards[] = {
        {120, 150, -15, 0.8f, 1.0f, Card(CARD_RED, NUMBER, 7)},
        {280, 480, 10, 1.2f, 0.9f, Card(CARD_GREEN, NUMBER, 2)},
        {900, 180, 20, 0.6f, 0.85f, Card(CARD_RED, NUMBER, 9)},
        {1100, 250, -10, 1.0f, 1.0f, Card(CARD_BLUE, NUMBER, 0)},
        {1150, 380, 15, 0.9f, 0.95f, Card(CARD_RED, DRAW_TWO)},
        {1050, 600, -20, 0.7f, 1.1f, Card(CARD_YELLOW, NUMBER, 5)},
        {800, 650, 25, 1.1f, 0.8f, Card(CARD_YELLOW, DRAW_TWO)},
        {50, 750, 12, 0.85f, 0.9f, Card(CARD_GREEN, SKIP)}
    };
    
    for (int i = 0; i < 8; i++) {
        float floatval = sin(time * cards[i].speed + i) * 10.0f;
        float cardX = cards[i].x;
        float cardY = cards[i].y + floatval;
        
        string path = GetCardTexturePath(cards[i].card);
        string filename = path.substr(path.find_last_of("/") + 1);
        Texture2D texture = cardTextures.back;
        for (int j = 0; j < cardTextures.cardCount; j++) {
            if (strcmp(cardTextures.cards[j].name, filename.c_str()) == 0) {
                texture = cardTextures.cards[j].tex;
                break;
            }
        }
        
        if (texture.id == 0) continue;
        
        float cardWidth = 100 * cards[i].scale;
        float cardHeight = 150 * cards[i].scale;
        
        Vector2 origin = {cardWidth / 2, cardHeight / 2};
        
        Rectangle src = {0, 0, (float)texture.width, (float)texture.height};
        Rectangle dst = {cardX, cardY, cardWidth, cardHeight};
        DrawTexturePro(texture, src, dst, origin, cards[i].rotation, WHITE);
    }
    
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 120});
    
    Player& winner = game.getCurrentPlayer();
    string winnerName = winner.getName();
    
    const char* gameOverText = "GAME OVER!";
    int gameOverSize = 100;
    int gameOverWidth = MeasureText(gameOverText, gameOverSize);
    int gameOverX = (GetScreenWidth() - gameOverWidth) / 2;
    int gameOverY = 100;
    
    DrawText(gameOverText, gameOverX + 4, gameOverY + 4, gameOverSize, Color{0, 0, 0, 150});
    DrawText(gameOverText, gameOverX, gameOverY, gameOverSize, Color{255, 215, 0, 255});
    
    string winnerText = winnerName + " WINS!";
    int winnerSize = 50;
    int winnerWidth = MeasureText(winnerText.c_str(), winnerSize);
    int winnerX = (GetScreenWidth() - winnerWidth) / 2;
    int winnerY = gameOverY + 120;
    
    DrawText(winnerText.c_str(), winnerX + 2, winnerY + 2, winnerSize, Color{0, 0, 0, 150});
    DrawText(winnerText.c_str(), winnerX, winnerY, winnerSize, WHITE);
    
    int scoreboardY = winnerY + 80;
    int lineHeight = 35;
    const char* scoreTitle = "FINAL SCORES";
    int scoreTitleWidth = MeasureText(scoreTitle, 28);
    DrawText(scoreTitle, (GetScreenWidth() - scoreTitleWidth) / 2 + 2, scoreboardY + 2, 28, Color{0, 0, 0, 150});
    DrawText(scoreTitle, (GetScreenWidth() - scoreTitleWidth) / 2, scoreboardY, 28, Color{200, 200, 200, 255});
    
    scoreboardY += 45;
    
    Node<Player>* temp = game.getPlayers().getHead();
    if (temp) {
        do {
            Player& p = temp->getData();
            int handValue = p.getScore();
            
            string playerScore;
            Color scoreColor;
            
            if (handValue == 0) {
                playerScore = p.getName() + ": 0 pts";
                scoreColor = GREEN;
            } else {
                playerScore = p.getName() + ": -" + to_string(handValue) + " pts";
                scoreColor = YELLOW; 
            }
            
            int textWidth = MeasureText(playerScore.c_str(), 24);
            int textX = (GetScreenWidth() - textWidth) / 2;
            
            DrawText(playerScore.c_str(), textX + 2, scoreboardY + 2, 24, Color{0, 0, 0, 150});
            DrawText(playerScore.c_str(), textX, scoreboardY, 24, scoreColor);
            
            scoreboardY += lineHeight;
            temp = temp->getNext();
        } while (temp != game.getPlayers().getHead());
    }
    
    int centerX = GetScreenWidth() / 2;
    int buttonWidth = 320;
    int buttonHeight = 60;
    int buttonY = scoreboardY + 30;
    int buttonSpacing = 75;
    
    struct MenuButton {
        const char* text;
        Color color;
        Color hoverColor;
        int yOffset;
    };
    
    MenuButton buttons[] = {
        {"PLAY AGAIN", YELLOW, Color{255, 230, 0, 255}, 0},
        {"MAIN MENU", BLUE, Color{50, 100, 180, 255}, buttonSpacing},
        {"EXIT GAME", RED, Color{220, 50, 50, 255}, buttonSpacing * 2}
    };
    
    Vector2 mousePos = GetMousePosition();
    
    for (int i = 0; i < 3; i++) {
        int by = buttonY + buttons[i].yOffset;
        Rectangle buttonRect = {
            (float)(centerX - buttonWidth/2), 
            (float)by, 
            (float)buttonWidth, 
            (float)buttonHeight
        };
        
        bool isHovered = CheckCollisionPointRec(mousePos, buttonRect);
        Color buttonColor = isHovered ? buttons[i].hoverColor : buttons[i].color;
        
        DrawRectangleRounded(buttonRect, 0.15f, 10, buttonColor);
        
        DrawRectangleRoundedLines(buttonRect, 0.15f, 10, Color{0, 0, 0, 100});
        
        int textWidth = MeasureText(buttons[i].text, 28);
        DrawText(buttons[i].text, centerX - textWidth/2, by + 16, 28, BLACK);
        
        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (i == 0) { 
                game.reset();
                currentScreen = SCREEN_GAMEPLAY;
            } else if (i == 1) {
                game.reset();
                currentScreen = SCREEN_MAIN_MENU;
            } else if (i == 2) {
                currentScreen = SCREEN_EXIT;
            }
        }
    }
    
    DrawText("Press ENTER for Play Again  |  ESC to Exit", 
             GetScreenWidth() / 2 - 220, 
             GetScreenHeight() - 40, 
             18, 
             Color{200, 200, 200, 255});
    
    if (IsKeyPressed(KEY_ENTER)) {
        game.reset();
        currentScreen = SCREEN_GAMEPLAY;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = SCREEN_EXIT;
    }
    if (IsKeyPressed(KEY_M)) {
        game.reset();
        currentScreen = SCREEN_MAIN_MENU;
    }
}
};

int main() {
    Game game;
    UnoGUI gui(game);
    gui.init();

    while (!WindowShouldClose()) {
        if (!gui.runFrame()) break;
    }

    gui.shutdown();
    return 0;
}
