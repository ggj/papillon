#include "hud.h"
#include "assets.h"
#include "app.h"

Hud::Hud()
{
//	sptLifesP1.Load(SPT_TREE);
//	sptLifesP1.SetPosition(0, 0);
//	sptLifesP1.SetPriority(100000);
//	sptLifesP1.SetAnimation((u32)0);
//	//pScene->Add(&sptLifesP1);
}

Hud::~Hud()
{
	pScene->Remove(&sptLifesP1);
	sptLifesP1.Unload();
}

void Hud::SetFirstPlayerLifes(u32 value)
{
	if (value)
		sptLifesP1.SetCurrentFrame(value - 1);
}
