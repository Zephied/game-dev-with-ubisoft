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

//------------------------------------------------------------------------
// Eample data....
//------------------------------------------------------------------------
CSimpleSprite* testSprite;

struct Platform
{
	float posx;
	float posy;
	int width;
	int height;
} platform;

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
		platforms.clear();
	}

	bool Collision() {
		float charPosX, charPosY;
		testSprite->GetPosition(charPosX, charPosY);
		float charWidth = testSprite->GetWidth();
		float charHeight = testSprite->GetHeight();

		charPosX -= charWidth / 2;
		charPosY -= charHeight / 2;

		for (const auto& platform : platforms) {
			if (charPosX < platform.posx + platform.width &&
				charPosX + charWidth > platform.posx &&
				charPosY < platform.posy + platform.height &&
				charPosY + charHeight > platform.posy) {
				return true;
			}
		}

		return false;
	}

	bool Collision(float charPosX, float charPosY, float charWidth, float charHeight) {
		charPosX -= charWidth / 2;
		charPosY -= charHeight / 2;

		for (const auto& platform : platforms) {
			if (charPosX < platform.posx + platform.width &&
				charPosX + charWidth > platform.posx &&
				charPosY < platform.posy + platform.height &&
				charPosY + charHeight > platform.posy) {
				return true;
			}
		}

		return false;
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

} World;
//------------------------------------------------------------------------

struct MainCharacter
{
	void InitCharacter() {
		testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
		testSprite->SetPosition(800.0f, 400.0f);
		float speed = 1.0f / 15.0f;
		testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
		testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
		testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
		testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
		testSprite->SetScale(1.0f);

		width = testSprite->GetWidth();
		height = testSprite->GetHeight();
	}

	void DestroyCharacter() {
		delete testSprite;
	}

	void MoveCharacter() {
		testSprite->GetPosition(posx, posy);
		float speed = App::GetController().GetLeftThumbStickX() * 2.5;

		if (App::GetController().GetLeftThumbStickX() < 0.5f) {
			World.MovePlatformsX(-speed);
		}
		else if (App::GetController().GetLeftThumbStickX() > 0.5f) {
			World.MovePlatformsX(-speed);
		}
	}

	void Jump() {
		testSprite->GetPosition(posx, posy);

		if (App::GetController().CheckButton(XINPUT_GAMEPAD_A) && !isJumping) {
			isJumping = true;
			verticalVelocity = jumpStrength;
		}

		if (isJumping) {
			verticalVelocity -= gravity;
			float newPosy = posy + verticalVelocity;

			if (!World.Collision(posx, newPosy, width, height)) {
				World.MovePlatformsY(-verticalVelocity);
			}
			else {
				isJumping = false;
				verticalVelocity = 0.0f;
			}
		}

	}

	void UpdateCharacter(float deltaTime) {
		if (App::GetController().GetLeftThumbStickX() < 0.5f) {
			testSprite->SetAnimation(ANIM_LEFT);
			MoveCharacter();
		}
		else if (App::GetController().GetLeftThumbStickX() > 0.5f) {
			testSprite->SetAnimation(ANIM_RIGHT);
			MoveCharacter();
		}
		else {
			testSprite->SetFrame(0);
		}
		Jump();

		if (World.Collision(posx, posy, width, height)) {
			isJumping = false;
			verticalVelocity = 0.0f;
		}
		
		if (!World.Collision(posx, posy, width, height)) {
			posy -= gravity;
			verticalVelocity = 0.5f;
			testSprite->SetPosition(posx, posy);
		}
	}

	enum
	{
		ANIM_FORWARDS,
		ANIM_BACKWARDS,
		ANIM_LEFT,
		ANIM_RIGHT,
	};

	float sprintSpeed = 2.0f;
	float verticalVelocity = 10.0f;
	bool isJumping = false;
	const float jumpStrength = 5.0f;
	const float gravity = 0.5f;
	float width;
	float height;
	float posx;
	float posy;
} MainCharacter;

void Init()
{
	World.AddPlatform(200.f, 200.f, 800, 100);
	World.AddPlatform(200.f, 450.f, 100, 600);
	World.AddPlatform(800.f, 600.f, 100, 50);

	//------------------------------------------------------------------------
	// Example Sprite Code....
	//------------------------------------------------------------------------
	MainCharacter.InitCharacter();
}

void Update(float deltaTime)
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	MainCharacter.UpdateCharacter(deltaTime);
}

void DrawRectangle(int width, int height, float posx, float posy)
{
	float posx2 = posx + width;
	float posy2 = posy + height;
	App::DrawLine(posx, posy, posx2, posy);
	App::DrawLine(posx, posy, posx, posy2);
	App::DrawLine(posx2, posy, posx2, posy2);
	App::DrawLine(posx2, posy2, posx, posy2);

}

void Render()
{
	testSprite->Draw();

	for (const auto& platform : World.platforms) {
		DrawRectangle(platform.width, platform.height, platform.posx, platform.posy);
	}


	float playerPosX, playerPosY;
	testSprite->GetPosition(playerPosX, playerPosY);
	float playerWidth = testSprite->GetWidth();
	float playerHeight = testSprite->GetHeight();

	playerPosX -= playerWidth / 2;
	playerPosY -= playerHeight / 2;

	DrawRectangle(playerWidth, playerHeight, playerPosX, playerPosY);
}
void Shutdown()
{
	MainCharacter.DestroyCharacter();
	World.RemovePlatform();
}
