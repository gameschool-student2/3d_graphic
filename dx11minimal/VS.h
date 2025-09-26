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

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 vpos : POSITION0;
    float4 wpos : POSITION1;
    float4 vnorm : NORMAL1;
    float2 uv : TEXCOORD0;
};

float3 rotY(float3 pos, float a)
{
    float3x3 m =
    {
        cos(a), 0, sin(a),
        0, 1, 0,
        -sin(a), 0, cos(a)
    };
    pos = mul(pos, m);
    return pos;
}

float3 ball(float2 p)
{
    float radius = 10;
    float n = (float)drawConst[0];

    p.x = (p.x / n) * 3.141592653589793;
    p.y = (p.y / n) * 3.141592653589793 / 2;

    float3 pos = float3(cos(p.x) * cos(p.y) * radius, sin(p.y) * radius, sin(p.x) * cos(p.y) * radius);

    return pos;
}

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

    float3 t = normalize(pos1 - pos0);
    float3 b = normalize(pos2 - pos0);
    float3 h = normalize(cross(t, b));

    output.pos = mul(float4(pos0.xyz, 1), mul(view[0], proj[0]));
    output.vpos = mul(output.pos, view[0]);
    output.wpos = float4(pos0.xyz, 1);
    output.uv = pos.xy / n;
    output.vnorm = float4(h.xyz, 1);

    return output;
}
