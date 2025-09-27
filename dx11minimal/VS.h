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

float3 calculatePositionOnCurve(float u, float p, float q, float radius) {

    const float cu = cos(u);
    const float su = sin(u);
    const float quOverP = q / p * u;
    const float cs = cos(quOverP);

    float3 position;
    position.x = radius * (2 + cs) * 0.5 * cu;
    position.y = radius * (2 + cs) * su * 0.5;
    position.z = radius * sin(quOverP) * 0.5;

    return position;
}

float3 torus_knot(float2 p)
{
    float r = 5;
    float r2 = 10;
    float tube = 2;
    float n = (float)drawConst[0];

    p.x = (p.x / n) * 3.1415926536;
    p.y = (p.y / n) * 3.1415926536 * 2;

    float3 pos = float3(0, 0, 0);

    float3 p1 = calculatePositionOnCurve(-p.y, 2, 3, r2);
    float3 p2 = calculatePositionOnCurve(-p.y + 0.01, 2, 3, r2);

    float3 t = p2 - p1;
    float3 norm = p2 + p1;
    float3 b = cross(t, norm);
    norm = cross(b, t);

    b = normalize(b);
    norm = normalize(norm);

    float cx = tube * cos(p.x);
    float cy = tube * sin(p.x);

    pos.x = p1.x + (cx * norm.x + cy * b.x);
    pos.y = p1.y + (cx * norm.y + cy * b.y);
    pos.z = p1.z + (cx * norm.z + cy * b.z);

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

    pos.xyz = torus_knot(pos.xy);

    output.pos = mul(pos, mul(view[0], proj[0]));
    output.uv = float2(1, -1) * p / 2. + .5;

    return output;
}
