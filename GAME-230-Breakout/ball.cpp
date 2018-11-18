#include "ball.h"

ball::ball(float radius, int WIDTH, int HEIGHT, int initVel, int type, Texture &tex)
{
	this->WIDTH = WIDTH;
	this->HEIGHT = HEIGHT;
	this->type = type;
	ballShape.setTexture(&tex);
	ballShape.setRadius(radius);
	ballShape.setOrigin(radius, radius);
	if (type == -1) {
		ballShape.setPosition(WIDTH / 2, HEIGHT / 2);
		//ballShape.setFillColor(Color::Red);
	}
	else {
		ballShape.setPosition((rand() % (WIDTH - 400)) + 200, (rand() % (HEIGHT - 100)) + 50);
		if (type == 0) {
			ballShape.setFillColor(Color::Green);
		}
		if (type == 1) {
			ballShape.setFillColor(Color::Yellow);
		}
	}
	this->initVel = initVel;
	vel = Vector2f(initVel, 0);
}

ball::~ball()
{
}

int ball::SetVel(Vector2f newVel) {
	vel.x = newVel.x;
	vel.y = newVel.y;
	return 0;
}

int ball::UpdatePosition(float dt) {

	ballShape.setPosition(ballShape.getPosition() + vel * dt);
	return 0;
}

CircleShape ball::getBall() {
	return ballShape;
}

Vector2f ball::getVel() {
	return vel;
}

int ball::getType() {
	return type;
}

int ball::reset(int turn) {
	if (type == -1) {
		ballShape.setPosition(WIDTH / 2, HEIGHT / 2);
		if (turn % 2 == 0) {
			vel = Vector2f(initVel, 0);
		}
		else {
			vel = Vector2f(-initVel, 0);
		}
	}
	else {
		ballShape.setPosition((rand() % (WIDTH - 400)) + 200, (rand() % (HEIGHT - 100)) + 50);
		type = rand() % 2;
		if (type == 0) {
			ballShape.setFillColor(Color::Green);
		}
		if (type == 1) {
			ballShape.setFillColor(Color::Yellow);
		}
	}

	return 0;
}