#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include <vector>
#include <string>
#include "iostream"

using namespace sf;
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

int WIDTH = 1200;
int HEIGHT = 1000;
int initVel = 600;
int tempVel;
int augPerHit = 25;
float radius = 20;
Vector2f paddleSize = Vector2f(200, 40);
Vector2f paddleVel = Vector2f(-400, 0);
Vector2f brickSize = Vector2f(198, 48);
Font font;
Texture ballTex;
Texture paddleTex;
Texture brickTex;
Texture toughBrickTex;
Texture ironBrickTex;
Texture backTex;
Sprite background;
//board
SoundBuffer buf;
Sound sound;
//break
SoundBuffer bBuf;
Sound bSound;
//damage
SoundBuffer dBuf;
Sound dSound;
//lose life
SoundBuffer sBuf;
Sound sSound;
//hitPaddle
SoundBuffer pBuf;
Sound pSound;
//win
SoundBuffer wBuf;
Sound wSound;
int lScore;
int rScore;
Text lScoreText;
Text rScoreText;
Text MainText;
int currentIndex;
int kicker;
int level;
int lives;

int updateMainMenu(Text mainMenuChioce[], RectangleShape &currentChoiceRect);
int renderMainMenuFrame(Text mainMenuChioce[], RectangleShape currentChoiceRect);
int update_state(float dt, ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall);
void render_frame(ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall);
int updateEndInterface(Text endChioce[], RectangleShape &currentChoiceRect);
int renderEndInterfaceFrame(Text endChioce[], RectangleShape &currentChoiceRect);

int reset(ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall);
int resetGame(ball &newBall);
int resetPaddle(paddle &lPaddle, paddle &rPaddle);

int checkWin();
float length(Vector2f v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

RenderWindow window;
std::vector<std::unique_ptr<brick>> bricks;

int initBricks(int level);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	window.create(VideoMode(WIDTH, HEIGHT), "Just Another Breakout");
	srand((unsigned)time(NULL));

	Clock clock;
	int mainMenuFlag = 1;
	int gameInterfaceFlag = 0;
	int endInterfaceFlag = 0;
	int temp = 0;
	level = 0;
	lives = 3;
	RectangleShape currentChoiceRect;
	currentIndex = 0;
	Vector2u TextureSize;
	Vector2u WindowSize;
	currentChoiceRect.setSize(Vector2f(30, 30));
	currentChoiceRect.setFillColor(Color::Red);

	font.loadFromFile("font.ttf");
	ballTex.loadFromFile("ball.png");
	paddleTex.loadFromFile("paddle.png");
	brickTex.loadFromFile("brick.png");
	toughBrickTex.loadFromFile("toughbrick.png");
	ironBrickTex.loadFromFile("ironbrick.png");
	buf.loadFromFile("knock.wav");
	sound.setBuffer(buf);
	bBuf.loadFromFile("break.wav");
	bSound.setBuffer(bBuf);
	dBuf.loadFromFile("damage.wav");
	dSound.setBuffer(dBuf);
	sBuf.loadFromFile("score.wav");
	sSound.setBuffer(sBuf);
	pBuf.loadFromFile("paddle.wav");
	pSound.setBuffer(pBuf);
	wBuf.loadFromFile("win.wav");
	wSound.setBuffer(wBuf);

	if (!backTex.loadFromFile("background.jpg"))
	{
		return -1;
	}
	else
	{
		TextureSize = backTex.getSize(); //Get size of texture.
		WindowSize = window.getSize();             //Get size of window.
		float ScaleX = (float)WindowSize.x / TextureSize.x;
		float ScaleY = (float)WindowSize.y / TextureSize.y;     //Calculate scale.
		background.setTexture(backTex);
		background.setScale(ScaleX, ScaleY);      //Set scale.  
	}

	ball newBall = ball(radius, WIDTH, HEIGHT, initVel, -1, ballTex);
	int type = rand() % 2;
	ball powerBall = ball(radius + 15, WIDTH, HEIGHT, 0, type, ballTex);
	paddle lPaddle = paddle(paddleSize, WIDTH, HEIGHT, -1, paddleVel, paddleTex);
	paddle rPaddle = paddle(paddleSize, WIDTH, HEIGHT, 1, paddleVel, paddleTex);
	//paddle obstacle = paddle(paddleSize, WIDTH, HEIGHT, 0, Vector2f(0, 200), obstacleTex);
	tempVel = initVel;

	Text mainMenuChoice[2];
	mainMenuChoice[0].setFont(font);
	mainMenuChoice[0].setCharacterSize(50);
	mainMenuChoice[0].setString("Start");
	mainMenuChoice[0].setFillColor(Color::White);
	mainMenuChoice[0].setPosition(WIDTH / 2 - 100, HEIGHT / 2);

	mainMenuChoice[1].setFont(font);
	mainMenuChoice[1].setCharacterSize(50);
	mainMenuChoice[1].setString("Exit");
	mainMenuChoice[1].setFillColor(Color::White);
	mainMenuChoice[1].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 60);

	Text endChoice[3];
	endChoice[0].setFont(font);
	endChoice[0].setCharacterSize(50);
	endChoice[0].setString("Play Again");
	endChoice[0].setFillColor(Color::White);
	endChoice[0].setPosition(WIDTH / 2 - 100, HEIGHT / 2);

	endChoice[1].setFont(font);
	endChoice[1].setCharacterSize(50);
	endChoice[1].setString("Back to Main Menu");
	endChoice[1].setFillColor(Color::White);
	endChoice[1].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 60);

	endChoice[2].setFont(font);
	endChoice[2].setCharacterSize(50);
	endChoice[2].setString("Exit");
	endChoice[2].setFillColor(Color::White);
	endChoice[2].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 120);

	lScore = 0;
	rScore = 0;
	lScoreText.setFont(font);
	lScoreText.setCharacterSize(50);
	lScoreText.setString("Left Lives: III");
	lScoreText.setFillColor(Color::White);
	lScoreText.setPosition(20, 0);
	rScoreText.setFont(font);
	rScoreText.setCharacterSize(50);
	rScoreText.setString("Scores: 0");
	rScoreText.setFillColor(Color::White);
	rScoreText.setPosition(WIDTH - 400, 0);

	MainText.setFont(font);
	MainText.setCharacterSize(100);
	MainText.setString("Just Another Breakout");
	MainText.setFillColor(Color::White);
	MainText.setPosition(150, 150);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();
		if (mainMenuFlag) {
			temp = updateMainMenu(mainMenuChoice, currentChoiceRect);
			renderMainMenuFrame(mainMenuChoice, currentChoiceRect);
			if (temp == 1) {
				mainMenuFlag = 0;
				gameInterfaceFlag = 1;
				currentIndex = 0;
				MainText.setString("");
				initBricks(level);
			}
			else if (temp == -1) {
				break;
			}
		}
		if (gameInterfaceFlag) {
			temp = update_state(dt, newBall, lPaddle, rPaddle, powerBall);
			render_frame(newBall, lPaddle, rPaddle, powerBall);
			if (temp == 1) {
				gameInterfaceFlag = 0;
				endInterfaceFlag = 1;
				MainText.setString("Game Over!");
				MainText.setPosition(400, 200);
			}
		}
		if (endInterfaceFlag) {
			temp = updateEndInterface(endChoice, currentChoiceRect);
			renderEndInterfaceFrame(endChoice, currentChoiceRect);
			if (temp == 0) {
				currentIndex = 0;
				endInterfaceFlag = 0;
				gameInterfaceFlag = 1;
				resetGame(newBall);
				MainText.setString("");
			}
			if (temp == 1) {
				currentIndex = 0;
				endInterfaceFlag = 0;
				mainMenuFlag = 1;
				MainText.setString("Just Another Breakout");
				MainText.setPosition(150, 150);
				resetGame(newBall);
			}
			if (temp == 2) {
				break;
			}
		}

		window.display();
	}
	bricks.clear();
	return 0;
}

int update_state(float dt, ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall)
{
	Vector2f bp = newBall.getBall().getPosition();
	Vector2f lpp = lPaddle.getPaddle().getPosition();
	Vector2f rpp = rPaddle.getPaddle().getPosition();
	//Vector2f op = obstacle.getPaddle().getPosition();
	Vector2f pp = powerBall.getBall().getPosition();
	Vector2f temp;
	float len;
	int kick = 0;
	static Vector2f ppp = rpp;
	static int startFlag = 1;
	static int flag = 0;
	std::string text;
	//win
	if (checkWin()) {
		if (flag == 0) {
			wSound.play();
			flag = 1;
			MainText.setString("Level Clear!");
			MainText.setPosition(400, 400);
		}
		if (wSound.getStatus() != SoundSource::Playing && flag == 1) {
			flag = 0;
			startFlag = 1;
			MainText.setString("");
			reset(newBall, lPaddle, rPaddle, powerBall);
			ppp = rPaddle.getPaddle().getPosition();
			level = (level + 1) % 3;
			newBall.levelUp();
			initBricks(level);
		}
		return 0;
	}
	//lose
	if (bp.y >= HEIGHT - 10) {
		if (flag == 0) {
			lives--;
			text = "Left lives: ";
			for (int i = 0; i < lives; i++) {
				text = text + "I";
			}
			lScoreText.setString(text);
			sSound.play();
			flag = 1;
		}
		if (sSound.getStatus() != SoundSource::Playing && flag == 1) {
			flag = 0;
			startFlag = 1;
			reset(newBall, lPaddle, rPaddle, powerBall);
			ppp = rPaddle.getPaddle().getPosition();
			if (lives == 0) {
				return 1;
			}
		}
		return 0;
	}
	if (startFlag) {
		newBall.startMove(rpp, ppp, dt);
		ppp = rpp;
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			startFlag = 0;

		}
	}
	//collision
	if (!startFlag) {
		for (int i = 0; i < bricks.size();)
		{
			if (newBall.collidBrickDetect(bricks[i]->getRect())) {
				if (!bricks[i]->getHitFlag()) {
					bricks[i]->setHitFlag(1);
					int type = bricks[i]->hit();
					//std::cout << "x: " << newBall.getBall().getPosition().x << " y: " << newBall.getBall().getPosition().y << std::endl;
					kick = 3;
					if (type) {
						rScore += (type * 10);
						rScoreText.setString("Scores: " + std::to_string(rScore));
						bricks.erase(bricks.begin() + i);
						kick = 4;
						continue;
					}
				}	
			}
			else if (bricks[i]->getHitFlag()) {
				bricks[i]->setHitFlag(0);
			}
			i++;
		}
		if (newBall.collidPaddleDetect(rPaddle.getPaddle())) {
			kick = 2;
		}
		newBall.UpdatePosition(dt);
	}
	//board
	if (bp.y <= radius) {
		newBall.SetVel(Vector2f(newBall.getVel().x, abs(newBall.getVel().y)));
		kick = 1;
	}
	if (bp.x <= radius) {
		newBall.SetVel(Vector2f(abs(newBall.getVel().x), newBall.getVel().y));
		kick = 1;
	}
	if (bp.x >= WIDTH - radius) {
		newBall.SetVel(Vector2f(-abs(newBall.getVel().x), newBall.getVel().y));
		kick = 1;
	}

	////press Left
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		rPaddle.UpdatePosition(dt, 0);
	}
	////press Right
	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		rPaddle.UpdatePosition(dt, 1);
	}
	
	switch (kick)
	{
	case 1:
		sound.play();
		kick = 0;
		break;
	case 2:
		pSound.play();
		kick = 0;
		break;
	case 3:
		dSound.play();
		kick = 0;
		break;
	case 4:
		bSound.play();
		kick = 0;
		break;
	}

	return 0;
}

void render_frame(ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall)
{
	window.clear();
	window.draw(background);
	window.draw(lScoreText);
	window.draw(rScoreText);
	window.draw(newBall.getBall());
	//window.draw(lPaddle.getPaddle());
	window.draw(rPaddle.getPaddle());
	for (int i = 0; i < bricks.size(); ++i)
	{
		bricks[i]->draw(window);
	}
	window.draw(MainText);
}

int reset(ball &newBall, paddle &lPaddle, paddle &rPaddle, ball &powerBall) {
	//tempVel = initVel;
	newBall.reset();
	//lPaddle.reset();
	rPaddle.reset();
	return 0;
}

int updateMainMenu(Text mainMenuChioce[], RectangleShape &currentChoiceRect) {
	static int upFlag = 0;
	static int downFlag = 0;
	static int enterFlag = 0;
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		upFlag = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		downFlag = 1;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Up) && upFlag == 1) {
		mainMenuChioce[currentIndex].setColor(Color::White);
		currentIndex = (currentIndex -1 + 2) % (2);
		upFlag = 0;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Down) && downFlag == 1) {
		mainMenuChioce[currentIndex].setColor(Color::White);
		currentIndex = (currentIndex + 1) % (2);
		downFlag = 0;
	}
	mainMenuChioce[currentIndex].setColor(Color::Red);
	currentChoiceRect.setPosition(Vector2f(mainMenuChioce[currentIndex].getPosition().x - 50, mainMenuChioce[currentIndex].getPosition().y + 20));
	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		enterFlag = 1;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Enter) && enterFlag == 1) {
		mainMenuChioce[currentIndex].setColor(Color::White);
		enterFlag = 0;
		if (currentIndex == 1) {
			return -1;
		}
		return 1;
	}
	return 0;
}

int renderMainMenuFrame(Text mainMenuChioce[], RectangleShape currentChoiceRect) {
	window.clear();
	window.draw(background);
	window.draw(MainText);
	for (int i = 0; i < 2; i++) {
		window.draw(mainMenuChioce[i]);
	}
	window.draw(currentChoiceRect);
	return 0;
}

int updateEndInterface(Text endChioce[], RectangleShape &currentChoiceRect) {
	static int upFlag = 0;
	static int downFlag = 0;
	static int enterFlag = 0;
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		upFlag = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		downFlag = 1;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Up) && upFlag == 1) {
		endChioce[currentIndex].setColor(Color::White);
		currentIndex = (currentIndex - 1 + 3) % 3;
		upFlag = 0;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Down) && downFlag == 1) {
		endChioce[currentIndex].setColor(Color::White);
		currentIndex = (currentIndex + 1 + 3) % 3;
		downFlag = 0;
	}
	endChioce[currentIndex].setColor(Color::Red);
	currentChoiceRect.setPosition(Vector2f(endChioce[currentIndex].getPosition().x - 50, endChioce[currentIndex].getPosition().y + 20));
	if (Keyboard::isKeyPressed(Keyboard::Enter)) {
		enterFlag = 1;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Enter) && enterFlag == 1) {
		endChioce[currentIndex].setColor(Color::White);
		enterFlag = 0;
		return currentIndex;
	}
	return -1;
}

int renderEndInterfaceFrame(Text endChioce[], RectangleShape &currentChoiceRect) {
	window.clear();
	window.draw(background);
	window.draw(MainText);
	window.draw(lScoreText);
	window.draw(rScoreText);
	for (int i = 0; i < 3; i++) {
		window.draw(endChioce[i]);
	}
	window.draw(currentChoiceRect);
	return 0;
}

int resetPaddle(paddle &lPaddle, paddle &rPaddle) {
	rPaddle.reset(paddleSize, paddleVel);
	lPaddle.reset(paddleSize, paddleVel);
	return 0;
}

int resetGame(ball &newBall) {
	lives = 3;
	level = 0;
	rScore = 0;
	newBall.setBasicVel(initVel);
	lScoreText.setString("Left lives: III");
	rScoreText.setString("Scores: 0");
	initBricks(level);
	return 0;
}

int initBricks(int level) {
	int index = 0;
	if (!bricks.empty()) {
		bricks.clear();
	}
	if (level == 0) {
		for (int i = 80; i < 320; i += 60) {
			for (int j = 0; j < WIDTH; j += 200) {
				brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 1, 1, brickTex, brickTex);
				bricks.push_back(std::unique_ptr<brick>(b));
			}
		}
	}
	if (level == 1) {
		for (int i = 80; i < 320; i += 60) {
			for (int j = 0; j < WIDTH; j += 200) {
				if (index == 1 || index == 4 || index == 6 || index == 8 || index == 9 || index == 11 || index == 13 || index == 16) {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 2, 2, brickTex, toughBrickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}
				else {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 1, 1, brickTex, brickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}				
				index++;
			}
		}
	}
	if (level == 2) {
		for (int i = 80; i < 320; i += 60) {
			for (int j = 0; j < WIDTH; j += 200) {
				if (index == 1 || index == 4 || index == 6 || index == 8 || index == 9 || index == 11 || index == 13 || index == 16) {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 2, 2, brickTex, toughBrickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}
				else if (index == 7 || index == 10 || index == 20 || index == 21) {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, -1, 3, ironBrickTex, ironBrickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}
				else if (index == 2 || index == 3) {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 1, 4, brickTex, brickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}
				else {
					brick* b = new brick(brickSize, Vector2f(j, i), WIDTH, HEIGHT, 1, 1, brickTex, brickTex);
					bricks.push_back(std::unique_ptr<brick>(b));
				}
				index++;
			}
		}
	}
	return 0;
}

int checkWin() {
	for (int i = 0; i < bricks.size(); i++) {
		if (bricks[i]->getType() != 3) {
			return 0;
		}
	}
	return 1;
}