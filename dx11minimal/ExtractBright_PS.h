Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 PS(VS_OUTPUT input) : SV_Target
{
    float bloomThreshold = 0.5;

    float3 color = inputTexture.Sample(samplerState, input.uv).rgb;

    // Вычисляем яркость (luminance)
    float luminance = dot(color, float3(0.299f, 0.587f, 0.114f));

    // Выделяем только яркие области
    float bright = max(luminance - bloomThreshold, 0.0f);

    // Умножаем на исходный цвет для сохранения цветности
    float3 bloomColor = color * bright * 4;

    return float4(bloomColor, 1.0f);
}