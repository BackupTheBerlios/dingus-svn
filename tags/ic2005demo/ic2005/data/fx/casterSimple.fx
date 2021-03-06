#include "lib/shared.fx"
#include "lib/structs.fx"

SPos vsMain( SPos i ) {
	SPos o;
	o.pos = mul( i.pos, mViewProj );
	return o;
}

half4 psMain( SPos i ) : COLOR {
	return 0.6;
}


technique tec11
{
	pass P0 {
		VertexShader = compile vs_1_1 vsMain();
		PixelShader = compile ps_1_1 psMain();
	}
	pass PLast {
	}
}
