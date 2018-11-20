#include "brick.h"

brick::brick(Vector2f size, Vector2f pos, int WIDTH, int HEIGHT, int hp, int type, Texture &hp1Tex, Texture &hp2Tex) {
	rect.setSize(size);
	rect.setPosition(pos);
	this->HEIGHT = HEIGHT;
	this->WIDTH = WIDTH;
	this->hp = hp;
	this->type = type;
	this->hp1Tex = &hp1Tex;
	this->hp2Tex = &hp2Tex;
	this->hitFlag = 0;
	if (type == 1 || type == 3) {
		rect.setTexture(&hp1Tex);
	}
	if (type == 2) {
		rect.setTexture(&hp2Tex);
	}
	if (type == 4) {
		rect.setFillColor(Color::Transparent);
	}
}

int brick::draw(RenderWindow &window) {
	window.draw(rect);
	return 0;
}

int brick::hit() {
	if (type != 3) {
		hp--;
		if (hp == 0) {
			return type;
		}
		else {
			rect.setTexture(hp1Tex);
		}
	}
	return 0;
}

RectangleShape brick::getRect() {
	return rect;
}

int brick::getType() {
	return type;
}

int brick::getHitFlag() {
	return hitFlag;
}

int brick::setHitFlag(int flag) {
	hitFlag = flag;
	return 0;
}