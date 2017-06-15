#pragma once

#include "Component.h"
#include "FVector2.h"
#include "FMatrix3x3.h"

#include "FBounds.h"

/// <summary> 
/// Transform component.
/// Represtents transform of an game entity that can be rotated, translated and scaled.
/// </summary>
class FTransform2D : public Component
{
	public:

	//FVector2 location;
	//float theta = 0.0F;
	//FVector2 scale;

	/// <summary> 
	/// The matrix of this transform.
	/// </summary>
	FMatrix3x3 matrix;

	private:

	public:

	void BeginLoad() override;
	void Tick() override;
	void Destroy() override;

	public:

	/// <summary> 
	/// Set location of this transform.
	/// </summary>
	void     SetLocation(const FVector2& location);

	/// <summary> 
	/// Get location of this transform.
	/// </summary>
	FVector2 GetLocation();

	/// <summary> 
	/// Get rotation angle of this transform.
	/// </summary>
	float    GetTheta();

	/// <summary> 
	/// Get scale of this transform.
	/// </summary>
	FVector2 GetScale();

	/// <summary> 
	/// Translate this transform by given Vector
	/// </summary>
	void     Translate(const FVector2& translation);

	/// <summary> 
	/// Rotate this transform by given angel
	/// </summary>
	void     Rotate(float theta);

	/// <summary> 
	/// Scale this transform by given Vector
	/// </summary>
	void	 Scale(const FVector2& scale);
};