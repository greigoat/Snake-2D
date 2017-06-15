#pragma once

#include "FVector2.h"
#include <vector>

/// <summary> 
/// Represents 3 by 3 matrix of an transform
/// </summary>
class FMatrix3x3
{
	FMatrix3x3();
	~FMatrix3x3();

	private:

	float m[3][3];

	public:

	FMatrix3x3 operator*(const FMatrix3x3& other);

	/// <summary> 
	/// Set this matrix to identity state
	/// </summary>
	void SetIdentity();

	/// <summary> 
	/// Transform vertices by this matrix
	/// </summary>
	void Transform(std::vector<FVector2>& outVertices);

	/// <summary> 
	/// Translate matrix by given vector
	/// </summary>
	void Translate(const FVector2& translation);

	/// <summary> 
	/// Rotate matrix by given angle
	/// </summary>
	void Rotate(float theta);

	/// <summary> 
	/// Scale matrix by given vector
	/// </summary>
	void Scale(const FVector2& scale);

	friend class FTransform2D;
};