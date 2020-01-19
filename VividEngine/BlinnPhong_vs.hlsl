/**
	BlinnPhong_vs.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 01/19/20
*/

struct VSIn
{
	float4 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct PSIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD0;
};

cbuffer TransformConstants : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer CameraConstants
{
    float3 cameraPosition;
    float padding;
};

cbuffer ColorBuffer
{
    float4 debugColor;
}

PSIn VSMain(VSIn input)
{
    PSIn output = (PSIn)0;
    
    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.color = debugColor;

    output.normal = mul(input.normal, (float3x3)worldMatrix);

    output.normal = normalize(output.normal);

    float4 worldPosition = mul(input.position, worldMatrix);
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
    output.viewDirection = normalize(output.viewDirection);
    
    return output;
}