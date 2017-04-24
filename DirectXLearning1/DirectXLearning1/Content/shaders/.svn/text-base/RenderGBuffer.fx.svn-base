shared float4x4 World;
shared float4x4 View;
shared float4x4 Projection;
shared float4x4 LightViewProj;

float specularIntensity = 0.8f;
float specularPower = 0.5f; 

float4 SelectedTint; 

texture2D diffuseTexture;
sampler diffuseSampler = sampler_state
{
        MagFilter = LINEAR;
        MinFilter = LINEAR;
        MipFilter = LINEAR;
        AddressU = WRAP;
        AddressV = WRAP;
        
        Texture = <diffuseTexture>;
};

texture2D specularTexture;
sampler specularSampler = sampler_state
{
        MagFilter = LINEAR;
        MinFilter = LINEAR;
        MipFilter = LINEAR;
        AddressU = WRAP;
        AddressV = WRAP;
        
        Texture = <specularTexture>;
};

texture2D NormalMap;
sampler NormalMapSampler = sampler_state
{
        MagFilter = LINEAR;
        MinFilter = LINEAR;
        MipFilter = LINEAR;
        AddressU = WRAP;
        AddressV = WRAP;
        
        Texture = <NormalMap>;
};

struct CreateShadowMap_VSOut
{
    float4 Position : POSITION0;
    float2 Depth : TEXCOORD1;
};

CreateShadowMap_VSOut CreateShadowMap_VertexShader(float4 Position: POSITION0)
{
    CreateShadowMap_VSOut Out;
    
    Out.Position = mul(Position, mul(World, LightViewProj)); 
    Out.Depth.x = Out.Position.z;
    Out.Depth.y = Out.Position.w; 
    
    return Out;
}

float4 CreateShadowMap_PixelShader(CreateShadowMap_VSOut input) : COLOR0
{ 
    float depth = 1 - (input.Depth.x / input.Depth.y);
    return float4(depth,0,0,1);
}

struct VertexShaderInput
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
    float2 Depth : TEXCOORD1;
    float3x3 tangentToWorld : TEXCOORD2;

};

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
    VertexShaderOutput output;

    float4 worldPosition = mul(input.Position, World);
    float4 viewPosition = mul(worldPosition, View);
    output.Position = mul(viewPosition, Projection);

    output.TexCoord = input.TexCoord;
    
    output.tangentToWorld[0] = mul(input.Tangent, World);
    output.tangentToWorld[1] = mul(cross(input.Tangent, input.Normal), World);
    output.tangentToWorld[2] = mul(input.Normal, World);
	
    output.Depth.x = output.Position.z;
    output.Depth.y = output.Position.w;
    return output;
}
struct PixelShaderOutput
{
    half4 Color : COLOR0;
    half4 Normal : COLOR1;
    half4 Depth : COLOR2;
};

PixelShaderOutput PixelShaderFunction(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.Color = tex2D(diffuseSampler, input.TexCoord) * SelectedTint; 
    output.Color.a = specularIntensity;
    
    float3 normalFromMap = tex2D(NormalMapSampler, input.TexCoord);
    normalFromMap = 2.0f * normalFromMap - 1.0f;
    normalFromMap = mul(normalFromMap, input.tangentToWorld);
    normalFromMap = normalize(normalFromMap);
    output.Normal.rgb = 0.5f * (normalFromMap + 1.0f);
    
    output.Normal.a = specularPower;
    
    output.Depth = input.Depth.x / input.Depth.y;
    return output;
}

technique CreateGBuffer
{
    pass Pass1
    {
        VertexShader = compile vs_2_0 VertexShaderFunction();
        PixelShader = compile ps_2_0 PixelShaderFunction();
    }
}

technique CreateShadowMap
{
    pass Pass1
    {
        VertexShader = compile vs_3_0 CreateShadowMap_VertexShader();
        PixelShader = compile ps_3_0 CreateShadowMap_PixelShader();
    }
}

