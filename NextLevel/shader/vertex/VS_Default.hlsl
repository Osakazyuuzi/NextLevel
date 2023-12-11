#include "../common/common.hlsli"

void main(in VS_IN In, out PS_IN Out)
{
    // ���[�J�����W���X�N���[�����W��
    Out.Position = In.Position; // ���[�J�����W
    Out.Position = mul(Out.Position, World); // ���[���h���W
    Out.WorldPos = Out.Position; // ���[���h���W��ݒ�
    Out.Position = mul(Out.Position, View); // �r���[���W
    Out.Position = mul(Out.Position, Projection); // �v���W�F�N�V�������W
    Out.ScreenPos = Out.Position; // �X�N���[�����W��ݒ�
    
    // UV���W��ݒ�
    Out.TexCoord = In.TexCoord;
    
    // �@����ݒ�
    Out.Normal = mul(World, In.Normal);
}