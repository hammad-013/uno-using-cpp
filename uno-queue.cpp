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
void dealInitialCards(queue<Card> players[], int numPlayers, queue<Card> &deck)
 {
    for (int r = 0; r < 7; r++)

    {

     for (int p = 0; p < numPlayers; p++)
           {

            players[p].push(drawCard(deck));
        }
    }
}
void showPlayerHand(queue<Card> &hand)
 {
    int n = hand.size();
    cout << "Hand (" << n << "): ";
    for (int i = 0; i < n; i++)
        {
        Card c = hand.front(); hand.pop();
        cout << "[" << i << "] " << c.color << " " << c.value;
        if (i + 1 < n) cout << " | ";
        hand.push(c);
    }
    cout << endl;
}


string chooseWildColor()
{
    string col;
    while (true)
        {
        cout << "Choose a color (R/B/G/Y): ";
        cin >> col;
        if (col == "R" || col == "B" || col == "G" || col == "Y"|| col == "r" || col == "b" || col == "g" || col == "y")
            break;
        cout << "wrong choice again enter correct "<<endl;
    }
    if(col=="r"||col=="R")
    {
        return "Red";
        }
    if(col=="b"||col=="B")
    {
        return "Blue";
        }
    if(col=="g"||col=="G")
    {
        return "Green";
        }
 else
    return "Yellow";
}


bool isValidMove(Card &played, Card &top, string forcedColor)
 {
    if (played.value=="Wild" || played.value=="+4")
    {
        return true;
        }
    if (forcedColor!="" && played.color==forcedColor)
    {
        return true;
        }
    if (forcedColor=="" && (played.color==top.color || played.value==top.value))
    {
        return true;
        }
    return false;
}






