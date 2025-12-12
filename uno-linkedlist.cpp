#include <iostream>
using namespace std;

template <typename T> class Node {
  T data;
  Node<T> *next;

public:
  Node(T d) : data(d), next(nullptr) {}

  T getData() { return data; }
  void setData(T d) { data = d; }

  Node<T> *getNext() { return next; }
  void setNext(Node<T> *n) { next = n; }
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
      return;
    }

    newNode->setNext(head);
    head = newNode;
  }

  void insertEnd(T val) {
    Node<T> *newNode = new Node<T>(val);

    if (isEmpty()) {
      head = tail = newNode;
      return;
    }

    tail->setNext(newNode);
    tail = newNode;
  }

  void deleteFront() {
    if (isEmpty())
      return;

    Node<T> *temp = head;
    head = head->getNext();

    if (head == nullptr)
      tail = nullptr;

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

    Node<T> *temp = head;

    while (temp->getNext() != tail) {
      temp = temp->getNext();
    }

    delete tail;
    tail = temp;
    tail->setNext(nullptr);
  }

  T deleteAt(int pos) {
    if (head == NULL) {
      throw runtime_error("List is empty");
    }
    if (pos < 0) {
      throw runtime_error("Invalid Position");
    }
    if (pos == 0) {
      T toReturn = head->getData();
      deleteFront();
      return toReturn;
    }

    Node<T> *temp = head;
    for (int i = 1; i < pos && temp->getNext() != NULL; i++) {
      temp = temp->getNext();
    }

    if (temp->getNext() == NULL) {
      throw runtime_error("Position out of range");
    }

    Node<T> *toDelete = temp->getNext();
    T toReturn = toDelete->getData();
    temp->setNext(toDelete->getNext());

    if (toDelete == tail) {
      tail = temp;
    }

    delete toDelete;
    return toReturn;
  }

  Node<T> *getHead() const { return head; }

  Node<T> *getTail() const { return tail; }
};


enum Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    WILD
};

enum CardType {
    NUMBER,
    SKIP,
    REVERSE,
    DRAW_TWO,
    WILD_COLOR,
    WILD_DRAW_FOUR
};

class Card {
public:
    Color color;
    CardType type;
    int number; // valid only if card is 

    Card() : color(WILD), type(NUMBER), number(-1) {}
    Card(Color c, CardType t, int n = -1) : color(c), type(t), number(n) {}

    string toString() const {
        string colorStr;
        switch (color) {
            case RED: colorStr = "Red"; break;
            case GREEN: colorStr = "Green"; break;
            case BLUE: colorStr = "Blue"; break;
            case YELLOW: colorStr = "Yellow"; break;
            case WILD: colorStr = "Wild"; break;
        }

        string typeStr;
        switch (type) {
            case NUMBER: typeStr = to_string(number); break;
            case SKIP: typeStr = "Skip"; break;
            case REVERSE: typeStr = "Reverse"; break;
            case DRAW_TWO: typeStr = "Draw Two"; break;
            case WILD_COLOR: typeStr = "Wild Color"; break;
            case WILD_DRAW_FOUR: typeStr = "Wild Draw Four"; break;
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

    bool isEmpty() {
        return cards.getHead() == NULL;
    }
    
};

int main() {
     
    Card c(RED,SKIP);
    Card c2(YELLOW,NUMBER, 9);
    cout << c.toString() << endl;
    cout << c2.toString() << endl;
    Deck d;
    cout << d.isEmpty() << endl;

    return 0; 
}