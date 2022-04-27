#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool quit = false;

//delta time for frame rate indepencence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

//vars for angles
float x, y, oldAngle;

float playerAngle;

//set player speed
float playerSpeed = 400.0f;

//for direction
float xDir, xDirOld;
float yDir, yDirOld;

//payer score integer variable
int playerScore;

//variables for precision loss
float pos_X, pos_Y;

//create rect for position
SDL_Rect playerPos;

SDL_Point center;

//bullet stuff
#include "bullet.h"
vector<Bullet> bulletList;

//declare sounds
//small sounds
Mix_Chunk* laser;
Mix_Chunk* explode;
//longer sound
Mix_Music* bgm;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i].active == false)
		{
			//play laser sound
			Mix_PlayChannel(-1, laser, 0);

			bulletList[i].active = true;

			bulletList[i].posRect.x = pos_X;

			bulletList[i].posRect.y = pos_Y;

			bulletList[i].pos_x = pos_X;

			bulletList[i].pos_y = pos_Y;

			bulletList[i].xDir = xDirOld;

			bulletList[i].yDir = yDirOld;

			bulletList[i].Reposition();

			break;
		}
	}
}

//large rocks
#include "largeRock.h"

vector<LargeRock> largeRockList;


//small rocks
#include "smallRock.h"

vector<SmallRock> smallRockList;


int main(int argc, char* argv[])
{
	//seed random num
	srand(time(NULL));


	SDL_Window* window; //pointer

	//render variable
	SDL_Renderer* renderer = NULL;

	SDL_Init(SDL_INIT_EVERYTHING);

	//create app window with settings
	window = SDL_CreateWindow(
		"Space Rocks",					//window title
		SDL_WINDOWPOS_UNDEFINED,		//x position
		SDL_WINDOWPOS_UNDEFINED,		//y position
		1024,							//width, in pixels
		768,							//height, in pixels
		SDL_WINDOW_OPENGL				// flags
	);

	//check window was created successfully
	if (window == NULL) {
		//case if not made
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	//create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//background image -- CREATE


	//create sdl Surface
	SDL_Surface* surface = IMG_Load("./Assets/bkgd.png");

	//create bkgd texture
	SDL_Texture* bkgd;

	//place surface into texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	//create rect for menu graphics
	SDL_Rect bkgdPos;

	//sdl event to handle input
	SDL_Event event;

	//set pos
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	//background -- CREATE END

	//player image -- CREATE

	//create surface
	surface = IMG_Load("./Assets/player.png");

	//create bkgd texture
	SDL_Texture* player;

	//place surface into texture
	player = SDL_CreateTextureFromSurface(renderer, surface);

	//free the surface
	SDL_FreeSurface(surface);

	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 50;
	playerPos.h = 48;

	center.x = playerPos.w / 2;
	center.y = playerPos.h / 2;

	pos_X = playerPos.x;
	pos_Y = playerPos.y;

	//player direction x = right = +1, left =-1
	//y = -1 up, down +1
	xDir = 1;
	yDir = 0;

	xDirOld = 1;
	yDirOld = 0;


	//player -- CREATE END

	//fill bullets list
	for (int i = 0; i < 10; i++) {
		Bullet tempBullet(renderer, -1000.0f, -1000.0f);

		bulletList.push_back(tempBullet);
	}

	//audio playback
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//load laser, explosion and bkg sound
	laser = Mix_LoadWAV("./Assets/laser.wav");

	//load laser, explosion and bkg sound
	explode = Mix_LoadWAV("./Assets/explode.wav");

	//LEVEL UP *************************
	//rocks to start game
	int numOfLargeRocks = 1;
	int numOfSmallRocks = 2;
	int totalNumRocks = numOfLargeRocks + numOfSmallRocks;

	//total rocks destroyed
	int totalRocksDest = 0;

	//fill large rocks
	for (int i = 0; i < numOfLargeRocks; i++) {
		LargeRock templRock(renderer, -1000.0f, -1000.0f);

		largeRockList.push_back(templRock);
	}

	//fill small rocks
	for (int i = 0; i < numOfSmallRocks; i++) {
		SmallRock tempsRock(renderer, -1000.0f, -1000.0f);

		smallRockList.push_back(tempsRock);
	}

	//activate large rocks for lvl 1
	for (int i = 0; i < numOfLargeRocks; i++)
	{
		largeRockList[i].Reposition();

	}

	//intit player score
	playerScore = 0;

	//display initial level
	cout << "You have reached level " << numOfLargeRocks << endl;

	while(!quit)
	{

		//create delta time
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		//check for input
		if (SDL_PollEvent(&event))
		{
			//close window by X button
			if (event.type == SDL_QUIT) {
				quit = true;
				break;
			}

			switch (event.type) {

				//look for keypress
			case SDL_KEYUP:

				//check spacebar
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					CreateBullet();
					break;
				default:
					break;
				}
			}
		} //input end

		//player movement
				//get keyboard state
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		//check arrows pressed
		if (currentKeyStates[SDL_SCANCODE_LEFT]) {
			xDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
			xDir = 1.0f;
		}
		else {
			xDir = 0;
		}

		//check arrows pressed
		if (currentKeyStates[SDL_SCANCODE_UP]) {
			yDir = -1.0f;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
			yDir = 1.0f;
		}
		else {
			yDir = 0;
		}

		//UPDATE ***************************************

		if (xDir != 0 || yDir != 0)
		{
			x = playerPos.x - xDir;
			y = playerPos.y - yDir;

			//calc angle
			playerAngle = atan2(yDir, xDir) * 180 / 3.14;

			//update old
			oldAngle = playerAngle;

			xDirOld = xDir;
			yDirOld = yDir;

		}
		else
		{
			oldAngle = playerAngle;
		}

		//new position
		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;

		//precison lose
		playerPos.x = (int)(pos_X + 0.5f);
		playerPos.y = (int)(pos_Y + 0.5f);

		// player wrapping
		if (playerPos.x < (0 - playerPos.w))
		{
			playerPos.x = 1024;
			pos_X = playerPos.x;
		}

		//right
		if (playerPos.x > 1024)
		{
			playerPos.x = (0 - playerPos.w);
			pos_X = playerPos.x;
		}

		//top
		if (playerPos.y < (0 - playerPos.w))
		{
			playerPos.y = 768;
			pos_Y = playerPos.y;
		}

		//bottom
		if (playerPos.y > 768)
		{
			playerPos.y = (0 - playerPos.w);
			pos_Y = playerPos.y;
		}


		//update bullet list
		for (int i = 0; i < bulletList.size(); i++) {
			if (bulletList[i].active == true) {
				bulletList[i].Update(deltaTime);
			}
		}

		//update lRocks list
		for (int i = 0; i < largeRockList.size(); i++) {
			if (largeRockList[i].active == true) {
				largeRockList[i].Update(deltaTime);

			}
		}

		//update sRocks list
		for (int i = 0; i < smallRockList.size(); i++) {
			if (smallRockList[i].active == true) {
				smallRockList[i].Update(deltaTime);
			}
		}


		//collison ********
		//bullet with large rock
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				for (int j = 0; j < largeRockList.size(); j++)
				{
					if (SDL_HasIntersection(&bulletList[i].posRect, &largeRockList[j].posRect))
					{
						//play explosion
						Mix_PlayChannel(-1, explode, 0);

						//give player points
						playerScore += 50;
						//display score
						cout << "Player Score: " << playerScore << endl;

						//level up
						int smRockCounter = 0;
						

						for (int i = 0; i < smallRockList.size(); i++)
						{
							if (smallRockList[i].active == false)
							{
								smallRockList[i].Reposition(largeRockList[j].posRect.x, largeRockList[j].posRect.y);

								smRockCounter++;
							}

							//2 rocks exit loop
							if (smRockCounter == 2)
							{
								break;
							}
						}

						largeRockList[j].Deactivate();

						bulletList[i].Deactivate();

						//level up
						totalRocksDest++;
					}
				}
			}
		}

		//bullet with small rock
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				for (int j = 0; j < smallRockList.size(); j++)
				{
					if (SDL_HasIntersection(&bulletList[i].posRect, &smallRockList[j].posRect))
					{
						//play explosion
						Mix_PlayChannel(-1, explode, 0);

						smallRockList[j].Deactivate();

						bulletList[i].Deactivate();

						//give player points
						playerScore += 100;

						//display score
						cout << "Player Score: " << playerScore << endl;
						
						//level up
						totalRocksDest++;
						
						//check to see if rocks destroyed
						if (totalRocksDest >= totalNumRocks)
						{
							totalRocksDest = 0;


							//add rocks to level
							numOfLargeRocks++;
							numOfSmallRocks += 2;

							//display level
							cout << "You have reached level " << numOfLargeRocks << endl;

							totalNumRocks = numOfLargeRocks + numOfSmallRocks;

							largeRockList.clear();

							smallRockList.clear();

							//rebuil list with new numbers
							//load large rocks
							for (int i = 0; i < numOfLargeRocks;i++) {
								LargeRock tempRock(renderer, -1000.0f, -1000.0f);

								largeRockList.push_back(tempRock);
							}

							//load small rocks
							for (int i = 0; i < numOfSmallRocks; i++)
							{
								SmallRock tempRock(renderer, -1000.0f, -1000.0f);

								smallRockList.push_back(tempRock);
							}

							//activate large for next level
							for (int i = 0; i < numOfLargeRocks; i++)
							{
								largeRockList[i].Reposition();
								
							}

						}

					}
				}
			}
		}

		////check for collision
		////enemy against player
		//for (int i = 0; i < enemyList.size(); i++)
		//{
		//	if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
		//	{
		//		//play explosion
		//		Mix_PlayChannel(-1, explode, 0);

		//		enemyList[i].Reset();

		//		playerLives -= 1;

		//		//check for lose
		//		if (playerLives <= 0) {
		//			level2 = false;
		//			gameState = LOSE;
		//		}
		//	}
		//}


		//UPDATE ***************************************


	//DRAW ******************************

	//clear old buffer
	SDL_RenderClear(renderer);

	//prepare to draw bkgd1
	SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

	//draw bullet list
	for (int i = 0; i < bulletList.size(); i++) {
		if (bulletList[i].active == true) {
			bulletList[i].Draw(renderer);
		}
	}

	//draw lRock list
	for (int i = 0; i < largeRockList.size(); i++) {
		if (largeRockList[i].active == true) {
			largeRockList[i].Draw(renderer);
		}
	}

	//draw sRock list
	for (int i = 0; i < smallRockList.size(); i++) {
		if (smallRockList[i].active == true) {
			smallRockList[i].Draw(renderer);
		}
	}

	//draw player
	SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);

	//draw new info to screen
	SDL_RenderPresent(renderer);

	//DRAW END ***********************

	} //end game loop

		//close and destroy window
	SDL_DestroyWindow(window);

	//clean up
	SDL_Quit();

	return 0;
}