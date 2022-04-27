#include "largeRock.h"
#include <iostream>

using namespace std;

LargeRock::LargeRock(SDL_Renderer* renderer, float x, float y) {

	active = false;

	speed = 200.0;

	SDL_Surface* surface = IMG_Load("./Assets/bigRock.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	posRect.x = x;
	posRect.y = y;

	pos_x = x;
	pos_y = y;

	xDir = 0;
	yDir = 0;

	rockAngle = 0.0f;

	//center of rock
	rockCenter.x = posRect.w / 2;
	rockCenter.y = posRect.h / 2;

}

//wrapping
void LargeRock::Deactivate()
{
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;

	pos_x = posRect.x;
	pos_y = posRect.y;
}

void LargeRock::Reposition()
{
	active = true;
	int location = rand() % (4) + 1;
	int direction = rand() % (2) + 1;

	if (location == 1)//left
	{
		posRect.x = -posRect.w;
		pos_x = posRect.x;

		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_y = posRect.y;

		if (direction == 1)
		{
			xDir = 1;
			yDir = -1;
		}
		else {
			xDir = 1;
			yDir = 1;
		}

	}
	else if (location == 2)//right
	{
		posRect.x = 1024;
		pos_x = posRect.x;

		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_y = posRect.y;

		if (direction == 1)
		{
			xDir = -1;
			yDir = -1;
		}
		else {
			xDir = -1;
			yDir = 1;
		}
	}
	else if (location == 3)//top
	{
		posRect.y = -posRect.h;
		pos_y = posRect.y;

		int bottomOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % bottomOfScreen + posRect.w;
		pos_x = posRect.x;

		if (direction == 1)
		{
			xDir = -1;
			yDir = 1;
		}
		else {
			xDir = 1;
			yDir = 1;
		}
	}
	else if (location == 4)//bottom
	{
		posRect.y = 768;
		pos_y = posRect.y;

		int bottomOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % bottomOfScreen + posRect.w;
		pos_x = posRect.x;

		if (direction == 1)
		{
			xDir = -1;
			yDir = -1;
		}
		else {
			xDir = 1;
			yDir = -1;
		}
	}
}

void LargeRock::Update(float deltatTime)
{
	if (active)
	{
	
		pos_x += (speed * xDir) * deltatTime;
		pos_y += (speed * yDir) * deltatTime;

		posRect.x = (int)(pos_x + 0.5f);
		posRect.y = (int)(pos_y + 0.5f);

		rockAngle += .1;

		//wrapping
		if (posRect.x < (0 - posRect.w))
		{
			posRect.x = 1024;
			pos_x = posRect.x;
		}

		//right
		if (posRect.x > 1024)
		{
			posRect.x = (0 - posRect.w);
			pos_x = posRect.x;
		}

		//top
		if (posRect.y < (0 - posRect.w))
		{
			posRect.y = 768;
			pos_y = posRect.y;
		}

		//bottom
		if (posRect.y > 768)
		{
			posRect.y = (0 - posRect.w);
			pos_y = posRect.y;
		}

	}

}

void LargeRock::Draw(SDL_Renderer* renderer) {

	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);

}

LargeRock::~LargeRock()
{
	//SDL_DestroyTexture(texture);
}