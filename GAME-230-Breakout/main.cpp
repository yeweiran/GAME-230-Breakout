#include "ball.h"
#include "paddle.h"

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
int HEIGHT = 800;
int initVel = 500;
int tempVel;
int augPerHit = 25;
float radius = 20;
Vector2f paddleSize = Vector2f(30, 120);
Vector2f paddleVel = Vector2f(0, -250);
Font font;
Texture ballTex;
Texture paddleTex;
Texture obstacleTex;
Texture backTex;
Sprite background;
SoundBuffer buf;
Sound sound;
SoundBuffer sBuf;
Sound sSound;
SoundBuffer pBuf;
Sound pSound;
int lScore;
int rScore;
Text lScoreText;
Text rScoreText;
Text MainText;
int currentIndex;
int kicker;

enum mode {
	AI,
	PvP,
	OBSTACLE,
	POWERUPS,
	EXIT
};
mode gameMode;

int updateMainMenu(Text mainMenuChioce[], RectangleShape &currentChoiceRect);
int renderMainMenuFrame(Text mainMenuChioce[], RectangleShape currentChoiceRect);
int update_state(float dt, ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall);
void render_frame(ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall);
int updateEndInterface(Text endChioce[], RectangleShape &currentChoiceRect);
int renderEndInterfaceFrame(Text endChioce[], RectangleShape &currentChoiceRect);

int reset(ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall);
int resetGame(paddle &lPaddle, paddle &rPaddle);
int resetPaddle(paddle &lPaddle, paddle &rPaddle);
float length(Vector2f v)
{
	return sqrtf(v.x*v.x + v.y*v.y);
}

RenderWindow window;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);
	window.create(VideoMode(WIDTH, HEIGHT), "Just Another Pong Game");
	srand((unsigned)time(NULL));

	Clock clock;
	int mainMenuFlag = 1;
	int gameInterfaceFlag = 0;
	int endInterfaceFlag = 0;
	int temp = 0;
	RectangleShape currentChoiceRect;
	currentIndex = 0;
	Vector2u TextureSize;
	Vector2u WindowSize;
	currentChoiceRect.setSize(Vector2f(30, 30));
	currentChoiceRect.setFillColor(Color::Red);

	font.loadFromFile("font.ttf");
	ballTex.loadFromFile("ball.png");
	paddleTex.loadFromFile("woodpaddle.png");
	obstacleTex.loadFromFile("ironplate.png");
	backTex.loadFromFile("back.png");
	buf.loadFromFile("knock.wav");
	sound.setBuffer(buf);
	sBuf.loadFromFile("score.wav");
	sSound.setBuffer(sBuf);
	pBuf.loadFromFile("powerup.wav");
	pSound.setBuffer(pBuf);

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
	paddle obstacle = paddle(paddleSize, WIDTH, HEIGHT, 0, Vector2f(0, 200), obstacleTex);
	tempVel = initVel;

	Text mainMenuChoice[5];
	mainMenuChoice[0].setFont(font);
	mainMenuChoice[0].setCharacterSize(50);
	mainMenuChoice[0].setString("vs AI");
	mainMenuChoice[0].setFillColor(Color::White);
	mainMenuChoice[0].setPosition(WIDTH / 2 - 100, HEIGHT / 2);

	mainMenuChoice[1].setFont(font);
	mainMenuChoice[1].setCharacterSize(50);
	mainMenuChoice[1].setString("vs Human");
	mainMenuChoice[1].setFillColor(Color::White);
	mainMenuChoice[1].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 60);

	mainMenuChoice[2].setFont(font);
	mainMenuChoice[2].setCharacterSize(50);
	mainMenuChoice[2].setString("Obstacle Mode");
	mainMenuChoice[2].setFillColor(Color::White);
	mainMenuChoice[2].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 120);

	mainMenuChoice[3].setFont(font);
	mainMenuChoice[3].setCharacterSize(50);
	mainMenuChoice[3].setString("PowerUps Mode");
	mainMenuChoice[3].setFillColor(Color::White);
	mainMenuChoice[3].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 180);

	mainMenuChoice[4].setFont(font);
	mainMenuChoice[4].setCharacterSize(50);
	mainMenuChoice[4].setString("Exit");
	mainMenuChoice[4].setFillColor(Color::White);
	mainMenuChoice[4].setPosition(WIDTH / 2 - 100, HEIGHT / 2 + 240);

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
	lScoreText.setCharacterSize(100);
	lScoreText.setString(std::to_string(lScore));
	lScoreText.setFillColor(Color::White);
	lScoreText.setPosition(50, 50);
	rScoreText.setFont(font);
	rScoreText.setCharacterSize(100);
	rScoreText.setString(std::to_string(rScore));
	rScoreText.setFillColor(Color::White);
	rScoreText.setPosition(WIDTH - 100, 50);

	MainText.setFont(font);
	MainText.setCharacterSize(100);
	MainText.setString("Just Another Pong Game");
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
			}
			else if (temp == -1) {
				break;
			}
		}
		if (gameInterfaceFlag) {
			temp = update_state(dt, newBall, lPaddle, rPaddle, obstacle, powerBall);
			render_frame(newBall, lPaddle, rPaddle, obstacle, powerBall);
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
				resetGame(lPaddle, rPaddle);
			}
			if (temp == 1) {
				currentIndex = 0;
				endInterfaceFlag = 0;
				mainMenuFlag = 1;
				MainText.setString("Just Another Pong Game");
				MainText.setPosition(150, 150);
				resetGame(lPaddle, rPaddle);
			}
			if (temp == 2) {
				break;
			}
		}

		window.display();
	}

	return 0;
}

int update_state(float dt, ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall)
{
	Vector2f bp = newBall.getBall().getPosition();
	Vector2f lpp = lPaddle.getPaddle().getPosition();
	Vector2f rpp = rPaddle.getPaddle().getPosition();
	Vector2f op = obstacle.getPaddle().getPosition();
	Vector2f pp = powerBall.getBall().getPosition();
	Vector2f temp;
	float len;
	int kick = 0;
	static int flag = 0;
	//score
	if (bp.x <= 0) {
		if (flag == 0) {
			rScore++;
			rScoreText.setString(std::to_string(rScore));
			sSound.play();
			flag = 1;
		}
		if (sSound.getStatus() != SoundSource::Playing && flag == 1) {
			flag = 0;
			reset(newBall, lPaddle, rPaddle, obstacle, powerBall);
			if (lScore == 5 || rScore == 5) {
				return 1;
			}
		}
		return 0;
	}
	if (bp.x >= WIDTH) {
		if (flag == 0) {
			lScore++;
			lScoreText.setString(std::to_string(lScore));
			sSound.play();
			flag = 1;
		}
		if (sSound.getStatus() != SoundSource::Playing && flag == 1) {
			flag = 0;
			reset(newBall, lPaddle, rPaddle, obstacle, powerBall);
			if (lScore == 5 || rScore == 5) {
				return 1;
			}
		}
		return 0;
	}
	//collision
	//lp
	if ((bp.x >= lpp.x && bp.x < lpp.x + lPaddle.getSize().x && (abs(lpp.y - bp.y) < radius || abs(bp.y - lpp.y - lPaddle.getSize().y) < radius))
		|| (bp.y > lpp.y && bp.y < lpp.y + lPaddle.getSize().y && bp.x - lpp.x - lPaddle.getSize().x < radius)
		|| length(Vector2f(bp.x - lpp.x - lPaddle.getSize().x, bp.y - lpp.y)) < radius
		|| length(Vector2f(bp.x - lpp.x - lPaddle.getSize().x, bp.y - lpp.y - lPaddle.getSize().y)) < radius) {
		temp = bp - Vector2f(lpp.x - lPaddle.getSize().y / 2, lpp.y + lPaddle.getSize().y / 2);
		len = length(temp);
		newBall.SetVel(Vector2f(temp.x*tempVel / len, temp.y*tempVel / len));
		tempVel += augPerHit;
		kicker = -1;
		kick = 1;
	}
	//rp
	if ((bp.x > rpp.x && bp.x <= rpp.x + rPaddle.getSize().x && (abs(rpp.y - bp.y) < radius || abs(bp.y - rpp.y - rPaddle.getSize().y) < radius))
		|| (bp.y > rpp.y && bp.y < rpp.y + rPaddle.getSize().y && rpp.x - bp.x < radius)
		|| length(Vector2f(bp.x - rpp.x, bp.y - rpp.y)) < radius
		|| length(Vector2f(bp.x - rpp.x, bp.y - rpp.y - rPaddle.getSize().y)) < radius) {
		temp = bp - Vector2f(rpp.x + rPaddle.getSize().x + rPaddle.getSize().y / 2, rpp.y + rPaddle.getSize().y / 2);
		len = length(temp);
		newBall.SetVel(Vector2f(temp.x*tempVel / len, temp.y*tempVel / len));
		tempVel += augPerHit;
		kicker = 1;
		kick = 1;
	}
	//board
	if (bp.y <= radius) {
		newBall.SetVel(Vector2f(newBall.getVel().x, abs(newBall.getVel().y)));
		kick = 1;
	}
	if (bp.y >= HEIGHT - radius) {
		newBall.SetVel(Vector2f(newBall.getVel().x, -abs(newBall.getVel().y)));
		kick = 1;
	}

	//press Up
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		rPaddle.UpdatePosition(dt, 0);
	}
	//press Down
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		rPaddle.UpdatePosition(dt, 1);
	}
	//AI
	if (gameMode == AI || gameMode == OBSTACLE || gameMode == POWERUPS) {
		if (bp.y < lpp.y + lPaddle.getSize().y / 2) {
			lPaddle.UpdatePosition(dt, 0);
		}
		if (bp.y > lpp.y + lPaddle.getSize().y / 2) {
			lPaddle.UpdatePosition(dt, 1);
		}
	}
	//PvP
	if (gameMode == PvP) {
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			lPaddle.UpdatePosition(dt, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::S)) {
			lPaddle.UpdatePosition(dt, 1);
		}
	}
	//Obstacle Mode
	if (gameMode == OBSTACLE) {
		obstacle.UpdatePosition(dt, 0);
		if (bp.x >= op.x && bp.x <= op.x + paddleSize.x) {
			if (op.y - bp.y < radius && op.y >= bp.y) {
				newBall.SetVel(Vector2f(newBall.getVel().x, -abs(newBall.getVel().y)));
				kick = 1;
			}
			if (bp.y - op.y - paddleSize.y < radius && bp.y > op.y + paddleSize.y) {
				newBall.SetVel(Vector2f(newBall.getVel().x, abs(newBall.getVel().y)));
				kick = 1;
			}
			//tempVel += augPerHit;
		}
		else if (bp.y >= op.y && bp.y <= op.y + paddleSize.y) {
			if (bp.x - op.x - paddleSize.x <= radius && bp.x >= op.x + paddleSize.x) {
				newBall.SetVel(Vector2f(abs(newBall.getVel().x), newBall.getVel().y));
				kick = 1;
			}
			if (op.x - bp.x <= radius && op.x >= bp.x) {
				newBall.SetVel(Vector2f(-abs(newBall.getVel().x), newBall.getVel().y));
				kick = 1;
			}
			//tempVel += augPerHit;
		}
		else if (length(Vector2f(bp.x - op.x, bp.y - op.y)) < radius && op.x > bp.x && op.y > bp.y) {
			newBall.SetVel(Vector2f(-abs(newBall.getVel().x), -abs(newBall.getVel().y)));
			kick = 1;
		}
		else if (length(Vector2f(bp.x - op.x - paddleSize.x, bp.y - op.y)) < radius && bp.x > op.x + paddleSize.x && op.y > bp.y) {
			newBall.SetVel(Vector2f(abs(newBall.getVel().x), -abs(newBall.getVel().y)));
			kick = 1;
		}
		else if (length(Vector2f(bp.x - op.x, bp.y - op.y - paddleSize.y)) < radius && op.x > bp.x && bp.y > op.y + paddleSize.y) {
			newBall.SetVel(Vector2f(-abs(newBall.getVel().x), abs(newBall.getVel().y)));
			kick = 1;
		}
		else if (length(Vector2f(bp.x - op.x - paddleSize.x, bp.y - op.y - paddleSize.y)) < radius
			&& bp.x > op.x + paddleSize.x && bp.y > op.y + paddleSize.y) {
			newBall.SetVel(Vector2f(abs(newBall.getVel().x), abs(newBall.getVel().y)));
			kick = 1;
		}
	}
	//PowerUps Mode
	if (gameMode == POWERUPS) {
		if (length(bp - pp) <= radius + 15) {
			pSound.play();
			if (kicker == -1) {
				lPaddle.powerUp(powerBall.getType());
			}
			else if (kicker == 1) {
				rPaddle.powerUp(powerBall.getType());
			}
			powerBall.reset(0);
		}
	}
	if (kick == 1) {
		sound.play();
		kick = 0;
	}
	newBall.UpdatePosition(dt);

	return 0;
}

void render_frame(ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall)
{
	window.clear();
	window.draw(background);
	window.draw(lScoreText);
	window.draw(rScoreText);
	window.draw(newBall.getBall());
	window.draw(lPaddle.getPaddle());
	window.draw(rPaddle.getPaddle());
	if (gameMode == OBSTACLE) {
		window.draw(obstacle.getPaddle());
	}
	if (gameMode == POWERUPS) {
		window.draw(powerBall.getBall());
	}
}

int reset(ball &newBall, paddle &lPaddle, paddle &rPaddle, paddle &obstacle, ball &powerBall) {
	tempVel = initVel;
	newBall.reset(lScore + rScore);
	lPaddle.reset();
	rPaddle.reset();
	if (gameMode == OBSTACLE) {
		obstacle.reset();
	}
	if (gameMode == POWERUPS) {
		powerBall.reset(0);
	}
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
		currentIndex = (currentIndex - 1 + EXIT + 1) % (EXIT + 1);
		upFlag = 0;
	}
	if (!Keyboard::isKeyPressed(Keyboard::Down) && downFlag == 1) {
		mainMenuChioce[currentIndex].setColor(Color::White);
		currentIndex = (currentIndex + 1 + EXIT + 1) % (EXIT + 1);
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
		switch (currentIndex)
		{
		case AI:
			gameMode = AI;
			break;
		case PvP:
			gameMode = PvP;
			break;
		case OBSTACLE:
			gameMode = OBSTACLE;
			break;
		case POWERUPS:
			gameMode = POWERUPS;
			break;
		case EXIT:
			return -1;
			break;
		}
		return 1;
	}
	return 0;
}

int renderMainMenuFrame(Text mainMenuChioce[], RectangleShape currentChoiceRect) {
	window.clear();
	window.draw(background);
	window.draw(MainText);
	for (int i = 0; i < EXIT + 1; i++) {
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

int resetGame(paddle &lPaddle, paddle &rPaddle) {
	lScore = 0;
	rScore = 0;
	lScoreText.setString("0");
	rScoreText.setString("0");
	if (gameMode == POWERUPS) {
		resetPaddle(lPaddle, rPaddle);
	}
	return 0;
}
