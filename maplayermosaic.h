#ifndef __MAPLAYERMOSAIC_H__
#define __MAPLAYERMOSAIC_H__

#include "imaplayer.h"

#define MAX_SPRITESENTITIES 1024 * 4 //256

class MapLayerMosaic : public IMapLayer
{
	public:
		MapLayerMosaic();
		virtual ~MapLayerMosaic();

		virtual void Initialize(Point2u tileSize, u32 count, const LayerMosaicHeader *data);
		virtual void Reset();

		virtual Point2i ViewAt(Point2i pos);

		virtual void SetWrap(BOOL b);
		virtual BOOL GetWrap() const;

        virtual u32 GetObjectCount() const;
        virtual ISprite *GetObject(u32 at);

		/// Called to instantiate each metadata object.
		/*! For each object in a metadata layer, this method will be called to construct
			a customized metadata object. You should override this method to instantiate your
			own custom objects. When overriding this method, do not call the base method, as
			it will instantiate a new object and may leak.

			\param entry a struct of Layer Object basic information.
			\return An instance of a custom object from IMetadataObject type.
		*/
        virtual ISprite *CreateSprite(const LayerMosaicHeader *entry, u32 prio);

		// IMapLayer
		virtual MapLayerMosaic *AsMosaic();

        virtual void Add(ISceneObject *obj);
        virtual void Remove(ISceneObject *obj);
        virtual u32 Size() const;
        virtual ISceneObject *GetChildAt(u32 i);

		// ISceneObject
		virtual void Update(f32 delta);
		virtual void Render();

	private:
		SceneNode<MAX_SPRITESENTITIES> cScene;
		Array<ISprite *, MAX_SPRITESENTITIES> arObjects;

		u32 iObjects;
		Point2u ptiTileSize;

		s32 iPosX;
		s32 iPosY;
		s32 iOffsetX;
		s32 iOffsetY;

		u32 iMapWidth;
		u32 iMapHeight;
		u32 iViewWidth;
		u32 iViewHeight;

		BOOL bWrap;
};

#endif // __MAPLAYERMOSAIC_H__
