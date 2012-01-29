#include "spritepop.h"
#include "app.h"

SpritePop::SpritePop(const char *sprite, u32 ms, f32 x, f32 y, bool pulse)
	: sptPop()
	, movPop()
	, tlPop()
	, kfPop()
{
	sptPop.Load(sprite);
	sptPop.SetPriority(100000);
	sptPop.SetBlending(BlendModulateAlpha);
	//pScene->Add(&sptPop);

	u32 targetFrames = 30 * ms / 1000;

	if (pulse)
	{
		kfPop[0].bTween = TRUE;
		kfPop[0].iColorA = 0;
		kfPop[0].fEasing = 1.f;

		kfPop[1].bTween = TRUE;
		kfPop[1].iColorA = 255;
		kfPop[1].fEasing = -1.f;

		kfPop[2].bTween = TRUE;
		kfPop[2].iColorA = 0;
		kfPop[2].iEvent = Keyframe::KEYFRAME_EVENT_JUMPTOFRAME;
		kfPop[2].iFrameToJump = 0;
		kfPop[2].fEasing = 1.f;

		kfPop[3].pstrName = "hiding";
		kfPop[3].bTween = TRUE;
		kfPop[3].iColorA = 255;

		kfPop[4].bTween = TRUE;
		kfPop[4].iColorA = 0;
		kfPop[4].iEvent = Keyframe::KEYFRAME_EVENT_STOP;

		tlPop.AddKeyframe(0, &kfPop[0]);
		tlPop.AddKeyframe(targetFrames / 2.f, &kfPop[1]);
		tlPop.AddKeyframe(targetFrames, &kfPop[2]);
		tlPop.AddKeyframe(targetFrames + 1 + static_cast<f32>(targetFrames) * .1f, &kfPop[3]);
		tlPop.AddKeyframe(targetFrames + 1 + static_cast<f32>(targetFrames) * .1f + 10, &kfPop[4]);
		tlPop.SetObject(&sptPop);
	}
	else
	{
		kfPop[0].bTween = TRUE;
		kfPop[0].iColorA = 0;

		kfPop[1].bTween = TRUE;
		kfPop[1].iColorA = 255;
		kfPop[1].iEvent = Keyframe::KEYFRAME_EVENT_STOP;

		kfPop[2].pstrName = "hiding";
		kfPop[2].bTween = TRUE;
		kfPop[2].iColorA = 255;

		kfPop[3].bTween = TRUE;
		kfPop[3].iColorA = 0;
		kfPop[3].iEvent = Keyframe::KEYFRAME_EVENT_STOP;

		tlPop.AddKeyframe(0, &kfPop[0]);
		tlPop.AddKeyframe(targetFrames, &kfPop[1]);
		tlPop.AddKeyframe(targetFrames + 1, &kfPop[2]);
		tlPop.AddKeyframe(targetFrames + 1 + static_cast<f32>(targetFrames) * .5f, &kfPop[3]);
		tlPop.SetObject(&sptPop);
	}

	movPop.AddTimeline(&tlPop);
	SetPosition(x, y);
	pScene->Add(&movPop);
}

SpritePop::~SpritePop()
{
	pScene->Remove(&movPop);
}

void SpritePop::Show()
{
	tlPop.GotoAndPlay(static_cast<u32>(0));
}

void SpritePop::Hide()
{
	if (tlPop.GetCurrentKeyframe() == &kfPop[0] || tlPop.GetCurrentKeyframe() == &kfPop[1])
		tlPop.GotoAndPlay("hiding");
}

bool SpritePop::IsVisible() const
{
	return tlPop.GetCurrentKeyframe() != &kfPop[3] && tlPop.GetCurrentKeyframe() != &kfPop[4];
}

void SpritePop::SetPosition(f32 x, f32 y)
{
	movPop.SetPosition(x - sptPop.GetWidth() / 2.f, y - sptPop.GetHeight() / 2.f);
}

void SpritePop::AddPosition(f32 x, f32 y)
{
	movPop.AddPosition(x, y);
}
