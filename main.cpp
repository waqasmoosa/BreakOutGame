#include <iostream>
#include "SDL.h"

using namespace std;

const int screen_width = 600;
const int screen_height = 800;
const int no_of_columns = 5;
const int no_of_rows = 12;

struct PaddleData
{
	double x;
	double y;
	double width;
	double height;
	double speed_x;
};

void init_playerpaddle(PaddleData *leftpaddle)
{
	leftpaddle->x = screen_width / 2 - 30;
	leftpaddle->y = screen_height - 20;
	leftpaddle->width = 60;
	leftpaddle->height = 10;
	leftpaddle->speed_x = 15;
}

struct BallData
{
	double x;
	double y;
	double width;
	double height;
	double speed_y;
	double speed_x;
};

void init_ball(BallData *ball)
{
	ball->x = screen_width / 2;
	ball->y = screen_height / 2;
	ball->width = 10;
	ball->height = 10;
	ball->speed_y = 0.01;
	ball->speed_x = 0.01;
}

void Ballupdate(BallData *balldata)
{
	balldata->x += balldata->speed_x;
	balldata->y += balldata->speed_y;

	if (balldata->y + balldata->height >= screen_height)
	{
		balldata->x = screen_width / 2;
		balldata->y = screen_height / 2;

	}

	if (balldata->y < 0)
	{
		balldata->speed_y *= -1;
	}

	if (balldata->x + balldata->width >= screen_width)
	{
		balldata->speed_x *= -1;
		
	}
	if (balldata->x < 0)
	{
		balldata->speed_x *= -1;
	}

}

struct BrickData
{
	double x;
	double y;
	double height;
	double width;
};

void init_bricks(int no_of_columns, int no_of_rows)
{
	int gap_x = 45;
	BrickData **bricks;
	bricks = new BrickData *[no_of_columns];
	for (int i = 0; i < no_of_columns; i++)
	{
		cout << "ran1\n";
		bricks[i] = new BrickData[no_of_rows];

		for (int j = 0; j < no_of_rows; j++)
		{
			bricks[i][j].x = 10;
			bricks[i][j].y = 10;
			bricks[i][j].width = 40 ;
			bricks[i][j].height = 10;
			cout << "ran2\n";
			if (bricks[i][j].x < 0)
			{
				bricks[i][j].x += gap_x;
				gap_x += 45;
				cout << "ran3\n";
			}
		}
	}

}



//MAIN PROGRAM

int main(int argv, char **argc)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("BreakOut Game", 450, 150, screen_width, screen_height, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

	int score = 0;
	//Initializing Bricks Rects
	//const int number_of_columns = 5;
	//const int number_of_rows = 13;
	//BrickData brick[number_of_columns][number_of_rows];


	//Initializing Paddles

	PaddleData *playerpaddle = new PaddleData;
	init_playerpaddle(playerpaddle);
	SDL_Rect paddle_rectangle;



	//Initializing Ball
	BallData *ball = new BallData;
	init_ball(ball);
	SDL_Rect ball_rectangle;


	//Initializing Bricks
	
	BrickData brick[no_of_columns][no_of_rows];
	
	for (int i = 0; i < no_of_columns; i++)
	{
		int gap_y = 20;
		int gap_x = 40;
		for (int j = 0; j < no_of_rows; j++)
		{
			brick[i][j].x = 10;
			brick[i][j].y = 10;
			brick[i][j].width = 40;
			brick[i][j].height = 10;
			if (brick[i][j].x > 0)
			{
				brick[i][j].x += gap_x;
				gap_x += 40;
			}
			if (brick[i][j].x + brick[i][j].width < 600)
			{
				brick[i][j].y += gap_y;
				//gap_y += 20;

			}
		}
	}
	SDL_Rect brick_rect;
	

	


	bool quit = false;

	//Game Loop

	while (quit == false)
	{

		//Event Loop
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				cout << "SomeOne Clicked the X button" << "\n";
				quit = true;
			}

			else if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					playerpaddle->x -= playerpaddle->speed_x;
					if (playerpaddle->x < 0)
					{
						playerpaddle->x = 0;
					}
					break;
				case SDLK_RIGHT:
					playerpaddle->x += playerpaddle->speed_x;
					if (playerpaddle->x + playerpaddle->width >= 600)
					{
						playerpaddle->x = 540;
					}
					break;
				}
			}

		}


		//Game Stuff
		Ballupdate(ball);

		//paddle and ball collision
		if (ball->x < playerpaddle->x + playerpaddle->width && ball->x > playerpaddle->x && ball->y + ball->height > playerpaddle->y)
		{
			//ball->speed_x *= -1;
			ball->speed_y *= -1;
		}

		for (int i = 0; i < no_of_columns; i++)
		{
			for (int j = 0; j < no_of_rows; j++)
			{
				if (ball->x < brick[i][j].x + brick[i][j].width && ball->x > brick[i][j].x && ball->y < brick[i][j].y + brick[i][j].height && ball->y > brick[i][j].y)
				{
					ball->speed_x *= 1;
					ball->speed_y *=-1;
					//score++;
					//cout << "Your Score is : " << score++ <<"\n";
					brick[i][j].x = 0;
					brick[i][j].y = 0;
					brick[i][j].width = 0;
					brick[i][j].height = 0;
				}
				
			}
		}

		//Draw

		//Draw Game Window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);


		//Drawing Bricks On The Screen
		for (int i = 0; i < no_of_columns; i++)
		{
			for (int j = 0; j < no_of_rows; j++)
			{
				brick_rect.x = brick[i][j].x;
				brick_rect.y = brick[i][j].y;
				brick_rect.w = brick[i][j].width;
				brick_rect.h = brick[i][j].height;
				SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
				SDL_RenderFillRect(renderer, &brick_rect);
			}
		}

		//Drawing Player Paddle on Screen
		paddle_rectangle.x = playerpaddle->x;
		paddle_rectangle.y = playerpaddle->y;
		paddle_rectangle.w = playerpaddle->width;
		paddle_rectangle.h = playerpaddle->height;

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(renderer, &paddle_rectangle);


		//Drawing Ball on Screen
		ball_rectangle.x = ball->x;
		ball_rectangle.y = ball->y;
		ball_rectangle.w = ball->width;
		ball_rectangle.h = ball->height;

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(renderer, &ball_rectangle);
		


		SDL_RenderPresent(renderer);


	}



	//system("Pause");
	return 0;
}
