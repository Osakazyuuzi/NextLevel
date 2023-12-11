#include "ModelData.h"

#include <assimp/scene.h>
#include <io.h>

#include "src/engine/graphics/windows/Graphics.h"
#include "src/common/core/resource/material/MaterialData.h"
#include "src/common/core/resource/texture/TextureManager.h"
#include "src/common/core/resource/material/MaterialManager.h"
#include "src/common/core/utils/CreateTexture.h"

#ifdef _DEBUG
#if _MSC_VER >= 1930
#pragma comment(lib, "assimp-vc143-mtd")
#elif _MSC_VER >= 1920
#pragma comment(lib, "assimp-vc142-mtd")
#else
#pragma comment(lib, "assimp-vc141-mtd")
#endif
#pragma comment(lib, "IrrXMLd")
#pragma comment(lib, "zlibstaticd")
#else
#if _MSC_VER >= 1930
#pragma comment(lib, "assimp-vc143-mt")
#elif _MSC_VER >= 1920
#pragma comment(lib, "assimp-vc142-mt")
#else
#pragma comment(lib, "assimp-vc141-mt")
#endif
#pragma comment(lib, "IrrXML")
#pragma comment(lib, "zlibstatic")
#endif

namespace NextLevel
{
	namespace resource
	{
		void ModelData::Draw()
		{
			// プリミティブトポロジ設定
			graphics::Graphics::GetInstance().GetRenderContext()->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// マテリアル設定
			graphics::Graphics::GetInstance().GetRenderContext()->SetMaterial(*(MaterialManager::GetInstance().GetMaterialData(m_MaterialID).value().get()));

			for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
			{
				aiMesh* mesh = m_AiScene->mMeshes[m];
				aiString path;
				aiMaterial* pMaterial = m_AiScene->mMaterials[mesh->mMaterialIndex];
				pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path);
				std::string textureName(path.data);

				// テクスチャIDを取得
				auto it = m_TextureID.find(textureName);
				if (it != m_TextureID.end()) {
					// テクスチャ設定
					graphics::Graphics::GetInstance().GetRenderContext()->SetTexture(0, resource::TextureManager::GetInstance().GetTextureData(it->second)->GetResource().Get());
				}

				// 頂点バッファ設定
				graphics::Graphics::GetInstance().GetRenderContext()->SetVertexBuffer(m_VertexBuffer[m], sizeof(Vertex));

				// インデックスバッファ設定
				graphics::Graphics::GetInstance().GetRenderContext()->SetIndexBuffer(m_IndexBuffer[m]);

				// ポリゴン描画
				graphics::Graphics::GetInstance().GetRenderContext()->DrawIndexed(mesh->mNumFaces * 3);
			}
		}


		aiScene* ModelData::ImportFile(const char* pFile, unsigned int pFlags)
		{
			// default pp steps
			static unsigned int ppsteps =
				// aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
				aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
				aiProcess_ValidateDataStructure | // perform a full validation of the loader's output
				aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
				aiProcess_RemoveRedundantMaterials | // remove redundant materials
				aiProcess_FindDegenerates | // remove degenerated polygons from the import
				aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
				aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
				aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
				aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
				aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
				aiProcess_OptimizeMeshes | // join small meshes, if possible;
				aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
				0;

			aiScene* pScene;
			aiPropertyStore* props = aiCreatePropertyStore();
			aiSetImportPropertyInteger(props, AI_CONFIG_IMPORT_TER_MAKE_UVS, 1);
			aiSetImportPropertyFloat(props, AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
			aiSetImportPropertyInteger(props, AI_CONFIG_PP_SBP_REMOVE, 0);

			aiSetImportPropertyInteger(props, AI_CONFIG_GLOB_MEASURE_TIME, 1);
			// aiSetImportPropertyInteger(props, AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

			// Call ASSIMPs C-API to load the file
			pScene = (aiScene*)aiImportFileExWithProperties(pFile,
				pFlags |
				ppsteps | /* configurable pp steps */
				aiProcess_GenSmoothNormals | // generate smooth normal vectors if not existing
				aiProcess_SplitLargeMeshes | // split large, unrenderable meshes into submeshes
				aiProcess_Triangulate | // triangulate polygons with more than 3 edges
				aiProcess_SortByPType | // make 'clean' meshes which consist of a single typ of primitives
				0,
				nullptr,
				props);

			aiReleasePropertyStore(props);

			return pScene;
		}


		bool ModelData::Load(std::string FileName)
		{
			// ファイルが存在するか確認
			struct stat buffer;
			if (stat(FileName.c_str(), &buffer) != 0) {
				// ファイルが存在しない場合
				return false;
			}

			m_fileName = FileName;

			//	char szPath[_MAX_PATH];
			char szDrive[_MAX_DRIVE], szDir[_MAX_DIR], szFName[_MAX_FNAME], szExt[_MAX_EXT];
			_splitpath_s(FileName.c_str(), szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFName, _MAX_FNAME, szExt, _MAX_EXT);

			const std::string modelPath(FileName);

			m_AiScene = ImportFile(FileName.c_str(), aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
			if (!m_AiScene) {
				return false;
			}

			m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
			m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];


			//変形後頂点配列生成
			m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];

			//再帰的にボーン生成
			CreateBone(m_AiScene->mRootNode);



			for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
			{
				aiMesh* mesh = m_AiScene->mMeshes[m];

				// 頂点バッファ生成
				{
					Vertex* vertex = new Vertex[mesh->mNumVertices];

					for (unsigned int v = 0; v < mesh->mNumVertices; v++)
					{
						vertex[v].Position = DirectX::XMFLOAT4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.0f);
						vertex[v].Normal = DirectX::XMFLOAT4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.0f);
						vertex[v].TexCoord = DirectX::XMFLOAT2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
					}

					D3D11_BUFFER_DESC bd;
					ZeroMemory(&bd, sizeof(bd));
					bd.Usage = D3D11_USAGE_DYNAMIC;
					bd.ByteWidth = sizeof(Vertex) * mesh->mNumVertices;
					bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
					bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

					D3D11_SUBRESOURCE_DATA sd;
					ZeroMemory(&sd, sizeof(sd));
					sd.pSysMem = vertex;

					graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

					delete[] vertex;
				}


				// インデックスバッファ生成
				{
					unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

					for (unsigned int f = 0; f < mesh->mNumFaces; f++)
					{
						const aiFace* face = &mesh->mFaces[f];

						assert(face->mNumIndices == 3);

						index[f * 3 + 0] = face->mIndices[0];
						index[f * 3 + 1] = face->mIndices[1];
						index[f * 3 + 2] = face->mIndices[2];
					}

					D3D11_BUFFER_DESC bd;
					ZeroMemory(&bd, sizeof(bd));
					bd.Usage = D3D11_USAGE_DEFAULT;
					bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
					bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
					bd.CPUAccessFlags = 0;

					D3D11_SUBRESOURCE_DATA sd;
					ZeroMemory(&sd, sizeof(sd));
					sd.pSysMem = index;

					graphics::Graphics::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

					delete[] index;
				}



				//変形後頂点データ初期化
				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					DEFORM_VERTEX deformVertex;
					deformVertex.Position = mesh->mVertices[v];
					deformVertex.Normal = mesh->mNormals[v];
					deformVertex.BoneNum = 0;

					for (unsigned int b = 0; b < 4; b++)
					{
						deformVertex.BoneName[b] = "";
						deformVertex.BoneWeight[b] = 0.0f;
					}

					m_DeformVertex[m].push_back(deformVertex);
				}


				//ボーンデータ初期化
				for (unsigned int b = 0; b < mesh->mNumBones; b++)
				{
					aiBone* bone = mesh->mBones[b];

					m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

					//変形後頂点にボーンデータ格納
					for (unsigned int w = 0; w < bone->mNumWeights; w++)
					{
						aiVertexWeight weight = bone->mWeights[w];

						int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

						m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
						m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
						m_DeformVertex[m][weight.mVertexId].BoneNum++;

						assert(m_DeformVertex[m][weight.mVertexId].BoneNum <= 4);
					}
				}
			}

			//テクスチャ読み込み
			std::string dir = FileName;
			dir = dir.substr(0, dir.find_last_of('/') + 1);

			for (unsigned int i = 0; i < m_AiScene->mNumMaterials; i++) {
				const aiMaterial* material = m_AiScene->mMaterials[i];
				aiString path;

				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					// そのまま探索
					int textureID = resource::TextureManager::GetInstance().GetTextureID(path.C_Str()).value();

					// モデルと同じ階層を探索
					if (textureID < 0) {
						std::string file = dir;
						file += path.C_Str();
						resource::TextureManager::GetInstance().GetTextureID(file.c_str()).value();
					}
					else
					{
						// 登録
						m_TextureID[path.C_Str()] = textureID;
					}

					// ファイル名のみで探索
					if (textureID < 0) {
						std::string file = path.C_Str();
						size_t idx = file.find_last_of('\\');
						if (idx != std::string::npos)
						{
							file = file.substr(idx + 1);
							file = dir + file;
							textureID = resource::TextureManager::GetInstance().GetTextureID(file.c_str()).value();
						}
					}
					else
					{
						// 登録
						m_TextureID[path.C_Str()] = textureID;
					}

					// 読み込み失敗
					if (textureID < 0) {
						return false;
					}
					else
					{
						// 登録
						m_TextureID[path.C_Str()] = textureID;
					}
				}
			}

			return true;
		}


		void ModelData::LoadAnimation(std::string FileName, const char* Name)
		{
			m_Animation[Name] = ImportFile(FileName.c_str(), aiProcess_ConvertToLeftHanded);
			assert(m_Animation[Name]);
			m_animInfo[Name] = FileName;
		}


		void ModelData::CreateBone(aiNode* node)
		{
			BONE bone;

			m_Bone[node->mName.C_Str()] = bone;

			for (unsigned int n = 0; n < node->mNumChildren; n++)
			{
				CreateBone(node->mChildren[n]);
			}

		}


		void ModelData::Uninit()
		{
			for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
			{
				m_VertexBuffer[m]->Release();
				m_IndexBuffer[m]->Release();
			}

			delete[] m_VertexBuffer;
			delete[] m_IndexBuffer;

			delete[] m_DeformVertex;

			aiReleaseImport(m_AiScene);


			for (std::pair<const std::string, const aiScene*> pair : m_Animation)
			{
				aiReleaseImport(pair.second);
			}

		}


		void ModelData::Update(const char* AnimationName, int Frame)
		{
			if (m_Animation.count(AnimationName) == 0)
				return;

			if (!m_Animation[AnimationName]->HasAnimations())
				return;

			//アニメーションデータからボーンマトリクス算出
			aiAnimation* animation = m_Animation[AnimationName]->mAnimations[0];

			for (auto pair : m_Bone)
			{
				BONE* bone = &m_Bone[pair.first];

				aiNodeAnim* nodeAnim = nullptr;

				for (unsigned int c = 0; c < animation->mNumChannels; c++)
				{
					if (animation->mChannels[c]->mNodeName == aiString(pair.first))
					{
						nodeAnim = animation->mChannels[c];
						break;
					}
				}


				int f;

				aiQuaternion rot(1.0f, 0.0f, 0.0f, 0.0f);
				aiVector3D pos(0.0f, 0.0f, 0.0f);

				if (nodeAnim)
				{
					f = Frame % nodeAnim->mNumRotationKeys;
					rot = nodeAnim->mRotationKeys[f].mValue;
				}


				bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

			}



			//再帰的にボーンマトリクスを更新
			aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI_F, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
			UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);


			//頂点変換（CPUスキニング）
			for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
			{
				aiMesh* mesh = m_AiScene->mMeshes[m];

				D3D11_MAPPED_SUBRESOURCE ms;
				graphics::Graphics::GetInstance().GetDeviceContext()->Map(m_VertexBuffer[m], 0,
					D3D11_MAP_WRITE_DISCARD, 0, &ms);

				Vertex* vertex = (Vertex*)ms.pData;

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

					aiMatrix4x4 matrix[4];
					aiMatrix4x4 outMatrix;
					matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
					matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
					matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
					matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

					//ウェイトを考慮してマトリクス算出
					{
						outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
							+ matrix[1].a1 * deformVertex->BoneWeight[1]
							+ matrix[2].a1 * deformVertex->BoneWeight[2]
							+ matrix[3].a1 * deformVertex->BoneWeight[3];

						outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
							+ matrix[1].a2 * deformVertex->BoneWeight[1]
							+ matrix[2].a2 * deformVertex->BoneWeight[2]
							+ matrix[3].a2 * deformVertex->BoneWeight[3];

						outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
							+ matrix[1].a3 * deformVertex->BoneWeight[1]
							+ matrix[2].a3 * deformVertex->BoneWeight[2]
							+ matrix[3].a3 * deformVertex->BoneWeight[3];

						outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
							+ matrix[1].a4 * deformVertex->BoneWeight[1]
							+ matrix[2].a4 * deformVertex->BoneWeight[2]
							+ matrix[3].a4 * deformVertex->BoneWeight[3];



						outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
							+ matrix[1].b1 * deformVertex->BoneWeight[1]
							+ matrix[2].b1 * deformVertex->BoneWeight[2]
							+ matrix[3].b1 * deformVertex->BoneWeight[3];

						outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
							+ matrix[1].b2 * deformVertex->BoneWeight[1]
							+ matrix[2].b2 * deformVertex->BoneWeight[2]
							+ matrix[3].b2 * deformVertex->BoneWeight[3];

						outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
							+ matrix[1].b3 * deformVertex->BoneWeight[1]
							+ matrix[2].b3 * deformVertex->BoneWeight[2]
							+ matrix[3].b3 * deformVertex->BoneWeight[3];

						outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
							+ matrix[1].b4 * deformVertex->BoneWeight[1]
							+ matrix[2].b4 * deformVertex->BoneWeight[2]
							+ matrix[3].b4 * deformVertex->BoneWeight[3];



						outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
							+ matrix[1].c1 * deformVertex->BoneWeight[1]
							+ matrix[2].c1 * deformVertex->BoneWeight[2]
							+ matrix[3].c1 * deformVertex->BoneWeight[3];

						outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
							+ matrix[1].c2 * deformVertex->BoneWeight[1]
							+ matrix[2].c2 * deformVertex->BoneWeight[2]
							+ matrix[3].c2 * deformVertex->BoneWeight[3];

						outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
							+ matrix[1].c3 * deformVertex->BoneWeight[1]
							+ matrix[2].c3 * deformVertex->BoneWeight[2]
							+ matrix[3].c3 * deformVertex->BoneWeight[3];

						outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
							+ matrix[1].c4 * deformVertex->BoneWeight[1]
							+ matrix[2].c4 * deformVertex->BoneWeight[2]
							+ matrix[3].c4 * deformVertex->BoneWeight[3];



						outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
							+ matrix[1].d1 * deformVertex->BoneWeight[1]
							+ matrix[2].d1 * deformVertex->BoneWeight[2]
							+ matrix[3].d1 * deformVertex->BoneWeight[3];

						outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
							+ matrix[1].d2 * deformVertex->BoneWeight[1]
							+ matrix[2].d2 * deformVertex->BoneWeight[2]
							+ matrix[3].d2 * deformVertex->BoneWeight[3];

						outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
							+ matrix[1].d3 * deformVertex->BoneWeight[1]
							+ matrix[2].d3 * deformVertex->BoneWeight[2]
							+ matrix[3].d3 * deformVertex->BoneWeight[3];

						outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
							+ matrix[1].d4 * deformVertex->BoneWeight[1]
							+ matrix[2].d4 * deformVertex->BoneWeight[2]
							+ matrix[3].d4 * deformVertex->BoneWeight[3];

					}

					deformVertex->Position = mesh->mVertices[v];
					deformVertex->Position *= outMatrix;


					//法線変換用に移動成分を削除
					outMatrix.a4 = 0.0f;
					outMatrix.b4 = 0.0f;
					outMatrix.c4 = 0.0f;

					deformVertex->Normal = mesh->mNormals[v];
					deformVertex->Normal *= outMatrix;


					//頂点バッファへ書き込み
					vertex[v].Position.x = deformVertex->Position.x;
					vertex[v].Position.y = deformVertex->Position.y;
					vertex[v].Position.z = deformVertex->Position.z;

					vertex[v].Normal.x = deformVertex->Normal.x;
					vertex[v].Normal.y = deformVertex->Normal.y;
					vertex[v].Normal.z = deformVertex->Normal.z;

					vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
					vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;
				}
				graphics::Graphics::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
			}
		}


		void ModelData::Update(const char* AnimationName1, int Frame1, const char* AnimationName2, int Frame2, float BlendRate)
		{
			if (m_Animation.count(AnimationName1) == 0)
				return;
			if (m_Animation.count(AnimationName2) == 0)
				return;

			if (!m_Animation[AnimationName1]->HasAnimations())
				return;
			if (!m_Animation[AnimationName2]->HasAnimations())
				return;

			//アニメーションデータからボーンマトリクス算出
			aiAnimation* animation1 = m_Animation[AnimationName1]->mAnimations[0];
			aiAnimation* animation2 = m_Animation[AnimationName2]->mAnimations[0];



			for (auto pair : m_Bone)
			{
				BONE* bone = &m_Bone[pair.first];


				aiNodeAnim* nodeAnim1 = nullptr;

				for (unsigned int c = 0; c < animation1->mNumChannels; c++)
				{
					if (animation1->mChannels[c]->mNodeName == aiString(pair.first))
					{
						nodeAnim1 = animation1->mChannels[c];
						break;
					}
				}


				aiNodeAnim* nodeAnim2 = nullptr;

				for (unsigned int c = 0; c < animation2->mNumChannels; c++)
				{
					if (animation2->mChannels[c]->mNodeName == aiString(pair.first))
					{
						nodeAnim2 = animation2->mChannels[c];
						break;
					}
				}


				int f;

				aiQuaternion rot1;
				aiVector3D pos1;

				if (nodeAnim1)
				{
					f = Frame1 % nodeAnim1->mNumRotationKeys;//簡易実装
					rot1 = nodeAnim1->mRotationKeys[f].mValue;

					f = Frame1 % nodeAnim1->mNumPositionKeys;//簡易実装
					pos1 = nodeAnim1->mPositionKeys[f].mValue;
				}


				aiQuaternion rot2;
				aiVector3D pos2;

				if (nodeAnim2)
				{
					f = Frame2 % nodeAnim2->mNumRotationKeys;//簡易実装
					rot2 = nodeAnim2->mRotationKeys[f].mValue;

					f = Frame2 % nodeAnim2->mNumPositionKeys;//簡易実装
					pos2 = nodeAnim2->mPositionKeys[f].mValue;
				}


				aiVector3D pos(0.0f, 0.0f, 0.0f);

				aiQuaternion rot;
				aiQuaternion::Interpolate(rot, rot1, rot2, BlendRate);//球面線形補間

				bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

			}





			//再帰的にボーンマトリクスを更新
			aiMatrix4x4 rootMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), aiQuaternion(AI_MATH_PI_F, 0.0f, 0.0f), aiVector3D(0.0f, 0.0f, 0.0f));
			UpdateBoneMatrix(m_AiScene->mRootNode, rootMatrix);


			//頂点変換（CPUスキニング）
			for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
			{
				aiMesh* mesh = m_AiScene->mMeshes[m];

				D3D11_MAPPED_SUBRESOURCE ms;
				graphics::Graphics::GetInstance().GetDeviceContext()->Map(m_VertexBuffer[m], 0,
					D3D11_MAP_WRITE_DISCARD, 0, &ms);

				Vertex* vertex = (Vertex*)ms.pData;

				for (unsigned int v = 0; v < mesh->mNumVertices; v++)
				{
					DEFORM_VERTEX* deformVertex = &m_DeformVertex[m][v];

					aiMatrix4x4 matrix[4];
					aiMatrix4x4 outMatrix;
					matrix[0] = m_Bone[deformVertex->BoneName[0]].Matrix;
					matrix[1] = m_Bone[deformVertex->BoneName[1]].Matrix;
					matrix[2] = m_Bone[deformVertex->BoneName[2]].Matrix;
					matrix[3] = m_Bone[deformVertex->BoneName[3]].Matrix;

					//ウェイトを考慮してマトリクス算出
					{
						outMatrix.a1 = matrix[0].a1 * deformVertex->BoneWeight[0]
							+ matrix[1].a1 * deformVertex->BoneWeight[1]
							+ matrix[2].a1 * deformVertex->BoneWeight[2]
							+ matrix[3].a1 * deformVertex->BoneWeight[3];

						outMatrix.a2 = matrix[0].a2 * deformVertex->BoneWeight[0]
							+ matrix[1].a2 * deformVertex->BoneWeight[1]
							+ matrix[2].a2 * deformVertex->BoneWeight[2]
							+ matrix[3].a2 * deformVertex->BoneWeight[3];

						outMatrix.a3 = matrix[0].a3 * deformVertex->BoneWeight[0]
							+ matrix[1].a3 * deformVertex->BoneWeight[1]
							+ matrix[2].a3 * deformVertex->BoneWeight[2]
							+ matrix[3].a3 * deformVertex->BoneWeight[3];

						outMatrix.a4 = matrix[0].a4 * deformVertex->BoneWeight[0]
							+ matrix[1].a4 * deformVertex->BoneWeight[1]
							+ matrix[2].a4 * deformVertex->BoneWeight[2]
							+ matrix[3].a4 * deformVertex->BoneWeight[3];



						outMatrix.b1 = matrix[0].b1 * deformVertex->BoneWeight[0]
							+ matrix[1].b1 * deformVertex->BoneWeight[1]
							+ matrix[2].b1 * deformVertex->BoneWeight[2]
							+ matrix[3].b1 * deformVertex->BoneWeight[3];

						outMatrix.b2 = matrix[0].b2 * deformVertex->BoneWeight[0]
							+ matrix[1].b2 * deformVertex->BoneWeight[1]
							+ matrix[2].b2 * deformVertex->BoneWeight[2]
							+ matrix[3].b2 * deformVertex->BoneWeight[3];

						outMatrix.b3 = matrix[0].b3 * deformVertex->BoneWeight[0]
							+ matrix[1].b3 * deformVertex->BoneWeight[1]
							+ matrix[2].b3 * deformVertex->BoneWeight[2]
							+ matrix[3].b3 * deformVertex->BoneWeight[3];

						outMatrix.b4 = matrix[0].b4 * deformVertex->BoneWeight[0]
							+ matrix[1].b4 * deformVertex->BoneWeight[1]
							+ matrix[2].b4 * deformVertex->BoneWeight[2]
							+ matrix[3].b4 * deformVertex->BoneWeight[3];



						outMatrix.c1 = matrix[0].c1 * deformVertex->BoneWeight[0]
							+ matrix[1].c1 * deformVertex->BoneWeight[1]
							+ matrix[2].c1 * deformVertex->BoneWeight[2]
							+ matrix[3].c1 * deformVertex->BoneWeight[3];

						outMatrix.c2 = matrix[0].c2 * deformVertex->BoneWeight[0]
							+ matrix[1].c2 * deformVertex->BoneWeight[1]
							+ matrix[2].c2 * deformVertex->BoneWeight[2]
							+ matrix[3].c2 * deformVertex->BoneWeight[3];

						outMatrix.c3 = matrix[0].c3 * deformVertex->BoneWeight[0]
							+ matrix[1].c3 * deformVertex->BoneWeight[1]
							+ matrix[2].c3 * deformVertex->BoneWeight[2]
							+ matrix[3].c3 * deformVertex->BoneWeight[3];

						outMatrix.c4 = matrix[0].c4 * deformVertex->BoneWeight[0]
							+ matrix[1].c4 * deformVertex->BoneWeight[1]
							+ matrix[2].c4 * deformVertex->BoneWeight[2]
							+ matrix[3].c4 * deformVertex->BoneWeight[3];



						outMatrix.d1 = matrix[0].d1 * deformVertex->BoneWeight[0]
							+ matrix[1].d1 * deformVertex->BoneWeight[1]
							+ matrix[2].d1 * deformVertex->BoneWeight[2]
							+ matrix[3].d1 * deformVertex->BoneWeight[3];

						outMatrix.d2 = matrix[0].d2 * deformVertex->BoneWeight[0]
							+ matrix[1].d2 * deformVertex->BoneWeight[1]
							+ matrix[2].d2 * deformVertex->BoneWeight[2]
							+ matrix[3].d2 * deformVertex->BoneWeight[3];

						outMatrix.d3 = matrix[0].d3 * deformVertex->BoneWeight[0]
							+ matrix[1].d3 * deformVertex->BoneWeight[1]
							+ matrix[2].d3 * deformVertex->BoneWeight[2]
							+ matrix[3].d3 * deformVertex->BoneWeight[3];

						outMatrix.d4 = matrix[0].d4 * deformVertex->BoneWeight[0]
							+ matrix[1].d4 * deformVertex->BoneWeight[1]
							+ matrix[2].d4 * deformVertex->BoneWeight[2]
							+ matrix[3].d4 * deformVertex->BoneWeight[3];

					}

					deformVertex->Position = mesh->mVertices[v];
					deformVertex->Position *= outMatrix;


					//法線変換用に移動成分を削除
					outMatrix.a4 = 0.0f;
					outMatrix.b4 = 0.0f;
					outMatrix.c4 = 0.0f;

					deformVertex->Normal = mesh->mNormals[v];
					deformVertex->Normal *= outMatrix;


					//頂点バッファへ書き込み
					vertex[v].Position.x = deformVertex->Position.x;
					vertex[v].Position.y = deformVertex->Position.y;
					vertex[v].Position.z = deformVertex->Position.z;

					vertex[v].Normal.x = deformVertex->Normal.x;
					vertex[v].Normal.y = deformVertex->Normal.y;
					vertex[v].Normal.z = deformVertex->Normal.z;

					vertex[v].TexCoord.x = mesh->mTextureCoords[0][v].x;
					vertex[v].TexCoord.y = mesh->mTextureCoords[0][v].y;
				}
				graphics::Graphics::GetInstance().GetDeviceContext()->Unmap(m_VertexBuffer[m], 0);
			}
		}


		void ModelData::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
		{
			BONE* bone = &m_Bone[node->mName.C_Str()];

			//マトリクスの乗算順番に注意
			aiMatrix4x4 worldMatrix;

			worldMatrix *= matrix;

			worldMatrix *= node->mTransformation;

			worldMatrix *= bone->AnimationMatrix;


			bone->Matrix = worldMatrix;
			bone->Matrix *= bone->OffsetMatrix;

			for (unsigned int n = 0; n < node->mNumChildren; n++)
			{
				UpdateBoneMatrix(node->mChildren[n], worldMatrix);
			}
		}

	}
}