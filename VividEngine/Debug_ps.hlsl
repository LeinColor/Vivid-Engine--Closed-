/**
	Debug_ps.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 01/19/20
*/
struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PSMain(PSIn input) : SV_TARGET
{
    return input.color;
}