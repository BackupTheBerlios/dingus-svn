#include "lib/shared.fx"
#include "lib/structs.fx"
#include "lib/commonWalls.fx"


SPosColTexp vsMain( SPosN i ) {
	SPosColTexp o;
	o.pos = mul( i.pos, mViewProj );
	o.uvp = mul( i.pos, mShadowProj );
	o.color = gWallLight( i.pos.xyz, i.normal*2-1 );
	return o;
}

half4 psMain( SPosColTexp i ) : COLOR {
	half3 col = tex2Dproj( smpShadow, i.uvp ) * i.color;
	return half4( col, 1 );
}


technique tec0
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}
	pass PLast {
		Texture[0] = NULL;
	}
}
