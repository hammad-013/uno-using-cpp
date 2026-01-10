#include "raylib.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

mt19937 rng(random_device{}());

int randomValue(int a, int b) {
  uniform_int_distribution<int> dist(a, b);
  return dist(rng);
}

const int CARD_WIDTH = 110;
const int CARD_HEIGHT = 160;
const int WINNING_SCORE = 500;

template <typename T> class Node {
  T data;
  Node<T> *next;
  Node<T> *prev;

public:
  Node(T d) : data(d), next(nullptr), prev(nullptr) {}
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
  LinkedList() : head(nullptr), tail(nullptr) {}

  LinkedList(const LinkedList &other) : head(nullptr), tail(nullptr) {
    if (other.isEmpty())
      return;
    Node<T> *current = other.head;
    do {
      insertEnd(current->getData());
      current = current->getNext();
    } while (current != other.head);
  }

  LinkedList &operator=(const LinkedList &other) {
    if (this == &other)
      return *this;
    clear();
    if (!other.isEmpty()) {
      Node<T> *current = other.head;
      do {
        insertEnd(current->getData());
        current = current->getNext();
      } while (current != other.head);
    }
    return *this;
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
      head = tail = nullptr;
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
      head = tail = nullptr;
      return;
    }
    Node<T> *temp = tail;
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

  Node<T> *getHead() const { return head; }
  Node<T> *getTail() const { return tail; }

  ~LinkedList() { clear(); }

  void clear() {
    if (!head)
      return;
    tail->setNext(nullptr);
    head->setPrev(nullptr);
    Node<T> *current = head;
    while (current != nullptr) {
      Node<T> *next = current->getNext();
      delete current;
      current = next;
    }
    head = nullptr;
    tail = nullptr;
  }
};

enum CardColor { CARD_RED, CARD_GREEN, CARD_BLUE, CARD_YELLOW, WILD };

enum CardType { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD_COLOR, WILD_DRAW_FOUR };

enum GameState {
  PLAYER_TURN,
  WILD_COLOR_CHOICE,
  GAME_OVER,
  CHALLENGE_PENDING,
  ROUND_OVER
};

enum Screen {
  SCREEN_MAIN_MENU,
  SCREEN_GAMEPLAY,
  SCREEN_EXIT,
  SCREEN_HOW_TO_PLAY
};

class Card {
public:
  CardColor color;
  CardType type;
  int number;

  Card() : color(WILD), type(WILD_COLOR), number(-1) {}
  Card(CardColor c, CardType t, int n = -1) : color(c), type(t), number(n) {}

  int getScore() const {
    if (type == NUMBER)
      return number;
    if (type == SKIP || type == REVERSE || type == DRAW_TWO)
      return 20;
    if (type == WILD_COLOR || type == WILD_DRAW_FOUR)
      return 50;
    return 0;
  }

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
    string colorStr = getColorString();
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

  bool isEmpty() { return cards.getHead() == nullptr; }

  void shuffle() {
    if (cards.getHead() == nullptr ||
        cards.getHead()->getNext() == cards.getHead())
      return;

    vector<Card> tempCards;
    Node<Card> *current = cards.getHead();
    do {
      tempCards.push_back(current->getData());
      current = current->getNext();
    } while (current != cards.getHead());

    for (int i = tempCards.size() - 1; i > 0; --i) {
      int j = randomValue(0, (int)i);
      swap(tempCards[i], tempCards[j]);
    }

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
    if (pile.isEmpty())
      return;

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
  int totalScore;

public:
  Player() : name(""), id(-1), totalScore(0) {}
  Player(string n, int i) : name(n), id(i), totalScore(0) {}

  string getName() const { return name; }
  int getId() const { return id; }
  int getTotalScore() const { return totalScore; }
  void addToTotalScore(int points) { totalScore += points; }
  void resetTotalScore() { totalScore = 0; }

  int getRoundScore() const {
    int total = 0;
    if (!hand.getHead())
      return total;
    Node<Card> *temp = hand.getHead();
    do {
      total += temp->getData().getScore();
      temp = temp->getNext();
    } while (temp != hand.getHead());
    return total;
  }

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

  bool hasMatchingNumberOrAction(const Card &topCard) const {
    Node<Card> *temp = hand.getHead();
    if (!temp)
      return false;

    do {
      const Card &card = temp->getData();

      if (card.color == topCard.color) {
        return true;
      }

      if (card.type == NUMBER && topCard.type == NUMBER &&
          card.number == topCard.number) {
        return true;
      }

      if (card.type != NUMBER && card.type == topCard.type) {
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

  LinkedList<Card> &getHand() { return hand; }

  void clearHand() { hand.clear(); }

  bool hasWon() const { return totalScore >= WINNING_SCORE; }
};

struct SpecialCardEffect {
  int drawCards = 0;
  bool skipNext = false;
  bool reverse = false;
  bool needsColorChoice = false;
  bool challengePossible = false;
  bool awaitingChallenge = false;

  void reset() {
    drawCards = 0;
    skipNext = false;
    reverse = false;
    needsColorChoice = false;
    challengePossible = false;
    awaitingChallenge = false;
  }
};

class Game {
  GameState gameState;
  CardColor currentColor;
  Deck deck;
  DiscardPile discardPile;
  LinkedList<Player> players;
  int totalPlayers;
  Node<Player> *currentPlayer;
  Node<Player> *challengedPlayer;
  Node<Player> *challengerPlayer;
  int direction;
  bool hasDrawnThisTurn;
  int pendingDrawCount;
  bool challengePending;
  CardColor colorBeforeWild;

  bool hasCalledUno;
  bool forgotToCallUno;
  Node<Player> *playerWhoForgotUno;
  bool penaltyApplied;
  string penaltyMessage;
  float penaltyMessageTimer;

  Node<Player> *roundWinner;
  string roundSummary;
  bool showRoundSummary;
  float roundSummaryTimer;

public:
  Game()
      : gameState(PLAYER_TURN), totalPlayers(4), direction(1),
        hasDrawnThisTurn(false), pendingDrawCount(0), challengePending(false),
        hasCalledUno(false), forgotToCallUno(false),
        playerWhoForgotUno(nullptr), penaltyApplied(false), penaltyMessage(""),
        penaltyMessageTimer(0.0f), roundWinner(nullptr),
        showRoundSummary(false), roundSummaryTimer(3.0f) {
    for (int i = 0; i < totalPlayers; i++) {
      players.insertEnd(Player("Player " + to_string(i + 1), i));
    }
    currentPlayer = players.getHead();
    challengedPlayer = nullptr;
    challengerPlayer = nullptr;
  }

  LinkedList<Player> &getPlayers() { return players; }

  bool getHasDrawnCard() const { return hasDrawnThisTurn; }
  bool isPlayerTurn() const { return gameState == PLAYER_TURN; }
  bool isWaitingForWildColor() const { return gameState == WILD_COLOR_CHOICE; }
  bool isGameOver() const { return gameState == GAME_OVER; }
  bool isChallengePending() const { return gameState == CHALLENGE_PENDING; }
  bool isRoundOver() const { return gameState == ROUND_OVER; }
  Node<Player> *getChallengedPlayer() const { return challengedPlayer; }
  Node<Player> *getChallengerPlayer() const { return challengerPlayer; }
  bool getShowRoundSummary() const { return showRoundSummary; }
  string getRoundSummary() const { return roundSummary; }
  float getRoundSummaryTimer() const { return roundSummaryTimer; }

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
           firstCard.type == SKIP || firstCard.type == DRAW_TWO ||
           firstCard.type == WILD_DRAW_FOUR) {
      deck.insertCardToBottom(firstCard);
      deck.shuffle();
      firstCard = deck.drawCard();
    }

    discardPile.addCard(firstCard);
    currentColor = firstCard.color;
    beginTurn();
  }

  bool canCallUno() const {
    return currentPlayer->getData().getHandSize() == 2 && !hasCalledUno &&
           gameState == PLAYER_TURN;
  }

  bool hasPlayerCalledUno() const { return hasCalledUno; }

  string getPenaltyMessage() const { return penaltyMessage; }

  float getPenaltyMessageTimer() const { return penaltyMessageTimer; }

  void updatePenaltyTimer(float deltaTime) {
    if (penaltyMessageTimer > 0.0f) {
      penaltyMessageTimer -= deltaTime;
      if (penaltyMessageTimer <= 0.0f) {
        penaltyMessage = "";
      }
    }

    if (showRoundSummary) {
      roundSummaryTimer -= deltaTime;
      if (roundSummaryTimer <= 0.0f) {
        showRoundSummary = false;
        Node<Player> *temp = players.getHead();
        do {
          if (temp->getData().hasWon()) {
            gameState = GAME_OVER;
            return;
          }
          temp = temp->getNext();
        } while (temp != players.getHead());

        startNewRound();
      }
    }
  }

  void calculateRoundScores() {
    if (!roundWinner)
      return;

    Player &winner = roundWinner->getData();
    int roundPoints = 0;

    Node<Player> *temp = players.getHead();
    do {
      Player &player = temp->getData();
      if (&player != &winner) {
        roundPoints += player.getRoundScore();
      }
      temp = temp->getNext();
    } while (temp != players.getHead());

    winner.addToTotalScore(roundPoints);

    roundSummary = winner.getName() + " wins the round!\n";
    roundSummary += "Points earned: " + to_string(roundPoints) + "\n";
    roundSummary += "Total score: " + to_string(winner.getTotalScore());

    if (winner.hasWon()) {
      roundSummary += "\n\n" + winner.getName() + " REACHED 500 POINTS!";
    }

    showRoundSummary = true;
    roundSummaryTimer = 5.0f;
  }

  void startNewRound() {
    Node<Player> *temp = players.getHead();
    do {
      temp->getData().clearHand();
      temp = temp->getNext();
    } while (temp != players.getHead());

    gameState = PLAYER_TURN;
    currentColor = CARD_RED;
    direction = 1;
    hasDrawnThisTurn = false;
    pendingDrawCount = 0;
    challengePending = false;
    challengedPlayer = nullptr;
    challengerPlayer = nullptr;
    hasCalledUno = false;
    forgotToCallUno = false;
    playerWhoForgotUno = nullptr;
    penaltyApplied = false;
    penaltyMessage = "";
    penaltyMessageTimer = 0.0f;
    roundWinner = nullptr;

    deck = Deck();
    discardPile = DiscardPile();

    if (roundWinner) {
      currentPlayer = roundWinner->getNext();
    } else {
      currentPlayer = players.getHead();
    }

    start();
  }

  void callUno() {
    if (canCallUno()) {
      hasCalledUno = true;
    }
  }

  Player &getCurrentPlayer() { return currentPlayer->getData(); }

  Card getTopCard() const { return discardPile.getTopCard(); }

  CardColor getCurrentColor() const { return currentColor; }

  bool isCardValid(Card selectedCard) {
    Card top = discardPile.getTopCard();
    if (selectedCard.color == WILD || selectedCard.type == WILD_COLOR ||
        selectedCard.type == WILD_DRAW_FOUR ||
        selectedCard.color == currentColor ||
        (selectedCard.type == NUMBER && top.type == NUMBER &&
         selectedCard.number == top.number) ||
        (selectedCard.type != NUMBER && selectedCard.type == top.type)) {
      return true;
    }
    return false;
  }

  bool canPlayWildDrawFour(const Player &player) { return true; }

  Card safeDraw() {
    if (deck.isEmpty()) {
      discardPile.resetIntoDeck(deck);
      deck.shuffle();
    }
    return deck.drawCard();
  }

  SpecialCardEffect applyCardEffect(const Card &card) {
    SpecialCardEffect effect;

    switch (card.type) {
    case REVERSE:
      effect.reverse = true;
      direction *= -1;
      break;

    case SKIP:
      effect.skipNext = true;
      break;

    case DRAW_TWO:
      effect.drawCards = 2;
      effect.skipNext = true;
      break;

    case WILD_COLOR:
      effect.needsColorChoice = true;
      break;

    case WILD_DRAW_FOUR:
      effect.drawCards = 4;
      effect.needsColorChoice = true;
      effect.challengePossible = true;
      break;

    default:
      break;
    }

    return effect;
  }

  void applyEffects(const SpecialCardEffect &effect) {
    if (effect.drawCards > 0) {
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();
      for (int i = 0; i < effect.drawCards; i++) {
        target->getData().addToHand(safeDraw());
      }
    }
    if (effect.skipNext) {
      moveToNextPlayer();
    }
  }

  void beginTurn() {
    if (forgotToCallUno && playerWhoForgotUno != nullptr && !penaltyApplied) {
      Player &forgotPlayer = playerWhoForgotUno->getData();
      for (int i = 0; i < 2; i++) {
        forgotPlayer.addToHand(safeDraw());
      }
      penaltyMessage = forgotPlayer.getName() + " forgot to call UNO! +2 cards";
      penaltyMessageTimer = 5.0f;
      penaltyApplied = true;
      forgotToCallUno = false;
      playerWhoForgotUno = nullptr;
    }

    gameState = PLAYER_TURN;
    hasDrawnThisTurn = false;
    pendingDrawCount = 0;
    challengePending = false;
    challengedPlayer = nullptr;
    challengerPlayer = nullptr;
    hasCalledUno = false;
    penaltyApplied = false;
  }

  void endTurn() {
    moveToNextPlayer();
    beginTurn();
  }

  void handleDraw() {
    if (gameState != PLAYER_TURN || hasDrawnThisTurn) {
      return;
    }

    Player &player = currentPlayer->getData();
    Card drawn = safeDraw();
    player.addToHand(drawn);
    hasDrawnThisTurn = true;

    hasCalledUno = false;

    int handSize = player.getHandSize();
    if (handSize > 0) {
      Card lastCard = player.peekCard(handSize - 1);
      if (!isCardValid(lastCard)) {
        endTurn();
      }
    }
  }

  bool handlePlay(int cardIndex) {
    Player &player = currentPlayer->getData();

    int currentHandSize = player.getHandSize();

    if (currentHandSize == 2 && !hasCalledUno) {
      forgotToCallUno = true;
      playerWhoForgotUno = currentPlayer;
    } else if (currentHandSize == 2 && hasCalledUno) {
      forgotToCallUno = false;
      playerWhoForgotUno = nullptr;
    }

    Card played = player.playCard(cardIndex);

    if (player.getHandSize() == 0) {
      discardPile.addCard(played);
      roundWinner = currentPlayer;
      calculateRoundScores();
      gameState = ROUND_OVER;
      return true;
    }

    if (played.type == WILD_DRAW_FOUR) {
      colorBeforeWild = currentColor;
    }

    discardPile.addCard(played);
    if (played.color != WILD) {
      currentColor = played.color;
    }

    SpecialCardEffect effect = applyCardEffect(played);
    if (effect.reverse && totalPlayers == 2) {
      effect.skipNext = true;
    }

    if (effect.needsColorChoice) {
      pendingDrawCount = effect.drawCards;
      if (effect.challengePossible) {
        challengedPlayer = (direction == 1) ? currentPlayer->getNext()
                                            : currentPlayer->getPrev();
        challengePending = true;
      }
      gameState = WILD_COLOR_CHOICE;
      return true;
    }

    if (effect.drawCards > 0) {
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();
      for (int i = 0; i < effect.drawCards; i++) {
        target->getData().addToHand(safeDraw());
      }
    }

    if (effect.skipNext) {
      moveToNextPlayer();
    }

    endTurn();
    return true;
  }

  void chooseWildColor(CardColor color) {
    if (gameState != WILD_COLOR_CHOICE)
      return;

    currentColor = color;

    if (challengePending) {
      gameState = CHALLENGE_PENDING;
      return;
    }

    Node<Player> *target =
        (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();

    for (int i = 0; i < pendingDrawCount; i++) {
      target->getData().addToHand(safeDraw());
    }

    pendingDrawCount = 0;
    endTurn();
  }

  void handleChallenge(bool acceptChallenge) {
    if (gameState != CHALLENGE_PENDING)
      return;

    Player &challenger = challengedPlayer->getData();
    Player &wildPlayer = currentPlayer->getData();

    if (!acceptChallenge) {
      for (int i = 0; i < 4; i++) {
        challenger.addToHand(safeDraw());
      }
      pendingDrawCount = 0;
      challengePending = false;
      moveToNextPlayer();
      endTurn();
      return;
    }

    bool hadMatchingColor = wildPlayer.hasColor(colorBeforeWild);

    if (hadMatchingColor) {
      for (int i = 0; i < 4; i++) {
        wildPlayer.addToHand(safeDraw());
      }
    } else {
      for (int i = 0; i < 6; i++) {
        challenger.addToHand(safeDraw());
      }
      moveToNextPlayer();
    }

    pendingDrawCount = 0;
    challengePending = false;
    endTurn();
  }

  void moveToNextPlayer() {
    currentPlayer =
        (direction == 1) ? currentPlayer->getNext() : currentPlayer->getPrev();
  }

  void passTurn() {
    if (gameState != PLAYER_TURN || !hasDrawnThisTurn) {
      return;
    }
    Player &player = currentPlayer->getData();
    int handSize = player.getHandSize();
    if (handSize > 0) {
      Card lastCard = player.peekCard(handSize - 1);
      if (isCardValid(lastCard)) {
        endTurn();
      }
    }
  }

  void reset() {
    Node<Player> *temp = players.getHead();
    do {
      temp->getData().resetTotalScore();
      temp = temp->getNext();
    } while (temp != players.getHead());

    gameState = PLAYER_TURN;
    currentColor = CARD_RED;
    direction = 1;
    hasDrawnThisTurn = false;
    pendingDrawCount = 0;
    challengePending = false;
    challengedPlayer = nullptr;
    challengerPlayer = nullptr;
    hasCalledUno = false;
    forgotToCallUno = false;
    playerWhoForgotUno = nullptr;
    penaltyApplied = false;
    penaltyMessage = "";
    penaltyMessageTimer = 0.0f;
    roundWinner = nullptr;
    showRoundSummary = false;
    roundSummaryTimer = 3.0f;

    deck = Deck();
    discardPile = DiscardPile();

    temp = players.getHead();
    do {
      temp->getData().clearHand();
      temp = temp->getNext();
    } while (temp != players.getHead());

    currentPlayer = players.getHead();

    start();
  }
};

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
  Game &game;
  CardTextures cardTextures;
  int hoveredCardIndex = -1;
  Rectangle drawButton;
  Rectangle resetButton;
  Rectangle colorButtons[4];
  Rectangle challengeButton;
  Rectangle acceptButton;
  const char *colorNames[4] = {"Red", "Green", "Blue", "Yellow"};
  Color colorValues[4] = {RED, GREEN, BLUE, YELLOW};
  Rectangle passButton;

  Rectangle unoButton;

public:
  int screenWidth = 1440;
  int screenHeight = 900;

  UnoGUI(Game &g) : game(g) {
    currentScreen = SCREEN_MAIN_MENU;
    drawButton = {0, 0, 150, 50};
    passButton = {0, 0, 150, 50};

    int btnWidth = 140;
    int btnHeight = 60;
    int centerX = screenWidth / 2 - (btnWidth * 2 + 20) / 2;
    int centerY = screenHeight / 2 - btnHeight / 2;

    colorButtons[0] = {(float)centerX, (float)(centerY - 80), (float)btnWidth,
                       (float)btnHeight};
    colorButtons[1] = {(float)(centerX + btnWidth + 20), (float)(centerY - 80),
                       (float)btnWidth, (float)btnHeight};
    colorButtons[2] = {(float)centerX, (float)(centerY + 20), (float)btnWidth,
                       (float)btnHeight};
    colorButtons[3] = {(float)(centerX + btnWidth + 20), (float)(centerY + 20),
                       (float)btnWidth, (float)btnHeight};

    challengeButton = {(float)(screenWidth / 2 - 200),
                       (float)(screenHeight / 2 + 80), 160, 60};

    acceptButton = {(float)(screenWidth / 2 + 40),
                    (float)(screenHeight / 2 + 80), 160, 60};

    unoButton = {20, 100, 120, 60};
  }

  void loadCardTextures() {
    cardTextures.back = LoadTexture("assets/back.jpg");
    cardTextures.cardCount = 0;

    vector<string> files = {
        "Blue_0.jpg",       "Red_Reverse.jpg",   "Green_Draw_2.jpg",
        "Wild.jpg",         "Wild_Draw_4.jpg",   "Red_0.jpg",
        "Red_1.jpg",        "Red_2.jpg",         "Red_3.jpg",
        "Red_4.jpg",        "Red_5.jpg",         "Red_6.jpg",
        "Red_7.jpg",        "Red_8.jpg",         "Red_9.jpg",
        "Red_Skip.jpg",     "Red_Draw_2.jpg",    "Green_0.jpg",
        "Green_1.jpg",      "Green_2.jpg",       "Green_3.jpg",
        "Green_4.jpg",      "Green_5.jpg",       "Green_6.jpg",
        "Green_7.jpg",      "Green_8.jpg",       "Green_9.jpg",
        "Green_Skip.jpg",   "Green_Reverse.jpg", "Green_Draw_2.jpg",
        "Blue_1.jpg",       "Blue_2.jpg",        "Blue_3.jpg",
        "Blue_4.jpg",       "Blue_5.jpg",        "Blue_6.jpg",
        "Blue_7.jpg",       "Blue_8.jpg",        "Blue_9.jpg",
        "Blue_Skip.jpg",    "Blue_Reverse.jpg",  "Blue_Draw_2.jpg",
        "Yellow_0.jpg",     "Yellow_1.jpg",      "Yellow_2.jpg",
        "Yellow_3.jpg",     "Yellow_4.jpg",      "Yellow_5.jpg",
        "Yellow_6.jpg",     "Yellow_7.jpg",      "Yellow_8.jpg",
        "Yellow_9.jpg",     "Yellow_Skip.jpg",   "Yellow_Reverse.jpg",
        "Yellow_Draw_2.jpg"};

    for (auto &f : files) {
      if (cardTextures.cardCount < 60) {
        strcpy(cardTextures.cards[cardTextures.cardCount].name, f.c_str());
        cardTextures.cards[cardTextures.cardCount].tex =
            LoadTexture(("assets/" + f).c_str());
        cardTextures.cardCount++;
      }
    }
  }

  void drawCardTexture(const Card &card, int x, int y, bool faceDown = false) {
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
    }

    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = {(float)x, (float)y, CARD_WIDTH, CARD_HEIGHT};
    Vector2 origin = {0, 0};
    DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
  }

  void init() {
    InitWindow(screenWidth, screenHeight, "UNO");
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
    game.updatePenaltyTimer(GetFrameTime());

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
    case SCREEN_HOW_TO_PLAY:
      drawHowToPlay();
      break;
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
        {120, 150, -15, 0.8f, 1.0f, Card(CARD_RED, NUMBER, 7)},
        {280, 480, 10, 1.2f, 0.9f, Card(CARD_GREEN, NUMBER, 2)},
        {900, 180, 20, 0.6f, 0.85f, Card(CARD_RED, NUMBER, 9)},
        {1100, 250, -10, 1.0f, 1.0f, Card(CARD_BLUE, NUMBER, 0)},
        {1150, 380, 15, 0.9f, 0.95f, Card(CARD_RED, DRAW_TWO)},
        {1050, 600, -20, 0.7f, 1.1f, Card(CARD_YELLOW, NUMBER, 5)},
        {800, 650, 25, 1.1f, 0.8f, Card(CARD_YELLOW, DRAW_TWO)},
        {50, 750, 12, 0.85f, 0.9f, Card(CARD_GREEN, SKIP)}};

    for (int i = 0; i < 8; i++) {
      float floatval = sin(time * cards[i].speed + i) * 10.0f;
      float cardX = cards[i].x;
      float cardY = cards[i].y + floatval;

      string path = GetCardTexturePath(cards[i].card);
      string filename = path.substr(path.find_last_of("/") + 1);
      Texture2D texture;
      for (int j = 0; j < cardTextures.cardCount; j++) {
        if (strcmp(cardTextures.cards[j].name, filename.c_str()) == 0) {
          texture = cardTextures.cards[j].tex;
          break;
        }
      }
      if (texture.id == 0)
        continue;

      float cardWidth = 100 * cards[i].scale;
      float cardHeight = 150 * cards[i].scale;
      Vector2 origin = {cardWidth / 2, cardHeight / 2};
      Rectangle src = {0, 0, (float)texture.width, (float)texture.height};
      Rectangle dst = {cardX, cardY, cardWidth, cardHeight};
      DrawTexturePro(texture, src, dst, origin, cards[i].rotation, WHITE);
    }

    if (currentScreen == SCREEN_MAIN_MENU) {
      const char *title = "UNO";
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
        const char *text;
        Color color;
        Color hoverColor;
        int yOffset;
      };

      MenuButton buttons[] = {
          {"START GAME", YELLOW, Color{255, 230, 0, 255}, 0},
          {"HOW TO PLAY", BLUE, Color{50, 100, 180, 255}, buttonSpacing},
          {"EXIT", RED, Color{220, 50, 50, 255}, buttonSpacing * 2}};

      Vector2 mousePos = GetMousePosition();
      for (int i = 0; i < 3; i++) {
        int by = buttonY + buttons[i].yOffset;
        Rectangle buttonRect = {(float)(centerX - buttonWidth / 2), (float)by,
                                (float)buttonWidth, (float)buttonHeight};

        bool isHovered = CheckCollisionPointRec(mousePos, buttonRect);
        Color buttonColor =
            isHovered ? buttons[i].hoverColor : buttons[i].color;

        DrawRectangleRounded(buttonRect, 0.15f, 10, buttonColor);
        int textWidth = MeasureText(buttons[i].text, 28);
        DrawText(buttons[i].text, centerX - textWidth / 2, by + 16, 28, BLACK);

        if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          if (i == 0) {
            game.start();
            currentScreen = SCREEN_GAMEPLAY;
          } else if (i == 1) {
            currentScreen = SCREEN_HOW_TO_PLAY;
          } else if (i == 2) {
            currentScreen = SCREEN_EXIT;
          }
        }
      }

      DrawText("Press ESC to Exit", 10, GetScreenHeight() - 30, 20, BLACK);

      if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = SCREEN_EXIT;
      }
    }
  }

  void drawHowToPlay() {
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(),
                           Color{30, 30, 60, 255}, Color{20, 20, 40, 255});

    const char *title = "HOW TO PLAY UNO";
    int titleSize = 60;
    int titleWidth = MeasureText(title, titleSize);
    DrawText(title, (GetScreenWidth() - titleWidth) / 2, 40, titleSize, YELLOW);

    Rectangle backButton = {40, 40, 120, 50};
    bool backHovered = CheckCollisionPointRec(GetMousePosition(), backButton);
    DrawRectangleRounded(backButton, 0.3f, 8, backHovered ? RED : DARKGRAY);
    DrawText("BACK", backButton.x + 30, backButton.y + 15, 24, WHITE);

    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      currentScreen = SCREEN_MAIN_MENU;
      return;
    }

    Rectangle contentArea = {50, 120, (float)(GetScreenWidth() - 100),
                             (float)(GetScreenHeight() - 200)};
    DrawRectangleRec(contentArea, Color{0, 0, 0, 150});
    DrawRectangleLinesEx(contentArea, 2, GOLD);

    static float scrollOffset = 0.0f;
    if (IsKeyDown(KEY_DOWN))
      scrollOffset -= 2.0f;
    if (IsKeyDown(KEY_UP))
      scrollOffset += 2.0f;

    float wheel = GetMouseWheelMove();
    scrollOffset += wheel * 20.0f;

    if (scrollOffset > 0)
      scrollOffset = 0;

    BeginScissorMode((int)contentArea.x, (int)contentArea.y,
                     (int)contentArea.width, (int)contentArea.height);

    int yPos = (int)contentArea.y + 30 + (int)scrollOffset;
    int lineHeight = 34;
    int sectionSpacing = 30;

    DrawText("CARD RULES:", contentArea.x + 20, yPos, 32, YELLOW);
    yPos += 40;

    DrawText("Draw Two Card:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Next player draws 2 cards and misses turn", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- Playable on matching color or another Draw Two",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += sectionSpacing;

    DrawText("Reverse Card:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Reverses direction of play", contentArea.x + 60, yPos, 20,
             WHITE);
    yPos += lineHeight;
    DrawText("- Playable on matching color or another Reverse",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += sectionSpacing;

    DrawText("Skip Card:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Next player is skipped (loses their turn)", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- Playable on matching color or another Skip", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += sectionSpacing;

    DrawText("Wild Draw 4 Card:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Choose color + next player draws 4 cards", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += lineHeight;

    DrawText("- Challenge: If illegal, player draws 4 instead",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- If legal, challenger draws 6 cards", contentArea.x + 60, yPos,
             20, WHITE);
    yPos += sectionSpacing;

    DrawText("Wild Color Choice Card:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Choose any color to continue play", contentArea.x + 60, yPos,
             20, WHITE);
    yPos += lineHeight;
    DrawText("- Can be played even with other playable cards",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += sectionSpacing * 2;

    DrawText("GAME RULES:", contentArea.x + 20, yPos, 32, YELLOW);
    yPos += 40;

    DrawText("UNO Call:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Must yell UNO when playing second-to-last card",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- If caught not calling UNO, draw 2 penalty cards",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += sectionSpacing;

    DrawText("Going Out:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- Round ends when a player has no cards left", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- Last card can be Draw Two/Wild Draw Four", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- Next player draws cards before scoring", contentArea.x + 60,
             yPos, 20, WHITE);
    yPos += sectionSpacing;

    DrawText("Deck Depletion:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- If DRAW pile empties, reshuffle DISCARD pile",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += sectionSpacing * 2;

    DrawText("SCORING:", contentArea.x + 20, yPos, 32, YELLOW);
    yPos += 40;

    DrawText("Round winner gets points from opponents' hands:",
             contentArea.x + 40, yPos, 24, WHITE);
    yPos += lineHeight;

    DrawText("- Number cards (0-9): Face value", contentArea.x + 60, yPos, 20,
             LIGHTGRAY);
    yPos += lineHeight;
    DrawText("- Draw Two/Reverse/Skip: 20 points", contentArea.x + 60, yPos, 20,
             LIGHTGRAY);
    yPos += lineHeight;
    DrawText("- Wild/Wild Draw Four/Wild Color: 50 points", contentArea.x + 60,
             yPos, 20, LIGHTGRAY);
    yPos += sectionSpacing;

    DrawText("Winning:", contentArea.x + 40, yPos, 24, GREEN);
    yPos += lineHeight;
    DrawText("- First player to reach 500 points wins the game",
             contentArea.x + 60, yPos, 20, WHITE);
    yPos += lineHeight;
    DrawText("- New round starts if no one reaches 500", contentArea.x + 60,
             yPos, 20, WHITE);

    EndScissorMode();

    DrawText(
        "Use mouse wheel or UP/DOWN arrows to scroll",
        GetScreenWidth() / 2 -
            MeasureText("Use mouse wheel or UP/DOWN arrows to scroll", 18) / 2,
        GetScreenHeight() - 40, 18, LIGHTGRAY);
  }

  void updateHoveredCard(const Player &player, int startX, int y, int cardWidth,
                         int cardHeight, int spacing) {
    hoveredCardIndex = -1;
    if (!game.isPlayerTurn())
      return;

    Vector2 mousePos = GetMousePosition();
    int handSize = player.getHandSize();

    for (int i = 0; i < handSize; i++) {
      int x = startX + i * (cardWidth + spacing);
      Rectangle normalRect = {(float)x, (float)y, (float)cardWidth,
                              (float)cardHeight};
      Rectangle liftedRect = {(float)x, (float)y - 25, (float)cardWidth,
                              (float)cardHeight};

      if (CheckCollisionPointRec(mousePos, normalRect) ||
          CheckCollisionPointRec(mousePos, liftedRect)) {
        hoveredCardIndex = i;
        return;
      }
    }
  }

  void handleColorSelection() {
    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < 4; i++) {
      if (CheckCollisionPointRec(mouse, colorButtons[i]) &&
          IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CardColor chosen;
        switch (i) {
        case 0:
          chosen = CARD_RED;
          break;
        case 1:
          chosen = CARD_GREEN;
          break;
        case 2:
          chosen = CARD_BLUE;
          break;
        case 3:
          chosen = CARD_YELLOW;
          break;
        default:
          return;
        }
        game.chooseWildColor(chosen);
        return;
      }
    }
  }

  void handleChallengeSelection() {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
      return;

    Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, challengeButton)) {
      game.handleChallenge(true);
      return;
    }

    if (CheckCollisionPointRec(mouse, acceptButton)) {
      game.handleChallenge(false);
      return;
    }
  }

  void handleInput() {
    if (game.isGameOver() || game.isRoundOver())
      return;

    if (game.isChallengePending()) {
      handleChallengeSelection();
      return;
    }

    if (game.isWaitingForWildColor()) {
      handleColorSelection();
      return;
    }

    if (!game.isPlayerTurn())
      return;

    Player &player = game.getCurrentPlayer();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (CheckCollisionPointRec(GetMousePosition(), unoButton)) {
        if (game.canCallUno()) {
          game.callUno();
        }
        return;
      }

      if (CheckCollisionPointRec(GetMousePosition(), drawButton)) {
        game.handleDraw();
        return;
      }

      if (game.getHasDrawnCard() &&
          CheckCollisionPointRec(GetMousePosition(), passButton)) {
        game.passTurn();
        return;
      }

      if (hoveredCardIndex != -1 && hoveredCardIndex < player.getHandSize()) {
        Card selected = player.peekCard(hoveredCardIndex);
        if (game.isCardValid(selected)) {
          game.handlePlay(hoveredCardIndex);
        }
      }
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      currentScreen = SCREEN_EXIT;
    }
  }

  void drawRoundSummary() {
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 200});

    Rectangle summaryBox = {(float)(screenWidth / 2 - 300),
                            (float)(screenHeight / 2 - 200), 600, 400};
    DrawRectangleRec(summaryBox, Color{30, 30, 30, 240});
    DrawRectangleLinesEx(summaryBox, 3, YELLOW);

    DrawText("ROUND SUMMARY",
             screenWidth / 2 - MeasureText("ROUND SUMMARY", 40) / 2,
             summaryBox.y + 30, 40, YELLOW);

    string summary = game.getRoundSummary();

    vector<string> lines;
    size_t start = 0;
    size_t end = summary.find('\n');
    while (end != string::npos) {
      lines.push_back(summary.substr(start, end - start));
      start = end + 1;
      end = summary.find('\n', start);
    }
    lines.push_back(summary.substr(start));

    int lineY = summaryBox.y + 100;
    for (const string &line : lines) {
      int textWidth = MeasureText(line.c_str(), 24);
      DrawText(line.c_str(), screenWidth / 2 - textWidth / 2, lineY, 24, WHITE);
      lineY += 35;
    }

    int countdown = (int)game.getRoundSummaryTimer() + 1;
    string countdownText = "Next round in " + to_string(countdown) + "...";
    DrawText(countdownText.c_str(),
             screenWidth / 2 - MeasureText(countdownText.c_str(), 20) / 2,
             summaryBox.y + summaryBox.height - 50, 20, LIGHTGRAY);
  }

  void drawGameplay() {
    if (game.isGameOver()) {
      drawGameOver();
      return;
    }

    if (game.isRoundOver() && game.getShowRoundSummary()) {
      drawRoundSummary();
      return;
    }

    if (game.isChallengePending()) {
      DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 180});

      Node<Player> *challenged = game.getChallengedPlayer();
      string challengerName = challenged->getData().getName();

      string title = challengerName + " - Wild Draw 4 Challenge!";
      int titleWidth = MeasureText(title.c_str(), 40);
      DrawText(title.c_str(), screenWidth / 2 - titleWidth / 2,
               screenHeight / 2 - 100, 40, YELLOW);

      DrawText("You can challenge the previous play!", screenWidth / 2 - 180,
               screenHeight / 2 - 40, 20, WHITE);
      DrawText("If they had a matching color: they draw 4",
               screenWidth / 2 - 180, screenHeight / 2, 18, LIGHTGRAY);
      DrawText("If they didn't: you draw 6", screenWidth / 2 - 110,
               screenHeight / 2 + 25, 18, LIGHTGRAY);

      Vector2 mousePos = GetMousePosition();

      bool challengeHovered = CheckCollisionPointRec(mousePos, challengeButton);
      DrawRectangleRounded(challengeButton, 0.3f, 8,
                           challengeHovered ? ORANGE : RED);
      int textWidth = MeasureText("CHALLENGE", 24);
      DrawText("CHALLENGE",
               challengeButton.x + (challengeButton.width - textWidth) / 2,
               challengeButton.y + 18, 24, WHITE);

      bool acceptHovered = CheckCollisionPointRec(mousePos, acceptButton);
      DrawRectangleRounded(acceptButton, 0.3f, 8,
                           acceptHovered ? LIGHTGRAY : GREEN);
      textWidth = MeasureText("DRAW 4", 24);
      DrawText("DRAW 4", acceptButton.x + (acceptButton.width - textWidth) / 2,
               acceptButton.y + 18, 24, WHITE);

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (challengeHovered) {
          game.handleChallenge(true);
        } else if (acceptHovered) {
          game.handleChallenge(false);
        }
      }
      return;
    }

    Vector2 mousePos = GetMousePosition();

    bool unoHovered = CheckCollisionPointRec(mousePos, unoButton);
    bool canCallUno = game.canCallUno();

    Color unoButtonColor;

    if (canCallUno) {
      if (game.hasPlayerCalledUno()) {
        unoButtonColor = GREEN;
      } else {
        unoButtonColor = unoHovered ? YELLOW : DARKGREEN;
      }
    } else {
      unoButtonColor = GRAY;
    }

    DrawRectangleRounded(unoButton, 0.3f, 8, unoButtonColor);
    DrawRectangleRoundedLines(unoButton, 0.3f, 8, DARKGRAY);

    int unoTextWidth = MeasureText("UNO!", 28);
    DrawText("UNO!", unoButton.x + (unoButton.width - unoTextWidth) / 2,
             unoButton.y + 16, 28, BLACK);

    if (canCallUno && game.hasPlayerCalledUno()) {
      DrawText("Called!", unoButton.x + 25, unoButton.y + unoButton.height + 5,
               16, GREEN);
    }

    DrawRectangle(0, 0, screenWidth, 80, Color{0, 0, 0, 150});
    DrawText("TOTAL SCORES:", 20, 30, 24, YELLOW);

    Node<Player> *temp = game.getPlayers().getHead();
    int scoreX = 250;
    int scoreSpacing = 200;
    do {
      Player &p = temp->getData();
      string scoreText = p.getName() + ": " + to_string(p.getTotalScore());
      DrawText(scoreText.c_str(), scoreX, 30, 24, WHITE);

      if (&p == &game.getCurrentPlayer()) {
        DrawRectangle(scoreX - 10, 25, MeasureText(scoreText.c_str(), 24) + 20,
                      30, Color{255, 255, 255, 50});
      }

      scoreX += scoreSpacing;
      temp = temp->getNext();
    } while (temp != game.getPlayers().getHead());

    if (game.getPenaltyMessageTimer() > 0.0f) {
      string msg = game.getPenaltyMessage();
      int msgWidth = MeasureText(msg.c_str(), 24);
      int msgX = screenWidth / 2 - msgWidth / 2;
      int msgY = 100;

      DrawRectangle(msgX - 20, msgY - 10, msgWidth + 40, 50,
                    Color{0, 0, 0, 200});
      DrawRectangleLines(msgX - 20, msgY - 10, msgWidth + 40, 50, RED);
      DrawText(msg.c_str(), msgX, msgY, 24, RED);
    }

    Player &currentPlayer = game.getCurrentPlayer();

    if (game.getHasDrawnCard()) {
      int handSize = currentPlayer.getHandSize();
      bool drawnCardIsPlayable = false;
      if (handSize > 0) {
        Card lastCard = currentPlayer.peekCard(handSize - 1);
        drawnCardIsPlayable = game.isCardValid(lastCard);
      }
      if (drawnCardIsPlayable) {
        DrawRectangle(screenWidth / 2 - 200, 120, 400, 80,
                      Color{50, 50, 50, 200});
        DrawText("You drew a playable card!", screenWidth / 2 - 150, 130, 20,
                 YELLOW);
        DrawText("Click it to play or click PASS", screenWidth / 2 - 140, 160,
                 18, WHITE);
        passButton.x = screenWidth / 2 - 75;
        passButton.y = 210;
        bool mouseOverPass =
            CheckCollisionPointRec(GetMousePosition(), passButton);
        DrawRectangleRounded(passButton, 0.3f, 8,
                             mouseOverPass ? YELLOW : DARKGREEN);
        DrawText("PASS", passButton.x + 45, passButton.y + 15, 20, BLACK);
      } else {
        DrawRectangle(screenWidth / 2 - 200, 120, 400, 80,
                      Color{50, 50, 50, 200});
        DrawText("You drew a non-playable card", screenWidth / 2 - 150, 130, 20,
                 YELLOW);
        DrawText("Turn ending...", screenWidth / 2 - 80, 160, 18, WHITE);
      }
    }

    int centerY = (screenHeight - CARD_HEIGHT) / 2 - 20;
    drawCardTexture(Card(), screenWidth / 2 - CARD_WIDTH - 20, centerY, true);
    drawCardTexture(game.getTopCard(), screenWidth / 2 + 20, centerY);
    DrawText("DECK", screenWidth / 2 - CARD_WIDTH - 20,
             centerY + CARD_HEIGHT + 10, 15, WHITE);
    DrawText("DISCARD", screenWidth / 2 + 20, centerY + CARD_HEIGHT + 10, 15,
             WHITE);

    int infoY = centerY + CARD_HEIGHT + 60;
    DrawRectangle(screenWidth / 2 - 150, infoY - 15, 300, 70,
                  Color{0, 0, 0, 120});
    DrawRectangleLines(screenWidth / 2 - 150, infoY - 15, 300, 70,
                       Color{255, 255, 255, 60});

    int circleX = screenWidth / 2 - 90;
    int circleY = infoY + 20;
    int circleRadius = 20;
    Color circleColor;
    switch (game.getCurrentColor()) {
    case CARD_RED:
      circleColor = RED;
      break;
    case CARD_GREEN:
      circleColor = GREEN;
      break;
    case CARD_BLUE:
      circleColor = BLUE;
      break;
    case CARD_YELLOW:
      circleColor = YELLOW;
      break;
    case WILD:
      circleColor = BLACK;
      break;
    default:
      circleColor = GRAY;
    }
    DrawCircle(circleX, circleY, circleRadius, circleColor);
    DrawCircleLines(circleX, circleY, circleRadius, WHITE);
    if (game.getCurrentColor() == WILD) {
      DrawText("WILD", circleX - 22, circleY - 10, 16, WHITE);
    }
    DrawText("Current Color", circleX + 35, infoY + 10, 20, WHITE);

    int cardWidth = CARD_WIDTH;
    int cardHeight = CARD_HEIGHT;
    int handSize = currentPlayer.getHandSize();
    if (handSize == 0) {
      DrawText("Press ESC to exit", screenWidth / 2 - 100, screenHeight - 50,
               20, LIGHTGRAY);
      handleInput();
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
    if (spacing < minSpacing)
      spacing = minSpacing;
    int startX = (screenWidth - totalWidth) / 2;
    int baseY = screenHeight - cardHeight - 80;
    updateHoveredCard(currentPlayer, startX, baseY, cardWidth, cardHeight,
                      spacing);
    for (int i = 0; i < handSize; i++) {
      if (i == hoveredCardIndex)
        continue;
      Card card = currentPlayer.peekCard(i);
      int x = startX + i * (cardWidth + spacing);
      if (!game.isCardValid(card)) {
        string path = GetCardTexturePath(card);
        string filename = path.substr(path.find_last_of("/") + 1);
        Texture2D tex;
        for (int j = 0; j < cardTextures.cardCount; j++) {
          if (strcmp(cardTextures.cards[j].name, filename.c_str()) == 0) {
            tex = cardTextures.cards[j].tex;
            break;
          }
        }
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
      if (isPlayable) {
        DrawRectangle(x - 3, liftedY - 3, cardWidth + 6, cardHeight + 6,
                      Color{255, 255, 200, 80});
        drawCardTexture(card, x, liftedY);
        DrawText("Click to Play", x + 5, liftedY - 20, 12, GREEN);
        if (card.type == WILD_DRAW_FOUR) {
          DrawText("Wild Draw 4 - Challenge Possible", x - 20, liftedY - 40, 12,
                   YELLOW);
        }
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
        Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
        Rectangle dst = {(float)x, (float)liftedY, CARD_WIDTH, CARD_HEIGHT};
        DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, Color{255, 255, 255, 153});
        if (card.type == WILD_DRAW_FOUR &&
            game.canPlayWildDrawFour(currentPlayer)) {
          DrawText("Can Play Wild Draw 4", x + 5, liftedY - 20, 12, GREEN);
        } else {
          DrawText("Cannot Play", x + 10, liftedY - 20, 12, YELLOW);
        }
      }
    }
    drawButton.x = screenWidth / 2 - drawButton.width / 2;
    drawButton.y = screenHeight - 40;
    bool canDraw = !game.getHasDrawnCard();
    bool mouseOverDraw = CheckCollisionPointRec(GetMousePosition(), drawButton);
    Color buttonColor;
    if (canDraw) {
      buttonColor = mouseOverDraw ? YELLOW : DARKGREEN;
    } else {
      buttonColor = GRAY;
    }
    DrawRectangleRounded(drawButton, 0.3f, 8, buttonColor);
    DrawRectangleRoundedLines(drawButton, 0.3f, 8, DARKGRAY);
    DrawText("DRAW CARD",
             drawButton.x + drawButton.width / 2 -
                 MeasureText("DRAW CARD", 20) / 2,
             drawButton.y + drawButton.height / 2 - 10, 20, BLACK);
    if (game.isWaitingForWildColor()) {
      DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 160});
      DrawText("Choose a color:",
               screenWidth / 2 - MeasureText("Choose a color:", 40) / 2,
               screenHeight / 2 - 160, 40, WHITE);
      for (int i = 0; i < 4; i++) {
        bool hovered =
            CheckCollisionPointRec(GetMousePosition(), colorButtons[i]);
        Color col = hovered ? Fade(colorValues[i], 0.7f) : colorValues[i];
        DrawRectangleRounded(colorButtons[i], 0.3f, 12, col);
        DrawRectangleRoundedLines(colorButtons[i], 0.3f, 12, WHITE);
        int tx = colorButtons[i].x + colorButtons[i].width / 2 -
                 MeasureText(colorNames[i], 30) / 2;
        int ty = colorButtons[i].y + colorButtons[i].height / 2 - 15;
        DrawText(colorNames[i], tx, ty, 30, BLACK);
      }
      DrawText("Click a color to select",
               screenWidth / 2 - MeasureText("Click a color to select", 20) / 2,
               screenHeight / 2 + 140, 20, LIGHTGRAY);
    }
    handleInput();
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
        {50, 750, 12, 0.85f, 0.9f, Card(CARD_GREEN, SKIP)}};

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
      if (texture.id == 0)
        continue;

      float cardWidth = 100 * cards[i].scale;
      float cardHeight = 150 * cards[i].scale;
      Vector2 origin = {cardWidth / 2, cardHeight / 2};
      Rectangle src = {0, 0, (float)texture.width, (float)texture.height};
      Rectangle dst = {cardX, cardY, cardWidth, cardHeight};
      DrawTexturePro(texture, src, dst, origin, cards[i].rotation, WHITE);
    }

    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                  Color{0, 0, 0, 120});

    Node<Player> *winnerNode = nullptr;
    Node<Player> *temp = game.getPlayers().getHead();
    do {
      if (temp->getData().hasWon()) {
        winnerNode = temp;
        break;
      }
      temp = temp->getNext();
    } while (temp != game.getPlayers().getHead());

    if (!winnerNode) {
      winnerNode = game.getPlayers().getHead();
    }

    Player &winner = winnerNode->getData();
    string winnerName = winner.getName();

    const char *gameOverText = "GAME OVER!";
    int gameOverSize = 100;
    int gameOverWidth = MeasureText(gameOverText, gameOverSize);
    int gameOverX = (GetScreenWidth() - gameOverWidth) / 2;
    int gameOverY = 100;
    DrawText(gameOverText, gameOverX + 4, gameOverY + 4, gameOverSize,
             Color{0, 0, 0, 150});
    DrawText(gameOverText, gameOverX, gameOverY, gameOverSize,
             Color{255, 215, 0, 255});

    string winnerText = winnerName + " WINS THE GAME!";
    int winnerSize = 50;
    int winnerWidth = MeasureText(winnerText.c_str(), winnerSize);
    int winnerX = (GetScreenWidth() - winnerWidth) / 2;
    int winnerY = gameOverY + 120;
    DrawText(winnerText.c_str(), winnerX + 2, winnerY + 2, winnerSize,
             Color{0, 0, 0, 150});
    DrawText(winnerText.c_str(), winnerX, winnerY, winnerSize, WHITE);

    string finalScoreText =
        "Final Score: " + to_string(winner.getTotalScore()) + " points";
    DrawText(finalScoreText.c_str(),
             GetScreenWidth() / 2 - MeasureText(finalScoreText.c_str(), 30) / 2,
             winnerY + 70, 30, YELLOW);

    int scoreboardY = winnerY + 140;
    int lineHeight = 35;

    const char *scoreTitle = "FINAL SCORES";
    int scoreTitleWidth = MeasureText(scoreTitle, 28);
    DrawText(scoreTitle, (GetScreenWidth() - scoreTitleWidth) / 2 + 2,
             scoreboardY + 2, 28, Color{0, 0, 0, 150});
    DrawText(scoreTitle, (GetScreenWidth() - scoreTitleWidth) / 2, scoreboardY,
             28, Color{200, 200, 200, 255});

    scoreboardY += 45;
    temp = game.getPlayers().getHead();
    if (temp) {
      do {
        Player &p = temp->getData();
        string playerScore =
            p.getName() + ": " + to_string(p.getTotalScore()) + " pts";
        Color scoreColor = (&p == &winner) ? GOLD : YELLOW;

        int textWidth = MeasureText(playerScore.c_str(), 24);
        int textX = (GetScreenWidth() - textWidth) / 2;
        DrawText(playerScore.c_str(), textX + 2, scoreboardY + 2, 24,
                 Color{0, 0, 0, 150});
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
      const char *text;
      Color color;
      Color hoverColor;
      int yOffset;
    };

    MenuButton buttons[] = {
        {"PLAY AGAIN", YELLOW, Color{255, 230, 0, 255}, 0},
        {"MAIN MENU", BLUE, Color{50, 100, 180, 255}, buttonSpacing},
        {"EXIT GAME", RED, Color{220, 50, 50, 255}, buttonSpacing * 2}};

    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < 3; i++) {
      int by = buttonY + buttons[i].yOffset;
      Rectangle buttonRect = {(float)(centerX - buttonWidth / 2), (float)by,
                              (float)buttonWidth, (float)buttonHeight};

      bool isHovered = CheckCollisionPointRec(mousePos, buttonRect);
      Color buttonColor = isHovered ? buttons[i].hoverColor : buttons[i].color;

      DrawRectangleRounded(buttonRect, 0.15f, 10, buttonColor);
      DrawRectangleRoundedLines(buttonRect, 0.15f, 10, Color{0, 0, 0, 100});
      int textWidth = MeasureText(buttons[i].text, 28);
      DrawText(buttons[i].text, centerX - textWidth / 2, by + 16, 28, BLACK);

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
