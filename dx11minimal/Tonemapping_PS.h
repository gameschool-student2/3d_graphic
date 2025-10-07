Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

float3 ACESFilm(float3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

static const float2 center = float2(0.5, 0.5);

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 color = inputTexture.Sample(samplerState, input.uv);

    // Применяем ACES тональную компрессию
    float3 result = ACESFilm(color);

    // Гамма-коррекция
    result = pow(result, 1.0 / 2.2);

    return float4(result, 1);
}