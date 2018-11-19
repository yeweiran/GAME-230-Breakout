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

class brick {
public:
	brick(Vector2f size, Vector2f pos, int WIDTH, int HEIGHT, int hp, int type, Texture &hp1Tex, Texture &hp2Tex);
	int update();
	int draw(RenderWindow &window);
	int hit();
	RectangleShape getRect();
	int getType();
private:
	RectangleShape rect;
	Vector2f vel;
	int WIDTH;
	int HEIGHT;
	int hp;
	Texture *hp1Tex;
	Texture *hp2Tex;
	int type;
};