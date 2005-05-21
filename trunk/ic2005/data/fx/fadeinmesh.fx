#include "lib/shared.fx"
#include "lib/structs.fx"
#include "lib/commonWalls.fx"

float4x4	mWorld;
float4x4	mWVP;
float3		vNormal;
float3		vLightPos;

texture		tAlpha;
sampler2D	smpAlpha = sampler_state {
	Texture = (tAlpha);
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Clamp; AddressV = Clamp;
};

texture		tRefl;
sampler2D	smpRefl = sampler_state {
	Texture = (tRefl);
	MinFilter = Linear; MagFilter = Linear; MipFilter = Linear;
	AddressU = Clamp; AddressV = Clamp;
};


struct SOutput {
	float4	pos		: POSITION;
	half3	tol 	: COLOR;
	float2	uv		: TEXCOORD0;
	float4	uvp		: TEXCOORD1;
};


SOutput vsMain( SPosNTex i ) {
	SOutput o;
	float3 wpos = mul( i.pos, mWorld );
	o.pos = mul( i.pos, mWVP );

	o.uv.x = 1-i.uv.x;
	o.uv.y = 1-i.uv.y;
	o.uvp = mul( float4(wpos,1), mViewTexProj );
	
	float3 tolight = normalize( vLightPos - wpos );
	o.tol = tolight*0.5+0.5;

	return o;
}

half4 psMain( SOutput i ) : COLOR {
	half a = tex2D( smpAlpha, i.uv ).a;

	// lighting
	half3 n = vNormal;
	half3 tol = normalize( i.tol*2-1 );
	half l = gWallLightPS( n, tol );
	half3 col = l;

	col += tex2Dproj( smpRefl, i.uvp ).rgb * 0.2;
	return half4(col,a);
}


technique tec20
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_2_0 psMain();
		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = False;
	}
	pass PLast {
		AlphaBlendEnable = False;
		ZWriteEnable = True;
	}
}


technique tecFFP
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = NULL;

		AlphaBlendEnable = True;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZWriteEnable = False;

		Sampler[0] = (smpAlpha);
		ColorOp[0] = SelectArg1;
		ColorArg1[0] = Texture;
		AlphaOp[0] = SelectArg1;
		AlphaArg1[0] = Texture;

		ColorOp[1] = Disable;
		AlphaOp[1] = Disable;
	}
	pass PLast {
		AlphaBlendEnable = False;
		ZWriteEnable = True;
	}
}
