#include <cmath>
#include <iostream>
#include <raylib.h>
#include <string>

using namespace std;

const int windowWidth{1200}, windowHeight{800};

enum PlayerDirections
{
    LEFT,
    RIGHT,
    DOWN,
    UP
};

class Player
{
public:
    int score{0};
    float yPos;
    Rectangle rectangle;
    int movementSpeed;

    Player(float xPosition, float yPosition, int movementSpeed = 10)
    {
        xPos = xPosition;
        yPos = yPosition;
        rectangle = {float(xPos), float(yPos), 40, 200};
        this->movementSpeed = movementSpeed;
    }

    void increaseScore()
    {
        score++;
    }

    void move(PlayerDirections direction, int windowHeight, int ballVerticalSpeed = -1)
    {
        switch (direction)
        {
        case DOWN:
            if ((yPos + rectangle.height + movementSpeed) < windowHeight)
            {
                yPos += ballVerticalSpeed == -1 ? movementSpeed : ballVerticalSpeed < movementSpeed ? ballVerticalSpeed
                                                                                                    : movementSpeed;
            }
            /* code */
            break;
        case UP:
            if ((yPos - movementSpeed) > 0)
            {
                yPos -= ballVerticalSpeed == -1 ? movementSpeed : ballVerticalSpeed < movementSpeed ? ballVerticalSpeed
                                                                                                    : movementSpeed;
            }
            /* code */
            break;
        default:
            break;
        }

        rectangle.y = yPos;
    }

    void setMoveSpeed(int movementSpeed)
    {
        this->movementSpeed = movementSpeed;
    }

private:
    float xPos;
};

enum BallHorizontalDirections
{
    BALL_TO_LEFT,
    BALL_TO_RIGHT,
};

enum BallVerticalDirections
{
    BALL_TO_UP,
    BALL_TO_DOWN,
};

class Ball
{
public:
    int xPos;
    int yPos;
    int xInitialPos;
    int yInitialPos;
    int radius;
    Color color;
    int speed;
    int ySum;
    float yPosEndDestination{-1};
    BallHorizontalDirections ballHorizontalDirection;
    BallVerticalDirections ballVerticalDirection;

    Ball(int x, int y, int radiusProvided, Color colorProvided, int speedProvided)
    {
        xPos = x;
        yPos = y;
        xInitialPos = xPos;
        yInitialPos = yPos;
        radius = radiusProvided;
        color = colorProvided;
        speed = speedProvided;
        ballHorizontalDirection = BALL_TO_LEFT;
    }

    void resetPosition(bool gameAgainstCPU = false, int secondPlayerOffset = 0)
    {
        xPos = xInitialPos;
        yPos = yInitialPos;
        ySum = 0;
        yPosEndDestination = -1;
    }

    void changeBallHorizontalDirection()
    {
        if (ballHorizontalDirection == BALL_TO_LEFT)
        {
            ballHorizontalDirection = BALL_TO_RIGHT;
        }
        else if (ballHorizontalDirection == BALL_TO_RIGHT)
        {
            ballHorizontalDirection = BALL_TO_LEFT;
        }
    }

    void changeBallVerticalDirection()
    {
        if (ballVerticalDirection == BALL_TO_UP)
        {
            ballVerticalDirection = BALL_TO_DOWN;
        }
        else if (ballVerticalDirection == BALL_TO_DOWN)
        {
            ballVerticalDirection = BALL_TO_UP;
        }

        // calculateYCoordinateDestination();
    }

    void bounce(Player player)
    {
        float halfOfRectangle{player.rectangle.height / 2};
        float yPositionOfMiddlePointOfRectangle{float(player.yPos) + halfOfRectangle};

        ySum = fabs(yPositionOfMiddlePointOfRectangle - yPos) * 1 / 10;

        if (yPos < yPositionOfMiddlePointOfRectangle)
        {
            ballVerticalDirection = BALL_TO_UP;
        }
        else if (yPos > yPositionOfMiddlePointOfRectangle)
        {
            ballVerticalDirection = BALL_TO_DOWN;
        }

        // calculateYCoordinateDestination();
    }
};

int main()
{
    bool gameAgainstCPU{false};
    Ball ball{windowWidth / 2, windowHeight / 2, 20, WHITE, 9};
    Player player1{20, float(windowHeight) / 2 - 100}, player2{float(windowWidth) - 60, float(windowHeight) / 2 - 100, gameAgainstCPU ? 6 : 10};

    Color green{20, 160, 133, 255};

    InitWindow(windowWidth, windowHeight, "Pong game");

    SetTargetFPS(60);

    while (WindowShouldClose() == false)
    {
        ClearBackground(green);

        DrawText(TextFormat("Score: %i", player1.score), windowWidth / 4, 40, 20, WHITE);
        DrawText(TextFormat("Score: %i", player2.score), windowWidth - (windowWidth / 2), 40, 20, WHITE);

        // detect input
        // WASD - player1
        if (IsKeyDown(KEY_W))
        {
            player1.move(UP, windowHeight);
        }
        else if (IsKeyDown(KEY_S))
        {
            player1.move(DOWN, windowHeight);
        }

        // Input handling
        if (!gameAgainstCPU)
        {
            // detect input
            // up, down - player2
            if (IsKeyDown(KEY_UP))
            {
                player2.move(UP, windowHeight);
            }
            else if (IsKeyDown(KEY_DOWN))
            {
                player2.move(DOWN, windowHeight);
            }
        }

        // update state
        if (gameAgainstCPU)
        {
            int player2EndPos = player2.yPos + player2.rectangle.height;
            int player2MiddlePos = player2.yPos + (player2.rectangle.height / 2);

            if (ball.ballVerticalDirection == BALL_TO_UP)
            {
                if (ball.yPos < player2MiddlePos)
                {
                    player2.move(UP, windowHeight, ball.ySum);
                }
                else if (ball.yPos > player2MiddlePos)
                {
                    player2.move(DOWN, windowHeight, ball.ySum);
                }
                else
                {
                    player2.move(UP, windowHeight, ball.ySum);
                }
            }
            else if (ball.ballVerticalDirection == BALL_TO_DOWN)
            {
                if (player2MiddlePos < ball.yPos)
                {
                    player2.move(DOWN, windowHeight, ball.ySum);
                }
                else if (player2MiddlePos > ball.yPos)
                {
                    player2.move(UP, windowHeight, ball.ySum);
                }
                else
                {
                    player2.move(DOWN, windowHeight, ball.ySum);
                }
            }
        }

        if (ball.ballHorizontalDirection == BALL_TO_LEFT)
        {
            ball.xPos += ball.speed;
        }
        else if (ball.ballHorizontalDirection == BALL_TO_RIGHT)
        {
            ball.xPos -= ball.speed;
        }

        if (ball.ballVerticalDirection == BALL_TO_UP)
        {
            ball.yPos -= ball.ySum;
            if (gameAgainstCPU && ball.yPosEndDestination > 0)
            {
                ball.yPosEndDestination--;
            }
        }
        else if (ball.ballVerticalDirection == BALL_TO_DOWN)
        {
            ball.yPos += ball.ySum;
            if (gameAgainstCPU && ball.yPosEndDestination > 0)
            {
                ball.yPosEndDestination--;
            }
        }

        // Colision vertical
        if ((ball.yPos + ball.radius) > windowHeight)
        {
            ball.changeBallVerticalDirection();
        }
        else if ((ball.yPos - ball.radius) < 0)
        {
            ball.changeBallVerticalDirection();
        }

        // check if ball is colliding to the player platform
        bool isThePlayer1CollidingWithTheBall = CheckCollisionCircleRec(Vector2{float(ball.xPos), float(ball.yPos)}, ball.radius, player1.rectangle);
        bool isThePlayer2CollidingWithTheBall = CheckCollisionCircleRec(Vector2{float(ball.xPos), float(ball.yPos)}, ball.radius, player2.rectangle);

        // Colision horizontal
        if (isThePlayer1CollidingWithTheBall)
        {
            ball.bounce(player1);
            ball.changeBallHorizontalDirection();
        }
        else if (isThePlayer2CollidingWithTheBall)
        {
            ball.bounce(player2);
            ball.changeBallHorizontalDirection();
        }
        else
        {
            // augment score for each player
            if ((ball.xPos - ball.radius) > windowWidth)
            {
                ball.resetPosition();
                player1.increaseScore();
                ball.changeBallHorizontalDirection();
            }
            else if ((ball.xPos + ball.radius) < 0)
            {
                ball.resetPosition();
                player2.increaseScore();
                ball.changeBallHorizontalDirection();
            }
        }
        // drawing

        BeginDrawing();

        DrawCircle(ball.xPos, ball.yPos, ball.radius, ball.color);
        DrawRectangleRounded(player1.rectangle, 0.3f, 20, WHITE);
        DrawRectangleRounded(player2.rectangle, 0.3f, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}