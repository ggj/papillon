#ifndef __SPRITEPOP_H__
#define __SPRITEPOP_H__

#include "main.h"

class SpritePop
{
	public:
		SpritePop(const char *sprite, u32 ms, f32 x, f32 y, bool pulse = FALSE);
		~SpritePop();

		void Show();
		void Hide();
		bool IsVisible() const;

		void SetPosition(f32 x, f32 y);
		void AddPosition(f32 x, f32 y);


	private:
			Sprite sptPop;
			Movie movPop;
			Timeline tlPop;
			Keyframe kfPop[5];
};

#endif // __SPRITEPOP_H__
