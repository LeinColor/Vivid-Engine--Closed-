/**
	Texture_vs.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 02/07/20
*/

cbuffer TransformConstants : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VSIn
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PSIn
{
	float4 position : SV_Position;
	float2 tex : TEXCOORD0;
};

PSIn VSMain(VSIn input)
{
	PSIn output;
	
	input.position.w = 1.0f;
	
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	
	output.tex = input.tex;
	
	return output;
}