#ifndef __APP_H__
#define __APP_H__

#include "main.h"

class Menu;
class Game;
class Credits;
class Tutorial;
class Thanks;
class Options;
class Intro;

enum GameState
{
	GAME = 0,
	EXIT
};

class App : public IGameApp, public IEventSystemListener, public IEventInputKeyboardListener
{
	public:
		App();
		~App();

		virtual BOOL Initialize();
		virtual BOOL Update(f32 dt);
		virtual BOOL Reset();
		virtual BOOL Shutdown();
		virtual BOOL HasError() const;
		virtual void Destroy();
		virtual void Quit();

                // IEventSystemListener
		virtual void OnSystemShutdown(const EventSystem *ev);

		// IEventInputKeyboardListener
		virtual void OnInputKeyboardRelease(const EventInputKeyboard *ev);

	protected:
		virtual void OnEnterState();
		virtual void OnLeaveState();

	protected:
		SceneNode<1024> cScene;
		SceneNode<128> cSceneStatic;
		Viewport cViewport;
		Renderer cRenderer;

		GameState eState;
		Music musTheme1;

		Game *pGame;

	private:
		SEED_DISABLE_COPY(App);
};

#endif // __APP_H__

