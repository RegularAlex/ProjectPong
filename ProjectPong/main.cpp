//SDL Pong
//Includes:
//SDL
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
//Other 
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
//For Random usage
#include <cstdlib>
#include <ctime>
using namespace std; 

//Variables 
int ScreenHeight = 750; 
int ScreenWidth = 1250; 

int player_score = 0; 
int enemy_score = 0; 

//Function Definitions 
void play_game();
SDL_Rect rectcreation(int x, int y, int width, int height, int r, int g, int b, SDL_Renderer* render);
void text(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text, int fontsize);
bool checkCollision(SDL_Rect a, SDL_Rect b);

int main(int argc, char* argv[])
{
	//Initalisation 
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init;
	//Calling the game
	play_game();
	return 0;
}

void play_game()
{
	//Player Variables
	int playery_velocity = 0; 
	int enemyy_velocity = 0; 
	//bool firsttime = true;
	bool gameover = false; 

	//Enemy Variables
	int choice = 0;
	bool upboundary = false;
	bool downboundary = false;

	//Ball Variables
	//Direction for left and right
	int direction = 0;
	bool left = false; 
	bool right = false; 
	//Direction 2 for up and down
	int direction2 = 0;
	bool up = false; 
	bool down = false; 
	//Ball Velocity
	int ballx_velocity = 0; 
	int bally_velocity = 0;
	bool reset = false; 
	//
	int balllimiterx = 1250; 

	//Create a window 
	SDL_Window* PongWindow;
	PongWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);

	//Create a Renderer
	SDL_Renderer* PongRenderer = NULL; 
	PongRenderer = SDL_CreateRenderer(PongWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(PongRenderer, 0, 0, 0, 255);
	SDL_RenderClear(PongRenderer);

	//Colours 
	SDL_Color White{ 255,255,255 };
	//Drawing Text
	//Player
	stringstream updatedplayerscore; 
	updatedplayerscore << player_score;
	//Colour,text_render, x, y, text, fontsize
	text(White, PongRenderer, 935, 50, updatedplayerscore.str().c_str(), 30);
	SDL_RenderPresent(PongRenderer);
	SDL_RenderClear(PongRenderer);

	//Enemy
	stringstream updatedenemyscore; 
	updatedenemyscore << enemy_score; 
	text(White, PongRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
	SDL_RenderPresent(PongRenderer);
	SDL_RenderClear(PongRenderer);

	//Creation of Line down the centre of the screen
	SDL_Rect CentreLine = rectcreation(ScreenWidth/2 - 5 , 0, 5, 1250, 255, 255, 255, PongRenderer);
	//Drawing of the left and right lines
	SDL_Rect Line1250 = rectcreation(1250, 0, 5, 1250, 255, 255, 255, PongRenderer);
	SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, PongRenderer);
	SDL_RenderPresent(PongRenderer);

	//Creation of the ball 
	SDL_Rect BallRect = rectcreation(610, 350, 25, 25, 255, 255, 255, PongRenderer);
	SDL_RenderPresent(PongRenderer);

	//Creation of Player and Enemy Rects 
	//For the Function (X,Y,Width,Height,r,g,b,renderer)
	SDL_Rect PlayerRect = rectcreation(1210, 350, 25, 100, 255, 255, 255, PongRenderer);
	SDL_Rect EnemyRect = rectcreation(20 ,350 ,25 ,100 ,255 ,255 ,255 ,PongRenderer);
	SDL_RenderPresent(PongRenderer);

	//Calling Random for which way the enemy moves first
	srand(time(0));
	choice = 1 + rand() % 2;
	cout << choice << endl;
	if (choice == 1)
	{
		downboundary = true;
	}
	else if (choice == 2)
	{
		upboundary = true; 
	}

	//Calling Random for which way the ball moves first
	srand(time(0));
	direction = 1 + rand() % 2;
	if (direction == 1)
	{
		left = true;
	}
	else if (direction == 2)
	{
		right = true; 
	}

	SDL_Event PongEvent; 
	bool active = true; 
	while (active)
	{
		Uint64 start = SDL_GetPerformanceCounter();

		//Limiting Player Movement to the screen
		if (PlayerRect.y >= 750)
		{
			PlayerRect.y = 550;
			//cout << "below";
		}
		if (PlayerRect.y <= 0)
		{
			PlayerRect.y = 50; 
			//cout << "above";
		}
		//Enemy Movement
		if ((choice == 1) || (downboundary == true)) //Move up 
		{
			enemyy_velocity -= 100;
			//cout << "Being run" << endl; 
		}
		if ((choice == 2) || (upboundary == true)) //Move down
		{
			enemyy_velocity += 100;
			//cout << "Being ran" << endl;
		}

		EnemyRect.y += enemyy_velocity;

		//Limiting Enemy Movement to the screen 
		if (EnemyRect.y >= 750)
		{
			EnemyRect.y = 650;
			downboundary = true;
			upboundary = false;
			choice = 0;
			//cout << "Enemy Below";
		}
		if (EnemyRect.y <= 0)
		{
			EnemyRect.y = 0;
			downboundary = false;
			upboundary = true;
			choice = 0; 
			//cout << "Enemy Above";
		}

		//Ball Movement (Left and Right)
		if (left == true) //Move the ball to the left
		{
			ballx_velocity -= 50;
		}
		if (right == true) //Move the ball to the right
		{
			ballx_velocity += 50;
		}

		//Ball Movement (Up and Down)
		if (up == true)
		{
			bally_velocity -= 50; 
		}
		if (down == true)
		{
			bally_velocity += 50;
		}


		//Collisions with boundaries
		if (BallRect.y >= 750) //Should bounce upwards
		{
			//Game Recognises when the bottom of the screen is passed
			cout << "Bottom of the screen" << endl;
			//But non of the below is being ran? 
			if (left = true)
			{
				cout << "Bottom of Screen Left" << endl; //This is being ran
				bally_velocity -= 50; 
				up = true; 
				down = false; 
			}
			if (right = true)
			{
				cout << "Bottom of Screen Right" << endl; //This is being ran
				bally_velocity -= 50;
				up = true; 
				down = false; 
			}
		}

		if (BallRect.y <= 0) //Should bounce downwards
		{
			//Game recognises when the top of the screen is passed
			cout << "Top of the screen" << endl;
			//But non of the below is being ran?
			if (left = true)
			{
				cout << "Top of Screen Left" << endl; //This is being ran
				bally_velocity += 50;
				up = false; 
				down = true;
			}
			if (right = true)
			{
				cout << "Top of Screen Right" << endl; //This is being ran
				bally_velocity += 50;
				up = false; 
				down = true; 
			}
		}

		//if (BallRect.x >= 1250) //Enemy Scores 
		if(BallRect.x >= (PlayerRect.x + PlayerRect.w))
		{
			if (gameover == false)
			{
				enemy_score += 1;
				reset = true; 
				cout << BallRect.x << "Enemy Scores" << endl;
				//BallRect.x == 1250; 
				up = false;
				down = false;
			}
		}
		//if (BallRect.x <= 0 ) //Player Scores
		if(BallRect.x <= (EnemyRect.x + EnemyRect.w))
		{
			if (gameover == false)
			{
				player_score += 1;
				//Resetting the ball position
				reset = true;
				cout << BallRect.x << "Player Scores" << endl;
				//BallRect.x == 0; 
				up = false;
				down = false;
			}
		}

		//Checking for collision between the ball and the player/enemy paddle
		bool playerballcollision = checkCollision(PlayerRect,BallRect); 
		bool enemyballcollision = checkCollision(EnemyRect,BallRect);
		//cout << playerballcollision << "Player Collision Check" << endl;
		//cout << enemyballcollision << "Enemy Collision Check" << endl;
		if (playerballcollision == true)
		{
			cout << "Player Collision" << endl;
			//Set the NEW direction of the ball. 
			left = true; 
			right = false; 
			//Sorting out the up and down movement of the ball
			if ((up == false) && (down == true)) //Bouncing off the ball moves upwards from going downwards
			{
				up = true; 
				down = false; 
			}
			else if ((up == true) && (down == false))//Bouncing off the ball moves downwards from going upwards
			{
				up = false; 
				down = true; 
			}
			else if ((up == false) && (down == false))//If the ball makes contact straight on, then it can bounce off either way
			{

				srand(time(0));
				direction2 = 1 + rand() % 2;
				if (direction2 == 1)
				{
					up = true;
					down = false;
				}
				else if (direction2 == 2)
				{
					down = true;
					up = false;
				}
			}
			playerballcollision = false; 
		}
		if (enemyballcollision == true)
		{
			left = false; 
			right = true; 
			cout << "Enemy Collision" << endl;
			//Sorting out the up and down movement of the ball
			if ((up == false) && (down == true)) //Bouncing off the ball moves upwards
			{
				up = true;
				down = false;
			}
			else if ((up == true) && (down == false))//Bouncing off the ball moves downwards
			{
				up = false;
				down = true;
			}
			else if ((up == false) && (down == false))//If the ball makes contact straight on, then it can bounce off either way
			{

				srand(time(0));
				direction2 = 1 + rand() % 2;
				if (direction2 == 1)
				{
					up = true;
					down = false;
				}
				else if (direction2 == 2)
				{
					down = true;
					up = false;
				}
			}
			enemyballcollision = false; 
		}

		//Updates the ball position
		BallRect.x += ballx_velocity;
		BallRect.y += bally_velocity;

		//Code below allows for the ball to reach the edges of the screen. 
		ballx_velocity = 0 - 20;
		bally_velocity = 0; 

		//Redrawing Everything onto the screen 
		SDL_SetRenderDrawColor(PongRenderer, 0, 0, 0, 255); //Setting colour to black
		SDL_RenderClear(PongRenderer);
		SDL_RenderPresent(PongRenderer);
		SDL_SetRenderDrawColor(PongRenderer, 255, 255, 255, 255); //Setting colour to white
		//Text
		//Player Score
		stringstream updatedplayerscore;
		updatedplayerscore << player_score;
		text(White, PongRenderer, 935, 50, updatedplayerscore.str().c_str(), 30);
		//Enemy Score
		stringstream updatedenemyscore;
		updatedenemyscore << enemy_score;
		text(White, PongRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
		SDL_RenderFillRect(PongRenderer, &CentreLine); //(Re-)Drawing the Centre Line
		SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, PongRenderer);
		SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, PongRenderer);
		if (reset == true) //Resetting the Ball
		{
			//Without code below the ball just goes in one direction and continously scores
			BallRect.x = 610;
			BallRect.y = 350;
			cout << BallRect.x << " .x Reset 1" << endl; 
			cout << BallRect.y << " .y Reset 1" << endl;
			//Without the code below the ball spawns but moves crazily around
			ballx_velocity = 0;
			bally_velocity = 0;
			reset = false; //Without this the code doesnt reset
			//Without code below, the ball will only move one direction
			left = false;
			right = false;
			//Without code below, the ball doesnt move
			srand(time(0));
			direction = 1 + rand() % 2;
			if (direction == 1)
			{
				left = true;
			}
			else if (direction == 2)
			{
				right = true;
			}
			SDL_RenderFillRect(PongRenderer, &BallRect); //(Re-)Drawing the ball
			//playerballcollision = checkCollision(PlayerRect, BallRect);
			//enemyballcollision = checkCollision(EnemyRect, BallRect);
		}
		else
		{
			SDL_RenderFillRect(PongRenderer, &BallRect); //(Re-)Drawing the Ball
		}
		SDL_RenderFillRect(PongRenderer, &PlayerRect); //(Re-)Drawing the Player
		SDL_RenderFillRect(PongRenderer, &EnemyRect);  //(Re-)Drawing the enemy
		SDL_RenderPresent(PongRenderer);

		//Check for score (Exiting Condition)
		if (player_score == 10)
		{
			//Player Wins
			text(White, PongRenderer, 510 , 350, "YOU WIN!", 50);
			SDL_RenderPresent(PongRenderer);
			gameover = true; 
		}
		if (enemy_score == 10)
		{
			//Enemy Wins 
			text(White, PongRenderer, 510, 350, "YOU LOSE!", 50);
			SDL_RenderPresent(PongRenderer);
			gameover = true; 

		}

		while (SDL_PollEvent(&PongEvent))
		{
			if (PongEvent.type == SDL_QUIT)
			{
				active = false; 
				break; 
			}
			else if (PongEvent.type == SDL_KEYDOWN)
			{
				switch (PongEvent.key.keysym.sym)
				{
				case SDLK_UP:
					playery_velocity -= 100; 
					break; 
				case SDLK_DOWN:
					playery_velocity += 100;
					break;
				}
				PlayerRect.y += playery_velocity; //Updating the player position

				//Redrawing Everything onto the screen (After Player Action)
				SDL_SetRenderDrawColor(PongRenderer,0, 0, 0, 255); //Setting colour to black
				SDL_RenderClear(PongRenderer);
				SDL_RenderPresent(PongRenderer);
				SDL_SetRenderDrawColor(PongRenderer, 255, 255, 255, 255); //Setting colour to white
				//Text
                //Player Score
				stringstream updatedplayerscore;
				updatedplayerscore << player_score;
				text(White, PongRenderer, 935, 50, updatedplayerscore.str().c_str(), 30);
				//Enemy Score
				stringstream updatedenemyscore;
				updatedenemyscore << enemy_score;
				text(White, PongRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
				SDL_RenderFillRect(PongRenderer, &CentreLine); //(Re-)Drawing the Centre Line
				SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, PongRenderer);
				SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, PongRenderer);
				if (reset == true) //Resetting the ball 
				{
					//Reseting X and Y positions
					BallRect.x = 610;
					BallRect.y = 350;
					cout << BallRect.x << " .x Reset 2" << endl;
					cout << BallRect.y << " .y Reset 2" << endl;
					//Reseting the balls x and y velocity
					ballx_velocity = 0; 
					bally_velocity = 0;

					reset = false;

					//Reseting the balls direction
					left = false; 
					right = false; 
					srand(time(0));
					direction = 1 + rand() % 2;
					if (direction == 1)
					{
						left = true;
					}
					else if (direction == 2)
					{
						right = true;
					}
					SDL_RenderFillRect(PongRenderer, &BallRect); //(Re-)Drawing the ball
				}
				else
				{
					SDL_RenderFillRect(PongRenderer, &BallRect); //(Re-)Drawing the Ball
				}
				SDL_RenderFillRect(PongRenderer, &PlayerRect); //(Re-)Drawing the Player
				SDL_RenderFillRect(PongRenderer, &EnemyRect);  //(Re-)Drawing the enemy
				SDL_RenderPresent(PongRenderer);

			}
			else if (PongEvent.type == SDL_KEYUP)
			{
				switch (PongEvent.key.keysym.sym)
				{
				case SDLK_UP:
					playery_velocity += 100;
					break;
				case SDLK_DOWN:
					playery_velocity -= 100;
					break;
				}
			}
		}

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		//Cap to 60 FPS (16.666f)
		//Higher the number the slower the game
		SDL_Delay(floor(150.0f - elapsedMS));
	}

	SDL_DestroyWindow(PongWindow);
	SDL_Quit; 
	TTF_Quit; 
	IMG_Quit; 
}

SDL_Rect rectcreation(int x, int y, int width, int height, int r, int g, int b, SDL_Renderer* render)
{
	SDL_Rect button;
	button.x = x;
	button.y = y;
	button.w = width;
	button.h = height;

	SDL_SetRenderDrawColor(render, r, g, b, 255);
	SDL_RenderFillRect(render, &button);
	return button;
}

void text(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text, int fontsize)
{
	//Creation of Font
	TTF_Font* font = TTF_OpenFont("../ProjectPong/Resources/arial-font/arial.ttf", fontsize);
	//Location of Font
	SDL_Rect fontrect = { x,y };
	stringstream text_creation;
	text_creation << text;
	SDL_Surface* surface = TTF_RenderText_Blended(font, text_creation.str().c_str(), Colour);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(text_render, surface);
	SDL_QueryTexture(texture, NULL, NULL, &fontrect.w, &fontrect.h);
	SDL_RenderCopy(text_render, texture, NULL, &fontrect);
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//Sides of the rectangles 
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculating the sides of Rectangle A (Which will most likely be the player)
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculating the sides of Rectangle B (Which will most likely be the obstacle)
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//Checking to see if there has been a collision. 
	//If the two rects arent inside of each other, so no collision has occured
	if (bottomA <= topB)
	{
		return false;
	}
	if (topA > +bottomB)
	{
		return false;
	}
	if (rightA <= leftB)
	{
		return false;
	}
	if (leftA >= rightB)
	{
		return false;
	}

	//If there has been a collision 
	return true;


}
