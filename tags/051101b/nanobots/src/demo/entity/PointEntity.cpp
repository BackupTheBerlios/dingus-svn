#include "stdafx.h"
#include "PointEntity.h"
#include "../GameInfo.h"
#include "../game/GameColors.h"
#include "../game/GameDesc.h"
#include <dingus/utils/Random.h>


static const char* POINT_TYPENAMES[PTCOUNT] = {
	"PointAZN",
	"PointHoshimi",
	"PointInjection",
	"PointMission",
};

static const char* POINT_DECOR_NAMES[DECOR_POINT_TYPE_COUNT] = {
	"Decor1", "Decor1", "Decor1",
	"Decor2", "Decor2", "Decor2",
	"Decor3a", "Decor3b", "Decor4",
	"Decor5", "Decor6", "Decor7",
};



CPointEntity::CPointEntity( const CGameMap::SPoint& point )
:	CMeshEntity( point.type==PT_DECORATIVE ? POINT_DECOR_NAMES[ point.data ] : POINT_TYPENAMES[ point.type ], 1 )
,	mTimeOffset( gRandom.getFloat(0,10) )
,	mPoint( &point )
{
	const CGameMap& gmap = CGameInfo::getInstance().getGameDesc().getMap();

	D3DXMatrixRotationY( &mWorldMat, gRandom.getFloat(0,D3DX_PI*2) );
	mWorldMat.getOrigin().x =  point.x;
	mWorldMat.getOrigin().z = -point.y;

	switch( point.type ) {
	case PT_AZN:
	case PT_HOSHIMI:
		mAlphaBase = 0.40f;	mAlphaAmpl = 0.10f;
		mWorldMat.getOrigin().y = -gmap.getCell(point.x,point.y).height;
		mColor = &(D3DXCOLOR(point.colorTone).r);
		break;
	case PT_INJECTION:
		mAlphaBase = 0.20f;	mAlphaAmpl = 0.20f;
		mWorldMat.getOrigin().y = gmap.getCell(point.x,point.y).height-2.0f;
		mColor = &(D3DXCOLOR(point.colorMain).r);
		break;
	case PT_OBJECTIVE:
		mAlphaBase = 0.40f;	mAlphaAmpl = 0.10f;
		mWorldMat.getOrigin().y = 0.0f;
		mColor = &(D3DXCOLOR(point.colorTone).r);
		break;
	case PT_DECORATIVE:
		mWorldMat.getOrigin().y = -gmap.getCell(point.x,point.y).height;
		mAlphaBase = 0.50f;
		mAlphaAmpl = 0.10f;
		mColor.set(1,1,1,1);
	}
	
	if( getRenderMeshes( RM_NORMAL, 0 ) ) {
		(*getRenderMeshes( RM_NORMAL, 0 ))[0]->getParams().addVector4Ref( "vColor", mColor );
	}
}


CPointEntity::~CPointEntity()
{
}

void CPointEntity::update()
{
	// if we're decorative and they are turned off - set color to transparent and return
	if( (mPoint->type == PT_DECORATIVE) && !gAppSettings.drawDecoratives ) {
		mColor.w = 0.0f;
		return;
	}

	// if we're Hoshimi - go through all needles and see if any stands on me
	// TBD
	/*
	if( mPoint->type == PT_HOSHIMI ) {
		float t = CGameInfo::getInstance().getTime();
		const CGameReplay& replay = CGameInfo::getInstance().getReplay();
		int n = replay.getEntityCount();
		for( int i = 0; i < n; ++i ) {
			const CReplayEntity& e = replay.getEntity(i);
			if( e.getType() != ENTITY_NEEDLE )
				continue;
			if( !e.isAlive( t ) )
				continue;
			const CReplayEntity::SState& s = e.getTurnState(0);
			if( s.posx == mPoint->x && s.posy == mPoint->y ) {
				mColor.w = 0.0f;
				return;
			}
		}
	}
	*/
	double t = CSystemTimer::getInstance().getTimeS() - mTimeOffset;
	mColor.w = mAlphaBase + cosf( t + sinf( t * 2.1f ) ) * mAlphaAmpl;
	if( mPoint->type == PT_OBJECTIVE ) {
		SVector3 o = mWorldMat.getOrigin();
		D3DXMatrixRotationY( &mWorldMat, t * 0.5f );
		mWorldMat.getOrigin() = o;
	}
}

void CPointEntity::renderPoint( eRenderMode renderMode )
{
	if( mColor.w == 0.0f )
		return;
	render( renderMode, 0, false );
}
