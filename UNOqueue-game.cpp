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
     int count = 0;
    while (!deck.empty())
    {
        tempArray[count++] = deck.front();
    deck.pop();
    }
    srand(time(NULL));
    for (int i = 0; i < count; i++)
        {
        int r = rand() % count;

    swap(tempArray[i], tempArray[r]);
    }
    for (int i = 0; i < count; i++)
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


void showPlayerHand(queue<Card> player)
{
    queue<Card> temp = player;
    cout << "Player has cards: ";
    while(!temp.empty())
    {
        Card s = temp.front();
        cout << s.color << " " << s.value << ", ";
        temp.pop();
    }
    cout << endl;
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

void winnercondition(queue<Card> players[], int totalplayer, queue<Card> &deck)
{
    bool found=false;
    for(int i=0;i<totalplayer;i++)
        {
        if(players[i].empty())
        {
            cout<<"Congratulations! Player "<<i+1<<" wins the match!"<<endl;
            found=true;
        }
    }
    if(found)
     exit(0);

    if(deck.empty())
        {
        int minCards=players[0].size();
    int winner=0;
     bool tie=false;
        for(int i=1;i<totalplayer;i++)
            {
            if(players[i].size()<minCards)
            {
                minCards=players[i].size();
          winner=i;
         tie=false;
         }
            else if(players[i].size()==minCards)

        tie=true;
        }
        cout<<endl;
        cout<<"Deck is empty!"<<endl;
        if(tie)
            cout<<"Match draw!"<<endl;

        else
            cout<<"Congratulations! Player "<<winner+1<<" wins with fewer cards!"<<endl;
        exit(0);
    }
}

void playGame()
 {
    int totalplayer = 2;
    queue<Card> deck, discard;
    queue<Card> players[totalplayer];

    initializeDeck(deck);
    shuffleDeck(deck);
    dealInitialCards(players, totalplayer, deck);

    cout << " UNO GAME START "<<endl;


    while (true)
        {
        Card c = drawCard(deck);
        if (c.value != "Wild" && c.value != "+4")
         {
            discard.push(c);
            cout << "First discard = " << c.color << " " << c.value << endl;
            break;
        } else
         {
            deck.push(c);
            shuffleDeck(deck);
        }
    }

    int cur = 0;
    string forcedColor = "";
    int direction = 1;

    while (true)
        {
        winnercondition(players, totalplayer, deck);

        cout << "Player " << cur + 1 << " turn:" << endl;
        showPlayerHand(players[cur]);

        Card top = discard.back();
        if (forcedColor != "")
        {
            cout << "Forced color: " << forcedColor << endl;
        }
        cout << "Top discard: " << top.color << " " << top.value << endl;

        int handSize = players[cur].size();
        cout << "Choose card and -1 to draw): ";
        int choice;
        cin >> choice;
        cout<<endl;


        if (choice == -1)
            {
              if (deck.empty())
            {
                cout << "Deck empty, cannot draw"<<endl;

                if(cur==0)
                {
                    cur++;

                }
             else  if(cur==1)
                {
                    cur --;


                }
                continue;
            }

            Card drawn = drawCard(deck);

            players[cur].push(drawn);

            cout << "You drew: " << drawn.color << " " << drawn.value << endl;

            showPlayerHand(players[cur]);


            cout << "Choose a card to play from your hand (-1 to skip turn): ";
            cin >> choice;

            if (choice == -1)
            {
               if(cur==0)
                {
                    cur++;

                }

              else  if(cur==1)
                {
                    cur --;


                }
                continue;

            }

            handSize = players[cur].size();
        }


        if (choice < 0 || choice >= handSize)
            {
            cout << "Invalid choice"<<endl;
            continue;
        }


        queue<Card> tmp;
        Card play;
        for (int i = 0; i < handSize; i++)
        {
            Card c = players[cur].front();
            players[cur].pop();
            if (i == choice)
            {
                play = c;

                }
            else tmp.push(c);
        }
        players[cur] = tmp;


        if (!isValidMove(play, top, forcedColor))
            {
            cout << "Invalid move. You cannot play this card now.\n";
            players[cur].push(play);
                if(cur==0)
                {
                    cur++;

                }
               else if(cur==1)
                {
                    cur--;


                }
                continue;

        }

        discard.push(play);

        cout << "Player " << cur + 1 << " played: " << play.color << " " << play.value << endl;


        if (play.value == "Wild" || play.value == "+4")
        {

            forcedColor = chooseWildColor();
        }
        else
           {
               forcedColor = "";
           }


        if (play.value == "+4")
            { int next;
                if(cur==0)
                {
                    next= cur +1;
                }
              else if(cur==1)
                {
                    next=cur - 1;

                }

            cout << "Next player draws 4 cards and also his turn got skipped."<<endl;
            for (int i = 0; i < 4; i++)
               {

               if (deck.empty())
               {
                   break;
                   }
                else
                {
                    players[next].push(drawCard(deck));

                    }
                }

            continue;
        }
        if(top.value=="Draw2" && top.color==play.color&& play.value=="Draw2")
        {

        cout << "Invalid move. You cannot placed draw on draw "<<endl;

            players[cur].push(play);
             if(cur==0)
            {
                    cur++;

            }
          else  if(cur==1)
            {
                    cur --;


                }
                continue;

        }

        if (play.value == "Skip")
            {
            cout << "Next player got skipped" << endl;

            continue;
        }
         else if (play.value == "Draw2")
         {   int next;
                if(cur==0)
                {
                    next=cur+1;

                }
             else if(cur==1)
                {
                    next=cur -1;

                }


            cout << "Next player draws 2 cards. and also skipped" << endl;
            for (int i = 0; i < 2; i++)
                {
                    if (deck.empty())
                    {
                      break;
                    }
                else
                {
                    players[next].push(drawCard(deck));

                }
                }
            continue;
        }

  if (play.color == "Wild" && play.value == "Wild")
    {

    Card topBeforeWild = top;
    discard.push(play);

    cout << "Special Wild Wild rule activated!"<<endl;
    int ch;
    cout << "If you want to see Player " << cur + 1 << "'s hand then enter 1, else 2: ";
    cin >> ch;
    cout<<endl;
    if (ch == 2)
    {    if(cur==0)
       {
          cur++;

        }
      else  if(cur==1)
        {
          cur --;


        }
        continue;

    }
    int nextPlayer;
         if(cur==0)
       {
           nextPlayer=cur +1;

                }
      else if(cur==1)
     {
        nextPlayer = cur -1;


    }


    cout << "Player " << nextPlayer + 1 << " can see Player " << cur + 1 << "'s hand:"<<endl;
    showPlayerHand(players[cur]);


  bool found = false;
queue<Card> tmpHand;

while (!players[cur].empty())
{
    Card c = players[cur].front();
    players[cur].pop();

    if (c.color == topBeforeWild.color)
     {
        found = true;
    }

    tmpHand.push(c);
}


players[cur] = tmpHand;


    if (found)
    {
        cout << "Player " << nextPlayer + 1 << " matched the color!"<<endl;

        cout << "Player " << cur + 1 << " (who played Wild Wild) draws 4 cards."<<endl;

        for (int i = 0; i < 4; i++)
        {
           if (!deck.empty()) players[cur].push(drawCard(deck));
        }

    }
    else
     {
        cout << "Player " << nextPlayer + 1 << " could not match the color "<<endl;
        cout << "Player " << nextPlayer + 1 << " draws 6 cards "<<endl;
        for (int i = 0; i < 6; i++)
        {
             if (!deck.empty())
             {
                 players[nextPlayer].push(drawCard(deck));
             }
        }

    }
       if(cur==0)
       {
          cur++;

        }
      else  if(cur==1)
        {
          cur --;


        }
        continue;
           }

       else
        {  if(cur==0)
       {
          cur++;

        }
      else if(cur==1)
        {
          cur --;


        }

        }
    }
}


int main()
 {
    playGame();
    return 0;
}
