#include <iostream>
#include <string>
using namespace std;


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
        if (name == "WILD_NORMAL") return "WILD";
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
        if (name == "WILD_NORMAL") return true;
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
    string currentColor;
    bool over;

public:

    Game() : players{Player(0), Player(1), Player(2), Player(3)} {
        currentPlayer = 0;
        clockwise = true;
        currentColor = "";
        over = false;
    }

    void start() {
        deck.createDeck();


        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                players[i].addCard(deck.draw());
            }
        }


        string firstCard = deck.draw();
        Card c(firstCard);
        while (c.isWild()) {
            deck.discard(firstCard);
            firstCard = deck.draw();
            c = Card(firstCard);
        }
        deck.discard(firstCard);
        currentColor = c.getColor();

        over = false;
        currentPlayer = 0;
        clockwise = true;
    }

bool play(int playerIdx, int cardIndex, string& msg)
 {
        Player& p = players[playerIdx];
        string playedName = p.playCard(cardIndex);
        if (playedName == "")
            {
            msg = "Invalid card!";
            return false;
        }

        Card played(playedName);
        Card top(deck.topDiscard());

        if (!played.canPlayOn(top, currentColor))
        {
            p.addCard(playedName);
            msg = "Cannot play this card!";
            return false;
        }

        deck.discard(playedName);

        if (p.getHandSize() == 0)
            {
            over = true;
            msg = "Player " + to_string(playerIdx + 1) + " WINS!";
            return true;
        }

        if (!played.isWild())
            {
            currentColor = played.getColor();
        }

        string val = played.getValue();
        if (val == "DRAW_FOUR")
        {
            int next = getNext();
            for (int i = 0; i < 4; i++) players[next].addCard(deck.draw());
            currentPlayer = getNext();
        }
        else if (val == "DRAW_TWO")
        {
            int next = getNext();
            for (int i = 0; i < 2; i++) players[next].addCard(deck.draw());
            currentPlayer = getNext();
        }
        else if (val == "SKIP") {
            currentPlayer = getNext();
        }
        else if (val == "REVERSE") {
            clockwise = !clockwise;
        }

        currentPlayer = getNext();
        msg = "Played: " + playedName;
        return true;
    }

    int getNext() const {
        if (clockwise) {
            return (currentPlayer + 1) % 4;
        } else {
            return (currentPlayer - 1 + 4) % 4;
        }
    }

    void drawForCurrent() {
        players[currentPlayer].addCard(deck.draw());
    }

    void nextPlayer() {
        currentPlayer = getNext();
    }

    void setColor(string col) { currentColor = col; }


    int getCurrent() const { return currentPlayer; }
    string getColor() const { return currentColor; }
    string getTop() const { return deck.topDiscard(); }
    Player getPlayer(int i) const { return players[i]; }
    bool isOver() const { return over; }
    int drawSize() const { return deck.drawPileSize(); }
};
