#include "MaterialManager.h"

#include "Json/json.hpp"

#include <iostream>
#include <fstream>

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief Jsonファイルにデータを保存します。
		*/
		void MaterialManager::SaveToJson()
		{
			nlohmann::json jsonFile;

			for (const auto& pair : m_materialMap) {
				nlohmann::json materialJson;
				const MaterialData& material = *(pair.second.get());
				
				materialJson["Ambient"] = { material.m_Ambient.x, material.m_Ambient.y, material.m_Ambient.z, material.m_Ambient.w };
				materialJson["Diffuse"] = { material.m_Diffuse.x, material.m_Diffuse.y, material.m_Diffuse.z, material.m_Diffuse.w };
				materialJson["Specular"] = { material.m_Specular.x, material.m_Specular.y, material.m_Specular.z, material.m_Specular.w };
				materialJson["Emission"] = { material.m_Emission.x, material.m_Emission.y, material.m_Emission.z, material.m_Emission.w };
				materialJson["Shininess"] = material.m_Shininess;
				materialJson["TextureEnable"] = material.m_TextureEnable;

				jsonFile[std::to_string(pair.first)] = materialJson;
			}

			std::ofstream saveFile(SAVE_FILENAME);
			saveFile << jsonFile.dump(4);
		}

		/**
		* @brief Jsonファイルからデータを読み込みます。
		*/
		void MaterialManager::LoadFromJson()
		{
			// 保存ファイルを開く
			std::ifstream saveFile(SAVE_FILENAME);
			nlohmann::json jsonFile;
			saveFile >> jsonFile;

			for (auto it = jsonFile.begin(); it != jsonFile.end(); ++it) {
				int id = std::stoi(it.key());
				MaterialData material;

				material.m_Ambient = { it.value()["Ambient"][0], it.value()["Ambient"][1], it.value()["Ambient"][2], it.value()["Ambient"][3] };
				material.m_Diffuse = { it.value()["Diffuse"][0], it.value()["Diffuse"][1], it.value()["Diffuse"][2], it.value()["Diffuse"][3] };
				material.m_Specular = { it.value()["Specular"][0], it.value()["Specular"][1], it.value()["Specular"][2], it.value()["Specular"][3] };
				material.m_Emission = { it.value()["Emission"][0], it.value()["Emission"][1], it.value()["Emission"][2], it.value()["Emission"][3] };
				material.m_Shininess = it.value()["Shininess"];
				material.m_TextureEnable = it.value()["TextureEnable"];

				m_materialMap[id] = std::make_shared<MaterialData>(material);

				m_nNextId = id + 1;
			}
		}
	}
}