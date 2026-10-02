#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	bricks.clear();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	for (int i = 0; i < 5; i++) 
	{
		Box tempbrick;
		tempbrick.width = 10;
		tempbrick.height = 2;
		tempbrick.x_position = i * 12+2;
		tempbrick.y_position = 5;
		tempbrick.doubleThick = true;
		tempbrick.color = ConsoleColor::DarkGreen;

		bricks.push_back(tempbrick);
	}
}


void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	for (int i = 0; i < bricks.size(); i++)
	{
		bricks[i].Draw();
		
	}

	if (bricks.empty())
	{
		int textX = WINDOW_WIDTH / 2 - 13;
		int textY = WINDOW_HEIGHT / 2;

		Console::WordWrap(textX, textY, 30, "VICTORY!\n" "\t\t\tPress 'R' to play again");
	}
	else if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		int textX = WINDOW_WIDTH / 2 - 13;
		int textY = WINDOW_HEIGHT / 2;
		
		Console::ForegroundColor(ConsoleColor::Red);
		Console::WordWrap(textX, textY, 30, "YOU LOSE!\n" "\t\t\tPress 'R' to play again");
		Console::ResetColor();

	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = (int)bricks.size() - 1; i>=0 ; i--) {
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			ball.y_velocity *= -1;
			if (bricks[i].color == ConsoleColor::DarkGreen)
			{
				bricks[i].color = ConsoleColor::DarkYellow;
			}
			else if (bricks[i].color == ConsoleColor::DarkYellow)
			{
				bricks[i].color = ConsoleColor::DarkRed;
			}
			else if (bricks[i].color == ConsoleColor::DarkRed)
			{
				bricks[i].color = ConsoleColor::Black;
			}

			if (bricks[i].color == ConsoleColor::Black)
			{
				bricks.erase(bricks.begin() + i);
			}
			break;
			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector

		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	if (bricks.empty())
	{
		ball.moving = false;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
	}
}
