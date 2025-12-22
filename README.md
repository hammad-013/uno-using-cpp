# UNO Card Game in C++ Using Data Structures (with raylib GUI)

## Overview
An implementation of the classic UNO card game in C++, featuring multiple versions that demonstrate key data structures. The project is actively evolving with the addition of a graphical user interface (GUI) using raylib.

## Implementations

- **uno-linkedlist.cpp**
- **uno-queue.cpp**
- **uno-stack.cpp**


## Features

- Full standard UNO rules: color/number matching, action cards (Skip, Reverse, Draw Two, Wild, Wild Draw Four)
- Multi-player turn-based gameplay
- Random shuffling and dealing
- Console interface for core logic
- GUI: Graphical elements added using raylib (e.g., start screen; more in development)

## Prerequisites

### C++ Compiler
- C++ compiler (g++, clang++, MSVC)

### For GUI
- raylib installed and linked

### Installing raylib
Follow the official guide: [https://github.com/raysan5/raylib#build-and-installation](https://github.com/raysan5/raylib#build-and-installation)

*(Quick examples: `apt install libraylib-dev` on Ubuntu, or build from source.)*

## Build and Run

### Clone the repository:
```bash
git clone https://github.com/hammad-013/uno-using-cpp.git
cd uno-using-cpp
```

### Compile a version:
```bash
g++ -std=c++11 uno-stack.cpp -o uno
```
(Adjust flags for your platform, e.g., add -lGL -lm -lpthread -ldl -lrt -lX11 on Linux)

### Run a version:
```bash
./uno-linkedlist
```

## Gameplay
- Each player receives 7 cards.
- Match the top discard card by color or number.
- Use action cards strategically.
- First to empty hand wins!

## Project Structure
```
uno-using-cpp/
├── .gitignore
├── README.md                  # Documentation
├── uno-linkedlist.cpp         # Linked list implementation
├── uno-queue.cpp              # Queue implementation
├── uno-stack.cpp              # Stack implementation
```

## Contributors
- [Hammad Shabbir](https://github.com/hammad-013)
- [Arfa Tayyabah](https://github.com/arfa-tayyabah)
- [Abdul Rehman](https://github.com/Ghostaura-svg)
- [Momina Umar](https://github.com/MominaUmar74)
- [Junaid Gondal](https://github.com/junaid-gondal)
- [Sadia Khan](https://github.com/SadiaKhan899)

Thanks to everyone helping build this project! 💌
