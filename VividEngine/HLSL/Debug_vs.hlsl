/**
	Debug_vs.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 01/19/20
*/
struct VSIn
{
	float4 position : POSITION;
};

struct PSIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer TransformConstants : register( b0 )
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ColorBuffer
{
    float4 debugColor;
}

PSIn VSMain(VSIn input)
{
    PSIn output;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.color = debugColor;
    
    return output;
}