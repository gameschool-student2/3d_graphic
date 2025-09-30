Texture2D sceneTexture : register(t0);
Texture2D bloomTexture : register(t1);
SamplerState samplerState1 : register(s0);
SamplerState samplerState2 : register(s1);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 sceneColor = sceneTexture.Sample(samplerState1, input.uv).rgb;
    float3 bloomColor = bloomTexture.Sample(samplerState2, input.uv).rgb;

    // Смешиваем bloom с исходным изображением
    float3 result = sceneColor + bloomColor;

    return float4(result, 1.0f);
}