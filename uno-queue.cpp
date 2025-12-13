#include "raylib.h"

int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(DARKGREEN);
    DrawText("PLAYER 1 TURN", 20, 20, 20, RED);
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
