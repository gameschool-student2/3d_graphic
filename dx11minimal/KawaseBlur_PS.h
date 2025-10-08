Texture2D inputTexture : register(t1);
SamplerState samplerState : register(s1);

cbuffer frame : register(b4)
{
    float4 time;
    float4 aspect;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

static const float2 directions[4] =
{
    float2(1, 1),
    float2(-1, 1),
    float2(1, -1),
    float2(-1, -1)
};

static const int iterations = 16;

float4 PS(VS_OUTPUT input) : SV_Target
{
    float offsetScale = 0.002;

    uint width, height;
    float2 texelSize = float2(1, 1) / float2(width, height);

    float4 color = float4(0, 0, 0, 0);
    for (int i = 0; i < iterations; i++)
    {
        float offset = offsetScale * (i + 0.5);
        for (int j = 0; j < 4; j++)
        {
            float2 uv = input.uv + directions[j] * float2(aspect.x, 1) * offset;
            color += inputTexture.Sample(samplerState, uv);
        }
    }

    color /= (iterations * 4);
    return color;
}