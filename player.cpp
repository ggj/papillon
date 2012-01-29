#include "player.h"
#include "maplayermetadata.h"
#include "assets.h"
#include "maplayermosaic.h"

Player::Player(b2World *world, Map* map[2])
	: Actor(world)
    , eAnimation(ANIM_EGG)
    , state(EGG)
	, fElapsedDeathTime(0.0f)
	, fElapsedInvertTime(0.0f)
	, fElapsedGravityTime(0.0f)
	, fElapsedWeightTime(0.0f)
	, fElapsedShieldTime(0.0f)
	, bReducedGravity(FALSE)
	, bLockControls(FALSE)
	, iLives(5)
    , stateTimer(2.0f)
    , currentTimer(0.0f)
    , speed(0.01f)
    , moving(FALSE)
{    
    sptActor.Load(SPT_PAPILLON);
    sptActor.SetAnimation("egg");

    pMap1 = map[0];
	pMap2 = map[1];

    this->SetX(0.5f);
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
}

Player::~Player()
{
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
        moving = FALSE;
    }
}

void Player::StopThrust()
{
	if (bLockControls)
		return;

	vController.y = 0;
    if (GetState() == MAGGOT)
    {
        moving = TRUE;
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
    currentTimer += dt;

    if (currentTimer > stateTimer)
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
            sptActor.SetAnimation("maggot");
            this->StopAllSounds();
            sfxThrust.Play();
            break;
        }
        case ANIM_STOPPED_MAGGOT:
        {
            sptActor.SetAnimation("maggot");
            sptActor.SetCurrentFrame(0);
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
	bLockControls = TRUE;
	fElapsedDeathTime = 0.0f;
	vController.x = vController.y = 0;
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
            stateTimer = 2.0f;
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
            speed = 0.02f;
            stateTimer = 15.0f;
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
