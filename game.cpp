#include "main.h"
#include "app.h"
#include "game.h"
#include "map.h"
#include "maplayertiled.h"
#include "maplayermetadata.h"
#include "collisionobject.h"
#include "assets.h"
#include "hud.h"
#include "modifier.h"
#include <time.h>

#define BTN_LEFT_P1		1
#define BTN_RIGHT_P1	2
#define BTN_UP_P1		3
#define BTN_LEFT_P2		4
#define BTN_RIGHT_P2	5
#define BTN_UP_P2		6

Game::Game()
	: pHud(NULL)
	, pMap(NULL)
	, pCollision(NULL)
	, pPlayer1(NULL)
	, pPlayerKeyboard1(NULL)
	, pPlayerJoystick1(NULL)
	, fElapsedTime(0.0f)
	, fPowerupSpawnTimer(0.0f)
	, iFinishType(0)
	, iJoystickDpad1(0)
	, bIsFinished(FALSE)
	, bPaused(FALSE)
{
	pHud = New(Hud());

	if (nPlatform == TouchDevice)
	{
		btnLeftP1.Initialize(BTN_LEFT_P1);
		btnLeftP1.SetSprite(SPT_TREE);
		btnLeftP1.SetPosition(0.02f, 0.9f);
		btnLeftP1.SetPriority(PRIORITY_TOUCH);
		btnLeftP1.SetSpriteColor(1.0f, 1.0f, 1.0f, 0.5f);
		btnLeftP1.SetSpriteBlending(Seed::BlendModulate);
		btnLeftP1.AddListener(this);
		pGuiManager->Add(&btnLeftP1);
		pScene->Add(&btnLeftP1);
	}

//BOX2D ***********************************
	b2Vec2 gravity(0.0f, -10.0f);
	world = New(b2World(gravity, true));
// ****************************************

	pInput->AddKeyboardListener(this);
	pInput->AddJoystickListener(this);

	pMap = New(Map(world));
	pMap->SetWidth(1.0f);
	pMap->SetHeight(1.0f);
	pMap->SetPriority(5);

	u32 map = clock() % 5;
	pMap->Load(MAP_TESTE);
	pMap->SetPriority(PRIORITY_BG + 1);

	MapLayerTiled *bg = pMap->GetLayerAt(0)->AsTiled();
	if (bg)
	{
		bg->SetTileSet(SPT_TREE);
	}
	pCollision = pMap->GetLayerAt(1)->AsMetadata();
	pScene->Add(pMap);

        pPlayer1 = New(Player(world, pMap));
	pHud->SetFirstPlayerLifes(pPlayer1->GetLife());

//	sfxGong.Load(SFX_START_FIGHT);
//	sfxGong.SetVolume(0.5f);
//	pSoundSystem->Add(&sfxGong);
//	sfxGong.Play();

	f32 size = 4.0f;

        borderTop = New(CollisionObject(world));
	borderTop->SetPosition(0.0f, size);
	borderTop->SetWidth(static_cast<f32>(pScreen->GetWidth()));
	borderTop->SetHeight(size);
	borderTop->CreateStaticBody(borderTop->GetX(), borderTop->GetY(), borderTop->GetWidth(), borderTop->GetHeight());
        pScene->Add(borderTop);

	borderBottom = New(CollisionObject(world));
	borderBottom->SetPosition(0.0f, static_cast<f32>(pScreen->GetHeight()));
        borderBottom->SetWidth(static_cast<f32>(pScreen->GetWidth() * 3));
        borderBottom->SetHeight(20.0f);
	borderBottom->CreateStaticBody(borderBottom->GetX(), borderBottom->GetY(), borderBottom->GetWidth(), borderBottom->GetHeight());
	pScene->Add(borderBottom);

        borderLeft = New(CollisionObject(world));
	borderLeft->SetPosition(0.0f, static_cast<f32>(pScreen->GetHeight()));
	borderLeft->SetWidth(size);
	borderLeft->SetHeight(static_cast<f32>(pScreen->GetHeight()));
	borderLeft->CreateStaticBody(borderLeft->GetX(), borderLeft->GetY(), borderLeft->GetWidth(), borderLeft->GetHeight());
	pScene->Add(borderLeft);

        /*borderRight = New(CollisionObject(world));
	borderRight->SetPosition(static_cast<f32>(pScreen->GetWidth()) - size, static_cast<f32>(pScreen->GetHeight()));
	borderRight->SetWidth(size);
	borderRight->SetHeight(static_cast<f32>(pScreen->GetHeight()));
	borderRight->CreateStaticBody(borderRight->GetX(), borderRight->GetY(), borderRight->GetWidth(), borderRight->GetHeight());
        pScene->Add(borderRight);*/

        pPlayerKeyboard1 = pPlayer1;
        pPlayerJoystick1 = pPlayer1;
}

Game::~Game()
{
	pScene->Reset();

	pInput->RemoveJoystickListener(this);
	pInput->RemoveKeyboardListener(this);

	Delete(pMap);
	Delete(pPlayer1);

	Delete(pHud);
	pHud = NULL;

	Delete(borderTop);
	Delete(borderBottom);
	Delete(borderLeft);
	Delete(borderRight);

	Delete(world);
	world = NULL;
}

void Game::Update(f32 dt)
{
	if (bPaused)
		return;

	fElapsedTime += dt;

	world->Step(dt, 6, 2);
	world->ClearForces();

	pPlayer1->Update(dt, pCollision, NULL);//pPlayer2);
	for (u32 i = 0; i < arPowerUps.Size(); i++)
	{
		arPowerUps[i]->Update(dt, pCollision);
	}

	this->CheckModifierCollision();

	pHud->SetFirstPlayerLifes(pPlayer1->GetLife());

	if (pPlayer1->GetLife() <= 0)
	{
		iFinishType = 0;
		bPaused = TRUE;
	}
	else if (fElapsedTime > GLOBAL_PEACE_TIME)
	{
		iFinishType = 2;
		bPaused = TRUE;
	}

	if (arPowerUps.Size() < 5)
	{
		fPowerupSpawnTimer += dt;
		if (fPowerupSpawnTimer > 5.0f)
		{
			fPowerupSpawnTimer -= 5.0f;

			this->SpawnModifier();
		}
	}

//	if (bPaused)
//		this->ShowEndingScreen();
}

BOOL Game::IsFinished() const
{
	return bIsFinished;
}

u32 Game::GetFinishType() const
{
	return iFinishType;
}

void Game::CheckModifierCollision()
{
	Rect4f overlap;

	BOOL collide = TRUE;
	for (s32 i = arPowerUps.Size() - 1; i >= 0; i--)
	{
		collide = pPlayer1->CheckHit(arPowerUps[i]->GetBoundingBox(), overlap);
		if (collide)
		{
			this->ApplyModifier(arPowerUps[i]->GetType());
			Delete(arPowerUps[i]);
			arPowerUps.Del(i);

			continue;
		}
	}
}

void Game::ApplyModifier(ModifierType type)
{
}

void Game::SpawnModifier()
{
	u32 iType = pRand->Get((u32)4);
	ModifierType eType;
	switch (iType)
	{
		default:
		case 0:
		{
			eType = ModNone;
		}
		break;
	}

	Modifier *mod = New(Modifier(world, eType));
	mod->GetSprite().SetPosition(pRand->Get(1.0f), pRand->Get(1.0f));
	mod->CreateDinamycBody(mod->GetSprite().GetX(), mod->GetSprite().GetY(), mod->GetSprite().GetWidth(), mod->GetSprite().GetHeight(), COLLISION_OBJECT, COLLISION_GROUND | COLLISION_OBJECT);

	arPowerUps.Add(mod);
}

void Game::OnInputKeyboardPress(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (pPlayerKeyboard1)
	{
		switch (k.GetValue())
		{
			case Seed::KeyW:
			case 'w':
			{
				pPlayerKeyboard1->StartThrust();
			}
			break;

			case Seed::KeyA:
			case 'a':
			{
				pPlayerKeyboard1->StartLeft();
			}
			break;

			case Seed::KeyD:
			case 'd':
			{
				pPlayerKeyboard1->StartRight();
			}
			break;
		}
	}

	if (k.GetValue() == Seed::KeyReturn)
	{
		if (bPaused)
		{
			bIsFinished = TRUE;
		}
	}
}

void Game::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	if (pPlayerKeyboard1)
	{
		switch (k.GetValue())
		{
			case Seed::KeyW:
			case 'w':
			{
				pPlayerKeyboard1->StopThrust();
			}
			break;

			case Seed::KeyA:
			case 'a':
			{
				pPlayerKeyboard1->StopLeft();
			}
			break;

			case Seed::KeyD:
			case 'd':
			{
				pPlayerKeyboard1->StopRight();
			}
			break;
		}
	}
}

void Game::OnInputJoystickButtonPress(const EventInputJoystick *ev) const
{
}

void Game::OnInputJoystickButtonRelease(const EventInputJoystick *ev) const
{
}

void Game::OnInputJoystickDPadMove(const EventInputJoystick *ev) const
{
	if (pPlayerJoystick1)
	{
		u32 oldState = iJoystickDpad1;
		iJoystickDpad1 = ev->GetValue();

		u32 diff = oldState ^ iJoystickDpad1;

		u32 up = diff & 1;
		if (up)
		{
			u32 oldUp = oldState & 1;

			if (!oldUp)
				pPlayerJoystick1->StartThrust();
			else
				pPlayerJoystick1->StopThrust();
		}

		u32 left = diff & 8;
		if (left)
		{
			u32 oldLeft = oldState & 8;
			if (!oldLeft)
				pPlayerJoystick1->StartLeft();
			else
				pPlayerJoystick1->StopLeft();
		}

		u32 right = diff & 2;
		if (right)
		{
			u32 oldRight = oldState & 2;
			if (!oldRight)
				pPlayerJoystick1->StartRight();
			else
				pPlayerJoystick1->StopRight();
		}
	}
}

void Game::OnInputJoystickAxisMove(const EventInputJoystick *ev) const
{
	if (pPlayerJoystick1)
	{
	}
}

void Game::OnWidgetPress(const EventWidget *ev)
{
	switch (ev->GetSender()->GetId())
	{
		case BTN_LEFT_P1:
		{
			pPlayer1->StartLeft();
		}
		break;

		case BTN_RIGHT_P1:
		{
			pPlayer1->StartRight();
		}
		break;

		case BTN_UP_P1:
		{
			pPlayer1->StartThrust();
		}
		break;
	}
}

void Game::OnWidgetReleaseOut(const EventWidget *ev)
{
	this->OnWidgetRelease(ev);
}

void Game::OnWidgetRelease(const EventWidget *ev)
{
	switch (ev->GetSender()->GetId())
	{
		case BTN_LEFT_P1:
		{
			pPlayer1->StopLeft();
		}
		break;

		case BTN_RIGHT_P1:
		{
			pPlayer1->StopRight();
		}
		break;

		case BTN_UP_P1:
		{
			pPlayer1->StopThrust();
		}
		break;
	}
}
