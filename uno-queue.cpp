#include "raylib.h"

int main()
{
    InitWindow(800, 600, "UNO Card Image Test");
    SetTargetFPS(60);

    Texture2D cardTexture = LoadTexture("assets/cards/red_5.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGREEN);

        DrawTexture(cardTexture, 350, 200, WHITE);

        EndDrawing();
    }

    UnloadTexture(cardTexture);
    CloseWindow();
    return 0;
}
string getCardImagePath(const Card& c)
{
    string path = "assets/cards/";


    string color = c.color;
    for (char &ch : color)
        ch = tolower(ch);


    if (color == "wild")
    {
        if (c.value == "+4")
            return path + "wild_4.png";
        else
            return path + "wild_wild.png";
    }


    path += color + "_";

    string value = c.value;
    for (char &ch : value)
        ch = tolower(ch);

    path += value + ".png";

    return path;
}
string getCardImagePath(const Card& c)
{
    string path = "assets/cards/";


    string color = c.color;
    for (char &ch : color)
        ch = tolower(ch);


    if (color == "wild")
    {
        if (c.value == "+4")
            return path + "wild_4.png";
        else
            return path + "wild_wild.png";
    }

    // Normal cards
    path += color + "_";

    string value = c.value;
    for (char &ch : value)
        ch = tolower(ch);

    path += value + ".png";

    return path;
}
