#include "lib/shared.fx"
#include "lib/structs.fx"

float4x4 mWorld;
float4x4 mWorldView;
float4x4 mWVP;



SPosColTexp vsMain( SPosN i ) {
	SPosColTexp o;
	float4 wpos = mul( i.pos, mWorld );
	float3 tolight = normalize( vLightPos - wpos.xyz );
	float3 n = -float3( mWorld[2][0], mWorld[2][1], mWorld[2][2] );
	o.pos = mul( i.pos, mWVP );

	o.uvp = mul( wpos, mShadowProj );

	float diffuse = max( 0.5, dot( tolight, n ) );
	o.color = diffuse;
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

technique tecFFP
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = NULL;

		ColorOp[0] = SelectArg1;
		ColorArg1[0] = Diffuse;
		AlphaOp[0] = SelectArg1;
		AlphaArg1[0] = Diffuse;

		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;
	}
	pass PLast {
	}
}
