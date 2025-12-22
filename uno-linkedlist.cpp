#include <algorithm>
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

enum Color { RED, GREEN, BLUE, YELLOW, WILD };

enum CardType { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD_COLOR, WILD_DRAW_FOUR };

class Card {
public:
  Color color;
  CardType type;
  int number; // valid only if card is

  Card() : color(WILD), type(WILD_COLOR), number(-1) {}
  Card(Color c, CardType t, int n = -1) : color(c), type(t), number(n) {}

  string toString() const {
    string colorStr;
    switch (color) {
    case RED:
      colorStr = "Red";
      break;
    case GREEN:
      colorStr = "Green";
      break;
    case BLUE:
      colorStr = "Blue";
      break;
    case YELLOW:
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
    Color colors[] = {RED, GREEN, BLUE, YELLOW};
    for (Color color : colors) {
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

  Card getTopCard() {
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

  bool hasColor(Color color) const {

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

  bool hasPlayable(Card topCard, Color currentColor) {
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
  Color currentColor;
  Deck deck;
  DiscardPile discardPile;
  LinkedList<Player> players;
  int totalPlayers;
  Node<Player> *currentPlayer;
  int direction; // 1 = clockwise, -1 = counter-clockwise

public:
  Game() : isGameOver(false), totalPlayers(4), direction(1) {
    for (int i = 0; i < totalPlayers; i++) {
      players.insertEnd(Player("Player " + to_string(i + 1), i));
    }

    currentPlayer = players.getHead();
  }

  void start() {
    deck.shuffle();
    for (int i = 0; i < 1; i++) {
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
    currentColor = playedCard.color == WILD ? currentColor : playedCard.color;
    if (playedCard.type == REVERSE) {
      cout << "Direction reversed!" << endl;
      reverseDirection();
      return false;
    } else if (playedCard.type == SKIP) {
      Node<Player> *skipped = (direction == 1) ? currentPlayer->getNext()
                                               : currentPlayer->getPrev();

      cout << skipped->getData().getName() << " is skipped!" << endl;

      skipNextPlayer();

      return true;
    } else if (playedCard.type == DRAW_TWO) {
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();

      Player &nextPlayer = target->getData();

      cout << nextPlayer.getName() << " draws 2 cards and is skipped!" << endl;
      nextPlayer.addToHand(safeDraw());
      nextPlayer.addToHand(safeDraw());
      skipNextPlayer();

      return true;
    } else if (playedCard.type == WILD_COLOR) {
      int colorChoice;
      cout << "Choose a color (0-Red, 1-Green, 2-Blue, 3-Yellow): ";
      cin >> colorChoice;
      switch (colorChoice) {
      case 0:
        currentColor = RED;
        break;
      case 1:
        currentColor = GREEN;
        break;
      case 2:
        currentColor = BLUE;
        break;
      case 3:
        currentColor = YELLOW;
        break;
      default:
        cout << "Invalid color choice. Keeping previous color." << endl;
        break;
      }
      cout << "Color changed to "
           << (currentColor == RED     ? "Red"
               : currentColor == GREEN ? "Green"
               : currentColor == BLUE  ? "Blue"
                                       : "Yellow")
           << "!" << endl;
      return false;
    } else if (playedCard.type == WILD_DRAW_FOUR) {
      int colorChoice;
      cout << "Choose a color (0-Red, 1-Green, 2-Blue, 3-Yellow): ";
      cin >> colorChoice;
      switch (colorChoice) {
      case 0:
        currentColor = RED;
        break;
      case 1:
        currentColor = GREEN;
        break;
      case 2:
        currentColor = BLUE;
        break;
      case 3:
        currentColor = YELLOW;
        break;
      default:
        cout << "Invalid color choice. Keeping previous color." << endl;
        break;
      }
      Node<Player> *target = (direction == 1) ? currentPlayer->getNext()
                                              : currentPlayer->getPrev();

      Player &nextPlayer = target->getData();

      cout << nextPlayer.getName() << " draws 4 cards and is skipped!" << endl;
      for (int i = 0; i < 4; i++) {
        nextPlayer.addToHand(safeDraw());
      }
      skipNextPlayer();
      return true;
    }
    return false;
  }

  void printTurnHeader(Player &player) {
    player.printHand();
    cout << player.getName() << ", it's your turn. Top card is: "
         << discardPile.getTopCard().toString() << " | Current color: ";

    switch (currentColor) {
    case RED:
      cout << "Red";
      break;
    case GREEN:
      cout << "Green";
      break;
    case BLUE:
      cout << "Blue";
      break;
    case YELLOW:
      cout << "Yellow";
      break;
    case WILD:
      cout << "Wild";
      break;
    }
    cout << endl;
  }

bool handleDraw(Player &currentPlayer) {
    cout << currentPlayer.getName() << " chooses to draw a card." << endl;

    Card drawnCard = safeDraw();
    currentPlayer.addToHand(drawnCard);

    cout << currentPlayer.getName() << " drew card: " << drawnCard.toString()
         << endl;

    if (isCardValid(drawnCard)) {
        cout << "Drawn card is playable. Do you want to play it? (y/n): ";
        char playChoice;
        cin >> playChoice;
        if (playChoice == 'y' || playChoice == 'Y') {
            Card playedCard = currentPlayer.playCard(currentPlayer.getHandSize() - 1);
            discardPile.addCard(playedCard);
            bool turnEndedEarly = playSpecialCard(playedCard);
            if (turnEndedEarly) {
                if (currentPlayer.getHandSize() == 0) isGameOver = true;
                return true;
            }
        } else {
            cout << "Keeping drawn card. Turn ends." << endl;
            moveToNextPlayer();
            return true;
        }
    } else {
        cout << "Drawn card cannot be played. Turn ends." << endl;
        moveToNextPlayer();
        return true;
    }
    
    if (currentPlayer.getHandSize() == 0) {
        isGameOver = true;
    } else {
        moveToNextPlayer();
    }
    
    return true;
}

  bool handlePlay(Player &currentPlayer, int choice) {
    Card playedCard = currentPlayer.playCard(choice);
    discardPile.addCard(playedCard);

    bool turnEndedEarly = playSpecialCard(playedCard);
    if (turnEndedEarly) {
      if (currentPlayer.getHandSize() == 0) {
        isGameOver = true;
      }
      return true;
    }

    if (currentPlayer.getHandSize() == 0) {
      isGameOver = true;
    } else {
      moveToNextPlayer();
    }

    return true;
  }

  void playTurn() {
    if (deck.isEmpty()) {
      discardPile.resetIntoDeck(deck);
      deck.shuffle();
    }

    Player &player = currentPlayer->getData();

    printTurnHeader(currentPlayer->getData());

    int choice;
    bool validPlay = false;
    bool wantsToDraw = false;
    Card selectedCard;

    while (!validPlay) {
      cout << "Enter the index of the card to play: ";
      cin >> choice;

      if (choice == -1) {
        wantsToDraw = true;
        break;
      }

      if (choice < 0 || choice >= currentPlayer->getData().getHandSize()) {
        cout << "Invalid index. Try again." << endl;
        continue;
      }

      selectedCard = currentPlayer->getData().peekCard(choice);
      validPlay = isCardValid(selectedCard);

      if (validPlay && selectedCard.type == WILD_DRAW_FOUR &&
          currentPlayer->getData().hasColor(currentColor)) {

        cout << "You cannot play Wild Draw Four while holding a "
             << (currentColor == RED     ? "Red"
                 : currentColor == GREEN ? "Green"
                 : currentColor == BLUE  ? "Blue"
                                         : "Yellow")
             << " card. Choose a different card." << endl;

        validPlay = false;
      }
    }

    if (wantsToDraw) {
      handleDraw(currentPlayer->getData());

      return;
    }

    handlePlay(currentPlayer->getData(), choice);
  }

  bool gameOver() const { return isGameOver; }

  void run() {
    start();

    cout << ">>>>>  UNO  <<<<<" << endl;
    cout << "Each player has been dealt 7 cards." << endl;

    while (!gameOver()) {
      playTurn();
    }

    for (int i = 0; i < totalPlayers; i++) {
      if (getPlayerHandSize(i) == 0) {
        cout << getPlayerByIndex(i).getName() << " wins the game!" << endl;
        break;
      }
    }

    cout << "Thanks for playing UNO!" << endl;
  }
};

int main() {
  Game game;
  game.run();
  return 0;
}