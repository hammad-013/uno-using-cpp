UNO Game Implementation - Data Structures Project

A C++ implementation of the UNO card game using three different data structures: Stack, Queue, and Linked List. Developed as a university project for Data Structures course at Namal University Mianwali.
Features

    Three complete implementations using different data structures

    Interactive GUI built with Raylib library

    Supports 4 players with standard UNO rules

    Full rule enforcement (UNO call, challenges, scoring, etc.)

    Performance analysis and comparison of each data structure

Data Structures Used

    Stack: LIFO-based implementation for deck and discard piles

    Queue: FIFO-based implementation for sequential card control

    Linked List: Doubly linked list for flexible card operations

Project Structure

The project contains three separate implementations, each demonstrating how different data structures affect game performance and implementation complexity.
Time Complexity Analysis
Operation	Stack	Linked List	Queue
Access card by index	O(n)	O(n)	O(n)
Play card	O(n)	O(n)	O(n)
Display hand (GUI)	O(n²)	O(n)	O(n²)
Get hand size	O(1)	O(n)	O(1)
Requirements

    C++ Compiler (C++11 or later)

    Raylib library

Getting Started

    Clone the repository

    Ensure Raylib is installed

    Compile and run any of the three versions

Contributors

    Linked List Version: Hammad & Abdul Rehman

    Queue Version: Momina Umer & Junaid

    Stack Version: Arfa & Sadia

Course: Data Structures
Instructor: Mr. Abdul Rafay
Institution: Namal University Mianwali
