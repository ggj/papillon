#ifndef __MAIN_H__
#define __MAIN_H__

#include <Seed.h>
using namespace Seed;

class App;

extern ISceneNode *pScene;
extern ISceneNode *pSceneStatic;
extern App *pApp;

SEED_ENABLE_INSTANCING(Sprite)

enum eTeam
{
	TeamPanda,
	TeamMaya,

	MaxTeams
};

enum ePlatformType
{
	TouchDevice,
	PersonalComputer,
	GameConsole
};

extern ePlatformType nPlatform;

void seed_main(int argc, char **argv);

#endif // __MAIN_H__
