#ifndef __GAME_COLORS_H
#define __GAME_COLORS_H

#include <dingus/math/Vector4.h>


struct SColorEntry {
	void	set( D3DCOLOR color ) {
		D3DXCOLOR cc = color;
		v.set( cc.r, cc.g, cc.b, cc.a );
		c = color;
	}
	void	set( D3DXCOLOR color ) {
		v.set( color.r, color.g, color.b, color.a );
		c = color;
	}
	void	set( SVector4 color ) {
		D3DXCOLOR cc( color.x, color.y, color.z, color.w );
		v = color;
		c = cc;
	}
	SVector4	v;	// vector format
	D3DCOLOR	c;	// d3dcolor format
};


struct SColorSet {
	SColorEntry	main;	// clear color
	SColorEntry	tone;	// just small tint
};


struct SColors {
	SColorSet	team[G_MAX_PLAYER_COUNT];
	SColorSet	ptAZN;
	SColorSet	ptHoshimi;
};

extern SColors	gColors;


#endif
