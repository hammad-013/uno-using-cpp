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
