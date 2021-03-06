#include "stdafx.h"
#include "GameMap.h"
#include "../GameInfo.h"
#include "../ZipReader.h"
#include "../DemoResources.h"
#include "../game/GameColors.h"
#include <dingus/utils/Random.h>
#include <dingus/resource/TextureCreator.h>

#include <boost/crc.hpp>

// --------------------------------------------------------------------------

CGameMap::SPoint::SPoint( ePointType atype, int ax, int ay, int d )
: type(atype), x(ax), y(ay), data(d)
{
	switch( type ) {
	case PT_AZN:
		colorMain = gColors.ptAZN.main.c;
		colorTone = gColors.ptAZN.tone.c;
		break;
	case PT_HOSHIMI:
		colorMain = gColors.ptHoshimi.main.c;
		colorTone = gColors.ptHoshimi.tone.c;
		break;
	case PT_INJECTION:
		colorMain = gColors.team[data].main.c;
		colorTone = gColors.team[data].tone.c;
		break;
	}
}

// --------------------------------------------------------------------------

class CGameMapTextureCreator : public CFixedTextureCreator {
public:
	CGameMapTextureCreator() : CFixedTextureCreator( 256, 256, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED )
	{
	}

	virtual IDirect3DTexture9* createTexture() {
		IDirect3DTexture9* tex = CFixedTextureCreator::createTexture();

		// create the offscreen surface
		HRESULT hr;
		const CGameMap& gmap = CGameInfo::getInstance().getGameMap();
		CD3DDevice& dx = CD3DDevice::getInstance();
		IDirect3DSurface9* srcSurf = 0;
		hr = dx.getDevice().CreateOffscreenPlainSurface( gmap.getCellsX(), gmap.getCellsY(), D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &srcSurf, NULL );
		assert( SUCCEEDED(hr) );

		D3DLOCKED_RECT lr;
		srcSurf->LockRect( &lr, NULL, D3DLOCK_DISCARD );
		const char* linePtr = (const char*)lr.pBits;
		for( int y = 0; y < gmap.getCellsY(); ++y ) {
			D3DCOLOR* p = (D3DCOLOR*)linePtr;
			for( int x = 0; x < gmap.getCellsX(); ++x ) {
				const CGameMap::SCell& cell = gmap.getCell(x,y);
				switch( cell.type ) {
				case CELL_BLOOD1:	*p = 0xFF400000; break;
				case CELL_BLOOD2:	*p = 0xFF300000; break;
				case CELL_BLOOD3:	*p = 0xFF200000; break;
				default:			*p = 0x00000000; break;
				}
				++p;
			}
			linePtr += lr.Pitch;
		}
		srcSurf->UnlockRect();

		// now, filter this into the texture
		IDirect3DSurface9* dstSurf = 0;
		hr = tex->GetSurfaceLevel( 0, &dstSurf );
		assert( SUCCEEDED(hr) );
		hr = D3DXLoadSurfaceFromSurface( dstSurf, NULL, NULL, srcSurf, NULL, NULL, D3DX_FILTER_BOX, 0 );
		dstSurf->Release();
		srcSurf->Release();

		D3DXFilterTexture( tex, NULL, 0, D3DX_FILTER_BOX );
		return tex;
	}
};


// --------------------------------------------------------------------------

CGameMap::CGameMap()
:	mCellsX(0), mCellsY(0), mCells(NULL), mCRC(0)
{
}

CGameMap::~CGameMap()
{
	safeDeleteArray( mCells );
}


std::string CGameMap::initialize( const std::string& fileName, const std::string& mapName )
{
	mName = mapName;
	assert( !mCells );

	//
	// open zip file
	
	unzFile zipFile = unzOpen( fileName.c_str() );
	if( zipFile == NULL )
		return "Game map file '" + fileName + "' not found or is corrupt";

	//
	// read level bitmap

	int bmpSize;
	char* bmpFile = gReadFileInZip( zipFile, "map.bmp", bmpSize );
	if( !bmpFile ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - no cells map";
	}

	// compute CRC of the bitmap
	mCRC = boost::crc<32,0xFFFFFFFF,0,0,false,false>( bmpFile, bmpSize );


	HRESULT hr;
	D3DXIMAGE_INFO bitmapInfo;
	hr = D3DXGetImageInfoFromFileInMemory( bmpFile, bmpSize, &bitmapInfo );
	if( FAILED(hr) ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - incorrect cells map format";
	}
	assert( bitmapInfo.Width > 10 && bitmapInfo.Height > 10 );
	assert( bitmapInfo.Width * bitmapInfo.Height <= 256*256 );
	if( bitmapInfo.Width < 10 || bitmapInfo.Height < 10 ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - map is too small";
	}
	if( bitmapInfo.Width * bitmapInfo.Height > 256*256 ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - map is too large";
	}

	mCellsX = bitmapInfo.Width;
	mCellsY = bitmapInfo.Height;
	mCells = new SCell[ mCellsX * mCellsY ];

	CD3DDevice& dx = CD3DDevice::getInstance();
	IDirect3DSurface9* surface = 0;
	hr = dx.getDevice().CreateOffscreenPlainSurface( bitmapInfo.Width, bitmapInfo.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &surface, NULL );
	assert( SUCCEEDED(hr) );

	hr = D3DXLoadSurfaceFromFileInMemory( surface, NULL, NULL, bmpFile, bmpSize, NULL, D3DX_DEFAULT, 0, NULL );
	if( FAILED(hr) ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - incorrect cells map format";
	}

	D3DLOCKED_RECT lr;
	surface->LockRect( &lr, NULL, D3DLOCK_READONLY );
	const char* linePtr = (const char*)lr.pBits;
	for( int y = 0; y < mCellsY; ++y ) {
		const D3DCOLOR* p = (const D3DCOLOR*)linePtr;
		for( int x = 0; x < mCellsX; ++x ) {
			SCell& cell = mCells[y*mCellsX+x];
			switch( *p ) {
			case 0xFFff0000:	cell.type = CELL_BLOOD1; break;
			case 0xFF00ff00:	cell.type = CELL_BLOOD2; break;
			case 0xFF0000ff:	cell.type = CELL_BLOOD3; break;
			case 0xFFc0c0c0:	cell.type = CELL_PERF; break;
			default:			cell.type = CELL_BONE; break;
			}
			cell.height = MIN_CELL_HEIGHT;
			cell.nearBone = true;
			++p;
		}
		linePtr += lr.Pitch;
	}
	surface->UnlockRect();
	surface->Release();

	delete[] bmpFile;

	//
	// read special points

	int ptsSize;
	char* ptsFile = gReadFileInZip( zipFile, "entities.txt", ptsSize );
	if( !ptsFile ) {
		unzClose( zipFile );
		return "Error in game map file '" + fileName + "' - no entities info";
	}

	const char* tokens = "\n\r";
	const char* pline = strtok( (char*)gSkipUTFStart( ptsFile, ptsSize ), tokens );
	do {
		if( !pline )
			break;
		int etype, eposx, eposy;
		int fread = sscanf( pline, "%i:%i:%i", &etype, &eposx, &eposy );
		if( fread != 3 )
			break;
		mPoints.push_back( SPoint(ePointType(etype), eposx, eposy ) );
	} while( pline = strtok( NULL, tokens ) );

	delete[] ptsFile;

	//
	// close zip file

	unzClose( zipFile );

	//
	// calc cell heights

	calcCellHeights();

	//
	// register level texture
	CSharedTextureBundle::getInstance().registerTexture( RID_TEX_LEVEL, *new CGameMapTextureCreator() );

	return ""; // all ok!
}



void CGameMap::calcCellHeights()
{
	CRandomFast rnd;

	const int NEIGHBOUR_X[8] = { -1,  0,  1, -1, 1, -1, 0, 1 };
	const int NEIGHBOUR_Y[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

	int idx = 0;
	for( int y = 0; y < mCellsY; ++y ) {
		for( int x = 0; x < mCellsX; ++x, ++idx ) {
			if( !isBlood( mCells[idx].type ) )
				continue;

			// see if any of 8-neighbors is a bone
			bool nearBone = false;
			if( x < 1 || x >= mCellsX-1 || y < 1 || y >= mCellsY-1 )
				nearBone = true;
			else {
				for( int i = 0; i < 8; ++i ) {
					int nidx = idx + NEIGHBOUR_Y[i]*mCellsX + NEIGHBOUR_X[i];
					if( !isCellBlood(nidx) ) {
						nearBone = true;
						break;
					}
				}
			}
			mCells[idx].nearBone = nearBone;

			const int SAMPLING_R = 20;
			int emptyR = 0;
			for( int r = 1; r < SAMPLING_R; ++r ) {
				const int samples = round( 2*D3DX_PI*r * 0.25f );
				float dphi = 2*D3DX_PI / samples;
				float phi = gRandom.getFloat( dphi );
				bool empty = true;
				for( int i = 0; i < samples; ++i, phi += dphi ) {
					int px = round( x + cosf(phi) * r );
					int py = round( y + sinf(phi) * r );
					if( px < 0 || py < 0 || px >= mCellsX || py >= mCellsY ) {
						empty = false;
						break;
					}
					if( !isCellBlood(py*mCellsX+px) ) {
						empty = false;
						break;
					}
				}
				if( !empty )
					break;
				++emptyR;
			}
			mCells[idx].height += (1-powf(1-(float)emptyR/SAMPLING_R, 2)) * SAMPLING_R * 0.2f;
		}
	}
}
