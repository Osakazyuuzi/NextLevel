//=== �萔�o�b�t�@���`
// ���[���h�}�g���N�X
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
// �r���[�}�g���N�X
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
// �v���W�F�N�V�����}�g���N�X
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

// �}�e���A���N���X
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
// �}�e���A��
cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

// ���_�o�b�t�@
struct VS_IN
{
    float4 Position : POSITION0; // ���_���W
    float4 Normal : NORMAL0; // �@��
    float2 TexCoord : TEXCOORD0; // UV���W
};

// �s�N�Z���o�b�t�@
struct PS_IN
{
    float4 Position : SV_POSITION; // �ϊ�����W
    float3 Normal : NORMAL0; // �@��
    float2 TexCoord : TEXCOORD0; // UV���W
    float4 WorldPos : POSITION0; // ���[���h���W
    float4 ScreenPos : TEXCOORD1; // �X�N���[�����W
};