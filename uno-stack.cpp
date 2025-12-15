#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;


template <typename T>
class Stack
{
private:
    struct Node
    {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* topNode;
    int count;

public:
    Stack() : topNode(nullptr), count(0) {}


    Stack(const Stack& other) : topNode(nullptr), count(0)
    {
        if (other.topNode == nullptr) return;

        T* tempArray = new T[other.count];
        Node* current = other.topNode;
        int index = 0;
        while (current != nullptr)
        {
            tempArray[index++] = current->data;
            current = current->next;
        }
        for (int i = index - 1; i >= 0; i--)
        {
            push(tempArray[i]);
        }

        delete[] tempArray;
    }
    Stack& operator=(const Stack& other)
    {
        if (this != &other)
        {

            clear();
            if (other.topNode != nullptr)
            {
                T* tempArray = new T[other.count];

                Node* current = other.topNode;
                int index = 0;
                while (current != nullptr)
                {
                    tempArray[index++] = current->data;
                    current = current->next;
                }

                for (int i = index - 1; i >= 0; i--)
                {
                    push(tempArray[i]);
                }

                delete[] tempArray;
            }
        }
        return *this;
    }

    ~Stack()
    {
        clear();
    }

    void push(T value)
    {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    T pop()
    {
        if (isEmpty())
        {
            return T();
        }
        Node* temp = topNode;
        T value = topNode->data;
        topNode = topNode->next;
        delete temp;
        count--;
        return value;
    }

    T peek() const
    {
        if (isEmpty())
        {
            return T();
        }
        return topNode->data;
    }

    bool isEmpty() const
    {
        return topNode == nullptr;
    }

    int size() const
    {
        return count;
    }

    void clear()
    {
        while (!isEmpty())
        {
            pop();
        }
    }

    void shuffle()
    {
        if (isEmpty() || size() == 1)
            return;

        int n = size();
        Stack<T> temp1;
        while (!isEmpty())
        {
            temp1.push(pop());
        }
        for (int i = n - 1; i > 0; i--)
        {
            int j = rand() % (i + 1);

            Stack<T> temp2;
            T selectedItem;
            for (int k = 0; k <= i; k++)
            {
                T item = temp1.pop();
                if (k == j)
                {
                    selectedItem = item;
                }
                else
                {
                    temp2.push(item);
                }
            }

            push(selectedItem);
            while (!temp2.isEmpty())
            {
                temp1.push(temp2.pop());
            }
        }
        if (!temp1.isEmpty())
        {
            push(temp1.pop());
        }
    }
};
class Card
{
public:
    string fullName;

    Card() : fullName("") {}

    Card(string name) : fullName(name) {}

    string getColor()
    {
        if (fullName == "Wild" || fullName == "Wild_Draw_4")
        {
            return "Wild";
        }
        int underscorePos = -1;
        for (int i = 0; i < fullName.length(); i++)
        {
            if (fullName[i] == '_')
            {
                underscorePos = i;
                break;
            }
        }

        if (underscorePos == -1)
        {
            return "";
        }

        string color = "";
        for (int i = 0; i < underscorePos; i++)
        {
            color += fullName[i];
        }
        return color;
    }

    string getValue()
    {
        if (fullName == "Wild")
        {
            return "Wild";
        }
        if (fullName == "Wild_Draw_4")
        {
            return "Draw_4";
        }
        int underscorePos = -1;
        for (int i = 0; i < fullName.length(); i++)
        {
            if (fullName[i] == '_')
            {
                underscorePos = i;
                break;
            }
        }

        if (underscorePos == -1)
        {
            return fullName;
        }

        string value = "";
        for (int i = underscorePos + 1; i < fullName.length(); i++)
        {
            value += fullName[i];
        }
        return value;
    }

    bool isWild()
    {
        return (fullName == "Wild" || fullName == "Wild_Draw_4");
    }

    bool isSpecial()
    {
        string val = getValue();
        return (val == "Skip" || val == "Reverse" || val == "Draw_2" || val == "Draw_4");
    }

    bool canPlayOn(Card topCard, string currentColor)
    {
        if (isWild())
        {
            return true;
        }

        if (topCard.isWild())
        {
            return (getColor() == currentColor);
        }

        return (getColor() == topCard.getColor() || getValue() == topCard.getValue());
    }
};
class Hand
{
private:
    Stack<string> cards;

public:
    void addCard(string card)
    {
        cards.push(card);
    }

    int size()
    {
        return cards.size();
    }

    bool isEmpty()
    {
        return cards.isEmpty();
    }
    string getCardAt(int index)
    {
        if (index < 0 || index >= size())
        {
            return "";
        }

        Stack<string> temp;
        string result = "";
        int currentIndex = 0;
        while (!cards.isEmpty())
        {
            string card = cards.pop();
            if (currentIndex == index)
            {
                result = card;
            }
            temp.push(card);
            currentIndex++;
        }
        while (!temp.isEmpty())
        {
            cards.push(temp.pop());
        }

        return result;
    }
    bool removeCard(string cardToRemove)
    {
        Stack<string> temp;
        bool found = false;

        while (!cards.isEmpty())
        {
            string card = cards.pop();
            if (card == cardToRemove && !found)
            {
                found = true;
            }
            else
            {
                temp.push(card);
            }
        }

        while (!temp.isEmpty())
        {
            cards.push(temp.pop());
        }

        return found;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "  (empty hand)" << endl;
            return;
        }

        Stack<string> temp;
        int index = 0;

        while (!cards.isEmpty())
        {
            string card = cards.pop();
            temp.push(card);
        }

        while (!temp.isEmpty())
        {
            string card = temp.pop();
            cout << "  [" << index << "] " << card << endl;
            cards.push(card);
            index++;
        }
    }
    bool hasPlayableCard(Card topCard, string currentColor)
    {
        Stack<string> temp;
        bool canPlay = false;

        while (!cards.isEmpty())
        {
            string cardStr = cards.pop();
            Card card(cardStr);
            if (card.canPlayOn(topCard, currentColor))
            {
                canPlay = true;
            }
            temp.push(cardStr);
        }
        while (!temp.isEmpty())
        {
            cards.push(temp.pop());
        }

        return canPlay;
    }
};

class Player
{
public:
    int id;
    string name;
    Hand hand;

    Player() : id(0), name("") {}

    Player(int playerId, string playerName) : id(playerId), name(playerName) {}

    void drawCard(string card)
    {
        hand.addCard(card);
    }

    string playCard(int index)
    {
        string card = hand.getCardAt(index);
        if (card != "")
        {
            hand.removeCard(card);
        }
        return card;
    }

    int getHandSize()
    {
        return hand.size();
    }

    void showHand()
    {
        cout << name << "'s hand:" << endl;
        hand.display();
    }
};

class Deck
{
private:
    Stack<string> drawPile;
    Stack<string> discardPile;

public:
    void createDeck()
    {
        drawPile.clear();
        discardPile.clear();

        string colors[4] = {"Red", "Blue", "Green", "Yellow"};

        for (int c = 0; c < 4; c++)
        {
            string color = colors[c];
            drawPile.push(color + "_0");

            for (int num = 1; num <= 9; num++)
            {
                string numStr = "";
                int temp = num;
                while (temp > 0)
                {
                    numStr = char('0' + (temp % 10)) + numStr;
                    temp /= 10;
                }

                drawPile.push(color + "_" + numStr);
                drawPile.push(color + "_" + numStr);
            }

            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Draw_2");
            drawPile.push(color + "_Draw_2");
        }
        for (int i = 0; i < 4; i++)
        {
            drawPile.push("Wild");
            drawPile.push("Wild_Draw_4");
        }

        drawPile.shuffle();
    }

    string drawCard()
    {
        if (drawPile.isEmpty())
        {
            reshuffleDiscard();
        }

        if (drawPile.isEmpty())
        {
            return "";
        }

        return drawPile.pop();
    }

    void discard(string card)
    {
        discardPile.push(card);
    }

    string getTopDiscard()
    {
        if (discardPile.isEmpty())
        {
            return "";
        }
        return discardPile.peek();
    }

    void reshuffleDiscard()
    {
        if (discardPile.size() <= 1)
        {
            return;
        }

        string topCard = discardPile.pop();

        while (!discardPile.isEmpty())
        {
            drawPile.push(discardPile.pop());
        }

        drawPile.shuffle();
        discardPile.push(topCard);
    }

    int getDrawPileSize()
    {
        return drawPile.size();
    }

    int getDiscardPileSize()
    {
        return discardPile.size();
    }
    string flipStartCard()
    {
        string card = drawCard();
        if (card != "")
        {
            discard(card);
        }
        return card;
    }
};
class Game
{
private:
    Stack<Player> players;
    Deck deck;
    int currentPlayerIndex;
    int totalPlayers;
    bool clockwise;
    string currentColor;
    bool gameOver;
    bool calledUno;
    int skipNext;

    Player getPlayerAt(int index)
    {
        Stack<Player> temp;
        Player result;
        int count = 0;

        while (!players.isEmpty())
        {
            Player p = players.pop();
            if (count == index)
            {
                result = p;
            }
            temp.push(p);
            count++;
        }

        while (!temp.isEmpty())
        {
            players.push(temp.pop());
        }

        return result;
    }
    void updatePlayerAt(int index, Player updatedPlayer)
    {
        Stack<Player> temp;
        int count = 0;

        while (!players.isEmpty())
        {
            Player p = players.pop();
            if (count == index)
            {
                temp.push(updatedPlayer);
            }
            else
            {
                temp.push(p);
            }
            count++;
        }

        while (!temp.isEmpty())
        {
            players.push(temp.pop());
        }
    }

    int getNextPlayerIndex()
    {
        if (clockwise)
        {
            return (currentPlayerIndex + 1) % totalPlayers;
        }
        else
        {
            return (currentPlayerIndex - 1 + totalPlayers) % totalPlayers;
        }
    }

public:
    Game() : currentPlayerIndex(0), totalPlayers(0), clockwise(true),
             currentColor(""), gameOver(false), calledUno(false), skipNext(0) {}

    void addPlayer(string name)
    {
        Player p(totalPlayers + 1, name);
        players.push(p);
        totalPlayers++;
    }

    void startGame()
    {
        if (totalPlayers < 2)
        {
            cout << "Need at least 2 players to start!" << endl;
            return;
        }

        cout << "\n=== Starting UNO Game ===" << endl;
        cout << "Players: " << totalPlayers << endl << endl;


        deck.createDeck();


        cout << "Dealing 7 cards to each player..." << endl;
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            for (int j = 0; j < 7; j++)
            {
                p.drawCard(deck.drawCard());
            }
            updatePlayerAt(i, p);
        }


        string startCard = deck.flipStartCard();
        Card start(startCard);

        while (start.isWild())
        {
            deck.discard(deck.drawCard());
            startCard = deck.getTopDiscard();
            start = Card(startCard);
        }

        currentColor = start.getColor();

        cout << "\nStart card: " << startCard << endl;
        cout << "Current color: " << currentColor << endl;


        if (start.getValue() == "Skip")
        {
            cout << "Start card is Skip! First player skipped!" << endl;
            currentPlayerIndex = 1;
        }
        else if (start.getValue() == "Reverse")
        {
            cout << "Start card is Reverse! Playing counterclockwise!" << endl;
            clockwise = false;
        }
        else if (start.getValue() == "Draw_2")
        {
            cout << "Start card is Draw 2! First player draws 2 cards!" << endl;
            Player p = getPlayerAt(0);
            p.drawCard(deck.drawCard());
            p.drawCard(deck.drawCard());
            updatePlayerAt(0, p);
            currentPlayerIndex = 1;
        }

        gameOver = false;
    }

    void playTurn()
    {
        if (gameOver)
        {
            return;
        }

        Player currentPlayer = getPlayerAt(currentPlayerIndex);

        cout << "\n========================================" << endl;
        cout << "Turn: " << currentPlayer.name << endl;
        cout << "Direction: " << (clockwise ? "Clockwise" : "Counterclockwise") << endl;
        cout << "Top card: " << deck.getTopDiscard() << endl;
        cout << "Current color: " << currentColor << endl;
        cout << "Cards in hand: " << currentPlayer.getHandSize() << endl;
        cout << "========================================" << endl;

        currentPlayer.showHand();

        Card topCard(deck.getTopDiscard());
        bool hasPlayable = currentPlayer.hand.hasPlayableCard(topCard, currentColor);

        if (!hasPlayable)
        {
            cout << "\nNo playable cards! Drawing a card..." << endl;
            string drawnCard = deck.drawCard();
            currentPlayer.drawCard(drawnCard);
            cout << "Drew: " << drawnCard << endl;

            Card drawn(drawnCard);
            if (drawn.canPlayOn(topCard, currentColor))
            {
                cout << "Drawn card is playable! Do you want to play it? (1=Yes, 0=No): ";
                int choice;
                cin >> choice;

                if (choice == 1)
                {
                    currentPlayer.hand.removeCard(drawnCard);
                    deck.discard(drawnCard);
                    handleCardEffect(drawn, currentPlayer);
                    updatePlayerAt(currentPlayerIndex, currentPlayer);
                    checkWin(currentPlayer);
                    return;
                }
            }

            updatePlayerAt(currentPlayerIndex, currentPlayer);
            nextTurn();
            return;
        }

        cout << "\nEnter card index to play (or -1 to draw): ";
        int choice;
        cin >> choice;

        if (choice == -1)
        {
            string drawnCard = deck.drawCard();
            currentPlayer.drawCard(drawnCard);
            cout << "Drew: " << drawnCard << endl;
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            nextTurn();
            return;
        }

        string playedCard = currentPlayer.playCard(choice);

        if (playedCard == "")
        {
            cout << "Invalid card index!" << endl;
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return;
        }

        Card played(playedCard);

        if (!played.canPlayOn(topCard, currentColor))
        {
            cout << "Cannot play this card! It doesn't match color or value!" << endl;
            currentPlayer.hand.addCard(playedCard);
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return;
        }
        deck.discard(playedCard);
        cout << "\n" << currentPlayer.name << " played: " << playedCard << endl;
        if (currentPlayer.getHandSize() == 1)
        {
            cout << "\n*** " << currentPlayer.name << " has UNO! (1 card left) ***" << endl;
        }

        handleCardEffect(played, currentPlayer);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        checkWin(currentPlayer);
    }

    void handleCardEffect(Card played, Player& currentPlayer)
    {
        string value = played.getValue();

        if (played.isWild())
        {
            cout << "\nChoose color for Wild card:" << endl;
            cout << "1. Red" << endl;
            cout << "2. Blue" << endl;
            cout << "3. Green" << endl;
            cout << "4. Yellow" << endl;
            cout << "Choice: ";
            int colorChoice;
            cin >> colorChoice;

            if (colorChoice == 1) currentColor = "Red";
            else if (colorChoice == 2) currentColor = "Blue";
            else if (colorChoice == 3) currentColor = "Green";
            else if (colorChoice == 4) currentColor = "Yellow";
            else currentColor = "Red";

            cout << "New color: " << currentColor << endl;

            if (value == "Draw_4")
            {
                int nextIndex = getNextPlayerIndex();
                Player nextPlayer = getPlayerAt(nextIndex);
                cout << nextPlayer.name << " draws 4 cards and loses turn!" << endl;

                for (int i = 0; i < 4; i++)
                {
                    nextPlayer.drawCard(deck.drawCard());
                }
                updatePlayerAt(nextIndex, nextPlayer);
                currentPlayerIndex = getNextPlayerIndex();
            }
        }
        else if (value == "Skip")
        {
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);
            cout << nextPlayer.name << " is skipped!" << endl;
            currentPlayerIndex = getNextPlayerIndex();
            currentColor = played.getColor();
        }
        else if (value == "Reverse")
        {
            clockwise = !clockwise;
            cout << "Direction reversed! Now playing " << (clockwise ? "clockwise" : "counterclockwise") << endl;
            currentColor = played.getColor();
            if (totalPlayers == 2)
            {
                currentPlayerIndex = getNextPlayerIndex();
            }
        }
        else if (value == "Draw_2")
        {
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);
            cout << nextPlayer.name << " draws 2 cards and loses turn!" << endl;

            for (int i = 0; i < 2; i++)
            {
                nextPlayer.drawCard(deck.drawCard());
            }
            updatePlayerAt(nextIndex, nextPlayer);
            currentPlayerIndex = getNextPlayerIndex();
            currentColor = played.getColor();
        }
        else
        {
            currentColor = played.getColor();
        }
    }

    void nextTurn()
    {
        currentPlayerIndex = getNextPlayerIndex();
    }

    void checkWin(Player p)
    {
        if (p.getHandSize() == 0)
        {
            cout << "\n\n****************************************" << endl;
            cout << "*** " << p.name << " WINS THE GAME! ***" << endl;
            cout << "****************************************\n" << endl;
            gameOver = true;
        }
    }

    bool isGameOver()
    {
        return gameOver;
    }

    void showGameState()
    {
        cout << "\n--- Current Game State ---" << endl;
        cout << "Top card: " << deck.getTopDiscard() << endl;
        cout << "Current color: " << currentColor << endl;
        cout << "Direction: " << (clockwise ? "Clockwise" : "Counterclockwise") << endl;
        cout << "Draw pile: " << deck.getDrawPileSize() << " cards" << endl;

        cout << "\nPlayers:" << endl;
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            string marker = (i == currentPlayerIndex) ? " <-- CURRENT" : "";
            cout << "  " << p.name << ": " << p.getHandSize() << " cards" << marker << endl;
        }
        cout << endl;
    }
};

int main()
{
    srand(time(0));

    cout << "=== UNO Game ===" << endl << endl;

    Game game;
    cout << "Enter number of players (2-4): ";
    int numPlayers;
    cin >> numPlayers;

    if (numPlayers < 2 || numPlayers > 4)
    {
        cout << "Invalid number of players!" << endl;
        return 1;
    }

    cin.ignore();
    for (int i = 0; i < numPlayers; i++)
    {
        cout << "Enter name for Player " << (i + 1) << ": ";
        string name;
        getline(cin, name);
        game.addPlayer(name);
    }

    game.startGame();
    while (!game.isGameOver())
    {
        game.playTurn();

        if (!game.isGameOver())
        {
            game.nextTurn();

            cout << endl<< "Press Enter to continue...";
            cin.ignore();
        }
    }

    cout <<  endl<< "Thanks for playing UNO!" << endl;

    return 0;
}






