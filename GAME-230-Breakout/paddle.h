#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace sf;

class paddle
{
public:
	paddle(Vector2f size, int WIDTH, int HEIGHT, int side, Vector2f vel, Texture &tex);
	~paddle();
	RectangleShape getPaddle();
	int SetVel(Vector2f newVel);
	int UpdatePosition(float dt, int dir);
	int reset();
	int reset(Vector2f size, Vector2f vel);
	int powerUp(int type);
	Vector2f getSize();

private:
	RectangleShape rect;
	Vector2f vel;
	int side;
	int WIDTH;
	int HEIGHT;
};
