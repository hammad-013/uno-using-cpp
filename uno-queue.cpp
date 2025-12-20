#include <iostream>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

struct Card
 {
    string color;
    string value;
};


void initializeDeck(queue<Card> &deck)
{
    string colors[4] = {"Red", "Blue", "Green", "Yellow"};
    for (int i = 0; i < 4; i++)
        {
        string c = colors[i];
        deck.push({c, "0"});
        for (int n = 1; n <= 9; n++)
        {
            deck.push({c, to_string(n)});
            deck.push({c, to_string(n)});
  }
        deck.push({c, "Skip"});
        deck.push({c, "Skip"});
        deck.push({c, "Reverse"});
        deck.push({c, "Reverse"});
        deck.push({c, "Draw2"});
        deck.push({c, "Draw2"});
    }
    for (int i = 0; i < 4; i++)
    {
        deck.push({"Wild", "Wild"});
        deck.push({"Wild", "+4"});
    }
}


Card drawCard(queue<Card> &deck)
 {
    Card c = deck.front();
    deck.pop();
    return c;
}

void shuffleDeck(queue<Card> &deck)
{
    Card tempArray[108];
    int cont = 0;


    while (!deck.empty())
    {
        tempArray[cont] = deck.front();
        deck.pop();
        cont++;
    }

    srand(time(NULL));


    for (int i = 0; i < cont; i++)
    {
        int r = rand() % cont;

        Card temp = tempArray[i];
        tempArray[i] = tempArray[r];
        tempArray[r] = temp;
    }


    for (int i = 0; i < cont; i++)
    {
        deck.push(tempArray[i]);
    }
}


