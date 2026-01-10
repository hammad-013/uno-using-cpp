#include <iostream>
#include <cstring>
#include <ctime>
#include <vector>
#include <cstdlib>
#include <raylib.h>
using namespace std;


template <typename T>
class Stack
{
private:
    T* arr;
    int capacity;
    int top;

    void resize()
    {
        capacity *= 2;
        T* newArr = new T[capacity];
        for (int i = 0; i < top; i++)
        {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }


public:
    Stack() : capacity(10), top(0)
    {
        arr = new T[capacity];
    }

    Stack(const Stack& other) : capacity(other.capacity), top(other.top)
    {
        arr = new T[capacity];
        for (int i = 0; i < top; i++)
        {
            arr[i] = other.arr[i];
        }
    }
      Stack& operator=(const Stack& other)
    {
        if (this != &other)
        {
            delete[] arr;
            capacity = other.capacity;
            top = other.top;
            arr = new T[capacity];
            for (int i = 0; i < top; i++)
            {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
~Stack()
    {
        delete[] arr;
    }

    void push(T value)
    {
        if (top >= capacity)
        {
            resize();
        }
        arr[top++] = value;
    }
   

 T pop()
    {
        if (isEmpty())
        {
            return T();
        }
        return arr[--top];
    }

    T peek() const
    {
        if (isEmpty())
        {
            return T();
        }
        return arr[top - 1];
    }

    bool isEmpty() const
    {
        return top == 0;
    }

    int sizee() const
    {
        return top;
    }

    void Clear()
    {
        top = 0;
    }

       void shuffle()
    {
        if (isEmpty() || sizee() == 1)
            return;

        int n = sizee();
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

    int sizee()
    {
        return cards.sizee();
    }

    bool isEmpty()
    {
        return cards.isEmpty();
    }
     string getCardAt(int index)
    {
        if (index < 0 || index >= sizee())
            return "";

        Stack<string> temp;
        string result = "";

        int n = sizee();
        int targetFromTop = n - 1 - index;

        int currentFromTop = 0;
        for (int i = 0; i<=targetFromTop;i++)
        {
            string card = cards.pop();
            if (i == targetFromTop)
                result = card;
            temp.push(card);
        }

        while (!temp.isEmpty())
            cards.push(temp.pop());

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
                break;
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
};
class Player
{
public:
    int id;
    Hand hand;

    Player() : id(0) {}
    Player(int playerId) : id(playerId) {}

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
        return hand.sizee();
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
        drawPile.Clear();
        discardPile.Clear();

        string colors[4] = {"Red","Blue","Green","Yellow"};
        for (int c = 0; c < 4; c++)
        {
            string color = colors[c];
            drawPile.push(color + "_0");
              for (int num = 1; num<=9; num++)
            {
                string numStr = to_string(num);
                drawPile.push(color + "_" + numStr);
                drawPile.push(color + "_" + numStr);
            }
            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Skip");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Reverse");
            drawPile.push(color + "_Draw_2");
            drawPile.push(color + "_Draw_2");
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
    string getTopDiscard() const
    {
        if (discardPile.isEmpty())
        {
            return "";
        }
        return discardPile.peek();
    }
     void reshuffleDiscard()
    {
        if (discardPile.sizee() <= 1)
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
        return drawPile.sizee();
    }
    int getDiscardPileSize()
    {
        return discardPile.sizee();
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

    bool challengeAvailable;
    int lastWildDraw4Player;
    string lastDrawnCard;
    bool drawnCardPlayable;
    int roundWinner;

    Player getPlayerAt(int index)
    {
        Stack<Player> temp;
        Player result;
        int coun = 0;

        while (!players.isEmpty())
        {
            Player p = players.pop();
            if (coun == index)
            {
                result = p;
            }
            temp.push(p);
            coun++;
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
        int countt = 0;
        while (!players.isEmpty())
        {
            Player p = players.pop();
            if (countt == index)
            {
                temp.push(updatedPlayer);
            }
            else
            {
                temp.push(p);
            }
            countt++;
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
    int getCardPoints(string cardName)
    {
        Card card(cardName);
        string value = card.getValue();

        if (value == "1") return 1;
        if (value == "2") return 2;
        if (value == "3") return 3;
        if (value == "4") return 4;
        if (value == "5") return 5;
        if (value == "6") return 6;
        if (value == "7") return 7;
        if (value == "8") return 8;
        if (value == "9") return 9;
        if (value == "0") return 0;

        if (value == "Draw_2" || value == "Reverse" || value == "Skip") return 20;

        if (card.isWild()) return 50;

        return 0;
    }

    void calculateGameScore(int winnerIndex)
    {
        int totalPoints = 0;
        for (int i = 0; i < totalPlayers; i++)
        {
            if (i != winnerIndex)
            {
                Player p = getPlayerAt(i);
                for (int j = 0; j < p.getHandSize(); j++)
                {
                    string cardName = p.hand.getCardAt(j);
                    totalPoints += getCardPoints(cardName);
                }
            }
        }
        Player winner = getPlayerAt(winnerIndex);
        winner.addScore(totalPoints);
        updatePlayerAt(winnerIndex, winner);
    }
public:
    Game() : currentPlayerIndex(0), totalPlayers(4), clockwise(true),
     currentColor(""), gameOver(false), challengeAvailable(false),
     lastWildDraw4Player(-1), lastDrawnCard(""), drawnCardPlayable(false),
     roundWinner(-1)
    {
        for (int i = 0; i < 4; i++)
        {
            players.push(Player(i));
        }
    }

    int getTotalPlayers() const
    {
         return totalPlayers;
    }
    int getCurrentPlayerIndex() const
    {
         return currentPlayerIndex;
    }
    bool getClockwise() const
    {
         return clockwise;
    }
    string getCurrentColor() const
    {
         return currentColor;
    }
    string getTopDiscard() const
    {
         return deck.getTopDiscard();
    }
    Player getPlayerAtPublic(int index)
    {
         return getPlayerAt(index);
    }
    bool isGameOver() const
    {
         return gameOver;
    }
    int getDrawPileSize()
    {
         return deck.getDrawPileSize();
    }
    string getLastDrawnCard() const
    {
        return lastDrawnCard;
    }
    int getPlayerScore(int playerIndex)
    {
        if (playerIndex >= 0 && playerIndex < totalPlayers)
        {
            Player p = getPlayerAt(playerIndex);
            return p.getScore();
        }
        return 0;
    }
    int getRoundWinner() const
    {
        return roundWinner;
    }
    void checkScoreLimit()
    {
        if (getGameWinner() != -1)
        {
            gameOver = true;
        }
    }
    int getGameWinner()
    {
        for (int i = 0; i < totalPlayers; i++)
        {
            Player p = getPlayerAt(i);
            if (p.getScore() >= 500)
                return i;
        }
        return -1;
    }
    void startGame()
    {
        deck.createDeck();
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
        if (start.getValue() == "Skip")
        {
            currentPlayerIndex = 1;
        }
        else if (start.getValue() == "Reverse")
        {
            clockwise = false;
        }
        else if (start.getValue() == "Draw_2")
        {
            Player p = getPlayerAt(0);
            p.drawCard(deck.drawCard());
            p.drawCard(deck.drawCard());
            updatePlayerAt(0, p);
            currentPlayerIndex = 1;
        }
        gameOver = false;
    }
    string drawCard()
    {
        return deck.drawCard();
    }
    bool playCard(int cardIndex, string& message)
    {
        Player currentPlayer = getPlayerAt(currentPlayerIndex);
        string playedCard = currentPlayer.playCard(cardIndex);

        if (playedCard == "")
        {
            message = "Invalid card!";
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return false;
        }
        Card played(playedCard);
        Card topCard(deck.getTopDiscard());

        if (!played.canPlayOn(topCard, currentColor))
        {
            message = "Cannot play this card!";
            currentPlayer.hand.addCard(playedCard);
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            return false;
        }

        deck.discard(playedCard);
        int cardPoints = getCardPoints(playedCard);
        currentPlayer.addScore(cardPoints);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        // ADD THIS LINE - Check if score limit reached
        checkScoreLimit();

        message = "Player " + to_string(currentPlayer.id) + " played: " + playedCard;

        if (currentPlayer.getHandSize() == 0)
        {
            gameOver = true;
            roundWinner = currentPlayerIndex;
            message = "Player " + to_string(currentPlayer.id) + " WINS!";
            return true;
        }

        // ADD THIS CHECK - If game is over due to score
        if (gameOver && currentPlayer.getHandSize() > 0)
        {
            roundWinner = currentPlayerIndex;
            message = "Player " + to_string(currentPlayer.id) + " reached 100 points and WINS!";
            return true;
        }

        Player updatedPlayer = getPlayerAt(currentPlayerIndex);
        if (updatedPlayer.getHandSize() == 1)
        {
            message = "Player " + to_string(currentPlayer.id) + " played: " + playedCard + " - UNO!";
        }
        if (!played.isWild())
        {
            currentColor = played.getColor();
        }

        handleCardEffect(played, currentPlayer);
        return true;
    }
    void handleCardEffect(Card played, Player& currentPlayer)
    {
        string value = played.getValue();

        if (value == "Draw_4")
        {
            challengeAvailable = true;
            lastWildDraw4Player = currentPlayerIndex;

            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);

            for (int i = 0; i < 4; i++)
            {
                nextPlayer.drawCard(deck.drawCard());
            }
            updatePlayerAt(nextIndex, nextPlayer);
            currentPlayerIndex = getNextPlayerIndex();
        }
        else if (value == "Skip")
        {
            currentPlayerIndex = getNextPlayerIndex();
        }
        else if (value == "Reverse")
        {
            clockwise = !clockwise;
            if (totalPlayers == 2)
            {
                currentPlayerIndex = getNextPlayerIndex();
            }
        }
        else if (value == "Draw_2")
        {
            int nextIndex = getNextPlayerIndex();
            Player nextPlayer = getPlayerAt(nextIndex);

            for (int i = 0; i < 2; i++)
            {
    void nextTurn()
    {
        currentPlayerIndex = getNextPlayerIndex();

    }

    void setCurrentColor(string color)

    {

        currentColor = color;
    }


    void drawCardForCurrentPlayer()
    {
        Player currentPlayer = getPlayerAt(currentPlayerIndex);
        lastDrawnCard = deck.drawCard();

        currentPlayer.drawCard(lastDrawnCard);
        updatePlayerAt(currentPlayerIndex, currentPlayer);

        Card drawnCard(lastDrawnCard);
        Card topCard(deck.getTopDiscard());
        drawnCardPlayable = drawnCard.canPlayOn(topCard, currentColor);
    }
    bool canPlayDrawnCard()
    {
        return drawnCardPlayable && !lastDrawnCard.empty();
    }

    void clearDrawnCard()
    {
        lastDrawnCard = "";
        drawnCardPlayable = false;
    }
    bool canChallenge()
    {
        return challengeAvailable;
    }

    void acceptWildDraw4()
    {
        challengeAvailable = false;
    }

    bool processChallenge(string& message)
    {
        if (!challengeAvailable)
        {
            message = "No challenge available!";
            return false;
        }

        challengeAvailable = false;

        Player challengedPlayer = getPlayerAt(lastWildDraw4Player);
        Card topCard(deck.getTopDiscard());

        bool hadPlayableCard = false;
        for (int i = 0; i < challengedPlayer.getHandSize(); i++)
        {
            string cardName = challengedPlayer.hand.getCardAt(i);
            Card card(cardName);
            if (!card.isWild() && card.getColor() == currentColor)
            {
                hadPlayableCard = true;
                break;
            }
        }

        if (hadPlayableCard)
        {
            message = "Challenge successful! Player " + to_string(lastWildDraw4Player) + " draws 4 cards!";

            Player currentPlayer = getPlayerAt(currentPlayerIndex);
            for (int i = 0; i < 4; i++)
            {
                currentPlayer.hand.removeCard(currentPlayer.hand.getCardAt(0));
                challengedPlayer.drawCard(deck.drawCard());
            }
            updatePlayerAt(currentPlayerIndex, currentPlayer);
            updatePlayerAt(lastWildDraw4Player, challengedPlayer);

            return true;
        }
        else
        {
            message = "Challenge failed! You draw 2 more cards!";
            Player currentPlayer = getPlayerAt(currentPlayerIndex);
            currentPlayer.drawCard(deck.drawCard());
            currentPlayer.drawCard(deck.drawCard());
            updatePlayerAt(currentPlayerIndex, currentPlayer);

            return false;
        }
    }

};
