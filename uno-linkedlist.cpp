#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "raylib.h"

using namespace std;

mt19937 rng(random_device{}());

int randomValue(int a, int b) {
  uniform_int_distribution<int> dist(a, b);
  return dist(rng);
}

const int CARD_WIDTH = 110;
const int CARD_HEIGHT = 160;

template <typename T> class Node {
  T data;
  Node<T> *next;
  Node<T> *prev;

public:
  Node(T d) : data(d), next(NULL), prev(NULL) {}

  T &getData() { return data; }
  void setData(T data) { this->data = data; }

  Node<T> *getNext() { return next; }
  Node<T> *getPrev() { return prev; }

  void setNext(Node<T> *next) { this->next = next; }
  void setPrev(Node<T> *prev) { this->prev = prev; }
};

template <typename T> class LinkedList {
  Node<T> *head;
  Node<T> *tail;

public:
  LinkedList() {
    head = nullptr;
    tail = nullptr;
  }

  bool isEmpty() const { return head == nullptr; }

  void insertFront(T val) {
    Node<T> *newNode = new Node<T>(val);

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
    Node<T> *newNode = new Node<T>(val);

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
    if (isEmpty())
      return;

    if (head == tail) {
      delete head;
      head = tail = NULL;
      return;
    }

    Node<T> *temp = head;
    head = head->getNext();

    head->setPrev(tail);
    tail->setNext(head);

    delete temp;
  }

  void deleteEnd() {
    if (isEmpty())
      return;

    if (head == tail) {
      delete head;
      head = tail = NULL;
      return;
    }

    Node<T> *temp = tail;
    tail = tail->getPrev();

    tail->setNext(head);
    head->setPrev(tail);

    delete temp;
  }

  T deleteAt(int pos) {
    if (head == NULL) {
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

    Node<T> *temp = head;
    int count = 0;

    while (count < pos - 1) {
      temp = temp->getNext();
      count++;
      if (temp == head) {
        cout << "Position out of range" << endl;
        return T();
      }
    }

    Node<T> *toDelete = temp->getNext();
    if (toDelete == head) {
      cout << "Position out of range" << endl;
      return T();
    }

    T toReturn = toDelete->getData();

    temp->setNext(toDelete->getNext());

    if (toDelete->getNext() != NULL) {
      toDelete->getNext()->setPrev(temp);
    }

    if (toDelete == tail) {
      tail = temp;
      if (head != NULL) {
        head->setPrev(tail);
      }
    }

    if (toDelete->getNext() == toDelete) {
      head = tail = NULL;
    }

    toDelete->setNext(NULL);
    toDelete->setPrev(NULL);

    delete toDelete;
    return toReturn;
  }

  Node<T> *getHead() const { return head; }

  Node<T> *getTail() const { return tail; }

  ~LinkedList() {
    if (!head)
      return;

    tail->setNext(NULL);
    head->setPrev(NULL);

    while (head != NULL) {
      Node<T> *temp = head;
      head = head->getNext();
      delete temp;
    }
  }
};

enum CardColor { CARD_RED, CARD_GREEN, CARD_BLUE, CARD_YELLOW, WILD };

enum CardType { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD_COLOR, WILD_DRAW_FOUR };
enum TurnState { WAITING_FOR_INPUT, TURN_FINISHED, GAME_OVER };
enum Screen { SCREEN_MAIN_MENU, SCREEN_GAMEPLAY, SCREEN_EXIT };

class Card {
public:
  CardColor color;
  CardType type;
  int number; // valid only if card is

  Card() : color(WILD), type(WILD_COLOR), number(-1) {}
  Card(CardColor c, CardType t, int n = -1) : color(c), type(t), number(n) {}
  string getColorString() const {
    switch (color) {
    case CARD_RED:
      return "Red";
    case CARD_GREEN:
      return "Green";
    case CARD_BLUE:
      return "Blue";
    case CARD_YELLOW:
      return "Yellow";
    case WILD:
      return "Wild";
    default:
      return "Unknown";
    }
  }

  string toString() const {
    string colorStr;
    switch (color) {
    case CARD_RED:
      colorStr = "Red";
      break;
    case CARD_GREEN:
      colorStr = "Green";
      break;
    case CARD_BLUE:
      colorStr = "Blue";
      break;
    case CARD_YELLOW:
      colorStr = "Yellow";
      break;
    case WILD:
      colorStr = "Wild";
      break;
    }

    string typeStr;
    switch (type) {
    case NUMBER:
      typeStr = to_string(number);
      break;
    case SKIP:
      typeStr = "Skip";
      break;
    case REVERSE:
      typeStr = "Reverse";
      break;
    case DRAW_TWO:
      typeStr = "Draw Two";
      break;
    case WILD_COLOR:
      typeStr = "Wild Color";
      break;
    case WILD_DRAW_FOUR:
      typeStr = "Wild Draw Four";
      break;
    }

    return colorStr + " " + typeStr;
  }
};

string GetCardTexturePath(Card card) {
  if (card.type == WILD_COLOR)
    return "assets/Wild.jpg";
  if (card.type == WILD_DRAW_FOUR)
    return "assets/Wild_Draw_4.jpg";

  string color;
  switch (card.color) {
  case CARD_RED:
    color = "Red";
    break;
  case CARD_BLUE:
    color = "Blue";
    break;
  case CARD_GREEN:
    color = "Green";
    break;
  case CARD_YELLOW:
    color = "Yellow";
    break;
  default:
    break;
  }

  string value;
  switch (card.type) {
  case NUMBER:
    value = to_string(card.number);
    break;
  case SKIP:
    value = "Skip";
    break;
  case REVERSE:
    value = "Reverse";
    break;
  case DRAW_TWO:
    value = "Draw_2";
    break;
  default:
    break;
  }

  return "assets/" + color + "_" + value + ".jpg";
}

class Deck {
  LinkedList<Card> cards;

public:
  Deck() {
    CardColor colors[] = {CARD_RED, CARD_GREEN, CARD_BLUE, CARD_YELLOW};
    for (CardColor color : colors) {
      cards.insertEnd(Card(color, NUMBER, 0));
      for (int i = 1; i <= 9; ++i) {
        cards.insertEnd(Card(color, NUMBER, i));
        cards.insertEnd(Card(color, NUMBER, i));
      }
      cards.insertEnd(Card(color, SKIP));
      cards.insertEnd(Card(color, SKIP));
      cards.insertEnd(Card(color, REVERSE));
      cards.insertEnd(Card(color, REVERSE));
      cards.insertEnd(Card(color, DRAW_TWO));
      cards.insertEnd(Card(color, DRAW_TWO));
    }
    for (int i = 0; i < 4; ++i) {
      cards.insertEnd(Card(WILD, WILD_COLOR));
      cards.insertEnd(Card(WILD, WILD_DRAW_FOUR));
    }
  }

  Card drawCard() {
    Node<Card> *headNode = cards.getHead();
    if (headNode == nullptr) {
      cout << "Deck is empty" << endl;
      return Card();
    }
    Card drawnCard = headNode->getData();
    cards.deleteFront();
    return drawnCard;
  }

  void insertCardToBottom(Card c) { cards.insertEnd(c); }

  bool isEmpty() { return cards.getHead() == NULL; }

  void shuffle() { // time = O(n) +  space = O(n)
    if (cards.getHead() == nullptr || cards.getHead()->getNext() == nullptr) {
      return;
    }

    // linkedlist to array
    vector<Card> tempCards;
    Node<Card> *current = cards.getHead();
    if (!current)
      return;

    do {
      tempCards.push_back(current->getData());
      current = current->getNext();
    } while (current != cards.getHead());

    // shuffle array
    for (int i = tempCards.size() - 1; i > 0; --i) {
      int j = randomValue(0, i);
      swap(tempCards[i], tempCards[j]);
    }

    // array to linkedlist
    current = cards.getHead();
    for (const Card &c : tempCards) {
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
    Node<Card> *tailNode = pile.getTail();
    if (tailNode == nullptr) {
      cout << "Discard pile is empty" << endl;
      return Card();
    }
    return tailNode->getData();
  }

  void resetIntoDeck(Deck &deck) {
    if (pile.isEmpty()) {
      cout << "Discard pile is empty" << endl;
      return;
    }

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

public:
  Player() : name(""), id(-1) {}
  Player(string n, int i) : name(n), id(i) {}

  string getName() const { return name; }

  int getId() const { return id; }

  int getHandSize() const {
    if (!hand.getHead())
      return 0;

    int count = 0;
    Node<Card> *temp = hand.getHead();

    do {
      count++;
      temp = temp->getNext();
    } while (temp != hand.getHead());

    return count;
  }

  bool hasColor(CardColor color) const {

    Node<Card> *temp = hand.getHead();
    if (!temp)
      return false;

    do {
      if (temp->getData().color == color)
        return true;
      temp = temp->getNext();
    } while (temp != hand.getHead());

    return false;
  }

  bool hasPlayable(Card topCard, CardColor currentColor) {
    Node<Card> *temp = hand.getHead();
    if (!temp)
      return false;
    do {
      Card c = temp->getData();
      if (c.color == currentColor || c.type == topCard.type ||
          (c.type == NUMBER && topCard.type == NUMBER &&
           c.number == topCard.number) ||
          c.color == WILD) {
        return true;
      }
      temp = temp->getNext();
    } while (temp != hand.getHead());
    return false;
  }

  void addToHand(Card c) { hand.insertEnd(c); }

  Card playCard(int index) { return hand.deleteAt(index); }

  Card peekCard(int index) const {
    Node<Card> *temp = hand.getHead();
    for (int i = 0; i < index && temp != NULL; i++) {
      temp = temp->getNext();
    }
    if (temp == NULL) {
      cout << "Invalid Card Index" << endl;
      return Card();
    }
    return temp->getData();
  }

  void printHand() {
    Node<Card> *temp = hand.getHead();
    if (!temp) {
      cout << name << "'s hand is empty." << endl;
      return;
    }
    cout << name << "'s hand: ";
    int index = 0;

    do {
      cout << "[" << index << "] " << temp->getData().toString() << "  ";
      temp = temp->getNext();
      index++;
    } while (temp != hand.getHead());
    cout << endl;
  }
};

class Game {
  bool isGameOver;
  CardColor currentColor;
  Deck deck;
  DiscardPile discardPile;
  LinkedList<Player> players;
  int totalPlayers;
  Node<Player> *currentPlayer;
  int direction; // 1 = clockwise, -1 = counter-clockwise
  TurnState turnState = WAITING_FOR_INPUT;
  bool waitingForWildColor = false;
  CardType pendingWildType;

public:
  Game() : isGameOver(false), totalPlayers(4), direction(1) {
    for (int i = 0; i < totalPlayers; i++) {
      players.insertEnd(Player("Player " + to_string(i + 1), i));
    }

    currentPlayer = players.getHead();
  }

  void start() {
    deck.shuffle();
    for (int i = 0; i < 7; i++) {
      Node<Player> *temp = players.getHead();
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

  int getPlayerHandSize(int playerIndex) {
    return getPlayerByIndex(playerIndex).getHandSize();
  }

  Player &getPlayerByIndex(int index) {
    Node<Player> *temp = players.getHead();
    int i = 0;

    while (temp != NULL && i < index) {
      temp = temp->getNext();
      i++;
    }

    if (temp == NULL) {
      cout << "Player index out of range." << endl;
    }

    return temp->getData();
  }

  void moveToNextPlayer() {
    currentPlayer =
        (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();
  }

  void skipNextPlayer() {
    moveToNextPlayer();
    moveToNextPlayer();
  }

  void reverseDirection() { direction *= -1; }

  Player &getCurrentPlayer() { return currentPlayer->getData(); }

  Card getTopCard() const { return discardPile.getTopCard(); }

  CardColor getCurrentColor() const { return currentColor; }

  void setCurrentColor(CardColor color) { currentColor = color; }

  int getCurrentPlayerId() const { return currentPlayer->getData().getId(); }

  bool isCardValid(Card selectedCard) {
    if (selectedCard.color == WILD || selectedCard.type == WILD_COLOR ||
        selectedCard.type == WILD_DRAW_FOUR ||
        selectedCard.color == currentColor ||
        (selectedCard.type == NUMBER &&
         discardPile.getTopCard().type == NUMBER &&
         selectedCard.number == discardPile.getTopCard().number) ||
        (selectedCard.type != NUMBER &&
         selectedCard.type == discardPile.getTopCard().type)) {
      return true;
    } else {
      cout << "You cannot play " << selectedCard.toString() << " on "
           << discardPile.getTopCard().toString() << endl;
      return false;
    }
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
      reverseDirection();
      return false;
    }

    if (playedCard.type == SKIP) {
      skipNextPlayer();
      return true;
    }

    if (playedCard.type == DRAW_TWO) {
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();

      target->getData().addToHand(safeDraw());
      target->getData().addToHand(safeDraw());
      skipNextPlayer();
      return true;
    }

    if (playedCard.type == WILD_COLOR || playedCard.type == WILD_DRAW_FOUR) {

      waitingForWildColor = true;
      pendingWildType = playedCard.type;
      return false;
    }

    return false;
  }

  void chooseWildColor(CardColor color) {
    if (!waitingForWildColor)
      return;

    currentColor = color;

    if (pendingWildType == WILD_DRAW_FOUR) {
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();

      for (int i = 0; i < 4; i++) {
        target->getData().addToHand(safeDraw());
      }
      skipNextPlayer();
    } else if (pendingWildType == WILD_COLOR) {
      moveToNextPlayer();
    }

    waitingForWildColor = false;
    turnState = TURN_FINISHED;
  }

  void beginTurn() { turnState = WAITING_FOR_INPUT; }

  bool isWaitingForInput() const { return turnState == WAITING_FOR_INPUT; }

  bool isTurnFinished() const { return turnState == TURN_FINISHED; }

  void handleDraw() {
    Player &player = currentPlayer->getData();
    Card drawn = safeDraw();
    player.addToHand(drawn);

    if (isCardValid(drawn)) {
      turnState = WAITING_FOR_INPUT;
    } else {
      moveToNextPlayer();
      turnState = TURN_FINISHED;
    }
  }

  void advanceTurnIfNeeded() {
    if (turnState == TURN_FINISHED && !isGameOver) {
      beginTurn();
    }
  }

  bool handlePlay(int cardIndex) {
    if (turnState != WAITING_FOR_INPUT || waitingForWildColor)
      return false;
    Player &player = currentPlayer->getData();

    Card selected = player.peekCard(cardIndex);

    if (!isCardValid(selected)) {
      return false;
    }

    if (selected.type == WILD_DRAW_FOUR && player.hasColor(currentColor)) {
      return false;
    }

    Card playedCard = player.playCard(cardIndex);
    discardPile.addCard(playedCard);

    bool skipHandled = playSpecialCard(playedCard);

    if (player.getHandSize() == 0) {
      isGameOver = true;
      turnState = GAME_OVER;
      return true;
    }

    if (!skipHandled && !waitingForWildColor) {
      moveToNextPlayer();
    }

    if (!waitingForWildColor) {
      turnState = TURN_FINISHED;
    }

    return true;
  }

  bool gameOver() const { return isGameOver; }

    void reset() {
    isGameOver = false;
    currentColor = CARD_RED;
    turnState = WAITING_FOR_INPUT;
    waitingForWildColor = false;
    direction = 1;
    
    deck = Deck();
    discardPile = DiscardPile();
    
    players = LinkedList<Player>();
    for (int i = 0; i < totalPlayers; i++) {
      players.insertEnd(Player("Player " + to_string(i + 1), i));
    }
    
    currentPlayer = players.getHead();
    
    start();
    beginTurn();
  }
};

struct CardTextures {
  unordered_map<string, Texture2D> textures;
  Texture2D back;
};

class UnoGUI {
  Screen currentScreen;
  Game &game;
  CardTextures cardTextures;
  int hoveredCardIndex = -1;
  Rectangle drawButton;

public:
  int screenWidth = 1440;
  int screenHeight = 900;

  UnoGUI(Game &g) : game(g) { currentScreen = SCREEN_MAIN_MENU; }
  
  void loadCardTextures() {
    cardTextures.back = LoadTexture("assets/back.jpg");

    vector<string> files = {"Blue_0.jpg",
                            "Red_Reverse.jpg",
                            "Green_Draw_2.jpg",
                            "Wild.jpg",
                            "Wild_Draw_4.jpg",
                            "Red_0.jpg",
                            "Red_1.jpg",
                            "Red_2.jpg",
                            "Red_3.jpg",
                            "Red_4.jpg",
                            "Red_5.jpg",
                            "Red_6.jpg",
                            "Red_7.jpg",
                            "Red_8.jpg",
                            "Red_9.jpg",
                            "Red_Skip.jpg",
                            "Red_Draw_2.jpg",
                            "Green_0.jpg",
                            "Green_1.jpg",
                            "Green_2.jpg",
                            "Green_3.jpg",
                            "Green_4.jpg",
                            "Green_5.jpg",
                            "Green_6.jpg",
                            "Green_7.jpg",
                            "Green_8.jpg",
                            "Green_9.jpg",
                            "Green_Skip.jpg",
                            "Green_Reverse.jpg",
                            "Blue_1.jpg",
                            "Blue_2.jpg",
                            "Blue_3.jpg",
                            "Blue_4.jpg",
                            "Blue_5.jpg",
                            "Blue_6.jpg",
                            "Blue_7.jpg",
                            "Blue_8.jpg",
                            "Blue_9.jpg",
                            "Blue_Skip.jpg",
                            "Blue_Reverse.jpg",
                            "Blue_Draw_2.jpg",
                            "Yellow_0.jpg",
                            "Yellow_1.jpg",
                            "Yellow_2.jpg",
                            "Yellow_3.jpg",
                            "Yellow_4.jpg",
                            "Yellow_5.jpg",
                            "Yellow_6.jpg",
                            "Yellow_7.jpg",
                            "Yellow_8.jpg",
                            "Yellow_9.jpg",
                            "Yellow_Skip.jpg",
                            "Yellow_Reverse.jpg",
                            "Yellow_Draw_2.jpg"

    };

    for (auto &f : files) {
      cardTextures.textures[f] = LoadTexture(("assets/" + f).c_str());
    }
  }

  void drawCardTexture(const Card &card, int x, int y, bool faceDown = false) {
    Texture2D tex;

    if (faceDown) {
      tex = cardTextures.back;
    } else {
      string path = GetCardTexturePath(card);
      string filename = path.substr(path.find_last_of("/") + 1);
      tex = cardTextures.textures[filename];
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
    ClearBackground(DARKGREEN);
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
    DrawText("UNO", 600, 200, 80, YELLOW);
    DrawText("Press ENTER to Start", 560, 320, 30, WHITE);
    DrawText("Press ESC to Exit", 580, 370, 25, GRAY);

    if (IsKeyPressed(KEY_ENTER)) {
      game.start();
      game.beginTurn();
      currentScreen = SCREEN_GAMEPLAY;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      currentScreen = SCREEN_EXIT;
    }
  }

  void updateHoveredCard(const Player& player, int startX, int y, int cardWidth, int cardHeight, int spacing) {
    hoveredCardIndex = -1;
    
    if (!game.isWaitingForInput()) {
      return;
    }
    
    Vector2 mousePos = GetMousePosition();
    int handSize = player.getHandSize();
    
    for (int i = 0; i < handSize; i++) {
      int x = startX + i * (cardWidth + spacing);

      Rectangle normalRect = { (float)x, (float)y, (float)cardWidth, (float)cardHeight };
      Rectangle liftedRect = { (float)x, (float)y - 25, (float)cardWidth, (float)cardHeight };
      
      if (CheckCollisionPointRec(mousePos, normalRect) || 
          CheckCollisionPointRec(mousePos, liftedRect)) {
        hoveredCardIndex = i;
        return;
      }
    }
  }

  void handleInput() {
    if (!game.isWaitingForInput())
      return;

    Player &player = game.getCurrentPlayer();
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
      if (hoveredCardIndex != -1 && hoveredCardIndex < handSize) {
        Card selectedCard = player.peekCard(hoveredCardIndex);
        if (game.isCardValid(selectedCard)) {
          if (selectedCard.type == WILD_DRAW_FOUR && player.hasColor(game.getCurrentColor())) {
            return;
          }
          game.handlePlay(hoveredCardIndex);
        }
      }
    }

    if (IsKeyPressed(KEY_D)) {
      game.handleDraw();
    }

    if (IsKeyPressed(KEY_R))
      game.chooseWildColor(CARD_RED);
    if (IsKeyPressed(KEY_G))
      game.chooseWildColor(CARD_GREEN);
    if (IsKeyPressed(KEY_B))
      game.chooseWildColor(CARD_BLUE);
    if (IsKeyPressed(KEY_Y))
      game.chooseWildColor(CARD_YELLOW);

    if (IsKeyPressed(KEY_ESCAPE)) {
      currentScreen = SCREEN_EXIT;
    }
  }

  void drawGameplay() {
    DrawText("UNO Game", 20, 20, 30, WHITE);

    if (!game.gameOver()) {
      Player &currentPlayer = game.getCurrentPlayer();
      string playerName = currentPlayer.getName();

      DrawText(("Current Player: " + playerName).c_str(), 20, 60, 20, WHITE);

      Card topCard = game.getTopCard();
      DrawText(("Top Card: " + topCard.toString()).c_str(), 20, 90, 20, WHITE);
      
      string currentColorStr;
      switch (game.getCurrentColor()) {
        case CARD_RED: currentColorStr = "Red"; break;
        case CARD_GREEN: currentColorStr = "Green"; break;
        case CARD_BLUE: currentColorStr = "Blue"; break;
        case CARD_YELLOW: currentColorStr = "Yellow"; break;
        case WILD: currentColorStr = "Wild"; break;
      }
      DrawText(("Current Color: " + currentColorStr).c_str(), 20, 120, 20, WHITE);

      DrawText(("Hand Size: " + to_string(currentPlayer.getHandSize())).c_str(),
               20, 150, 20, WHITE);

      

      int cardWidth = CARD_WIDTH;
      int cardHeight = CARD_HEIGHT;

      int handSize = currentPlayer.getHandSize();
      if (handSize == 0) {
        int centerY = (screenHeight - CARD_HEIGHT) / 2;
        drawCardTexture(Card(), screenWidth / 2 - CARD_WIDTH - 20, centerY, true);
        drawCardTexture(game.getTopCard(), screenWidth / 2 + 20, centerY);
        
        DrawText("Press D to draw a card | Press ESC to exit", 
                 screenWidth/2 - 200, screenHeight - 50, 20, LIGHTGRAY);
        
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

      if (spacing < minSpacing) {
        spacing = minSpacing;
        totalWidth = cardWidth + (handSize - 1) * (cardWidth + spacing);
      }

      int startX = (screenWidth - totalWidth) / 2;
      int baseY = screenHeight - cardHeight - 30;

      updateHoveredCard(currentPlayer, startX, baseY, cardWidth, cardHeight, spacing);
      
      for (int i = 0; i < handSize; i++) {
        if (i == hoveredCardIndex) {
          continue; 
        }
        
        Card card = currentPlayer.peekCard(i);
        int x = startX + i * (cardWidth + spacing);
        
        if (!game.isCardValid(card)) {
          Texture2D tex;
          string path = GetCardTexturePath(card);
          string filename = path.substr(path.find_last_of("/") + 1);
          tex = cardTextures.textures[filename];
          
          Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
          Rectangle dst = {(float)x, (float)baseY, CARD_WIDTH, CARD_HEIGHT};
          Vector2 origin = {0, 0};

          DrawTexturePro(tex, src, dst, origin, 0.0f, Color{255, 255, 255, 204});
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
          DrawRectangle(x - 3, liftedY - 3, cardWidth + 6, cardHeight + 6, 
                       Color{255, 255, 200, 80});
          drawCardTexture(card, x, liftedY);
          
          DrawText("Click to Play", x + 5, liftedY - 20, 12, GREEN);
        } else {
          Texture2D tex;
          string path = GetCardTexturePath(card);
          string filename = path.substr(path.find_last_of("/") + 1);
          tex = cardTextures.textures[filename];
          
          Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
          Rectangle dst = {(float)x, (float)liftedY, CARD_WIDTH, CARD_HEIGHT};
          Vector2 origin = {0, 0};

          DrawTexturePro(tex, src, dst, origin, 0.0f, Color{255, 255, 255, 153});
          
          DrawText("Cannot Play", x + 10, liftedY - 20, 12, RED);
        }
      }

      int centerY = (screenHeight - CARD_HEIGHT) / 2;
      
      drawCardTexture(Card(), screenWidth / 2 - CARD_WIDTH - 20, centerY, true);
      
      drawCardTexture(game.getTopCard(), screenWidth / 2 + 20, centerY);
      
      DrawText("DECK", screenWidth / 2 - CARD_WIDTH - 20, centerY + CARD_HEIGHT + 10, 15, WHITE);
      DrawText("DISCARD", screenWidth / 2 + 20, centerY + CARD_HEIGHT + 10, 15, WHITE);
      
      DrawText("INSTRUCTIONS:", 20, screenHeight - 100, 18, YELLOW);
      DrawText("• Click a card to play it", 20, screenHeight - 80, 15, LIGHTGRAY);
      DrawText("• Press D to draw a card", 20, screenHeight - 60, 15, LIGHTGRAY);
      DrawText("• Press R/G/B/Y to choose Wild color", 20, screenHeight - 40, 15, LIGHTGRAY);
      DrawText("• Press ESC to exit", 20, screenHeight - 20, 15, LIGHTGRAY);

      handleInput();
      game.advanceTurnIfNeeded();
      
    } else {
      drawGameOver();
    }
  }

  void drawGameOver() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 200});
    
    Player &winner = game.getCurrentPlayer();
    
    DrawText("GAME OVER!", screenWidth / 2 - 150, screenHeight / 2 - 80, 50, RED);
    DrawText(("Winner: " + winner.getName()).c_str(), screenWidth / 2 - 100, screenHeight / 2 - 20, 30, GREEN);
    DrawText(("Final Score: " + to_string(winner.getHandSize())).c_str(), 
             screenWidth / 2 - 100, screenHeight / 2 + 20, 25, YELLOW);
    
    DrawText("Press ENTER to return to main menu", screenWidth / 2 - 200, screenHeight / 2 + 80, 20, WHITE);
    DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight / 2 + 110, 20, GRAY);

    if (IsKeyPressed(KEY_ENTER)) {
      game.reset();
      currentScreen = SCREEN_MAIN_MENU;
    }
    
    if (IsKeyPressed(KEY_ESCAPE)) {
      currentScreen = SCREEN_EXIT;
    }
  }
};

int main() {
  Game game;
  UnoGUI gui(game);

  gui.init();

  while (!WindowShouldClose()) {
    if (!gui.runFrame())
      break;
  }

  gui.shutdown();
  return 0;
}