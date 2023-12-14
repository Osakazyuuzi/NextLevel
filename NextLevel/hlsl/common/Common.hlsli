//=== 定数バッファを定義
// ワールドマトリクス
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
// ビューマトリクス
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
// プロジェクションマトリクス
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// マテリアルクラス
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 Dummy;
};
// マテリアル
cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

// 頂点バッファ
struct VS_IN
{
    float4 Position : POSITION0; // 頂点座標
    float4 Normal : NORMAL0; // 法線
    float2 TexCoord : TEXCOORD0; // UV座標
};

// ピクセルバッファ
struct PS_IN
{
    float4 Position : SV_POSITION; // 変換後座標
    float3 Normal : NORMAL0; // 法線
    float2 TexCoord : TEXCOORD0; // UV座標
    float4 WorldPos : POSITION0; // ワールド座標
    float4 ScreenPos : TEXCOORD1; // スクリーン座標
};