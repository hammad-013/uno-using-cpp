#include <iostream>
#include <cstring>
#include <ctime>
#include <vector>
#include <cstdlib>
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

};
~Stack()
    {
        Clear();
    }

    void push(T value)
    {
        Node<T>* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        top++;
    }

    T pop()
    {
        if (isEmpty())
        {
            return T();
        }
        Node<T>* temp = topNode;
        T value = topNode->data;
        topNode = topNode->next;
        delete temp;
        top--;
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

    int sizee() const
    {
        return top;
    }

    void Clear()
    {
        while (!isEmpty())
        {
            pop();
        }
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