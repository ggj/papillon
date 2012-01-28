#include "maplayermosaic.h"

MapLayerMosaic::MapLayerMosaic()
	: arObjects()
	, iObjects(0)
	, ptiTileSize(0, 0)
{
	arObjects.Truncate();
	cScene.SetParent(this);
}

MapLayerMosaic::~MapLayerMosaic()
{
	this->Reset();
}

void MapLayerMosaic::Reset()
{
	for (u32 i = 0; i < iObjects; i++)
	{
		Delete(arObjects[i]);
	}

	arObjects.Truncate();
	iObjects = 0;

	cScene.Reset();
}

void MapLayerMosaic::Initialize(Point2u tileSize, u32 count, const LayerMosaicHeader *data)
{
	ptiTileSize = tileSize;
	iObjects = count;

	for (u32 i = 0; i < iObjects; i++)
	{
		arObjects.Add(this->CreateSprite(&data[i]));
	}
}

ISprite *MapLayerMosaic::CreateSprite(const LayerMosaicHeader *entry)
{
	ISprite *obj = NULL;

	obj = New(SpriteInstantiable());
	obj->Load(_F(entry->iSpriteFileId));
	//obj->SetPosition(entry->fPosX * ptiTileSize.x, entry->fPosY * ptiTileSize.y);
	obj->SetPosition(entry->fPosX, entry->fPosY);
	obj->SetRotation(entry->fAngle);
	obj->SetScale(entry->fScaleX, entry->fScaleY);
	//obj->SetColor(entry->iColor);
	//obj->SetBlending(Seed::BlendModulate);
	// blending
	// collision

	cScene.Add(obj);

	return obj;
}

Point2i MapLayerMosaic::ViewAt(Point2i pos)
{
	cScene.SetPosition(-pos.x / static_cast<f32>(pScreen->GetWidth()), -pos.y / static_cast<f32>(pScreen->GetHeight()));

	return pos;
}

void MapLayerMosaic::Update(f32 dt)
{
	cScene.Update(dt);
}

void MapLayerMosaic::Render()
{
	for (u32 i = 0; i < iObjects; i++)
	{
		ISprite *obj = arObjects[i];
		obj->Render();
	}
}

void MapLayerMosaic::SetWrap(BOOL b)
{
	bWrap = b;
}

BOOL MapLayerMosaic::GetWrap() const
{
	return bWrap;
}

MapLayerMosaic *MapLayerMosaic::AsMosaic()
{
	return this;
}
