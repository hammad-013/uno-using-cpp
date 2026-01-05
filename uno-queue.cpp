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
