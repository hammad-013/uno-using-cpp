#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>
using namespace std;

mt19937 rng(random_device{}());

int randomValue(int a, int b) {
    uniform_int_distribution<int> dist(a, b);
    return dist(rng);
}

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
    for (int i = 1; i < pos && temp->getNext() != NULL; i++) {
      temp = temp->getNext();
    }

    if (temp->getNext() == NULL) {
      cout << "Position out of range" << endl;
      return T();
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

  ~LinkedList() {
    while (!isEmpty()) {
        deleteFront();
    }
}
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

    Card() : color(WILD), type(WILD_COLOR), number(-1) {}
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

    void insertCardToBottom(Card c) {
        cards.insertEnd(c);
    }

    bool isEmpty() {
        return cards.getHead() == NULL;
    }

    void shuffle() { // time = O(n) +  space = O(n)
        if (cards.getHead() == nullptr || cards.getHead()->getNext() == nullptr) {
        return;
    }

        // linkedlist to array
        vector<Card> tempCards;
        Node<Card>* current = cards.getHead();
        while(current != NULL) {
            tempCards.push_back(current->getData());
            current = current->getNext();
        }
        // shuffle array
        for(int i = tempCards.size() - 1; i > 0; --i) {
            int j = randomValue(0, i);
            swap(tempCards[i], tempCards[j]);
        }
        
        // array to linkedlist
        current = cards.getHead();
        for(const Card& c : tempCards) {
            current->setData(c);
            current = current->getNext();
        }
    }    
    
};

class DiscardPile {
    LinkedList<Card> pile;
public:
    void addCard(Card c) {
        pile.insertEnd(c);
    }

    Card getTopCard() {
        Node<Card>* tailNode = pile.getTail();
        if (tailNode == nullptr) {
            cout << "Discard pile is empty" << endl;
            return Card();
        }
        return tailNode->getData();
    }

    void resetIntoDeck(Deck& deck) {
        if(pile.isEmpty()) {
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

    bool isEmpty() {
        return pile.isEmpty();
    }
};

int main() {
     
    Card c(RED,SKIP);
    Card c2(YELLOW,NUMBER, 9);
    cout << c.toString() << endl;
    cout << c2.toString() << endl;
    Deck d;
    cout << d.isEmpty() << endl;
    Card dCard = d.drawCard();
    cout << dCard.toString() << endl;
    d.shuffle();
    dCard = d.drawCard();
    cout << dCard.toString() << endl;

    DiscardPile dPile;
    dCard = dPile.getTopCard();
    cout << dCard.toString() << endl;
    dPile.addCard(Card(YELLOW,NUMBER,4));
    dCard = dPile.getTopCard();
    cout << dCard.toString() << endl;

    return 0; 
}