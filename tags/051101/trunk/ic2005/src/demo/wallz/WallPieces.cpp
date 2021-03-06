#include "stdafx.h"
#include "WallPieces.h"
#include <dingus/math/Plane.h>
#include <dingus/math/MathUtils.h>
#include <dingus/renderer/RenderableBuffer.h>
//#include <dingus/utils/CpuTimer.h>
#include "../MeshEntity.h"


const float HALF_THICK = 0.02f;


extern int		gWallVertCount, gWallTriCount;



static inline float signedAngle2D( const SVector2& a, const SVector2& b )
{
	float sgn = a.x*b.y > b.x*a.y ? 1.0f : -1.0f;
	float dotp = a.dot( b ) / (a.length()*b.length());
	dotp = clamp( dotp, -1.0f, 1.0f );
	float ang = acosf( dotp );
	return sgn * ang;
}

// --------------------------------------------------------------------------


void CWallPiece3D::init( const CWall3D& w, int idx )
{
	const CWallPiece2D& piece = w.getWall2D().getPiece(idx);

	// construct VB/IB for this piece, with positions centered
	SVector2 pcenter = piece.getAABB().getCenter();

	static TIntVector	vertRemap;
	vertRemap.resize(0);
	vertRemap.resize( w.getWall2D().getVerts().size(), -1 );

	int i;

	int nidx = piece.getTriCount()*3;
	mIB.reserve( nidx * 2 + piece.getVertexCount()*6 );
	mVB.reserve( piece.getVertexCount()*4 );

	// construct one side
	for( i = 0; i < nidx; ++i ) {
		int oldIdx = piece.getIB()[i];
		int newIdx = vertRemap[oldIdx];
		if( newIdx < 0 ) {
			newIdx = mVB.size();
			vertRemap[oldIdx] = newIdx;

			SVector2 pos = w.getWall2D().getVerts()[oldIdx];
			pos -= pcenter;
			SVertexXyzNormal vtx;
			vtx.p.set( pos.x, pos.y, HALF_THICK );
			vtx.n.set( 0, 0, 1 );
			mVB.push_back( vtx );
		}
		mIB.push_back( newIdx );
	}
	for( i = 0; i < nidx/3; ++i ) {
		int iii = mIB[i*3+1];
		mIB[i*3+1] = mIB[i*3+2];
		mIB[i*3+2] = iii;
	}
	// construct another side
	int nverts = mVB.size();
	//int npolygon = piece.getPolygon().size();
	for( i = 0; i < nverts; ++i ) {
		SVertexXyzNormal vtx = mVB[i];
		vtx.p.z = -vtx.p.z;
		vtx.n.set( 0,0, -1 );
		mVB.push_back( vtx );
	}
	for( i = 0; i < nidx/3; ++i ) {
		int idx0 = mIB[i*3+0];
		int idx1 = mIB[i*3+1];
		int idx2 = mIB[i*3+2];
		mIB.push_back( idx0 + nverts );
		mIB.push_back( idx2 + nverts );
		mIB.push_back( idx1 + nverts );
	}
	// construct side caps. To conserve the geometry amount,
	// treat them as a single smoothing group.
	assert( nverts == piece.getVertexCount() );
	for( i = 0; i < nverts; ++i ) {
		int oldIdx0 = piece.getPolygon()[i];
		int oldIdx1 = piece.getPolygon()[(i+1)%nverts];
		int oldIdx2 = piece.getPolygon()[(i+nverts-1)%nverts];
		int idx0 = vertRemap[oldIdx0];
		int idx1 = vertRemap[oldIdx1];
		int idx2 = vertRemap[oldIdx2];
		assert( idx0 >= 0 && idx0 < nverts );
		assert( idx1 >= 0 && idx1 < nverts );
		assert( idx2 >= 0 && idx2 < nverts );
		SVertexXyzNormal v0 = mVB[idx0];
		SVertexXyzNormal v1 = v0;
		v1.p.z = -v1.p.z;

		const SVertexXyzNormal& vnear1 = mVB[idx1];
		const SVertexXyzNormal& vnear2 = mVB[idx2];

		SVector3 edge1 = vnear2.p - vnear1.p;
		SVector3 edge2 = v1.p - v0.p;
		SVector3 normal = edge1.cross( edge2 ).getNormalized();
		v0.n = v1.n = normal;

		mVB.push_back( v0 );
		mVB.push_back( v1 );
		int ibidx0 = nverts*2 + i*2 + 0;
		int ibidx1 = nverts*2 + i*2 + 1;
		int ibidx2 = nverts*2 + ((i+1)%nverts)*2 + 0;
		int ibidx3 = nverts*2 + ((i+1)%nverts)*2 + 1;
		mIB.push_back( ibidx0 );
		mIB.push_back( ibidx2 );
		mIB.push_back( ibidx1 );
		mIB.push_back( ibidx1 );
		mIB.push_back( ibidx2 );
		mIB.push_back( ibidx3 );
	}

	// construct initial mMatrix
	mMatrix.identify();
	mMatrix = w.getMatrix();
	mMatrix.getOrigin() += mMatrix.getAxisX() * pcenter.x;
	mMatrix.getOrigin() += mMatrix.getAxisY() * pcenter.y;

	mSize.set( piece.getAABB().getSize().x, piece.getAABB().getSize().y, HALF_THICK*2 );
}


void CWallPiece3D::preRender( int& vbcount, int& ibcount ) const
{
	vbcount = mVB.size();
	ibcount = mIB.size();
}

void CWallPiece3D::render( const SMatrix4x4& matrix, TPieceVertex* vb, unsigned short* ib, int baseIndex, int& vbcount, int& ibcount, BYTE alpha ) const
{
	int i;
	
	// VB
	const SVertexXyzNormal* srcVB = &mVB[0];
	vbcount = mVB.size();
	for( i = 0; i < vbcount; ++i ) {
		SVector3 p, n;
		D3DXVec3TransformCoord( &p, &srcVB->p, &matrix );
		D3DXVec3TransformNormal( &n, &srcVB->n, &matrix );
		vb->p = p;
		DWORD ncol = gVectorToColor( n );
		ncol |= (alpha<<24);
		vb->diffuse = ncol;
		++srcVB;
		++vb;
	}

	// IB
	const int* srcIB = &mIB[0];
	ibcount = mIB.size();
	for( i = 0; i < ibcount; ++i ) {
		int idx = *srcIB + baseIndex;
		assert( idx >= 0 && idx < 64000 );
		*ib = idx;
		++srcIB;
		++ib;
	}
}


// --------------------------------------------------------------------------


namespace polygon_merger {

	int				polygonCount;

	typedef std::set<int>	TIntSet;
	TIntSet			vertices;
	TIntSet			borderVertices;
	
	typedef std::map< int, TIntVector > TIntIntsMap;
	TIntIntsMap		vertexNexts;
	TIntIntsMap		vertexPrevs;

	TIntVector		vertexUseCount;
	TIntVector		vertexTraceID;

	enum eVertexType {
		VTYPE_NONE,		///< Not computed yet
		VTYPE_INTERIOR,	///< Completely interior
		VTYPE_SINGLE,	///< On the border, belongs to single polygon
		VTYPE_MULTI,	///< On the border, belongs to multiple polygons
		VTYPE_DONE,		///< Already fully processed (traced into result polygon)
	};
	TIntVector		vertexTypes;


	
	bool isVertexInterior( int idx )
	{
		assert( idx >= 0 && idx < vertexUseCount.size() );

		// vertex is interior if it's use count is >1, AND
		// it's all neighbors' use counts are >1, AND it has exactly "usecount"
		// different neighbors.
		int useCount = vertexUseCount[idx];
		assert( useCount >= 1 );
		if( useCount < 2 )
			return false;

		TIntIntsMap::const_iterator it;
		int i, n;

		it = vertexNexts.find( idx );
		assert( it != vertexNexts.end() );
		const TIntVector& vnext = it->second;
		n = vnext.size();
		assert( n == useCount );
		for( i = 0; i < n; ++i ) {
			if( vertexUseCount[vnext[i]] < 2 )
				return false;
		}

		it = vertexPrevs.find( idx );
		assert( it != vertexPrevs.end() );
		const TIntVector& vprev = it->second;
		n = vprev.size();
		assert( n == useCount );
		for( i = 0; i < n; ++i ) {
			int nidx = vprev[i];
			if( vertexUseCount[nidx] < 2 )
				return false;
			if( std::find( vnext.begin(), vnext.end(), nidx ) == vnext.end() )
				return false;
		}

		return true; // fall through: must be interior
	}



	void	markVertexTypes()
	{
		TIntSet::const_iterator vit, vitEnd = vertices.end();

		// first pass: mark all interior and single-border vertices
		for( vit = vertices.begin(); vit != vitEnd; ++vit ) {
			int idx = *vit;
			if( vertexUseCount[idx] == 1 ) {
				vertexTypes[idx] = VTYPE_SINGLE;
				borderVertices.insert( idx );
			} else if( isVertexInterior(idx) )
				vertexTypes[idx] = VTYPE_INTERIOR;
		}

		// now, the unmarked vertices are all shared and on border
		for( vit = vertices.begin(); vit != vitEnd; ++vit ) {
			int idx = *vit;
			if( vertexTypes[idx] == VTYPE_NONE ) {
				vertexTypes[idx] = VTYPE_MULTI;
				borderVertices.insert( idx );
			}
		}
	}

	int		getBorderIndex()
	{
		if( borderVertices.empty() )
			return -1;
		return *borderVertices.begin();
	}
	
	void	traceBorder( const TWallVertexVector& vb, TIntVector& polygon, TIntVector& ib )
	{
		static int traceID = 0;
		++traceID;

		int idx0 = getBorderIndex();
		assert( idx0 >= 0 && idx0 < vertexTypes.size() );
		
		ib.resize( 0 );
		polygon.resize( 0 );
		polygon.reserve( vertices.size()/2 );

		TIntVector localPolygon;
		localPolygon.reserve( 128 );
		TIntVector localIB;
		localIB.reserve( 128 );

		int idxPrev = idx0;
		int idx = idx0;
		int debugCounter = 0;

		int debugLoopCounter = 0;

		do {

			localIB.resize( 0 );

			bool willFormLoop;
			do{

				localPolygon.push_back( idx );
				borderVertices.erase( idx );
				vertexTraceID[idx] = traceID;
				assert( ++debugCounter <= vertices.size()*2 );

				// Next vertex is the neighbor of current, that is not interior
				// and that is not the previous one.
				// When there are many possible ones, trace based on angle.

				int idxNext = -1;

				SVector2 prevToCurr = vb[idx] - vb[idxPrev];
				if( prevToCurr.lengthSq() < 1.0e-6f )
					prevToCurr.set( -0.01f, -0.01f );

				TIntIntsMap::const_iterator it;
				it = vertexNexts.find( idx );
				assert( it != vertexNexts.end() );
				const TIntVector& vnext = it->second;
				int n = vnext.size();
				float bestAngle = 100.0f;
				for( int i = 0; i < n; ++i ) {
					int idx1 = vnext[i];
					if( idx1 != idxPrev && vertexTypes[idx1] != VTYPE_INTERIOR ) {
					//if( idx1 != idxPrev ) {
						SVector2 currToNext = vb[idx1] - vb[idx];
						float ang = signedAngle2D( prevToCurr, currToNext );
						if( ang < bestAngle ) {
							bestAngle = ang;
							idxNext = idx1;
						}
					}
				}
				assert( bestAngle > -4.0f && bestAngle < 4.0f );
				assert( idxNext >= 0 );

				willFormLoop = (vertexTraceID[idxNext] == traceID);

				// Optimization: if best angle is zero, then we're walking
				// in a straight line. Optimize out the current vertex.
				if( bestAngle == 0.0f && idx != idx0 && !willFormLoop ) {
					localPolygon.pop_back();
				}

				idxPrev = idx;
				idx = idxNext;

			} while( !willFormLoop );

			assert( localPolygon.size() >= 3 );
			//if( localPolygon.size() < 3 ) {
			//	return;
			//}

			assert( ++debugLoopCounter < vertices.size() );

			if( idx == idx0 ) {
				// The polygon is simple or we found the last loop.
				// Triangulate local and append to results.
				triangulator::process( vb, localPolygon, localIB );
				polygon.insert( polygon.end(), localPolygon.begin(), localPolygon.end() );
				ib.insert( ib.end(), localIB.begin(), localIB.end() );

				// We can have separated other loops. Try fetching them as well.
				idx0 = getBorderIndex();
				if( idx0 == -1 ) {
					return;
				} else {
					localPolygon.resize( 0 );
					idxPrev = idx0;
					idx = idx0;
				}
				
			} else {

				// The polygon must be complex, and we just found a closed loop.
				// Take only the loop, triangulate it, append to results, continue.
				TIntVector::const_iterator itLoopStart = 
					std::find( localPolygon.begin(), localPolygon.end(), idx );
				assert( itLoopStart != localPolygon.end() );

				// append to results
				TIntVector loopPolygon( itLoopStart, localPolygon.end() );
				triangulator::process( vb, loopPolygon, localIB );
				polygon.insert( polygon.end(), loopPolygon.begin(), loopPolygon.end() );
				ib.insert( ib.end(), localIB.begin(), localIB.end() );

				// continue - remove the looped polygon from local
				localPolygon.resize( itLoopStart - localPolygon.begin() );

			}

		} while( true );
	}


	void	begin( int totalVerts )
	{
		polygonCount = 0;

		vertices.clear();
		borderVertices.clear();

		vertexNexts.clear();
		vertexPrevs.clear();
		vertexUseCount.resize( 0 );
		vertexUseCount.resize( totalVerts, 0 );

		vertexTraceID.resize( totalVerts, -1 );

		vertexTypes.resize( 0 );
		vertexTypes.resize( totalVerts, VTYPE_NONE );
	}

	void	addPolygon( const TIntVector& ib )
	{
		++polygonCount;

		TIntVector::const_iterator vit, vitEnd = ib.end();
		for( vit = ib.begin(); vit != vitEnd; ++vit ) {
			int idx = *vit;
			assert( idx >= 0 && idx < vertexUseCount.size() );
			vertices.insert( idx );
			++vertexUseCount[idx];
			int idxNext = (vit==vitEnd-1) ? ib.front() : *(vit+1);
			int idxPrev = (vit==ib.begin()) ? ib.back() : *(vit-1);
			vertexNexts[idx].push_back( idxNext );
			vertexPrevs[idx].push_back( idxPrev );
		}
	}

	void end( const TWallVertexVector& vb, TIntVector& polygon, TIntVector& ib )
	{
		assert( polygonCount );
		if( polygonCount == 1 ) {
			
			// trivial case, just output input polygon
			polygon.resize( 0 );
			polygon.reserve( vertices.size() );
			int idx0 = *vertices.begin();
			int idx = idx0;
			do {
				polygon.push_back( idx );
				const TIntVector& vnext = vertexNexts[idx];
				assert( vnext.size() == 1 );
				idx = vnext[0];
			} while( idx != idx0 );


			triangulator::process( vb, polygon, ib );

		} else {
			
			// mark vertex types
			markVertexTypes();

			// trace and triangulate the polygon(s)
			traceBorder( vb, polygon, ib );
		}
	}

}; // namespace polygon_merger




// --------------------------------------------------------------------------



CWallPieceCombined* CWallPieceCombined::mInitPiece = NULL;
const CWall3D* CWallPieceCombined::mInitWall = NULL;
bool CWallPieceCombined::mInitRoot = false;


void CWallPieceCombined::initBegin( const CWall3D& w, TWallQuadNode* quadnode, bool root )
{
	assert( !mInitPiece );
	assert( !mInitWall );
	assert( mVB.empty() );
	assert( mIB.empty() );

	mInitPiece = this;
	mInitWall = &w;
	mInitRoot = root;

	mQuadNode = quadnode;

	mRenderID = -1;

	if( !mInitRoot )
		polygon_merger::begin( w.getWall2D().getVerts().size() );
	else {
		mBounds.getMin().set( 0, 0 );
		mBounds.getMax() = w.getWall2D().getSize();
		mCombinedPieces.reserve( w.getWall2D().getPieceCount() );
	}
}


void CWallPieceCombined::initAddPiece( int idx )
{
	mCombinedPieces.push_back( idx );

	if( !mInitRoot ) {

		const CWallPiece2D& piece = mInitWall->getWall2D().getPiece(idx);
		polygon_merger::addPolygon( piece.getPolygonVector() );

		mBounds.extend( piece.getAABB() );
	}
}


void CWallPieceCombined::initEnd( TWallQuadNode* quadtree )
{
	assert( mInitPiece == this );
	assert( mInitWall );

	// if we're root, just construct the full-wall quad
	if( mInitRoot ) {
		assert( mQuadNode == quadtree );
		mVB.resize( 4 );
		mIB.resize( 6 );
		const SVector2& bmin = mBounds.getMin();
		const SVector2& bmax = mBounds.getMax();

		// VB
		const SMatrix4x4& mat = mInitWall->getMatrix();
		{
			DWORD nrmCol = gVectorToColor( mat.getAxisZ() );

			SVertexXyzDiffuse vtx;
			vtx.p.set( bmin.x, bmin.y, 0 );
			D3DXVec3TransformCoord( &vtx.p, &vtx.p, &mat );
			vtx.diffuse = nrmCol;
			mVB[0] = vtx;
			vtx.p.set( bmin.x, bmax.y, 0 );
			D3DXVec3TransformCoord( &vtx.p, &vtx.p, &mat );
			vtx.diffuse = nrmCol;
			mVB[1] = vtx;
			vtx.p.set( bmax.x, bmin.y, 0 );
			D3DXVec3TransformCoord( &vtx.p, &vtx.p, &mat );
			vtx.diffuse = nrmCol;
			mVB[2] = vtx;
			vtx.p.set( bmax.x, bmax.y, 0 );
			D3DXVec3TransformCoord( &vtx.p, &vtx.p, &mat );
			vtx.diffuse = nrmCol;
			mVB[3] = vtx;
		}
		// IB
		{
			mIB[0] = 0;
			mIB[1] = 1;
			mIB[2] = 2;
			mIB[3] = 1;
			mIB[4] = 3;
			mIB[5] = 2;
		}

		mVBSizeNoCaps = mVB.size();
		mIBSizeNoCaps = mIB.size();

	} else {

		// for non-roots, construct proper polygon

		const TWallVertexVector& wallVerts = mInitWall->getWall2D().getVerts();

		// merge the polygons
		TIntVector polygon;
		TIntVector ibFront;
		polygon_merger::end( wallVerts, polygon, ibFront );


		static TIntVector	vertRemap;
		vertRemap.resize(0);
		vertRemap.resize( wallVerts.size(), -1 );

		int i;

		int nidx = ibFront.size();
		mIB.reserve( nidx + polygon.size()*6 );
		mVB.reserve( polygon.size()*3 );

		// construct the front side
		for( i = 0; i < nidx; ++i ) {
			int oldIdx = ibFront[i];
			int newIdx = vertRemap[oldIdx];
			if( newIdx < 0 ) {
				newIdx = mVB.size();
				vertRemap[oldIdx] = newIdx;

				SVector2 pos = wallVerts[oldIdx];
				SVector3 pos3( pos.x, pos.y, 0.0f );
				D3DXVec3TransformCoord( &pos3, &pos3, &mInitWall->getMatrix() );
				SVertexXyzDiffuse vtx;
				vtx.p = pos3;
				vtx.diffuse = gVectorToColor( mInitWall->getMatrix().getAxisZ() );
				mVB.push_back( vtx );
			}
			mIB.push_back( newIdx );
		}
		// reverse the ordering of triangles
		for( i = 0; i < nidx/3; ++i ) {
			int iii = mIB[i*3+1];
			mIB[i*3+1] = mIB[i*3+2];
			mIB[i*3+2] = iii;
		}

		mVBSizeNoCaps = mVB.size();
		mIBSizeNoCaps = mIB.size();

		// Don't construct the side caps for non-leaf pieces.
		// I think they never can be seen (not sure)
		if( !mQuadNode ) {
			
			int nverts = mVB.size();
			int npolygon = polygon.size();
			
			// Construct side caps. To conserve the geometry amount,
			// treat them as a single smoothing group.
			for( i = 0; i < nverts; ++i ) {
				int oldIdx0 = polygon[i];
				int oldIdx1 = polygon[(i+1)%npolygon];
				int oldIdx2 = polygon[(i+nverts-1)%npolygon];
				int idx0 = vertRemap[oldIdx0];
				int idx1 = vertRemap[oldIdx1];
				int idx2 = vertRemap[oldIdx2];
				assert( idx0 >= 0 && idx0 < nverts );
				assert( idx1 >= 0 && idx1 < nverts );
				assert( idx2 >= 0 && idx2 < nverts );
				SVertexXyzDiffuse v0 = mVB[idx0];
				SVertexXyzDiffuse v1 = v0;
				v1.p -= mInitWall->getMatrix().getAxisZ() * (HALF_THICK*2);

				const SVertexXyzDiffuse& vnear1 = mVB[idx1];
				const SVertexXyzDiffuse& vnear2 = mVB[idx2];

				SVector3 edge1 = vnear2.p - vnear1.p;
				SVector3 edge2 = v1.p - v0.p;
				SVector3 normal = edge1.cross( edge2 ).getNormalized();

				v0.diffuse = v1.diffuse = gVectorToColor( normal );
				
				mVB.push_back( v0 );
				mVB.push_back( v1 );

				int ibidx0 = i*2 + 0;
				int ibidx1 = i*2 + 1;
				int ibidx2 = ((i+1)%nverts)*2 + 0;
				int ibidx3 = ((i+1)%nverts)*2 + 1;
				mIB.push_back( ibidx0 );
				mIB.push_back( ibidx2 );
				mIB.push_back( ibidx1 );
				mIB.push_back( ibidx1 );
				mIB.push_back( ibidx2 );
				mIB.push_back( ibidx3 );
			}
		}

	}


	if( !mQuadNode ) {
		
		assert( mCombinedPieces.size()==1 );

		TWallQuadNode* node = quadtree->getNode( mBounds );
		assert( node );
		while( node ) {
			node->getData().leafs.push_back( this );
			node = node->getParent();
		}

	} else {

		// record ourselves in the quadtree
		assert( !mQuadNode->getData().combined );
		mQuadNode->getData().combined = this;
	}


	mInitPiece = NULL;
	mInitWall = NULL;
}


void CWallPieceCombined::preRenderSides( int& vbcount, int& ibcount ) const
{
	vbcount = mVBSizeNoCaps;
	ibcount = mIBSizeNoCaps;
}

void CWallPieceCombined::preRenderCaps( int& vbcount, int& ibcount ) const
{
	vbcount = mVB.size() - mVBSizeNoCaps;
	ibcount = mIB.size() - mIBSizeNoCaps;
}

void CWallPieceCombined::renderSides( TPieceVertex* vb, unsigned short* ib, int baseIndex, int& vbcount, int& ibcount ) const
{
	int i;

	preRenderSides( vbcount, ibcount );
	
	// VB
	const SVertexXyzDiffuse* srcVB = &mVB[0];
	for( i = 0; i < vbcount; ++i ) {
		vb->p = srcVB->p;
		vb->diffuse = srcVB->diffuse;
		++srcVB;
		++vb;
	}

	// IB
	const int* srcIB = &mIB[0];
	for( i = 0; i < ibcount; ++i ) {
		int idx = *srcIB + baseIndex;
		assert( idx >= 0 && idx < 64000 );
		*ib = idx;
		++srcIB;
		++ib;
	}
}

void CWallPieceCombined::renderCaps( TPieceVertex* vb, unsigned short* ib, int baseIndex, int& vbcount, int& ibcount ) const
{
	int i;

	preRenderCaps( vbcount, ibcount );
	
	// VB
	const SVertexXyzDiffuse* srcVB = &mVB[mVBSizeNoCaps];
	for( i = 0; i < vbcount; ++i ) {
		vb->p = srcVB->p;
		vb->diffuse = srcVB->diffuse;
		++srcVB;
		++vb;
	}

	// IB
	const int* srcIB = &mIB[mIBSizeNoCaps];
	for( i = 0; i < ibcount; ++i ) {
		int idx = *srcIB + baseIndex;
		assert( idx >= 0 && idx < 64000 );
		*ib = idx;
		++srcIB;
		++ib;
	}
}


// --------------------------------------------------------------------------


CWall3D::CWall3D( const SVector2& size, float smallestElemSize, const char* reflTextureID, const char* restoreTextureID )
: mWall2D(size,smallestElemSize)
, mPieces3D(NULL)
, mFracturedPieces(NULL)
, mPieceRestoreTimes(NULL)
, mQuadtree(NULL)
, mPiecesInited(false)
, mNeedsRenderingIntoVB(false)
{
	mMatrix.identify();

	int i;

	for( i = 0; i < RMCOUNT; ++i ) {
		mRenderables[i] = NULL;
	}
	
	mRenderables[RM_NORMAL] = new CRenderableIndexedBuffer( NULL, 0 );
	mRenderables[RM_NORMAL]->getParams().setEffect( *RGET_FX( restoreTextureID ? "wall_DnSR" : "wall_DnR" ) );
	mRenderables[RM_NORMAL]->getParams().addTexture( "tShadow", *RGET_S_TEX(RT_SHADOWBLUR) );
	mRenderables[RM_NORMAL]->getParams().addVector3( "vLightPos", LIGHT_POS_1 + LIGHT_WOFF );
	if( reflTextureID ) {
		mRenderables[RM_NORMAL]->getParams().addTexture( "tRefl", *RGET_S_TEX(reflTextureID) );
	}
	
	mRenderables[RM_REFLECTED] = new CRenderableIndexedBuffer( NULL, 0 );
	mRenderables[RM_REFLECTED]->getParams().setEffect( *RGET_FX( restoreTextureID ? "wall_DnS" : "wall_Dn" ) );
	mRenderables[RM_REFLECTED]->getParams().addTexture( "tShadow", *RGET_S_TEX(RT_SHADOWBLUR) );
	mRenderables[RM_REFLECTED]->getParams().addVector3( "vLightPos", LIGHT_POS_1 + LIGHT_WOFF );

	if( restoreTextureID ) {
		mFadeInMesh = new CMeshEntity( "FadeInMesh", "billboard" );
		if( reflTextureID ) {
			mFadeInMesh->getRenderMesh( RM_NORMAL )->getParams().addTexture( "tRefl", *RGET_S_TEX(reflTextureID) );
		}

		mResTimeGrid = new float[RESGRID_X*RESGRID_Y];
		for( i = 0; i < RESGRID_X*RESGRID_Y; ++i ) {
			mResTimeGrid[i] = -1.0f;
		}

		mRestoreTexture = RGET_S_TEX(restoreTextureID);

	} else {
		mFadeInMesh = NULL;
		mResTimeGrid = NULL;
		mRestoreTexture = NULL;
	}
}

CWall3D::~CWall3D()
{
	for( int i = 0; i < RMCOUNT; ++i ) {
		safeDelete( mRenderables[i] );
	}
	safeDelete( mFadeInMesh );

	stl_utils::wipe( mPiecesCombined );
	safeDeleteArray( mPieces3D );
	safeDeleteArray( mFracturedPieces );
	safeDeleteArray( mPieceRestoreTimes );

	safeDeleteArray( mQuadtree );
	safeDeleteArray( mResTimeGrid );
}

void CWall3D::initPieces()
{
	//cputimer::debug_interval tt0( "w3d init" );

	assert( !mPiecesInited );
	assert( !mFracturedPieces );
	assert( !mPieces3D );
	assert( mPiecesCombined.empty() );
	
	const int QUADTREE_DEPTH = 2;
	//const int QUADTREE_DEPTH = 3;
	mQuadtree = TWallQuadNode::create( SVector2(0,0), mWall2D.getSize(), QUADTREE_DEPTH, &mQuadtreeNodeCount );

	int i;
	int n = mWall2D.getPieceCount();

	mPieces3D = new CWallPiece3D[n];
	mFracturedPieces = new bool[n];
	mPieceRestoreTimes = new float[n];

	// init the leaf pieces
	for( i = 0; i < n; ++i ) {
		mPieces3D[i].init( *this, i );
		mFracturedPieces[i] = false;
		mPieceRestoreTimes[i] = -1.0f;

		CWallPieceCombined* wpc = new CWallPieceCombined();
		wpc->initBegin( *this, NULL, false );
		wpc->initAddPiece( i );
		wpc->initEnd( mQuadtree );
		mPiecesCombined.push_back( wpc );
	}

	// go through quadtree nodes and merge the pieces
	n = mQuadtreeNodeCount;
	for( i = 0; i < n; ++i ) {
		TWallQuadNode& node = mQuadtree[i];
		assert( !node.getData().combined );
		int npc = node.getData().leafs.size();
		if( npc > 0 ) {
			CWallPieceCombined* wpc = new CWallPieceCombined();
			wpc->initBegin( *this, &node, i==0 );
			for( int j = 0; j < npc; ++j )
				wpc->initAddPiece( node.getData().leafs[j]->getLeafIndex() );
			wpc->initEnd( mQuadtree );

			mPiecesCombined.push_back( wpc );
		}
	}

	// init fade-in mesh
	if( mFadeInMesh ) {
		SMatrix4x4 m = mMatrix;
		m.getOrigin() += m.getAxisX() * (mWall2D.getSize().x*0.5f);
		m.getOrigin() += m.getAxisY() * (mWall2D.getSize().y*0.5f);
		m.getOrigin() += m.getAxisZ() * (-HALF_THICK);
		m.getAxisX() *= mWall2D.getSize().x * -1.01f;
		m.getAxisY() *= mWall2D.getSize().y * 1.01f;
		mFadeInMesh->mWorldMat = m;
		mFadeInMesh->addLightToParams( LIGHT_POS_1 + LIGHT_WOFF );
		for( i = 0; i < RMCOUNT; ++i ) {
			CRenderableMesh* r = mFadeInMesh->getRenderMesh(eRenderMode(i));
			if( !r )
				continue;
			r->getParams().addVector3( "vNormal", m.getAxisZ() );
			r->getParams().addTexture( "tAlpha", *mRestoreTexture );
		}
	}

	// init AABB
	mWorldAABB.extend( SVector3(0,0,-HALF_THICK*2), 0.1f );
	mWorldAABB.extend( SVector3(mWall2D.getSize().x,mWall2D.getSize().y,HALF_THICK*2), 0.1f );
	mWorldAABB.transform( mMatrix );

	mPiecesInited = true;
	mNeedsRenderingIntoVB = true;
	mNeedsRenderFadeMesh = true;
}



bool CWall3D::intersectRay( const SLine3& ray, float& t ) const
{
	SPlane plane;
	D3DXPlaneFromPointNormal( &plane, &mMatrix.getOrigin(), &mMatrix.getAxisZ() );
	SVector3 pt;
	if( !plane.intersect( ray, pt ) ) {
		return false;
	}
	t = ray.project( pt );
	if( t < 0 )
		return false;
	return true;
}


void CWall3D::fracturePiecesInSphere( float t, const SVector3& pos, float radius, TIntVector& pcs,
		float restoreAfter, float restoreDuration, bool noRestore )
{
	if( !mPiecesInited )
		initPieces();

	pcs.resize( 0 );
	
	// to local space
	SVector3 locPos;
	D3DXVec3TransformCoord( &locPos, &pos, &mInvMatrix );

	if( locPos.z < -radius || locPos.z > radius )
		return;

	// remember restore times
	if( mResTimeGrid && !noRestore ) {
		float rad = radius*2.0f;
		float lx1 = (locPos.x - rad) / mWall2D.getSize().x * RESGRID_X;
		float lx2 = (locPos.x + rad) / mWall2D.getSize().x * RESGRID_X;
		float ly1 = (locPos.y - rad) / mWall2D.getSize().y * RESGRID_Y;
		float ly2 = (locPos.y + rad) / mWall2D.getSize().y * RESGRID_Y;
		int ix1 = (int)clamp( lx1, 0, RESGRID_X-1 );
		int ix2 = (int)clamp( lx2, 0, RESGRID_X-1 );
		int iy1 = (int)clamp( ly1, 0, RESGRID_Y-1 );
		int iy2 = (int)clamp( ly2, 0, RESGRID_Y-1 );
		float dx = mWall2D.getSize().x / RESGRID_X;
		float dy = mWall2D.getSize().y / RESGRID_Y;
		for( int iy = iy1; iy <= iy2; ++iy ) {
			float* resval = mResTimeGrid + RESGRID_X*iy + ix1;
			float fy = iy * dy;
			for( int ix = ix1; ix <= ix2; ++ix, ++resval ) {
				float fx = ix * dx;

				// don't touch restore grid outside the circle
				float diffX = fx-locPos.x;
				float diffY = fy-locPos.y;
				float diffR2 = diffX*diffX + diffY*diffY;
				if( diffR2 > rad*rad )
					continue;

				// restore time for this grid point - start at
				// t+restoreAfter at circle boundaries, later at circle
				// center
				float resTime = t + restoreAfter + (1.0f-diffR2/(rad*rad)) * restoreDuration;

				if( *resval < 0.0f )
					*resval = resTime;
				else
					*resval = max( (*resval), resTime );
			}
		}
	}

	// fetch the pieces

	float pieceRestoreTime;
	if( noRestore ) {
		pieceRestoreTime = t + 1.0e9f;
	} else {
		pieceRestoreTime = t + restoreAfter + restoreDuration;
	}

	// TODO: optimize, right now linear search!
	int n = mWall2D.getPieceCount();
	for( int i = 0; i < n; ++i ) {
		const CWallPiece2D& p = mWall2D.getPiece( i );
		SVector2 c = p.getAABB().getCenter();
		SVector3 tocenter = locPos - SVector3(c.x,c.y,0);
		if( tocenter.lengthSq() < radius*radius ) {
			mPieceRestoreTimes[i] = pieceRestoreTime;
			if( mFracturedPieces[i] )
				continue;
			pcs.push_back( i );
			fractureOutPiece( i );
		}
	}
}

void CWall3D::fracturePiecesInYRange( float t, float y1, float y2, TIntVector& pcs )
{
	if( !mPiecesInited )
		initPieces();

	// fetch the pieces
	pcs.resize( 0 );

	// TODO: optimize, right now linear search!
	float pieceRestoreTime = t + 1.0e6f;

	int n = mWall2D.getPieceCount();
	for( int i = 0; i < n; ++i ) {
		const CWallPiece2D& p = mWall2D.getPiece( i );
		SVector2 c = p.getAABB().getCenter();
		float y = c.y;
		if( mMatrix.getAxisY().y < 0.5f ) {
			SVector3 wc;
			D3DXVec3TransformCoord( &wc, &SVector3(c.x,c.y,0), &mMatrix );
			y = wc.y;
		}
		if( y >= y1 && y <= y2 ) {
			mPieceRestoreTimes[i] = pieceRestoreTime;
			if( mFracturedPieces[i] )
				continue;
			pcs.push_back( i );
			fractureOutPiece( i );
		}
	}
}

void CWall3D::fractureOutPiece( int index )
{
	assert( index >= 0 && index < mWall2D.getPieceCount() );
	assert( !mFracturedPieces[index] );
	mFracturedPieces[index] = true;
	mNeedsRenderingIntoVB = true;

	// mark as fractured in quadtree
	TWallQuadNode* node = mQuadtree->getNode( mWall2D.getPiece(index).getAABB() );
	while( node ) {
		++node->getData().fracturedOutCounter;
		node = node->getParent();
	}
}

void CWall3D::fractureInPiece( int index )
{
	assert( index >= 0 && index < mWall2D.getPieceCount() );
	assert( mFracturedPieces[index] );
	mFracturedPieces[index] = false;
	mPieceRestoreTimes[index] = -1.0f;
	mNeedsRenderingIntoVB = true;

	// mark as non fractured in quadtree
	TWallQuadNode* node = mQuadtree->getNode( mWall2D.getPiece(index).getAABB() );
	while( node ) {
		--node->getData().fracturedOutCounter;
		assert( node->getData().fracturedOutCounter >= 0 );
		node = node->getParent();
	}
}


void CWall3D::restoreAll()
{
	// restore all pieces
	int n = mWall2D.getPieceCount();
	for( int i = 0; i < n; ++i ) {
		if( mFracturedPieces[i] )
			fractureInPiece( i );
	}
}


void CWall3D::update( float t )
{
	if( !mPiecesInited )
		initPieces();

	// update restoration texture
	if( mRestoreTexture ) {
		const float RESTORE_FADE_TIME = 0.2f;

		bool hadZeros = false;
		bool had255s = false;
		bool hadOthers = false;

		D3DLOCKED_RECT lr;
		mRestoreTexture->getObject()->LockRect( 0, &lr, NULL, D3DLOCK_DISCARD );

		BYTE* texptr = (BYTE*)lr.pBits;
		float* resval = mResTimeGrid;
		for( int iy = 0; iy < RESGRID_Y; ++iy ) {
			for( int ix = 0; ix < RESGRID_X; ++ix, ++resval ) {
				BYTE v;
				float rt = *resval;
				if( rt < 0.0f ) {
					v = 255;
					hadZeros = true;
				} else if( t <= rt - RESTORE_FADE_TIME ) {
					v = 0;
					hadZeros = true;
				} else {
					float alpha = (t-rt+RESTORE_FADE_TIME) / RESTORE_FADE_TIME;
					if( alpha >= 1.0f ) {
						// fully restored
						v = 255;
						*resval = -1.0f;
						had255s = true;
					} else {
						v = int(alpha*255);
						hadOthers = true;
					}
				}
				texptr[ix] = v;
			}
			texptr += lr.Pitch;
		}

		mRestoreTexture->getObject()->UnlockRect( 0 );

		mNeedsRenderFadeMesh = (hadOthers) || (hadZeros && had255s);

		// restore needed pieces
		int n = mWall2D.getPieceCount();
		for( int i = 0; i < n; ++i ) {
			if( mFracturedPieces[i] && t>=mPieceRestoreTimes[i] )
				fractureInPiece( i );
		}
	}
}


bool CWall3D::renderIntoVB()
{
	mNeedsRenderingIntoVB = false;

	int nverts = 0, nindices = 0;
	mVBChunk = NULL;
	mIBChunk = NULL;

	int i;

	// render process:
	//
	// 1) go through quadtree, for each node that has no fractured out
	// pieces inside (or it's children), render the combined piece and
	// mark the node and all combined leaf pieces as rendered. Skip the nodes
	// that have their parent rendered.
	//
	// 2) render the remaining leaf pieces.

	static int renderID = 0;
	++renderID;

	std::vector<const CWallPieceCombined*>	piecesToRender;
	piecesToRender.reserve( 256 );


	int nnodes = mQuadtreeNodeCount;
	for( i = 0; i < nnodes; ++i )
	{
		TWallQuadNode& node = mQuadtree[i];
		SWallQuadData& data = node.getData();
		data.alreadyRendered = false;

		// if we have fractured out pieces somewhere inside, skip
		if( data.fracturedOutCounter > 0 )
			continue;

		// if we have no combined, skip
		if( !data.combined )
			continue;

		// if parent already rendered the combined piece, skip
		bool parentRendered = false;
		const TWallQuadNode* nodePar = node.getParent();
		while( nodePar ) {
			if( nodePar->getData().alreadyRendered ) {
				parentRendered = true;
				break;
			}
			nodePar = nodePar->getParent();
		}
		data.alreadyRendered = true;
		if( parentRendered )
			continue;

		// render the combined piece
		piecesToRender.push_back( data.combined );
		data.alreadyRendered = true;

		// mark the combined pieces as rendered
		int npcs = data.leafs.size();
		for( int j = 0; j < npcs; ++j ) {
			CWallPieceCombined* pcleaf = data.leafs[j];
			pcleaf->markRendered( renderID );
		}
	}

	int npieces = mWall2D.getPieceCount();
	for( i = 0; i < npieces; ++i )
	{
		if( mFracturedPieces[i] )
			continue;
		CWallPieceCombined* pcleaf = mPiecesCombined[i];
		if( pcleaf->isRendered( renderID ) )
			continue;
		// not rendered yet, add to render list
		piecesToRender.push_back( pcleaf );
		pcleaf->markRendered( renderID );
	}

	//
	// accumulate total vertex/index counts

	int n = piecesToRender.size();
	for( i = 0; i < n; ++i )
	{
		const CWallPieceCombined& pc = *piecesToRender[i];
		int nvb, nib;
		pc.preRenderSides( nvb, nib );
		nverts += nvb;
		nindices += nib;
	}
	int nvertsNoCaps = nverts;
	int nindicesNoCaps = nindices;
	for( i = 0; i < n; ++i )
	{
		const CWallPieceCombined& pc = *piecesToRender[i];
		int nvb, nib;
		pc.preRenderCaps( nvb, nib );
		nverts += nvb;
		nindices += nib;
	}


	gWallVertCount += nverts;
	gWallTriCount += nindices/3;

	if( !nverts || !nindices )
		return false;

	// lock and render
	assert( nindices < 64000 );
	mVBChunk = CDynamicVBManager::getInstance().allocateChunk( nverts, sizeof(TPieceVertex) );
	mIBChunk = CDynamicIBManager::getInstance().allocateChunk( nindices, 2 );
	TPieceVertex* vb = (TPieceVertex*)mVBChunk->getData();
	unsigned short* ib = (unsigned short*)mIBChunk->getData();
	int baseIndex = 0;
	for( i = 0; i < n; ++i ) {
		int nvb, nib;
		const CWallPieceCombined& pc = *piecesToRender[i];
		pc.renderSides( vb, ib, baseIndex, nvb, nib );
		baseIndex += nvb;
		vb += nvb;
		ib += nib;
	}
	for( i = 0; i < n; ++i ) {
		int nvb, nib;
		const CWallPieceCombined& pc = *piecesToRender[i];
		pc.renderCaps( vb, ib, baseIndex, nvb, nib );
		baseIndex += nvb;
		vb += nvb;
		ib += nib;
	}
	mVBChunk->unlock();
	mIBChunk->unlock();

	// setup renderables
	CD3DVertexDecl* vdecl = RGET_VDECL( CVertexFormat( CVertexFormat::V_POSITION | CVertexFormat::COLOR_MASK ) );

	for( i = 0; i < RMCOUNT; ++i )
	{
		if( mRenderables[i] ) {
			bool noSideCaps = (i == RM_REFLECTED);
			mRenderables[i]->resetVBs();

			mRenderables[i]->setVB( mVBChunk->getBuffer(), 0 );
			mRenderables[i]->setStride( mVBChunk->getStride(), 0 );
			mRenderables[i]->setBaseVertex( mVBChunk->getOffset() );
			mRenderables[i]->setMinVertex( 0 );
			mRenderables[i]->setNumVertices( noSideCaps ? nvertsNoCaps : nverts );

			mRenderables[i]->setIB( mIBChunk->getBuffer() );
			mRenderables[i]->setStartIndex( mIBChunk->getOffset() );
			mRenderables[i]->setPrimCount( (noSideCaps ? nindicesNoCaps : nindices) / 3 );

			mRenderables[i]->setPrimType( D3DPT_TRIANGLELIST );
			mRenderables[i]->setVertexDecl( vdecl );
		}
	}
	return true;
}


void CWall3D::render( eRenderMode rm )
{
	if( mFadeInMesh && mNeedsRenderFadeMesh )
		mFadeInMesh->render( rm );

	CRenderable* r = mRenderables[rm];
	if( !r )
		return;

	if( mNeedsRenderingIntoVB ||
		!mVBChunk || !mVBChunk->isValid() ||
		!mIBChunk || !mIBChunk->isValid() )
	{
		renderIntoVB();
	}
	G_RENDERCTX->attach( *r );
}
