#include "lib/shared.fx"
#include "lib/structs.fx"

float4x4	mWorld;
float4x4	mWorldView;
float4x4	mWVP;


texture		tNormalAO;
sampler2D	smpNormalAO = sampler_state {
	Texture = (tNormalAO);
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Wrap; AddressV = Wrap;
};

struct SOutput {
	float4 pos		: POSITION;
	float4 tolight	: COLOR0;		// skin space
	float4 toview	: COLOR1;		// skin space
	float2 uv		: TEXCOORD0;
};


SOutput vsMain( SPosTex i ) {
	SOutput o;

	float3 wpos = mul( i.pos, mWorld );
	o.pos = mul( i.pos, mWVP );

	float3x3 wT = transpose( (float3x3)mWorld );
	float3x3 wvT = transpose( (float3x3)mWorldView );
	
	float3 tolight = mul( vLightDir, wvT );
	o.tolight = float4( tolight*0.5+0.5, 1 );

	float3 toview = normalize( vEye - wpos );
	toview = mul( toview, wT );
	o.toview = float4( toview*0.5+0.5, 1 );

	o.uv = i.uv;

	return o;
}

half4 psMain( SOutput i ) : COLOR {
	// sample normal+AO map
	half4 normalAO = tex2D( smpNormalAO, i.uv );
	half3 normal = normalAO.rgb*2-1;
	half occ = normalAO.a * 1.0 + 0.0;
	
	half diffuse = saturate( dot( normal, i.tolight.xyz*2-1 ) );
	half rim = (1-saturate( dot( normal, i.toview.xyz*2-1 ) ));
	rim *= 0.3;

	const half3 cDiff = half3( 1.05, 1.1, 1.2 );
	const half3 cRim = half3( 0.99, 0.98, 0.96 );
	half3 col = cDiff * diffuse + cRim * rim;

	col = col * 0.5 + 0.5 * occ;
	return half4( col, 1 );
}


technique tec20
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain();
	}
	pass PLast {
	}
}

technique tecFFP
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = NULL;

		// TBD
		Sampler[0] = (smpNormalAO);
		ColorOp[0] = SelectArg1;
		ColorArg1[0] = Texture;
		AlphaOp[0] = SelectArg1;
		AlphaArg1[0] = Texture;
		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;
	}
	pass PLast {
	}
}
