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
    