/**
	BlinnPhong_ps.hlsl

	Purpose: this is simple color shader to debug.

	@author Lein
	@version 1.0 01/19/20
*/

struct PSIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD0;
};

cbuffer LightConstants
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
}

float4 PSMain(PSIn input) : SV_TARGET
{
    float4 color = ambientColor;
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float3 lightDir = -lightDirection;
    
    float lightIntensity = saturate(dot(input.normal, lightDir));

    if(lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
        color = saturate(color);
        float3 reflection = normalize(2 * lightIntensity * input.normal - lightDir);
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }
    color = color * input.color;

    color = saturate(color + specular);
    return color;
}