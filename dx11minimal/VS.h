cbuffer drawerV : register(b0)
{
    float drawConst[32];
}

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

struct VS_INPUT {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

VS_OUTPUT VS(uint vID : SV_VertexID)
{
    VS_OUTPUT output = (VS_OUTPUT)0;

    uint n = drawConst[0];
    uint instanceID = vID / 6;

    float row = instanceID % n;
    float col = instanceID / n;

    float2 quad[6] = { -1, -1, 1, -1, -1, 1, 1, -1, 1, 1, -1, 1 };
    float2 p = quad[vID % 6];

    float4 pos = float4(p, 0, 1);
    pos.y += col * 2;
    pos.x += row * 2;
    pos.xy -= (float)n - 1;

    float3 pos0 = ball(pos.xy);
    float3 pos1 = ball(pos.xy + float2(1, 0));
    float3 pos2 = ball(pos.xy + float2(0, 1));

    float3 tangent = normalize(pos1 - pos0);
    float3 binormal = normalize(pos2 - pos0);
    float3 normal = -normalize(cross(tangent, binormal));

    output.pos = mul(float4(pos0.xyz, 1), mul(view[0], proj[0]));
    output.vpos = mul(output.pos, view[0]);
    output.wpos = float4(pos0.xyz, 1);
    output.uv = pos.xy / n;
    output.vnorm = float4(normal.xyz, 1);

    return output;
}
