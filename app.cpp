#include "app.h"
#include "assets.h"
#include "game.h"

App::App()
	: eState(GAME)
	, pGame(NULL)
{
	cConfig.SetRendererDeviceType(Seed::RendererDeviceOpenGL14);
	//cConfig.SetVideoMode(Seed::Video_iPhonePortrait);
	cConfig.SetVideoMode(Seed::Video_1024x768);
	//cConfig.SetVideoMode(Seed::Video_Netbook);
	cConfig.SetFrameRate(Seed::FrameRateLockAt30);
	//cConfig.SetPlatformSimulation(Seed::SimulateIOS3G);
	//cConfig.bDebugSprite = TRUE;
	cConfig.SetApplicationTitle("Papillons");
	cConfig.SetApplicationDescription("Papillons");
}

App::~App()
{

}

BOOL App::Initialize()
{
	IGameApp::Initialize();

	pSystem->AddListener(this);
	pInput->AddKeyboardListener(this);

	/* ------- Rendering Initialization ------- */
	cScene.SetPriority(0);
	cSceneStatic.SetPriority(50000);

	cRenderer.Add(&cScene);
	cRenderer.Add(&cSceneStatic);

	cViewport.SetRenderer(&cRenderer);
	pViewManager->Add(&cViewport);
	pRendererManager->Add(&cRenderer);

	pSceneManager->Add(&cScene);
	pSceneManager->Add(&cSceneStatic);

	pScene = &cScene;
	pSceneStatic = &cSceneStatic;

	musTheme1.Load(BGM_MUS1);
	musTheme1.SetVolume(1.0f);
	musTheme1.SetAutoUnload(FALSE);
	pSoundSystem->PlayMusic(&musTheme1, 500);

	pRand->Initialize(clock());

	this->OnEnterState();
	/* ------- Rendering Initialization ------- */

	return TRUE;
}

void App::Destroy()
{
	if (pGame)
		Delete(pGame);
}

void App::Quit()
{
	this->Destroy();
	pSystem->Shutdown();
}

BOOL App::Shutdown()
{
	pSoundSystem->StopMusic();
	musTheme1.Unload();

	this->Destroy();

	pInput->RemoveKeyboardListener(this);
	pSystem->RemoveListener(this);

	return IGameApp::Shutdown();
}

BOOL App::Reset()
{
	return TRUE;
}

BOOL App::HasError() const
{
	return FALSE;
}

void App::OnSystemShutdown(const EventSystem *ev)
{
	this->Destroy();
	pSystem->Shutdown();
}

void App::OnInputKeyboardRelease(const EventInputKeyboard *ev)
{
	Key k = ev->GetKey();

	switch (k.GetValue())
	{
		case Seed::KeyEscape:
		{
/*			if (eState != THANKS)
			{
				this->OnLeaveState();
				eState = THANKS;
				this->OnEnterState();
			}*/
			this->Quit();
		}
		break;

		default:
		break;
	}
}

BOOL App::Update(f32 dt)
{
	switch (eState)
	{
		case GAME:
		{
			pGame->Update(dt);
			if (pGame->IsFinished())
			{
				this->OnLeaveState();
				eState = GAME;
				this->OnEnterState();
			}
		}
		break;

		case EXIT:
		break;

		default:
		break;
	}

	return TRUE;
}

void App::OnEnterState()
{
	switch (eState)
	{
		case GAME:
		{
			pGame = New(Game());
		}
		break;
		case EXIT:
		break;

		default:
		break;
	}
}

void App::OnLeaveState()
{
	switch (eState)
	{
		case GAME:
		{
			Delete(pGame);
			pGame = NULL;
		}
		break;

		case EXIT:
		break;

		default:
		break;
	}
}
