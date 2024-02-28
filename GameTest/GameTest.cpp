//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include <sstream>
//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------

struct Platform
{
	float posx;
	float posy;
	int width;
	int height;
} platform;

enum Collision {
	NONE,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

struct World
{
	std::vector<Platform> platforms;

	void AddPlatform(float posx, float posy, int width, int height) {
		Platform platform;
		platform.posx = posx;
		platform.posy = posy;
		platform.width = width;
		platform.height = height;
		platforms.push_back(platform);
	}

	void RemovePlatform() {
		platforms.erase(platforms.begin(), platforms.end());
	}

	void MovePlatformsX(float speed) {
		for (Platform& platform : platforms) {
			platform.posx += speed;
		}
	}

	void MovePlatformsY(float speed) {
		for (Platform& platform : platforms) {
			platform.posy += speed;
		}
	}

	Collision plateformCollision(float playerPosx, float playerPosy, float playerWidth, float playerHeight) {
		for (const auto& platform : platforms) {
			if (platform.posy <= playerPosy && platform.posy + platform.height >= playerPosy) {
				if (platform.posx < playerPosx) {
					if (playerPosx <= platform.posx + platform.width) {
						return LEFT;
					}
				}
				if (platform.posx > playerPosx) {
					if (playerPosx + playerWidth >= platform.posx) {
						return RIGHT;
					}
				}
			}
			if (platform.posx <= playerPosx && platform.posx + platform.width >= playerPosx) {
				if (platform.posy < playerPosy) {
					if (playerPosy <= platform.posy + platform.height) {
						return BOTTOM;
					}
				}
			}
		}
		return NONE;
	}

} World;
//------------------------------------------------------------------------

void DrawRectangle(int width, int height, float posx, float posy)
{
	float posx2 = posx + width;
	float posy2 = posy + height;
	App::DrawLine(posx, posy, posx2, posy);
	App::DrawLine(posx, posy, posx, posy2);
	App::DrawLine(posx2, posy, posx2, posy2);
	App::DrawLine(posx2, posy2, posx, posy2);
}

struct MainCharacter
{
	void InitCharacter() {
		posx = 500.0f;
		posy = 350.0f;
		testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
		width = testSprite->GetWidth();
		height = testSprite->GetHeight();
		testSprite->SetPosition(posx + (0.5 * width), posy + (0.5 * height));
		float speed = 1.0f / 15.0f;
		testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
		testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
		testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
		testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
		testSprite->SetScale(1.0f);
	}

	void DestroyCharacter() {
		delete testSprite;
	}

	float MoveCharacter() {
		return App::GetController().GetLeftThumbStickX() * 2.5;
	}

	void UpdateCharacter(float deltaTime) {
		Collision collision = World.plateformCollision(posx, posy, width, height);
		if (collision == BOTTOM) {
			gravity = 0.0f;
		}
		World.MovePlatformsY(gravity);
		gravity += GRAVITY_ACCELERATION;
		if (App::GetController().GetLeftThumbStickX() < 0.4f && collision != LEFT) {
			testSprite->SetAnimation(ANIM_LEFT);
			float speed = MoveCharacter();
			World.MovePlatformsX(-speed);
		}
		else if (App::GetController().GetLeftThumbStickX() > 0.6f && collision != RIGHT) {
			testSprite->SetAnimation(ANIM_RIGHT);
			float speed = MoveCharacter();
			World.MovePlatformsX(-speed);
		}
		else {
			testSprite->SetFrame(0);
		}
	}

	enum
	{
		ANIM_FORWARDS,
		ANIM_BACKWARDS,
		ANIM_LEFT,
		ANIM_RIGHT,
	};

	CSimpleSprite* testSprite;
	float gravity = 0.0f;
	const float GRAVITY_ACCELERATION = 0.5f;
	float sprintSpeed = 2.0f;
	float speed = 1.0f;
	float width;
	float height;
	float posx;
	float posy;
} MainCharacter;

void Init()
{
	World.AddPlatform(200.f, 100.f, 800, 100);
	World.AddPlatform(200.f, 150.f, 100, 600);
	World.AddPlatform(800.f, 200.f, 100, 500);

	MainCharacter.InitCharacter();
}

void Update(float deltaTime)
{
	MainCharacter.UpdateCharacter(deltaTime);
}

void Render()
{
	MainCharacter.testSprite->Draw();

	for (const auto& platform : World.platforms) {
		DrawRectangle(platform.width, platform.height, platform.posx, platform.posy);
	}

	float playerPosX, playerPosY;
	MainCharacter.testSprite->GetPosition(playerPosX, playerPosY);
	float playerWidth = MainCharacter.testSprite->GetWidth();
	float playerHeight = MainCharacter.testSprite->GetHeight();

	playerPosX -= playerWidth / 2;
	playerPosY -= playerHeight / 2;

	DrawRectangle(playerWidth, playerHeight, playerPosX, playerPosY);
}
void Shutdown()
{
	MainCharacter.DestroyCharacter();
	World.RemovePlatform();
}
