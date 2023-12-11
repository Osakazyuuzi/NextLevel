#ifndef ___SRC_COMMON_CORE_RESOURCE_MODEL_MODELDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_MODEL_MODELDATA_H___

#include <unordered_map>
#include <map>

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

#include <d3d11.h>
#include <wrl.h>
#include <string>

#include <DirectXMath.h>

struct aiScene;
struct aiNode;

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief 変形後頂点構造体
		*/
		struct DEFORM_VERTEX
		{
			aiVector3D Position;
			aiVector3D Normal;
			int BoneNum;
			std::string BoneName[4];
			float BoneWeight[4];
		};

		/**
		* @brief ボーン構造体
		*/
		struct BONE
		{
			aiMatrix4x4 Matrix;
			aiMatrix4x4 AnimationMatrix;
			aiMatrix4x4 OffsetMatrix;
		};

		/**
		* @class ModelData
		* @brief モデルデータを管理するクラス。
		*/
		class ModelData {
		public:
			struct Vertex
			{
				DirectX::XMFLOAT4 Position;
				DirectX::XMFLOAT4 Normal;
				float padding[2];
				DirectX::XMFLOAT2 TexCoord;
			};
		private:
			const aiScene* m_AiScene = nullptr;
			std::unordered_map<std::string, const aiScene*> m_Animation;

			ID3D11Buffer** m_VertexBuffer = nullptr;
			ID3D11Buffer** m_IndexBuffer = nullptr;

			std::vector<DEFORM_VERTEX>* m_DeformVertex = nullptr;//変形後頂点データ
			std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）

			void CreateBone(aiNode* Node);
			void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);
			aiScene* ImportFile(const char* pFile, unsigned int pFlags);

			std::unordered_map<const char*, std::string> m_animInfo;

			std::string m_fileName;

			std::map<std::string, int> m_TextureID;
			int m_MaterialID;
		public:

			bool Load(std::string FileName);
			void LoadAnimation(std::string FileName, const char* AnimationName);
			void Uninit();
			void Update(const char* AnimationName, int Frame);
			void Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate);
			void Draw();

			std::string GetFileName() {
				return m_fileName;
			}
			std::map<std::string, int>& GetTextureID()
			{
				return m_TextureID;
			}
			void SetMateiralID(int id) { m_MaterialID = id; }
			int& GetMaterialID()
			{
				return m_MaterialID;
			}
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_MODEL_MODELDATA_H___