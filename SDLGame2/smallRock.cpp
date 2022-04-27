#include "smallRock.h"
#include <iostream>

using namespace std;

SmallRock::SmallRock(SDL_Renderer* renderer, float x, float y) {

	active = false;

	speed = 400.0;

	SDL_Surface* surface = IMG_Load("./Assets/smRock.png");

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

void SmallRock::Reposition(float x, float y)
{
	active = true;

	posRect.x = x;
	posRect.y = y;

	pos_x = x;
	pos_y = y;

	speed = rand() % 2 + 3;
	speed *= 100;

	int upOrdown = rand() % 2 + 1;

	int leftOrright = rand() % 2 + 1;

	if (upOrdown == 1)//up
	{
		if (leftOrright == 1) //left
		{
			xDir = -1;
			yDir = -1;
		}
		else {
			xDir = 1;
			yDir = -1;
		}

	}
	else 
	{
		if (leftOrright == 1) //left
		{
			xDir = -1;
			yDir = 1;
		}
		else {
			xDir = 1;
			yDir = 1;
		}
	}
	
}

void SmallRock::Deactivate()
{
	active = false;

	posRect.x = -2000;
	posRect.y = -2000;

	pos_x = posRect.x;
	pos_y = posRect.y;

}


void SmallRock::Update(float deltatTime)
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

void SmallRock::Draw(SDL_Renderer* renderer) {

	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);

}

SmallRock::~SmallRock()
{
	//SDL_DestroyTexture(texture);
}