#include <iostream>
#include <raylib.h>

using namespace std;

Color Mint = Color{152, 255, 152, 255};
Color SeaGreen = Color{60, 179, 113, 255};
// Color SeaGreen = Color{60, 179, 113, 255};
Color Dandilion = Color{240, 225, 48, 255};

int PlayerScore = 0;
int CPUScore = 0;

const int ScreenWidth = 1280;
const int ScreenHeight = 720;
const int PaddleHeight = 120;
const int PaddleWidth = 25;
const int PaddleOffset = 10;

class Ball {
public:
    float PosX, PosY;
    int SpeedX, SpeedY;
    int Radius;

    void Draw() {
        DrawCircle(PosX, PosY, Radius, Dandilion);
    }

    void Update() {
        PosX += SpeedX;
        PosY += SpeedY;

        if (PosY + Radius >= GetScreenHeight() || PosY - Radius <= 0) {
            SpeedY *= -1;
        }

        if (PosX + Radius >= GetScreenWidth()) {
            PlayerScore++;
            resetBall();
        }

        if (PosX - Radius <= 0) {
            CPUScore++;
            resetBall();
        }
    }

private:
    void resetBall() {
        PosX = GetScreenWidth() / 2;
        PosY = GetScreenHeight() / 2;
        
        int speedChoices[2] = {-1, 1};
        SpeedX *= speedChoices[GetRandomValue(0, 1)];
        SpeedY *= speedChoices[GetRandomValue(0, 1)];
    }
};

Ball ball;

class Paddle {
public:
    float PosX, PosY;
    float Width, Height;
    int Speed;

    void Draw() {
        DrawRectangle(PosX, PosY, Width, Height, BLACK);
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            PosY -= Speed;
        }
        
        if (IsKeyDown(KEY_DOWN)) {
            PosY += Speed;
        }

        limitMovement();
    }

    void UpdateCollisions() {
        if (CheckCollisionCircleRec(Vector2{ball.PosX, ball.PosY}, ball.Radius, Rectangle{PosX, PosY, Width, Height})) {
            ball.SpeedX *= -1;
        }
    }

protected:
    void limitMovement() {
        if (PosY < 0) {
            PosY = 0;
        }

        if (PosY > GetScreenHeight() - Height) {
            PosY = GetScreenHeight() - Height;
        }
    }
};

class CPUPaddle: public Paddle {
public:
    void Update(int ballY) {
        if (PosY + Height / 2 > ballY) {
            PosY -= Speed;
        }

        if (PosY + Height / 2 < ballY) {
            PosY += Speed;
        }

        limitMovement();
    }
};

Paddle player;
CPUPaddle cpu;

void Update() {
    ball.Update();
    player.Update();
    cpu.Update(ball.PosY);
}

void Draw() {
    BeginDrawing();

    ClearBackground(Mint);

    // Middle Court Circle
    DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 160, WHITE);

    // Court Divider
    DrawLine(ScreenWidth / 2, 0, ScreenWidth / 2, ScreenHeight, SeaGreen);

    // Ball
    ball.Draw();
    // Paddle 1
    player.Draw();
    // Paddle 2
    cpu.Draw();

    // Scores
    DrawText(TextFormat("%i", PlayerScore), GetScreenWidth() / 4 - 40, 20, 80, BLACK);
    DrawText(TextFormat("%i", CPUScore), 3 * GetScreenWidth() / 4 - 40, 20, 80, BLACK);

    EndDrawing();
}

void UpdateCollisions() {
    player.UpdateCollisions();
    cpu.UpdateCollisions();
}

int main() {
    InitWindow(ScreenWidth, ScreenHeight, "Pong");

    int frameRate = 60;

    SetTargetFPS(frameRate);

    ball.Radius = 20;
    ball.PosX = ScreenWidth / 2;
    ball.PosY = ScreenHeight / 2;
    ball.SpeedX = 7;
    ball.SpeedY = 7;

    player.Width = 25;
    player.Height = 120;
    player.PosX = PaddleOffset;
    player.PosY = ScreenHeight / 2 - player.Height / 2;
    player.Speed = 5;

    cpu.Width = 25;
    cpu.Height = 120;
    cpu.PosX = ScreenWidth - cpu.Width - PaddleOffset;
    cpu.PosY = ScreenHeight / 2 - cpu.Height / 2;
    cpu.Speed = 5;

    while(!WindowShouldClose()) {
        // Input
        Update();
        // Logic
        UpdateCollisions();
        // Rendering
        Draw();
    }

    CloseWindow();

    return 0;
}