#include "PlayGame.h"

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

void PlayGame::SinglePlayer(SDL_Renderer* SingleRenderer, SDL_Window * SingleWindow)
{
	//Variables 
	//Screen
	int ScreenHeight = 750;
	int ScreenWidth = 1250;

	//Score
	int player_score = 0;
	int enemy_score = 0;

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

	//Contact Variables
	bool playercontact = false; 
	bool enemycontact = false; 

	//Colours 
	SDL_Color White{ 255,255,255 };
	//Drawing Text
	//Player
	stringstream updatedplayerscore;
	updatedplayerscore << player_score;
	//Colour,text_render, x, y, text, fontsize
	textcreate(White, SingleRenderer, 935, 50, updatedplayerscore.str().c_str(),30);
	SDL_RenderPresent(SingleRenderer);
	SDL_RenderClear(SingleRenderer);

	//Enemy
	stringstream updatedenemyscore;
	updatedenemyscore << enemy_score;
	textcreate(White, SingleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
	SDL_RenderPresent(SingleRenderer);
	SDL_RenderClear(SingleRenderer);

	//Creation of Line down the centre of the screen
	SDL_Rect CentreLine = rectcreation(ScreenWidth / 2 - 5, 0, 5, 1250, 255, 255, 255, SingleRenderer);
	//Drawing of the left and right lines
	SDL_Rect Line1250 = rectcreation(1250, 0, 5, 1250, 255, 255, 255, SingleRenderer);
	SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, SingleRenderer);
	SDL_RenderPresent(SingleRenderer);

	//Creation of the ball 
	SDL_Rect BallRect = rectcreation(610, 350, 25, 25, 255, 255, 255, SingleRenderer);
	SDL_RenderPresent(SingleRenderer);

	//Creation of Player and Enemy Rects 
	//For the Function (X,Y,Width,Height,r,g,b,renderer)
	SDL_Rect PlayerRect = rectcreation(1190, 350, 25, 100, 255, 255, 255, SingleRenderer); //1210
	SDL_Rect EnemyRect = rectcreation(40, 350, 25, 100, 255, 255, 255, SingleRenderer); //20 
	SDL_RenderPresent(SingleRenderer);

	//Calling Random for which way the enemy moves first
	srand(time(0));
	choice = 1 + rand() % 2;
	//cout << choice << endl;
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
		}
		if (PlayerRect.y <= 0)
		{
			PlayerRect.y = 50;
		}
		//Enemy Movement
		if ((choice == 1) || (downboundary == true)) //Move up 
		{
			enemyy_velocity -= 100;
		}
		if ((choice == 2) || (upboundary == true)) //Move down
		{
			enemyy_velocity += 100;
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
			if (left = true)
			{
				bally_velocity -= 50;
				up = true;
				down = false;
			}
			if (right = true)
			{
				bally_velocity -= 50;
				up = true;
				down = false;
			}
		}

		if (BallRect.y <= 0) //Should bounce downwards
		{
			if (left = true)
			{
				bally_velocity += 50;
				up = false;
				down = true;
			}
			if (right = true)
			{
				bally_velocity += 50;
				up = false;
				down = true;
			}
		}

		//if (BallRect.x >= 1250) //Enemy Scores 
		if (BallRect.x >= (PlayerRect.x + (PlayerRect.w * 2)))
		{
			if (gameover == false)
			{
				enemy_score += 1;
				reset = true;
				//cout << BallRect.x << "Enemy Scores" << endl;
				up = false;
				down = false;
			}
		}
		//if (BallRect.x <= 0 ) //Player Scores
		if (BallRect.x <= (EnemyRect.x - EnemyRect.w)) 
		{
			if (gameover == false)
			{
				player_score += 1;
				//Resetting the ball position
				reset = true;
				//cout << BallRect.x << "Player Scores" << endl;
				up = false;
				down = false;
			}
		}

		//Checking for collision between the ball and the player/enemy paddle
		bool playerballcollision = Collision(PlayerRect, BallRect);
		bool enemyballcollision = Collision(EnemyRect, BallRect);
		if (playerballcollision == true)
		{
			//cout << "Player Collision" << endl;
			playercontact = true; 
			enemycontact = false;
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
			//cout << "Enemy Collision" << endl;
			playercontact = false;
			enemycontact = true;
			left = false;
			right = true;
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
		ballx_velocity = 0 - 20; // 0 - 10 works for player collision upto 0 - 40 (0 - 10 works the best) 
		bally_velocity = 0;

		if (enemycontact == true)
		{
			ballx_velocity = 0 + 20;
			bally_velocity = 0;
		}
		if (playercontact == true)
		{
			ballx_velocity = 0 - 20;
			bally_velocity = 0;
		}

		//Redrawing Everything onto the screen 
		SDL_SetRenderDrawColor(SingleRenderer, 0, 0, 0, 255); //Setting colour to black
		SDL_RenderClear(SingleRenderer);
		SDL_RenderPresent(SingleRenderer);
		SDL_SetRenderDrawColor(SingleRenderer, 255, 255, 255, 255); //Setting colour to white
		//Text
		//Player Score
		stringstream updatedplayerscore;
		updatedplayerscore << player_score;
		textcreate(White, SingleRenderer, 935, 50, updatedplayerscore.str().c_str(), 30);
		//Enemy Score
		stringstream updatedenemyscore;
		updatedenemyscore << enemy_score;
		textcreate(White, SingleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
		SDL_RenderFillRect(SingleRenderer, &CentreLine); //(Re-)Drawing the Centre Line
		SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, SingleRenderer);
		SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, SingleRenderer);
		if (reset == true) //Resetting the Ball
		{
			//Without code below the ball just goes in one direction and continously scores
			BallRect.x = 610;
			BallRect.y = 350;
			//cout << BallRect.x << " .x Reset 1" << endl;
			//cout << BallRect.y << " .y Reset 1" << endl;
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
			SDL_RenderFillRect(SingleRenderer, &BallRect); //(Re-)Drawing the ball
		}

		else
		{
			SDL_RenderFillRect(SingleRenderer, &BallRect); //(Re-)Drawing the Ball
		}
		SDL_RenderFillRect(SingleRenderer, &PlayerRect); //(Re-)Drawing the Player
		SDL_RenderFillRect(SingleRenderer, &EnemyRect);  //(Re-)Drawing the enemy
		SDL_RenderPresent(SingleRenderer);

		//Check for score (Exiting Condition)
		if (player_score == 10)
		{
			//Player Wins
			textcreate(White, SingleRenderer, 510, 350, "YOU WIN!", 50);
			SDL_RenderPresent(SingleRenderer);
			gameover = true;
		}
		if (enemy_score == 10)
		{
			//Enemy Wins 
			textcreate(White, SingleRenderer, 510, 350, "YOU LOSE!", 50);
			SDL_RenderPresent(SingleRenderer);
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
				SDL_SetRenderDrawColor(SingleRenderer, 0, 0, 0, 255); //Setting colour to black
				SDL_RenderClear(SingleRenderer);
				SDL_RenderPresent(SingleRenderer);
				SDL_SetRenderDrawColor(SingleRenderer, 255, 255, 255, 255); //Setting colour to white
				//Text
				//Player Score
				stringstream updatedplayerscore;
				updatedplayerscore << player_score;
				textcreate(White, SingleRenderer, 935, 50, updatedplayerscore.str().c_str(), 30);
				//Enemy Score
				stringstream updatedenemyscore;
				updatedenemyscore << enemy_score;
				textcreate(White, SingleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedenemyscore.str().c_str(), 30);
				SDL_RenderFillRect(SingleRenderer, &CentreLine); //(Re-)Drawing the Centre Line
				SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, SingleRenderer);
				SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, SingleRenderer);
				if (reset == true) //Resetting the ball 
				{
					//Reseting X and Y positions
					BallRect.x = 610;
					BallRect.y = 350;
					//cout << BallRect.x << " .x Reset 2" << endl;
					//cout << BallRect.y << " .y Reset 2" << endl;
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
					SDL_RenderFillRect(SingleRenderer, &BallRect); //(Re-)Drawing the ball
				}
				else
				{
					SDL_RenderFillRect(SingleRenderer, &BallRect); //(Re-)Drawing the Ball
				}
				SDL_RenderFillRect(SingleRenderer, &PlayerRect); //(Re-)Drawing the Player
				SDL_RenderFillRect(SingleRenderer, &EnemyRect);  //(Re-)Drawing the enemy
				SDL_RenderPresent(SingleRenderer);

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
			PlayerRect.y += playery_velocity; //Updating the player position
			}
		}

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		//Cap to 60 FPS (16.666f)
		//Higher the number the slower the game
		SDL_Delay(floor(150.0f - elapsedMS));
	}

	SDL_DestroyWindow(SingleWindow);
	SDL_Quit;
	TTF_Quit;
	IMG_Quit;
}

void PlayGame::TwoPlayer(SDL_Renderer* DoubleRenderer, SDL_Window * DoubleWindow)
{
	//Variables 
	//Screen
	int ScreenHeight = 750;
	int ScreenWidth = 1250;

	//Score
	int player1_score = 0;
	int player2_score = 0;

	//Player Variables
	int player1y_velocity = 0;
	int player2y_velocity = 0;
	//bool firsttime = true;
	bool gameover = false;

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

	//Collision Variables
	bool player1contact = false; 
	bool player2contact = false;

	//Colours 
	SDL_Color White{ 255,255,255 };
	//Text Creation
	//Player 1 Text 
	stringstream updatedplayer1score;
	updatedplayer1score << player1_score;
	//Colour,text_render, x, y, text, fontsize
	textcreate(White, DoubleRenderer, 935, 50, updatedplayer1score.str().c_str(), 30);
	SDL_RenderPresent(DoubleRenderer);
	SDL_RenderClear(DoubleRenderer);

	//Player 2 Text
	stringstream updatedplayer2score;
	updatedplayer2score << player2_score;
	textcreate(White, DoubleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedplayer2score.str().c_str(), 30);
	SDL_RenderPresent(DoubleRenderer);
	SDL_RenderClear(DoubleRenderer);

	//Creation of Line down the centre of the screen
	SDL_Rect CentreLine = rectcreation(ScreenWidth / 2 - 5, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
	//Drawing of the left and right lines
	SDL_Rect Line1250 = rectcreation(1250, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
	SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
	SDL_RenderPresent(DoubleRenderer);

	//Creation of the ball 
	SDL_Rect BallRect = rectcreation(610, 350, 25, 25, 255, 255, 255, DoubleRenderer);
	SDL_RenderPresent(DoubleRenderer);

	//Creation of Player 1 and Player 2 Rects 
	//For the Function (X,Y,Width,Height,r,g,b,renderer)
	SDL_Rect Player1Rect = rectcreation(1190, 350, 25, 100, 255, 255, 255, DoubleRenderer);
	SDL_Rect Player2Rect = rectcreation(40, 350, 25, 100, 255, 255, 255, DoubleRenderer);
	SDL_RenderPresent(DoubleRenderer);

	//Calling Random for which way the ball moves first
	srand(time(0));
	//direction = 1 + rand() % 2;
	if (direction == 1)
	{
		left = true;
	}
	else if (direction == 2)
	{
		right = true;
	}
	//
	SDL_Event PongEvent;
	bool active = true;
	while (active)
	{
		Uint64 start = SDL_GetPerformanceCounter();

		//Limiting Player Movement to the screen
		if (Player1Rect.y >= 750)
		{
			Player1Rect.y = 550;
		}
		if (Player1Rect.y <= 0)
		{
			Player1Rect.y = 50;
		}
		//Player 2 being limited to the screen
		if (Player2Rect.y >= 750)
		{
			Player2Rect.y = 550;
		}
		if (Player2Rect.y <= 0)
		{
			Player2Rect.y = 50;
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
			if (left = true)
			{
				bally_velocity -= 50;
				up = true;
				down = false;
			}
			if (right = true)
			{
				bally_velocity -= 50;
				up = true;
				down = false;
			}
		}

		if (BallRect.y <= 0) //Should bounce downwards
		{
			if (left = true)
			{
				bally_velocity += 50;
				up = false;
				down = true;
			}
			if (right = true)
			{
				bally_velocity += 50;
				up = false;
				down = true;
			}
		}

		//if (BallRect.x >= 1250) //Player 2 Scores 
		if (BallRect.x >= (Player1Rect.x + Player1Rect.w))
		{
			if (gameover == false)
			{
				player2_score += 1;
				reset = true;
				//cout << BallRect.x << "Player 2 Scores" << endl;
				up = false;
				down = false;
			}
		}
		//if (BallRect.x <= 0 ) //Player 1 Scores
		if (BallRect.x <= (Player2Rect.x - Player2Rect.w))
		{
			if (gameover == false)
			{
				player1_score += 1;
				//Resetting the ball position
				reset = true;
				//cout << BallRect.x << "Player 1 Scores" << endl;
				up = false;
				down = false;
			}
		}

		//Checking for collision between the ball and the player(s) paddle
		bool player1ballcollision = Collision(Player1Rect, BallRect);
		bool player2ballcollision = Collision(Player2Rect, BallRect);
		if (player1ballcollision == true)
		{
			//cout << "Player 1 Collision" << endl;
			//Set the NEW direction of the ball. 
			player1contact = true;
			player2contact = false;
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
			player1ballcollision = false;
		}

		if (player2ballcollision == true)
		{
			//cout << "Player 2 Collision" << endl;
			player1contact = false; 
			player2contact = true; 
			left = false;
			right = true;
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
			player2ballcollision = false;
		}

		//Updates the ball position
		BallRect.x += ballx_velocity;
		BallRect.y += bally_velocity;

		//Code below allows for the ball to reach the edges of the screen. 
		ballx_velocity = 0 - 20;
		bally_velocity = 0;

		if (player2contact == true)
		{
			ballx_velocity = 0 + 20;
			bally_velocity = 0;
		}
		if (player1contact == true)
		{
			ballx_velocity = 0 - 20;
			bally_velocity = 0;
		}

		//Redrawing Everything onto the screen 
		SDL_SetRenderDrawColor(DoubleRenderer, 0, 0, 0, 255); //Setting colour to black
		SDL_RenderClear(DoubleRenderer);
		SDL_RenderPresent(DoubleRenderer);
		SDL_SetRenderDrawColor(DoubleRenderer, 255, 255, 255, 255); //Setting colour to white
		//Text
		//Player Score
		stringstream updatedplayer1score;
		updatedplayer1score << player1_score;
		textcreate(White, DoubleRenderer, 935, 50, updatedplayer1score.str().c_str(), 30);
		//Enemy Score
		stringstream updatedplayer2score;
		updatedplayer2score << player2_score;
		textcreate(White, DoubleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedplayer2score.str().c_str(), 30);
		SDL_RenderFillRect(DoubleRenderer, &CentreLine); //(Re-)Drawing the Centre Line
		SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
		SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
		if (reset == true) //Resetting the Ball
		{
			//Without code below the ball just goes in one direction and continously scores
			BallRect.x = 610;
			BallRect.y = 350;
			//cout << BallRect.x << " .x Reset 1" << endl;
			//cout << BallRect.y << " .y Reset 1" << endl;
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
			SDL_RenderFillRect(DoubleRenderer, &BallRect); //(Re-)Drawing the ball
		}
		else
		{
			SDL_RenderFillRect(DoubleRenderer, &BallRect); //(Re-)Drawing the Ball
		}
		SDL_RenderFillRect(DoubleRenderer, &Player1Rect); //(Re-)Drawing Player 1
		SDL_RenderFillRect(DoubleRenderer, &Player2Rect);  //(Re-)Drawing Player 2
		SDL_RenderPresent(DoubleRenderer);

		//Check for score (Exiting Condition)
		if (player1_score == 10)
		{
			//Player 1 Wins
			textcreate(White, DoubleRenderer, 450, 350, "PLAYER ONE WINS!", 50);
			SDL_RenderPresent(DoubleRenderer);
			gameover = true;
		}
		if (player2_score == 10)
		{
			//Player 2 Wins 
			textcreate(White, DoubleRenderer, 425, 350, "PLAYER TWO WINS!", 50);
			SDL_RenderPresent(DoubleRenderer);
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
					player1y_velocity -= 100;
					break;
				case SDLK_DOWN:
					player1y_velocity += 100;
					break;
				case SDLK_w:
					player2y_velocity -= 100; 
					break; 
				case SDLK_s:
					player2y_velocity += 100; 
					break; 
				}
				//Updating the player position
				Player1Rect.y += player1y_velocity; 
				Player2Rect.y += player2y_velocity;

				//Redrawing Everything onto the screen (After Player Action)
				SDL_SetRenderDrawColor(DoubleRenderer, 0, 0, 0, 255); //Setting colour to black
				SDL_RenderClear(DoubleRenderer);
				SDL_RenderPresent(DoubleRenderer);
				SDL_SetRenderDrawColor(DoubleRenderer, 255, 255, 255, 255); //Setting colour to white
				//Text
				//Player Score
				stringstream updatedplayer1score;
				updatedplayer1score << player1_score;
				textcreate(White, DoubleRenderer, 935, 50, updatedplayer1score.str().c_str(), 30);
				//Enemy Score
				stringstream updatedplayer2score;
				updatedplayer2score << player2_score;
				textcreate(White, DoubleRenderer, ((ScreenWidth / 2) - (ScreenWidth / 4)), 50, updatedplayer2score.str().c_str(), 30);
				SDL_RenderFillRect(DoubleRenderer, &CentreLine); //(Re-)Drawing the Centre Line
				SDL_Rect Line1250 = rectcreation(1245, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
				SDL_Rect Line0 = rectcreation(0, 0, 5, 1250, 255, 255, 255, DoubleRenderer);
				if (reset == true) //Resetting the ball 
				{
					//Reseting X and Y positions
					BallRect.x = 610;
					BallRect.y = 350;
					//cout << BallRect.x << " .x Reset 2" << endl;
					//cout << BallRect.y << " .y Reset 2" << endl;
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
					SDL_RenderFillRect(DoubleRenderer, &BallRect); //(Re-)Drawing the ball
				}
				else
				{
					SDL_RenderFillRect(DoubleRenderer, &BallRect); //(Re-)Drawing the Ball
				}
				SDL_RenderFillRect(DoubleRenderer, &Player1Rect); //(Re-)Drawing Player 1
				SDL_RenderFillRect(DoubleRenderer, &Player2Rect);  //(Re-)Drawing Player 2
				SDL_RenderPresent(DoubleRenderer);

			}
			else if (PongEvent.type == SDL_KEYUP)
			{
				switch (PongEvent.key.keysym.sym)
				{
				case SDLK_UP:
					player1y_velocity += 100;
					break;
				case SDLK_DOWN:
					player1y_velocity -= 100;
					break;
				case SDLK_w:
					player2y_velocity += 100;
					break;
				case SDLK_s:
					player2y_velocity -= 100;
					break;
				}
				Player1Rect.y += player1y_velocity;
				Player2Rect.y += player2y_velocity;
			}
		}

		Uint64 end = SDL_GetPerformanceCounter();
		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		//Cap to 60 FPS (16.666f)
		//Higher the number the slower the game
		SDL_Delay(floor(150.0f - elapsedMS));
	}

	SDL_DestroyWindow(DoubleWindow);
	SDL_Quit;
	TTF_Quit;
	IMG_Quit;
}

bool PlayGame::Collision(SDL_Rect a, SDL_Rect b)
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

SDL_Rect PlayGame::rectcreation(int x, int y, int width, int height, int r, int g, int b, SDL_Renderer* render)
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

void PlayGame::textcreate(SDL_Color Colour, SDL_Renderer* text_render, int x, int y, string text, int fontsize)
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