#include "ball.h"

ball::ball(float radius, int WIDTH, int HEIGHT, int initVel, int type, Texture &tex)
{
	this->WIDTH = WIDTH;
	this->HEIGHT = HEIGHT;
	this->type = type;
	this->radius = radius;
	ballShape.setTexture(&tex);
	ballShape.setRadius(radius);
	ballShape.setOrigin(radius, radius);
	if (type == -1) {
		ballShape.setPosition(WIDTH / 2, HEIGHT - 100 - radius + 3);
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
	basicVel = vel.x;
	sVel = Vector2f(100, 0);
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

int ball::reset() {
	ballShape.setPosition(WIDTH / 2, HEIGHT - 100 - radius + 3);
	return 0;
}

float Length(Vector2f v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

int ball::collidBrickDetect(RectangleShape rect) {
	Vector2f bp = ballShape.getPosition();
	Vector2f op = rect.getPosition();
	Vector2f brickSize = rect.getSize();
	if (bp.x >= op.x && bp.x <= op.x + brickSize.x) {
		if (op.y - bp.y < radius && op.y >= bp.y) {
			vel.y = -abs(vel.y);
			return 1;
		}
		if (bp.y - op.y - brickSize.y < radius && bp.y > op.y + brickSize.y) {
			vel.y = abs(vel.y);
			return 1;
		}
	}
	else if (bp.y >= op.y && bp.y <= op.y + brickSize.y) {
		if (bp.x - op.x - brickSize.x <= radius && bp.x >= op.x + brickSize.x) {
			vel.x = abs(vel.x);
			return 1;
		}
		if (op.x - bp.x <= radius && op.x >= bp.x) {
			vel.x = -abs(vel.x);
			return 1;
		}
	}
	else if (Length(Vector2f(bp.x - op.x, bp.y - op.y)) < radius && op.x > bp.x && op.y > bp.y) {
		vel.x = -abs(vel.x);
		vel.y = -abs(vel.y);
		return 1;
	}
	else if (Length(Vector2f(bp.x - op.x - brickSize.x, bp.y - op.y)) < radius && bp.x > op.x + brickSize.x && op.y > bp.y) {
		vel.x = abs(vel.x);
		vel.y = -abs(vel.y);
		return 1;
	}
	else if (Length(Vector2f(bp.x - op.x, bp.y - op.y - brickSize.y)) < radius && op.x > bp.x && bp.y > op.y + brickSize.y) {
		vel.x = -abs(vel.x);
		vel.y = abs(vel.y);
		return 1;
	}
	else if (Length(Vector2f(bp.x - op.x - brickSize.x, bp.y - op.y - brickSize.y)) < radius
		&& bp.x > op.x + brickSize.x && bp.y > op.y + brickSize.y) {
		vel.x = abs(vel.x);
		vel.y = abs(vel.y);
		return 1;
	}
	return 0;
}

int ball::startMove(Vector2f cpp, Vector2f ppp, float dt) {
	ballShape.setPosition(cpp - ppp + sVel * dt + ballShape.getPosition());
	if (ballShape.getPosition().x >= cpp.x + 200 - radius) {
		sVel.x = -abs(sVel.x);
	}
	if (ballShape.getPosition().x <= cpp.x + radius) {
		sVel.x = abs(sVel.x);
	}
	return 0;
}

int ball::collidPaddleDetect(RectangleShape rect) {
	Vector2f bp = ballShape.getPosition();
	Vector2f lpp = rect.getPosition();
	Vector2f paddleSize = rect.getSize();
	Vector2f temp;
	int len;
	if ((bp.x >= lpp.x && bp.x < lpp.x + paddleSize.x && (abs(lpp.y - bp.y) < radius && lpp.y > bp.y))
		|| (bp.y > lpp.y && bp.y < lpp.y + paddleSize.y
		&& ((bp.x - lpp.x - paddleSize.x < radius && bp.x > lpp.x + paddleSize.x) || (lpp.x - bp.x < radius && lpp.x > bp.x)))
		|| Length(Vector2f(bp.x - lpp.x - paddleSize.x, bp.y - lpp.y)) < radius
		|| Length(Vector2f(bp.x - lpp.x - paddleSize.x, bp.y - lpp.y - paddleSize.y)) < radius) {
		temp = bp - Vector2f(lpp.x + paddleSize.x / 2, lpp.y + paddleSize.x / 2);
		len = Length(temp);
		vel.x = temp.x*basicVel / len;
		vel.y = temp.y*basicVel / len;
		return 1;
	}
	return 0;
}

int ball::levelUp() {
	basicVel += 50;
	return 0;
}

int ball::setBasicVel(int vel) {
	basicVel = vel;
	return 0;
}