#include "../common/common.hlsli"

void main(in VS_IN In, out PS_IN Out)
{
    // ローカル座標をスクリーン座標に
    Out.Position = In.Position; // ローカル座標
    Out.Position = mul(Out.Position, World); // ワールド座標
    Out.WorldPos = Out.Position; // ワールド座標を設定
    Out.Position = mul(Out.Position, View); // ビュー座標
    Out.Position = mul(Out.Position, Projection); // プロジェクション座標
    Out.ScreenPos = Out.Position; // スクリーン座標を設定
    
    // UV座標を設定
    Out.TexCoord = In.TexCoord;
    
    // 法線を設定
    Out.Normal = mul(World, In.Normal);
}