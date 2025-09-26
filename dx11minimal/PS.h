cbuffer global : register(b5)
{
    float4 gConst[32];
};

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

cbuffer camera : register(b3)
{
    float4x4 world[2];
    float4x4 view[2];
    float4x4 proj[2];
};

cbuffer drawMat : register(b2)
{
    float4x4 model;
    float hilight;
};

cbuffer params : register(b1)
{
    float r, g, b;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 lightDir = float3(0, -1, 0);
    float4 ambientColor = float4(0.15, 0.15, 0.15, 1);
    float4 diffuseColor = float4(1, 1, 1, 1);
    float specularPower = 32;

    float lightIntensity = saturate(dot(input.normal, lightDir));
    float3 reflection = normalize(2.0f * lightIntensity * input.normal - lightDir);
    float4 specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    float4 color = ambientColor + diffuseColor * lightIntensity;

    color = saturate(color + specular);

    return color;
}