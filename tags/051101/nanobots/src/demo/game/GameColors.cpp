#include "stdafx.h"
#include "GameColors.h"

SColors		gColors;


void gInitColors()
{
	assert( G_MAX_PLAYERS == 3 );

	const float CM = 0.50f;
	const float CT = 0.75f;

	// computer team - white
	gColors.team[0].main.set( D3DXCOLOR(1,1,1,1) );
	gColors.team[0].tone.set( D3DXCOLOR(1,1,1,1) );
	// first team - greenish
	gColors.team[1].main.set( D3DXCOLOR(CM,1,CM,1) );
	gColors.team[1].tone.set( D3DXCOLOR(CT,1,CT,1) );
	// second team - blueish
	gColors.team[2].main.set( D3DXCOLOR(CM,CM,1,1) );
	gColors.team[2].tone.set( D3DXCOLOR(CT,CT,1,1) );
	// azn - cyan
	gColors.ptAZN.main.set( 0xFF30a0a0 );
	gColors.ptAZN.tone.set( 0xFF30a0a0 );
	// hoshimi - red
	gColors.ptHoshimi.main.set( 0xFFa03030 );
	gColors.ptHoshimi.tone.set( 0xFFa03030 );
	// objective - yellow
	gColors.ptObjective.main.set( 0xFFf0f030 );
	gColors.ptObjective.tone.set( 0xFFa0a030 );

	// texture colors
	gColors.texture[CCOLOR_BLOOD].set( 202.0f/255.0f, 70.0f/255.0f, 55.0f/255.0f );
	gColors.texture[CCOLOR_BONE].set( 202.0f/255.0f, 202.0f/255.0f, 202.0f/255.0f );
	gColors.texture[CCOLOR_NEURON].set( 65.0f/255.0f, 144.0f/255.0f, 202.0f/255.0f );
	
	// minimap colors
	gColors.minimap[CCOLOR_BLOOD][CELL_BLOOD1] = 0xFF501000;
	gColors.minimap[CCOLOR_BLOOD][CELL_BLOOD2] = 0xFF501000;
	gColors.minimap[CCOLOR_BLOOD][CELL_BLOOD3] = 0xFF501000;
	gColors.minimap[CCOLOR_BLOOD][CELL_BONE] = 0L;
	gColors.minimap[CCOLOR_BONE][CELL_BLOOD1] = 0xFF505050;
	gColors.minimap[CCOLOR_BONE][CELL_BLOOD2] = 0xFF505050;
	gColors.minimap[CCOLOR_BONE][CELL_BLOOD3] = 0xFF505050;
	gColors.minimap[CCOLOR_BONE][CELL_BONE] = 0L;
	gColors.minimap[CCOLOR_NEURON][CELL_BLOOD1] = 0xFF103050;
	gColors.minimap[CCOLOR_NEURON][CELL_BLOOD2] = 0xFF103050;
	gColors.minimap[CCOLOR_NEURON][CELL_BLOOD3] = 0xFF103050;
	gColors.minimap[CCOLOR_NEURON][CELL_BONE] = 0L;
}
