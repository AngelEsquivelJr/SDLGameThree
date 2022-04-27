#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <string>
using namespace std;

class SmallRock
{

public:

	bool active;
	SDL_Texture* texture;
	SDL_Rect posRect;
	float xDir, yDir;
	float speed;
	float pos_x, pos_y;
	SDL_Point rockCenter;
	float rockAngle;

	SmallRock(SDL_Renderer* renderer, float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer* renderer);

	void Reposition(float x, float y);

	//wrapping
	void Deactivate();

	~SmallRock();

};
