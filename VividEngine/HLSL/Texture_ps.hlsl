/**
	Texture_ps.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 02/07/20
*/

Texture2D shaderTexture;
SamplerState SampleType;

struct PSIn
{
	float4 position : SV_Position;
	float2 tex : TEXCOORD0;
};

float4 PSMain(PSIn input) : SV_Target
{
	float4 textureColor;
	
	textureColor = shaderTexture.Sample(SampleType, input.tex);
	
	return textureColor;
}