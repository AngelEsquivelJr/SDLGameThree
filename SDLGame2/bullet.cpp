#include "bullet.h"

Bullet::Bullet(SDL_Renderer* renderer, float x, float y) {

	active = false;

	speed = 800.0;

	SDL_Surface* surface = IMG_Load("./Assets/bull.png");

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	if (texture == NULL)
	{
		printf("Unable to load Image %s! SDL_image Error: %s\n",
			"./Assets/bull.png", IMG_GetError());
	}

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

}


//wrapping
void Bullet::Deactivate()
{
	active = false;

	posRect.x = -3000;
	posRect.y = -3000;

	pos_x = posRect.x;
	pos_y = posRect.y;
}

void Bullet::Reposition()
{
	if (xDir == 1 && yDir == 0)
	{
		posRect.x += (45);
		posRect.y += (11);
	}
	else if (xDir == -1 && yDir == 0)
	{
		posRect.x -= 8;
		posRect.y += (11);
	}
	else if (xDir == 0 && yDir == 1)
	{
		posRect.x += 20;
		posRect.y += 38;
	}
	else if (xDir == 0 && yDir == -1)
	{
		posRect.x += 18;
		posRect.y -= 16;
	}
	else if (xDir == 1 && yDir == -1)
	{
		posRect.x += 40;
		posRect.y -= 10;
	}
	else if (xDir == -1 && yDir == -1)
	{
		posRect.x -= 6;
		posRect.y -= 10;
	}
	else if (xDir == 1 && yDir == 1)
	{
		posRect.x += 40;
		posRect.y += 32;
	}
	else if (xDir == -1 && yDir == 1)
	{
		posRect.x -= 2;
		posRect.y += 32;
	}

	pos_x = posRect.x;
	pos_y = posRect.y;

}

void Bullet::Update(float deltatTime)
{
	if (active) {
		pos_x += (speed * xDir) * deltatTime;
		pos_y += (speed * yDir) * deltatTime;

		posRect.x = (int)(pos_x + 0.5f);
		posRect.y = (int)(pos_y + 0.5f);

		if (posRect.x < 0 || posRect.x > 1024 || posRect.y < 0 || posRect.y > 768)
		{
			posRect.x = -1000;
			posRect.y = -1000;

			pos_x = posRect.x;
			pos_y = posRect.y;

			active = false;
		}
	}

}

void Bullet::Draw(SDL_Renderer* renderer) {

	SDL_RenderCopy(renderer, texture, NULL, &posRect);

}

Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);
}