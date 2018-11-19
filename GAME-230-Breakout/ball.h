#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include <stdlib.h>
#include <time.h> 

using namespace sf;

class ball
{
public:
	ball(float radius, int WIDTH, int HEIGHT, int initVel, int type, Texture &tex);
	~ball();
	int SetVel(Vector2f newVel);
	int UpdatePosition(float dt);
	CircleShape getBall();
	Vector2f getVel();
	int reset();
	int getType();
	int collidBrickDetect(RectangleShape rect);
	int startMove(Vector2f cpp, Vector2f ppp, float dt);
	int collidPaddleDetect(RectangleShape rect);
	int levelUp();
	int setBasicVel(int vel);
private:
	CircleShape ballShape;
	Vector2f vel;
	int initVel;
	int WIDTH;
	int HEIGHT;
	int type;
	int radius;
	int basicVel;
	Vector2f sVel;
};