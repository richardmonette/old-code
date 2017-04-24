
#include "lighting.hlsl"

cbuffer ConstantBuffer
{
    float4x4 gViewProj;
	float4x4 gLightViewProj;

	float3 gEyePosW;

	float3 gLightPosW;
}

Texture2D shadowMaps[4];

Texture2D smokeTexture;

SamplerState gTriLinearSamp
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VS_IN
{
	float4 centerW : POSITION;
	float4 sizeW : SIZE;
	float4 color : COLOR0;
	float4 normal : NORMAL;
};

struct VS_OUT
{
    float4 centerW : POSITION;
	float4 sizeW : SIZE;
	float4 color : COLOR0;
	float4 normal : NORMAL;
};

struct GS_OUT
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
	float2 texC : TEXCOORD0;
	float4 color : COLOR0;
	float3 normal : TEXCOORD1;
	//float4 posL : TEXCOORD1;
	uint primID : SV_PrimitiveID;
};

VS_OUT VS(VS_IN vIn)
{
	VS_OUT vOut;

	vOut.centerW = vIn.centerW;
	vOut.sizeW = vIn.sizeW;
	vOut.color = vIn.color;
	vOut.normal = vIn.normal;

	return vOut;
}

[maxvertexcount(4)]
void GS(point VS_OUT gIn[1], uint primID : SV_PrimitiveID, inout TriangleStream<GS_OUT> triStream)
{
	float halfWidth = 0.5f * gIn[0].sizeW.x;
	float halfHeight = 0.5f * gIn[0].sizeW.y;

	float4 v[4];
	v[0] = float4(-halfWidth, -halfWidth, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfWidth, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfWidth, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfWidth, 0.0f, 1.0f);

	float2 texC[4];
	texC[0] = float2(0.0f, 1.0f);
	texC[1] = float2(1.0f, 1.0f);
	texC[2] = float2(0.0f, 0.0f);
	texC[3] = float2(1.0f, 0.0f);

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 look = gEyePosW.xyz - gIn[0].centerW.xyz;
	look = normalize(look);
	float3 right = cross(up, look);

	float4x4 W;
	W[0] = float4(right, 0.0f);
	W[1] = float4(up, 0.0f);
	W[2] = float4(look, 0.0f);
	W[3] = float4(gIn[0].centerW.xyz, 1.0f);

	float3 n[4];
	n[0] = mul(gIn[0].normal, W).xyz;
	n[1] = mul(gIn[0].normal, W).xyz;
	n[2] = mul(gIn[0].normal, W).xyz;
	n[3] = mul(gIn[0].normal, W).xyz;

	float4x4 WVP = mul(W, gViewProj);

	GS_OUT gOut;
	[unroll]
	for (int i = 0; i < 4; ++i)
	{
		gOut.posH = mul(v[i], WVP);
		//gOut.posL = mul(v[i], gLightViewProj);
		gOut.posW = mul(v[i], W).xyz;
		gOut.normalW = look;
		gOut.texC = texC[i];
		gOut.color = gIn[0].color;
		gOut.normal = gIn[0].normal.xyz;//n[i];//
		gOut.primID = primID;

		triStream.Append(gOut);
	}
}

struct SlicesOut
{ 
    float4 color0 : SV_Target0; 
    float4 color1 : SV_Target1; 
    float4 color2 : SV_Target2; 
	float4 color3 : SV_Target3; 
}; 

SlicesOut SHADOW_PS(GS_OUT pIn)// : SV_Target
{
	const int lut[16][16] = 
	{
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	};

	float dist = distance(pIn.posW.xyz, gLightPosW);
	
	int intd = (int)dist;

	float a = (0.5f - distance(pIn.texC.xy, float2(0.5f, 0.5f))) * pIn.color.a;

	float d[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	d[0]  = a * lut[intd][0];
	d[1]  = a * lut[intd][1];
	d[2]  = a * lut[intd][2];
	d[3]  = a * lut[intd][3];
	d[4]  = a * lut[intd][4];
	d[5]  = a * lut[intd][5];
	d[6]  = a * lut[intd][6];
	d[7]  = a * lut[intd][7];
	d[8]  = a * lut[intd][8];
	d[9]  = a * lut[intd][9];
	d[10] = a * lut[intd][10];
	d[11] = a * lut[intd][11];
	d[12] = a * lut[intd][12];
	d[13] = a * lut[intd][13];
	d[14] = a * lut[intd][14];
	d[15] = a * lut[intd][15];

	SlicesOut slicesOut;

	slicesOut.color0 = float4(d[0],  d[1],  d[2],  d[3]);
	slicesOut.color1 = float4(d[4],  d[5],  d[6],  d[7]);
	slicesOut.color2 = float4(d[8],  d[9],  d[10], d[11]);
	slicesOut.color3 = float4(d[12], d[13], d[14], d[15]);

	return slicesOut;
}

float4 GetPositionFromLight(float4 position)
{
	return mul(position, gLightViewProj);  
}

// DEFAULT SHADERS

struct VS_DEFAULT_IN
{
	float3 posL : POSITION;
	float4 normal : NORMAL;
};

struct VS_DEFAULT_OUT
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float4 normal : NORMAL;
};

VS_DEFAULT_OUT DEFAULT_VS(VS_DEFAULT_IN vIn)
{
	VS_DEFAULT_OUT vOut;

	vOut.posH = mul(float4(vIn.posL, 1.0f), gViewProj);
	vOut.posW = vIn.posL;

	return vOut;
}

float4 PS(GS_OUT pIn) : SV_Target
{
	float a = (0.5f - distance(pIn.texC.xy, float2(0.5f, 0.5f))) * pIn.color.a;

	float3 c = pIn.color.rgb;

	float4 lightingPosition = GetPositionFromLight(float4(pIn.posW,1));

	float2 ShadowTexC = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );
    ShadowTexC.y = 1.0f - ShadowTexC.y;

	float dist = distance(pIn.posW.xyz, gLightPosW);

	int intd_min = (int)floor(dist);
	int intd_max = (int)ceil(dist);
	
	intd_min -= 20;
	intd_max -= 20;

	float s[16];

	s[15]  = shadowMaps[0].Sample(gTriLinearSamp, ShadowTexC)[0];
	s[14]  = shadowMaps[0].Sample(gTriLinearSamp, ShadowTexC)[1];
	s[13]  = shadowMaps[0].Sample(gTriLinearSamp, ShadowTexC)[2];
	s[12]  = shadowMaps[0].Sample(gTriLinearSamp, ShadowTexC)[3];
	s[11]  = shadowMaps[1].Sample(gTriLinearSamp, ShadowTexC)[0];
	s[10]  = shadowMaps[1].Sample(gTriLinearSamp, ShadowTexC)[1];
	s[9]  =  shadowMaps[1].Sample(gTriLinearSamp, ShadowTexC)[2];
	s[8]  =  shadowMaps[1].Sample(gTriLinearSamp, ShadowTexC)[3];
	s[7]  =  shadowMaps[2].Sample(gTriLinearSamp, ShadowTexC)[0];
	s[6]  =  shadowMaps[2].Sample(gTriLinearSamp, ShadowTexC)[1];
	s[5] =   shadowMaps[2].Sample(gTriLinearSamp, ShadowTexC)[2];
	s[4] =   shadowMaps[2].Sample(gTriLinearSamp, ShadowTexC)[3];
	s[3] =   shadowMaps[3].Sample(gTriLinearSamp, ShadowTexC)[0];
	s[2] =   shadowMaps[3].Sample(gTriLinearSamp, ShadowTexC)[1];
	s[1] =   shadowMaps[3].Sample(gTriLinearSamp, ShadowTexC)[2];
	s[0] =   shadowMaps[3].Sample(gTriLinearSamp, ShadowTexC)[3];

	float startShadow = 1.0 - saturate(s[intd_min]);
	float endShadow   = 1.0 - saturate(s[intd_max]);

	float shadow = lerp(startShadow, endShadow, dist - floor(dist));

	if (intd_max > 15 || intd_min < 0)
	{
		c = float3(0,100,0);
	}

	return float4(c * shadow, a);
}

float4 DEFAULT_PS(VS_DEFAULT_OUT pIn) : SV_Target
{
	SurfaceInfo info;
	info.pos = pIn.posW;
	info.normal = float3(0,1,0);//pIn.normal;
	info.spec = float4(1,1,1,1);

	Light light;
	light.pos = gLightPosW;//float3(10,5,10);
	light.dir = float3(0,0,0);
	light.ambient = float4(0,0,0,1);
	light.diffuse = float4(2.5,2.5,2.5,2.5);
	light.spec = float4(0,0,0,1);
	light.spotPower = 0.0;
	light.range = 100.0;

	if (sin(pIn.posW.x * 2.0) * sin(pIn.posW.z * 2.0) > 0)
	{
		info.diffuse = float4(0.5,0.45,0.45,1); 
	}
	else
	{
		info.diffuse = float4(0.5,0.5,0.5,1);
	}

	float4 lightingPosition = GetPositionFromLight(float4(pIn.posW,1));
   
    float2 ShadowTexC = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );
    ShadowTexC.y = 1.0f - ShadowTexC.y;

	float shadowdepth = 1.0 - shadowMaps[0].Sample(gTriLinearSamp, ShadowTexC)[0];

	return float4(PointLight(info, light, gEyePosW) * shadowdepth, 1.0);
}

// DEBUG DRAW SHADERS

struct VS_DEBUG_IN
{
	float3 posL : POSITION;
};

struct VS_DEBUG_OUT
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float2 texC : TEXCOORD;
};

VS_DEBUG_OUT DEBUG_VS(VS_DEBUG_IN vIn)
{
	VS_DEBUG_OUT vOut;

	vOut.posH = float4(vIn.posL, 1);
	vOut.posW = vIn.posL;
	vOut.texC = (vIn.posL.xy + float2(1,1)) * 0.5;

	return vOut;
}

float4 DEBUG_PS(VS_DEBUG_OUT pIn) : SV_Target
{
	float4 c = shadowMaps[0].Sample(gTriLinearSamp, pIn.texC);
	c.a = 1;
	return c;//float4(pIn.texC,0,1);
}