#include "paddle.h"

paddle::paddle(Vector2f size, int WIDTH, int HEIGHT, int side, Vector2f vel, Texture &tex)
{
	rect.setSize(size);
	/*if (side == -1) {
		rect.setPosition(100, HEIGHT / 2 - size.y / 2);
	}
	else if (side == 1) {
		rect.setPosition(WIDTH - 100, HEIGHT / 2 - size.y / 2);
	}
	else if (side == 0) {
		rect.setPosition(WIDTH / 2, 0);
	}*/
	rect.setPosition(WIDTH / 2 - size.x / 2, HEIGHT -100);
	rect.setTexture(&tex);
	//rect.setFillColor(Color::White);
	this->side = side;
	this->vel = vel;
	this->HEIGHT = HEIGHT;
	this->WIDTH = WIDTH;
}

paddle::~paddle()
{
}

RectangleShape paddle::getPaddle() {
	return rect;
}

int paddle::SetVel(Vector2f newVel) {
	//vel.x = newVel.x;
	vel.y = newVel.y;
	return 0;
}

int paddle::UpdatePosition(float dt, int dir) {
	
	if (dir == 0 && rect.getPosition().x >= 0) {
		rect.setPosition(rect.getPosition() + vel * dt);
	}
	else if (dir == 1 && rect.getPosition().x + rect.getSize().x <= WIDTH) {
		rect.setPosition(rect.getPosition() - vel * dt);
	}
	return 0;
}

int paddle::reset() {
	rect.setPosition(WIDTH / 2 - rect.getSize().x / 2, HEIGHT - 100);
	return 0;
}

int paddle::reset(Vector2f size, Vector2f vel) {
	rect.setSize(size);
	this->vel = vel;
	return 0;
}

int paddle::powerUp(int type) {
	switch (type)
	{
	case 0:
		rect.setSize(Vector2f(rect.getSize().x, rect.getSize().y + 40));
		break;
	case 1:
		vel.y -= 80;
		break;
	}
	return 0;
}

Vector2f paddle::getSize() {
	return rect.getSize();
}