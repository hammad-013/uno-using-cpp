#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

template <typename T>
class Queue
{
private:
    struct Node
    {
        T data;
        Node* next;
    };

    Node* frontNode;
    Node* rearNode;
    int counts;

public:
    // Constructor
    Queue()
    {
        frontNode = nullptr;
        rearNode = nullptr;
        counts = 0;
    }
    // Copy Constructor
Queue(const Queue& other)
{
    frontNode = rearNode = nullptr;
    counts = 0;

    Node* curr = other.frontNode;
    while (curr != nullptr)
    {
        push(curr->data);
        curr = curr->next;
    }
}

// Assignment Operator
Queue& operator=(const Queue& other)
{
    if (this == &other)
        return *this;

    while (!emptys())
        pop();

    Node* curr = other.frontNode;
    while (curr != nullptr)
    {
        push(curr->data);
        curr = curr->next;
    }

    return *this;
}


    // Destructor
    ~Queue()
    {
        while (!emptys())
            pop();
    }

    // push(): insert at rear
    void push(const T& value)
    {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        if (emptys())
        {
            frontNode = rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        counts++;
    }

    // pop(): remove from front
    void pop()
    {
        if (emptys())
        {
            cout << "Queue is empty. Cannot pop.\n";
            return;
        }

        Node* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        counts--;

        if (frontNode == nullptr)
            rearNode = nullptr;
    }

    // front(): access front element
    T fronts()
    {
        if (emptys())
        {
            cout << "Queue is empty. No front element.\n";
            return T();   // return default value
        }
        return frontNode->data;
    }

    // back(): access last element
    T backs()
    {
        if (emptys())
        {
            cout << "Queue is empty. No back element.\n";
            return T();   // return default value
        }
        return rearNode->data;
    }

    // empty(): check if queue is empty
    bool emptys() const
    {
        return frontNode == nullptr;
    }

    // size(): number of elements
    int sizes() const
    {
        return counts;
    }
};

struct Card
 {
    string color;
    string value;
};


void initializeDeck(Queue<Card> &deck)
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


Card drawCard(Queue<Card> &deck)
 {
    Card c = deck.fronts();
    deck.pop();
    return c;
}


void shuffleDeck(Queue<Card> &deck)
{
    Card tempArray[108];
    int cont = 0;


    while (!deck.emptys())
    {
        tempArray[cont] = deck.fronts();
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



void dealInitialCards(Queue<Card> players[], int numPlayers, Queue<Card> &deck)
 {
    for (int r = 0; r < 7; r++)

    {

     for (int p = 0; p < numPlayers; p++)
           {

            players[p].push(drawCard(deck));
        }
    }
}


void showPlayerHand(Queue<Card> &hand)
 {
    int n = hand.sizes();
    cout << "Hand (" << n << "): ";
    for (int i = 0; i < n; i++)
        {
        Card c = hand.fronts(); hand.pop();
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

void winnercondition(Queue<Card> players[], int totalplayer, Queue<Card> &deck)
{
    bool found=false;
    for(int i=0;i<totalplayer;i++)
        {
        if(players[i].emptys())
        {
            cout<<"Congratulations! Player "<<i+1<<" wins the match!"<<endl;
            found=true;
        }
    }
    if(found)
     exit(0);

    if(deck.emptys())
        {
        int minCards=players[0].sizes();
    int winner=0;
     bool tie=false;
        for(int i=1;i<totalplayer;i++)
            {
            if(players[i].sizes()<minCards)
            {
                minCards=players[i].sizes();
          winner=i;
         tie=false;
         }
            else if(players[i].sizes()==minCards)

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
    Queue<Card> deck, discard;
    Queue<Card> players[totalplayer];

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

        Card top = discard.backs();
        if (forcedColor != "")
        {
            cout << "Forced color: " << forcedColor << endl;
        }
        cout << "Top discard: " << top.color << " " << top.value << endl;

        int handSize = players[cur].sizes();
        cout << "Choose card and -1 to draw): ";
        int choice;
        cin >> choice;
        cout<<endl;


        if (choice == -1)
            {
              if (deck.emptys())
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

            handSize = players[cur].sizes();
        }


        if (choice < 0 || choice >= handSize)
            {
            cout << "Invalid choice"<<endl;
            continue;
        }


        Queue<Card> tmp;
        Card play;
        for (int i = 0; i < handSize; i++)
        {
            Card c = players[cur].fronts();
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

          if (players[cur].sizes() == 1)
        {
            cout << "Say UNO! (1 = YES, 0 = NO): ";
            int sayUno;
            cin >> sayUno;
            cout << endl;

            if (sayUno != 1)
            {
                cout << "You forgot to say UNO! Penalty: Draw 4 cards." << endl;
                for (int i = 0; i < 4; i++)
                {
                    if (!deck.emptys())
                        {
                            players[cur].push(drawCard(deck));
                        }
                }
            }
            else
            {
                cout << "UNO!" << endl;
            }
        }
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

               if (deck.emptys())
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
        if (play.value == "Reverse")
            {
            cout << " game reversed so again " <<cur+1  <<" player's turn " << endl;

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
                    if (deck.emptys())
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
Queue<Card> tmpHand;

while (!players[cur].emptys())
{
    Card c = players[cur].fronts();
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
           if (!deck.emptys()) players[cur].push(drawCard(deck));
        }

    }
    else
     {
        cout << "Player " << nextPlayer + 1 << " could not match the color "<<endl;
        cout << "Player " << nextPlayer + 1 << " draws 6 cards "<<endl;
        for (int i = 0; i < 6; i++)
        {
             if (!deck.emptys())
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
