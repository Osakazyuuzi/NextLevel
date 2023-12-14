#ifndef ___SRC_COMMON_CORE_UTILS_DXOPERATOR_H___
#define ___SRC_COMMON_CORE_UTILS_DXOPERATOR_H___

#include <DirectXMath.h>
#include <Eigen/Dense>

namespace NextLevel
{
	/**
	* @brief Eigen�̍s���DirectX�̍s��ɕϊ����܂��B
	* @param _eigenMat Eigen�̍s��B
	* @return DirectX::XMMATRIX DirectX�̍s��B
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
	* @brief DirectX�̍s���Eigen�̍s��ɕϊ����܂��B
	* @param _dxMat DirectX�̍s��B
	* @return Eigen::Matrix4f Eigen�̍s��B
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