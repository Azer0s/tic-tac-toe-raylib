#include <stdio.h>
#include <raylib.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

#define WIDTH 600
#define HEIGHT 600
#define PAD_FACTOR 0.005

#define BOARD_SIZE 3
#define BOARD_SIZEF (BOARD_SIZE * 1.0)

bool res[BOARD_SIZE][BOARD_SIZE] = {0};
int winner = 0;

bool is_win(int board[BOARD_SIZE][BOARD_SIZE]) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    int p = board[i][0];
    bool is_win = true;

    for (int j = 1; j < BOARD_SIZE; j++) {
      is_win = is_win && p == board[i][j];
    }
    if (is_win && p != 0) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (j == 0 || j == BOARD_SIZE - 1) {
          res[i][j] = true;
        }
      }
      winner = p;
      return true;
    }

    p = board[0][i];
    is_win = true;
    for (int j = 1; j < BOARD_SIZE; j++) {
      is_win = is_win && p == board[j][i];
    }
    if (is_win && p != 0) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (j == 0 || j == BOARD_SIZE - 1) {
          res[j][i] = true;
        }
      }
      winner = p;
      return true;
    }
  }

  int m = board[0][0];
  bool is_win = true;
  for (int i = 0; i < BOARD_SIZE; i++) {
    is_win = is_win && board[i][i] == m;
  }
  if (is_win && m != 0) {
    for (int i = 0; i < BOARD_SIZE; i++) {
      if (i == 0 || i == BOARD_SIZE - 1) {
        res[i][i] = true;
      }
    }
    winner = m;
    return true;
  }

  m = board[0][BOARD_SIZE - 1];
  is_win = true;
  for (int i = 0; i < BOARD_SIZE; i++) {
    is_win = is_win && board[i][BOARD_SIZE - 1 - i] == m;
  }
  if (is_win && m != 0) {
    for (int i = 0; i < BOARD_SIZE; i++) {
      if (i == 0 || i == BOARD_SIZE - 1) {
        res[i][BOARD_SIZE - 1 - i] = true;
      }
    }
    winner = m;
    return true;
  }

  return false;
}

int main(int argc, char const *argv[]) {
  assert(BOARD_SIZE % 2 == 1 && "BOARD_SIZE must be an odd number!");

  InitWindow(WIDTH, HEIGHT, "Tic Tac Toe");
  SetTargetFPS(60);

  srand(time(NULL));

  int board[BOARD_SIZE][BOARD_SIZE] = {0};
  int player = (rand() % 2) + 1;
  bool won = false;
  printf("Starting game with player %d\n", player);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R)) {
      for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
          board[i][j] = 0;
          res[i][j] = false;
          won = false;
          winner = 0;
        }
      }
      player = (rand() % 2) + 1;
    }

    BeginDrawing();
    {
      if (is_win(board) && !won) {
        for (int i = 0; i < BOARD_SIZE; i++) {
          for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
              board[i][j] = -1;
            }
          }
        }
        won = true;
        printf("Player %d won!\n", winner);
      }

      ClearBackground(DARKGRAY);
      float mouseX = GetMouseX();
      float mouseY = GetMouseY();

      float padX = GetRenderWidth() * PAD_FACTOR;
      float padY = GetRenderHeight() * PAD_FACTOR;

      for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
          float x = ((GetRenderWidth() / BOARD_SIZEF) * i) + padX;
          float y = ((GetRenderHeight() / BOARD_SIZEF) * j) + padY;
          float width = (GetRenderWidth() / BOARD_SIZEF) - (2 * padX);
          float height = GetRenderHeight() / BOARD_SIZEF - (2 * padY);

          switch (board[i][j]) {
          case 0: {
              if ((mouseX >= x && x + width >= mouseX) &&
                (mouseY >= y && y + height >= mouseY)) {

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                  board[i][j] = player;
                  player = 3 - player;
                  continue;
                }

                DrawRectangle(x, y, width, height, WHITE);
                continue;
              }

              DrawRectangle(x, y, width, height, LIGHTGRAY);
            } break;
          case 1: {
              DrawRectangle(x, y, width, height, LIGHTGRAY);
              DrawRing((Vector2){x + width / 2, y + height / 2}, height * 0.3, height * 0.4, 0, 360, 100, RED);
            } break;
          case 2: {
              float pad = height * 0.2;
              DrawRectangle(x, y, width, height, LIGHTGRAY);
              DrawLineEx((Vector2){x + pad, y + pad}, (Vector2){x + width - pad, y + height - pad}, height * 0.1, BLUE);
              DrawLineEx((Vector2){x + width - pad, y + pad}, (Vector2){x + pad, y + height - pad}, height * 0.1, BLUE);
            } break;
          default: {
              DrawRectangle(x, y, width, height, LIGHTGRAY);
            } break;
          }
        }
      }

      if (won) {
        Vector2 first;
        Vector2 second;
        Vector2 *current = NULL;

        current = &first;

        for (int i = 0; i < BOARD_SIZE; i++) {
          for (int j = 0; j < BOARD_SIZE; j++) {
            if (res[i][j]) {
              current->x = i;
              current->y = j;

              if (current == &first) {
                current = &second;
              }
            }
          }
        }

        //printf("x = %f, y = %f\n", first.x, first.y);
        //printf("x = %f, y = %f\n", second.x, second.y);

        first.x = ((GetRenderWidth() / BOARD_SIZEF) * first.x) + padX;
        first.y = ((GetRenderHeight() / BOARD_SIZEF) * first.y) + padY;
        first.x += ((GetRenderWidth() / BOARD_SIZEF) - (2 * padX)) / 2.0;
        first.y += ((GetRenderHeight() / BOARD_SIZEF) - (2 * padY)) / 2.0;

        second.x = ((GetRenderWidth() / BOARD_SIZEF) * second.x) + padX;
        second.y = ((GetRenderHeight() / BOARD_SIZEF) * second.y) + padY;
        second.x += ((GetRenderWidth() / BOARD_SIZEF) - (2 * padX)) / 2.0;
        second.y += ((GetRenderHeight() / BOARD_SIZEF) - (2 * padY)) / 2.0;

        DrawLineEx(first, second, GetRenderWidth() * 0.04, WHITE);
      }
    }
    EndDrawing();
  }

  return 0;
}
