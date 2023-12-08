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
CSimpleSprite *testSprite;

//------------------------------------------------------------------------


struct Platform
{
	float posx = 200.f;
	float posy = 200.f;
	int width = 800;
	int height = 100;
} platform;

struct MainCharacter
{

	void InitCharacter() {
		testSprite = App::CreateSprite(".\\TestData\\Test.bmp", 8, 4);
		testSprite->SetPosition(800.0f, 800.0f);
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

	bool Collision() {
		float charPosX, charPosY;
		testSprite->GetPosition(charPosX, charPosY);
		float charWidth = testSprite->GetWidth();
		float charHeight = testSprite->GetHeight();

		charPosX -= charWidth / 2;
		charPosY -= charHeight / 2;

		if (charPosX < platform.posx + platform.width &&
			charPosX + charWidth > platform.posx &&
			charPosY < platform.posy + platform.height &&
			charPosY + charHeight > platform.posy) {
			return true;
		}

		return false;
	}

	void Sprint() {
		if (App::IsKeyPressed(VK_LSHIFT)) {
			sprintSpeed = 3.0f;
		}
		else {
			sprintSpeed = 1.0f;
		}
	}

	void Jump() {
		if (App::IsKeyPressed(VK_SPACE) && !isJumping) {
			verticalVelocity = jumpStrength;
			isJumping = true;
		}
	}

	float LeftMovement(float x, float y) {
		testSprite->SetAnimation(ANIM_LEFT);

		if (Collision() == false) {
			return x - 2 * sprintSpeed;
		}
		else {
			return x;
		}
	}

	float RightMovement(float x, float y) {
		testSprite->SetAnimation(ANIM_RIGHT);

		if (Collision() == false) {
			return x + 2 * sprintSpeed;
		}
		else {
			return x;
		}
	}

	void UpdateCharacter(float deltaTime) {
		float x;
		float y;

		Sprint();
		Jump();

		testSprite->GetPosition(x, y);
		testSprite->Update(deltaTime);

		float newY = y + verticalVelocity; // Calculate new y position
		verticalVelocity -= gravity;

		if (App::IsKeyPressed(VK_LEFT) || App::IsKeyPressed('Q')) {
			x = LeftMovement(x, y);
		}
		else if (App::IsKeyPressed(VK_RIGHT) || App::IsKeyPressed('D')) {
			x = RightMovement(x, y);
		}
		else {
			testSprite->SetFrame(0);
			if (!isJumping) {
				newY -= 3; // Calculate new y position
			}
		}

		testSprite->SetPosition(x, newY); // Temporarily set new position

		if (Collision()) {
			isJumping = false;
			verticalVelocity = 0.0f;
			testSprite->SetPosition(x, y); // If collision, revert to old position
		}
	}

	enum
	{
		ANIM_FORWARDS,
		ANIM_BACKWARDS,
		ANIM_LEFT,
		ANIM_RIGHT,
	};
	CSimpleSprite* testsprite;

	float sprintSpeed = 2.0f;
	float verticalVelocity = 0.0f;
	bool isJumping = false;
	const float jumpStrength = 15.0f;
	const float gravity = 0.5f;
	float width;
	float height;
	float posx;
	float posy;
}MainCharacter;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	//------------------------------------------------------------------------
	MainCharacter.InitCharacter();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
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
	DrawRectangle(800, 100, 200.f, 200.f);

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
}