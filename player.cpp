#include "player.h"
#include "maplayermetadata.h"
#include "assets.h"
#include "maplayermosaic.h"

Player::Player(b2World *world, Map* map[2])
	: Actor(world)
	, eAnimation(ANIM_EGG)
	, fElapsedDeathTime(0.0f)
	, fElapsedInvertTime(0.0f)
	, fElapsedGravityTime(0.0f)
	, fElapsedWeightTime(0.0f)
	, fElapsedShieldTime(0.0f)
	, bReducedGravity(FALSE)
	, bLockControls(FALSE)
	, iLives(5)
	, state(EGG)
	, speed(0.01f)
	, stateTimer(2.0f)
	, currentTimer(0.0f)
	, moving(FALSE)
	, started(FALSE)
	, hited(FALSE)
	, bHowtoFly(TRUE)
	, bHowtoSlow(TRUE)
	, bHowtoFlyEnd(TRUE)
	, bHowtoSlowEnd(TRUE)
	, sptHowtoFly(NULL)
	, sptHowtoSlow(NULL)
{
	sptActor.Load(SPT_PAPILLON);
	sptActor.SetAnimation("egg");

	pMap1 = map[0];
	pMap2 = map[1];

	this->SetX(0.333f);
	this->SetY(0.9f);
	this->SetWidth(sptActor.GetWidth() * pScreen->GetWidth() - PLAYER_BORDER * 2.0f);
	this->SetHeight(sptActor.GetHeight() * pScreen->GetHeight() - PLAYER_BORDER * 2.0f);

	sptActor.SetBlending(Seed::BlendModulate);
	sptActor.SetPriority(699);
	pScene->Add(&sptActor);

	CreateDinamycBody(GetX(), GetY(), GetWidth(), GetHeight(), COLLISION_PLAYER, COLLISION_GROUND);

//	sfxThrust.Load(nTeam == TeamPanda ? SFX_WINGS : SFX_JETPACK);
//	sfxThrust.SetVolume(nTeam == TeamPanda ? 0.3f : 0.05f);
//	pSoundSystem->Add(&sfxThrust);

//	sfxThrustFail.Load(nTeam == TeamPanda ? SFX_WINGS_FAIL : SFX_JETPACK_FAIL);
//	sfxThrustFail.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxThrustFail);

//	sfxDeath.Load(nTeam == TeamPanda ? SFX_BEAR: SFX_MAIA);
//	sfxDeath.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxDeath);

//	sfxHit.Load(SFX_HIT);
//	sfxHit.SetVolume(0.1f);
//	pSoundSystem->Add(&sfxHit);

//	sfxSteps.Load(SFX_STEPS);
//	sfxSteps.SetVolume(0.15f);
//	pSoundSystem->Add(&sfxSteps);

	arFollowers.Truncate();

	for (u32 i = 0; i < PLAYER_FOLLOWERS; i++)
	{
		arFollowers.Add();
		arFollowers[i].Load(SPT_PAPILLON);
		arFollowers[i].SetAnimation("butterfly");
		arFollowers[i].SetVisible(FALSE);
		pScene->Add(&arFollowers[i]);
	}

	this->SpawnFollower();
}

Player::~Player()
{
	for (u32 i = 0; i < arFollowers.Size(); i++)
	{
		pScene->Remove(&arFollowers[i]);
	}
	arFollowers.Truncate();

	pScene->Remove(&sptActor);
	sptActor.Unload();

	pSoundSystem->Remove(&sfxThrust);
	sfxThrust.Unload();

	pSoundSystem->Remove(&sfxThrustFail);
	sfxThrustFail.Unload();

	pSoundSystem->Remove(&sfxDeath);
	sfxDeath.Unload();

	pSoundSystem->Remove(&sfxHit);
	sfxHit.Unload();

	pSoundSystem->Remove(&sfxSteps);
	sfxSteps.Unload();

	if (sptHowtoFly)
		Delete(sptHowtoFly);
	sptHowtoFly = NULL;

	if (sptHowtoSlow)
		Delete(sptHowtoSlow);
	sptHowtoSlow = NULL;
}

void Player::StartThrust()
{
	if (bLockControls)
		return;

	if (GetState() == BUTTERFLY)
	{
		moving = TRUE;
		vController.y = 1;
	}
	if (GetState() == MAGGOT)
	{
		SetAnimation(ANIM_STOPPED_MAGGOT);
	}
}

void Player::StopThrust()
{
	if (bLockControls)
		return;

	vController.y = 0;
	if (GetState() == MAGGOT)
	{
		SetAnimation(ANIM_MOVING_MAGGOT);
	}
}

void Player::StartRight()
{
	if (bLockControls)
		return;

	//vController.x = 1;
	vController.x = 0;
}

void Player::StopRight()
{
	if (bLockControls)
		return;

	vController.x = 0;
}

void Player::StartLeft()
{
	if (bLockControls)
		return;

	//vController.x = -1;
	vController.x = 0;
}

void Player::StopLeft()
{
	if (bLockControls)
		return;

	vController.x = 0;
}

void Player::Update(f32 dt, MapLayerMetadata *collision, Player *player)
{
	if (hited && GetState() == MAGGOT)
	{
		if (sptActor.GetCurrentFrame() >= sptActor.GetNumFrames() - 1)
		{
			hited = FALSE;
			sptActor.SetAnimation("maggot");
			SetAnimation(ANIM_MOVING_MAGGOT);
		}
	}

	currentTimer += dt;

	if (currentTimer > stateTimer && started)
	{
		if (GetState() < BUTTERFLY)
		{
			SetState(PlayerState(GetState() + 1));
		}
		else
		{
			SetState(EGG);
		}
	}

	if (bInvertAxis)
	{
		fElapsedInvertTime += dt;
		if (fElapsedInvertTime > 5.0f)
		{
			bInvertAxis = FALSE;
		}
	}

	if (bReducedGravity)
	{
		fElapsedGravityTime += dt;
		if (fElapsedGravityTime > 5.0f)
		{
			fGravityModifier = 0.0f;
			bReducedGravity = FALSE;
		}
	}

	Actor::Update(dt);

	if (IsMoving())
	{
		f32 sizeX = pScreen->GetWidth() * PIXEL2METER;
		f32 sizeY = pScreen->GetHeight() * PIXEL2METER;

		f32 b2x = GetX() * pScreen->GetWidth() / sizeX;
		//f32 b2y = -(GetY() * pScreen->GetHeight() / sizeY);

		f32 b2w = GetWidth() * pScreen->GetWidth() / sizeX;
		//f32 b2h = GetHeight() * pScreen->GetHeight() / sizeY;

		f32 x = b2x + b2w * 0.5f;
		f32 y = body->GetPosition().y;

		/*body->SetTransform(b2Vec2(x, y), body->GetAngle());
		body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));*/

		float s = dt * 0.01f;

		if (sptHowtoFly)
		{
			sptHowtoFly->AddPosition(-dt * .1f, 0.0f);
			if (bHowtoFlyEnd)
			{
				sptHowtoFly->Hide();
				bHowtoFlyEnd = FALSE;
			}
		}
		if (sptHowtoSlow)
		{
			sptHowtoSlow->AddPosition(-dt * .1f, 0.0f);
			if (bHowtoSlowEnd)
			{
				sptHowtoSlow->Hide();
				bHowtoSlowEnd = FALSE;
			}
		}

		for (int i = 0; i < pMap1->GetLayerCount(); i++)
		{
			pMap2->GetLayerAt(i)->SetPriority(pMap1->GetLayerAt(i)->GetPriority());
			f32 size = pMap1->GetLayerAt(i)->GetWidth();

			if (pMap1->GetLayerAt(i)->GetPosition().x <= 0.0f)
			{
				pMap1->GetLayerAt(i)->AddPosition(Point2f(-s, 0.0f));
				pMap2->GetLayerAt(i)->SetPosition(Point2f(pMap1->GetLayerAt(i)->GetPosition().x + size, pMap1->GetLayerAt(i)->GetPosition().y));

				if (pMap2->GetLayerAt(i)->GetPosition().x <= 0.0f)
				{
					pMap1->GetLayerAt(i)->SetPosition(Point2f(1.0f, 0.0f));
				}
			}
			else
			{
				pMap2->GetLayerAt(i)->AddPosition(Point2f(-s, 0.0f));
				pMap1->GetLayerAt(i)->SetPosition(Point2f(pMap2->GetLayerAt(i)->GetPosition().x + size, pMap2->GetLayerAt(i)->GetPosition().y));
			}

			s += dt * speed;
		}
	}

	this->ResolveCollision(collision, player);
	this->ResolveAnimation();

	/*if (eAnimation == DEATH)
	{
		fElapsedDeathTime += dt;
		if (fElapsedDeathTime >= RESPAWN_TIME)
		{
					sptActor.SetPosition(-1.0f, -1.0f);
					body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					this->SetAnimation(FALL);
					bLockControls = FALSE;
		}
	}*/

	for (u32 i = 0; i < arFollowers.Size(); i++)
	{
		f32 dx = pRand->Get(-0.3f, 0.2f) * dt;
		f32 dy = pRand->Get(-0.3f, 0.3f) * dt;
		arFollowers[i].AddPosition(dx, dy);

		if (arFollowers[i].GetX() < (0.0f - arFollowers[i].GetWidth()))
			arFollowers[i].SetVisible(FALSE);
		else if (arFollowers[i].GetX() > 1.0f)
			arFollowers[i].SetVisible(FALSE);

		if (arFollowers[i].GetY() < (0.0f - arFollowers[i].GetHeight()))
			arFollowers[i].SetVisible(FALSE);
		else if (arFollowers[i].GetY() > 0.7f)
			arFollowers[i].AddY(-dy);
	}
}

void Player::SpawnFollower()
{
	u32 len = arFollowers.Size();

	for (u32 i = 0; i < len; i++)
	{
		if (!arFollowers[i].IsVisible())
		{
			arFollowers[i].SetCurrentFrame(pRand->Get(0, arFollowers[i].GetNumFrames() - 1));
			arFollowers[i].Play();
			//arFollowers[i].SetPosition(pRand->Get(0.0f, 0.93f), pRand->Get(0.0f, 0.7f));
			//arFollowers[i].SetPosition(sptActor.GetX(), sptActor.GetY());
			arFollowers[i].SetPosition(0.5f, 0.5f);
			arFollowers[i].SetBlending(Seed::BlendModulate);
			arFollowers[i].SetColor(pRand->Get(0.5f, 0.1f), pRand->Get(0.5f, 0.1f), pRand->Get(0.5f, 0.1f), 1.0f);
			arFollowers[i].SetPriority(698 + pRand->Get(0u, 2u));
			arFollowers[i].SetVisible(TRUE);
			return;
		}
	}
}

void Player::ResolveAnimation()
{
	/*else if (eAnimation == ANIM_COCOON)
	{
		if (!sptActor.IsFinished())
			return;
	}*/

	if (GetState() == BUTTERFLY)
	{
		if (vController.y > 0)
		{
			this->SetAnimation(ANIM_MOVING_BUTTERFLY);
		}
		else
		{
			if (vDir.y > 0)
				this->SetAnimation(ANIM_MOVING_BUTTERFLY);
			else  if (vDir.y == 0.0f)
				this->SetAnimation(ANIM_STOPPED_BUTTERFLY);
		}
	}
}

void Player::ResolveCollision(MapLayerMetadata *collision, Player *player)
{
	CollisionDataArray col;

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
	if (player)
	{
		player->SetPosition(player->sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, player->sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
	}

	Rect4f playerCollision;

	if (player && CheckHit(player->GetBoundingBox(), playerCollision))
	{
		if (this->GetBoundingBox().y < player->GetBoundingBox().y)
		{
			//this->SetAnimation(ANIM_HIT_MAGGOT);
			//player->Hit(this);
		}
	}

	if (body)
	{
		fLastX = sptActor.GetX();
		fLastY = sptActor.GetY();

		b2Vec2 pos = body->GetPosition();
		sptActor.SetPosition(pos.x * PIXEL2METER - sptActor.GetWidth() * 0.5f, pos.y * PIXEL2METER * -1.0f - sptActor.GetHeight() * 0.5f);

		vDir.x = sptActor.GetX() - fLastX;
		vDir.y = sptActor.GetY() - fLastY;
		vDir.Normalize();
	}

	this->SetPosition(sptActor.GetX() * pScreen->GetWidth() + PLAYER_BORDER, sptActor.GetY() * pScreen->GetHeight() + PLAYER_BORDER);
}

void Player::StopAllSounds()
{
	sfxThrust.Stop();
	sfxThrustFail.Stop();
	sfxDeath.Stop();
	sfxHit.Stop();
	sfxSteps.Stop();
}

void Player::SetAnimation(AnimationState animation)
{
	if (eAnimation == animation)
		return;

	eAnimation = animation;

	switch (eAnimation)
	{
		case ANIM_EGG:
		{
			sptActor.SetAnimation("egg");
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_MOVING_MAGGOT:
		{
			if (bHowtoSlow)
			{
				sptHowtoSlow = New(SpritePop(SPT_SPACESTOP, 7000, 0.35f, 0.7f));
				sptHowtoSlow->Show();
				bHowtoSlow = FALSE;
			}
			moving = TRUE;
			sptActor.Play();
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_STOPPED_MAGGOT:
		{
			moving = FALSE;
			sptActor.Stop();
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_HIT_MAGGOT:
		{
			sptActor.SetAnimation("hitmaggot");
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_COCOON:
		{
			sptActor.SetAnimation("cocoon");
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_MOVING_BUTTERFLY:
		{
			moving = TRUE;
			sptActor.SetAnimation("butterfly");
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_STOPPED_BUTTERFLY:
		{
			if (bHowtoFly)
			{
				sptHowtoFly = New(SpritePop(SPT_SPACEFLY, 10000, 0.5f, 0.7f));
				sptHowtoFly->Show();
				bHowtoFly = FALSE;
			}

			moving = FALSE;
			sptActor.SetAnimation("butterfly");
			sptActor.SetCurrentFrame(0);
			sptActor.Stop();
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
		case ANIM_HIT_BUTTERFLY:
		{
			sptActor.SetAnimation( "butterfly");
			this->StopAllSounds();
			sfxThrust.Play();
			break;
		}
	}

	body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	body->SetAngularVelocity(0.0f);
	CreateDinamycBody(GetX(), GetY(), GetWidth(), GetHeight(), COLLISION_PLAYER, COLLISION_GROUND);
}

void Player::Hit(Player *player)
{
	hited = TRUE;
	switch (GetState())
	{
		case MAGGOT:
		{
			SetAnimation(ANIM_HIT_MAGGOT);
			break;
		}
		case BUTTERFLY:
		{
			hited = FALSE;
			//SetAnimation(ANIM_HIT_BUTTERFLY);
			StopThrust();
			break;
		}
	}
}

void Player::IncLife()
{
	if (iLives < PLAYER_LIFES * 2)
		iLives++;
}

void Player::DecLife()
{
	if (iLives > 0)
		iLives--;
}

u32 Player::GetLife()
{
	return iLives;
}

void  Player::SetState(PlayerState state)
{
	this->state = state;
	currentTimer = 0.0f;
	sptActor.SetColor(255u, 255u, 255u, 255u);

	switch (state)
	{
		case EGG:
		{
			moving = FALSE;
			StopThrust();
			stateTimer = 2.0f;
			SetAnimation(ANIM_EGG);
			break;
		}
		case MAGGOT:
		{
			moving = TRUE;
			speed = 0.01f;
			stateTimer = 30.0f;
			sptActor.SetAnimation("maggot");
			SetAnimation(ANIM_MOVING_MAGGOT);
			break;
		}
		case COCOON:
		{
			moving = FALSE;
			stateTimer = 2.0f;
			SetAnimation(ANIM_COCOON);
			break;
		}
		case BUTTERFLY:
		{
			moving = FALSE;
			speed = 0.04f;
			stateTimer = 30.0f;
			SetAnimation(ANIM_STOPPED_BUTTERFLY);
			sptActor.SetColor(pRand->Get(0u, 255u), pRand->Get(0u, 255u), pRand->Get(0u, 255u), 255u);
			break;
		}
	}
}

PlayerState Player::GetState()
{
	return state;
}

BOOL Player::IsMoving()
{
	return moving;
}

f32 Player::GetSpeed()
{
	return speed;
}

void Player::Start()
{
	started = true;
}

BOOL Player::IsPlaying()
{
	return started;
}

u32 Player::GetLayerCount()
{
	return pMap1->GetLayerCount();
}
