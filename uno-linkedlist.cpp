#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "raylib.h"

using namespace std;

mt19937 rng(random_device{}());

int randomValue(int a, int b) {
  uniform_int_distribution<int> dist(a, b);
  return dist(rng);
}

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

class Card {
public:
  CardColor color;
  CardType type;
  int number; // valid only if card is

  Card() : color(WILD), type(WILD_COLOR), number(-1) {}
  Card(CardColor c, CardType t, int n = -1) : color(c), type(t), number(n) {}

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

    if (!skipHandled) {
      moveToNextPlayer();
    }

    turnState = TURN_FINISHED;
    return true;
  }

  bool gameOver() const { return isGameOver; }
};

class UnoGUI {
public:
  int screenWidth = 1440;
  int screenHeight = 900;
  void init() {
    InitWindow(screenWidth, screenHeight, "UNO - LinkedList Implementation");
    SetTargetFPS(60);
  }

  void shutdown() { CloseWindow(); }

  void beginFrame() {
    BeginDrawing();
    ClearBackground(DARKGREEN);
  }

  void endFrame() { EndDrawing(); }
};

int main() {
  Game game;
  UnoGUI gui;

  gui.init();

  game.start();
  game.beginTurn();

  int turnCounter = 0;

  while (!WindowShouldClose()) {
    gui.beginFrame();

    DrawText("UNO Game", 20, 20, 30, WHITE);

    if (!game.gameOver()) {
      int currentPlayerId = game.getCurrentPlayerId();
      Player &currentPlayer = game.getCurrentPlayer();
      string playerName = currentPlayer.getName();

      DrawText(("Current Player: " + playerName).c_str(), 20, 60, 20, WHITE);

      Card topCard = game.getTopCard();
      string topCardText = "Top Card: " + topCard.toString();
      DrawText(topCardText.c_str(), 20, 90, 20, WHITE);

      string colorText = "Current Color: ";
      switch (game.getCurrentColor()) {
      case CARD_RED:
        colorText += "Red";
        break;
      case CARD_GREEN:
        colorText += "Green";
        break;
      case CARD_BLUE:
        colorText += "Blue";
        break;
      case CARD_YELLOW:
        colorText += "Yellow";
        break;
      case WILD:
        colorText += "Wild";
        break;
      }
      DrawText(colorText.c_str(), 20, 120, 20, WHITE);

      int handSize = currentPlayer.getHandSize();
      DrawText(("Hand Size: " + to_string(handSize)).c_str(), 20, 150, 20,
               WHITE);

      DrawText("Your Cards:", 20, 190, 25, YELLOW);

      int startY = 220;
      for (int i = 0; i < handSize; i++) {
        Card card = currentPlayer.peekCard(i);

        Color textColor = WHITE;
        switch (card.color) {
        case CARD_RED:
          textColor = RED;
          break;
        case CARD_GREEN:
          textColor = GREEN;
          break;
        case CARD_BLUE:
          textColor = BLUE;
          break;
        case CARD_YELLOW:
          textColor = YELLOW;
          break;
        case WILD:
          textColor = PURPLE;
          break;
        }

        string cardText = "[" + to_string(i) + "] " + card.toString();
        DrawText(cardText.c_str(), 40, startY + (i * 25), 20, textColor);

        DrawRectangle(20, startY + (i * 25), 15, 20, textColor);
        DrawRectangleLines(20, startY + (i * 25), 15, 20, WHITE);
      }

      DrawText("Controls:", 500, 200, 25, YELLOW);
      DrawText("D - Draw a card", 520, 230, 20, WHITE);
      DrawText("0-9 - Play card at that index", 520, 260, 20, WHITE);
      DrawText("R/G/B/Y - Choose color for Wild card", 520, 290, 20, WHITE);
      DrawText("ESC - Exit", 520, 320, 20, WHITE);

      if (game.isWaitingForInput()) {
        DrawText("Status: Waiting for your move", 520, 350, 20, GREEN);
      } else if (game.isTurnFinished()) {
        DrawText("Status: Turn finished", 520, 350, 20, ORANGE);
      }

      if (game.isWaitingForInput()) {
        for (int i = KEY_ZERO; i <= KEY_NINE; i++) {
          if (IsKeyPressed(i)) {
            int cardIndex = i - KEY_ZERO;
            cout << playerName << " attempting to play card at index "
                 << cardIndex << endl;

            if (cardIndex < handSize) {
              if (game.handlePlay(cardIndex)) {
                cout << "Card played successfully!" << endl;
              } else {
                cout << "Invalid move!" << endl;
              }
            } else {
              cout << "Invalid card index!" << endl;
            }
            break;
          }
        }

        if (IsKeyPressed(KEY_D)) {
          cout << playerName << " drawing a card..." << endl;
          game.handleDraw();
        }

        if (IsKeyPressed(KEY_R)) {
          cout << playerName << " chose RED for wild card" << endl;
          game.chooseWildColor(CARD_RED);
        }
        if (IsKeyPressed(KEY_G)) {
          cout << playerName << " chose GREEN for wild card" << endl;
          game.chooseWildColor(CARD_GREEN);
        }
        if (IsKeyPressed(KEY_B)) {
          cout << playerName << " chose BLUE for wild card" << endl;
          game.chooseWildColor(CARD_BLUE);
        }
        if (IsKeyPressed(KEY_Y)) {
          cout << playerName << " chose YELLOW for wild card" << endl;
          game.chooseWildColor(CARD_YELLOW);
        }
      }

      game.advanceTurnIfNeeded();

      turnCounter++;
    } else {
      int screenWidth = 1440;
      int screenHeight = 900;
      DrawText("GAME OVER!", screenWidth / 2 - 100, screenHeight / 2 - 50, 40,
               RED);

      Player &currentPlayer = game.getCurrentPlayer();
      if (currentPlayer.getHandSize() == 0) {
        DrawText(("Winner: " + currentPlayer.getName() + "!").c_str(),
                 screenWidth / 2 - 100, screenHeight / 2, 30, GREEN);
      }

      DrawText("Press ESC to exit", screenWidth / 2 - 100,
               screenHeight / 2 + 50, 30, WHITE);

      if (IsKeyPressed(KEY_ESCAPE)) {
        break;
      }
    }

    gui.endFrame();
  }

  cout << "Game ended after " << turnCounter << " turns" << endl;
  gui.shutdown();

  return 0;
}
