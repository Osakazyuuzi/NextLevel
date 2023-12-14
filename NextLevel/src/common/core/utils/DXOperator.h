#ifndef ___SRC_COMMON_CORE_UTILS_DXOPERATOR_H___
#define ___SRC_COMMON_CORE_UTILS_DXOPERATOR_H___

#include <DirectXMath.h>
#include <Eigen/Dense>

namespace NextLevel
{
	/**
	* @brief Eigenの行列をDirectXの行列に変換します。
	* @param _eigenMat Eigenの行列。
	* @return DirectX::XMMATRIX DirectXの行列。
	*/
	DirectX::XMMATRIX Convert(const Eigen::Matrix4f& _eigenMat)
	{
		DirectX::XMMATRIX dxMat;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				dxMat.r[i].m128_f32[j] = _eigenMat(i, j);

		return dxMat;
	}

	/**
	* @brief DirectXの行列をEigenの行列に変換します。
	* @param _dxMat DirectXの行列。
	* @return Eigen::Matrix4f Eigenの行列。
	*/
	Eigen::Matrix4f Convert(const DirectX::XMMATRIX& _dxMat)
	{
		Eigen::Matrix4f eigenMat;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				eigenMat(i, j) = _dxMat.r[i].m128_f32[j];

		return eigenMat;
	}
}

#endif //!___SRC_COMMON_CORE_UTILS_DXOPERATOR_H___