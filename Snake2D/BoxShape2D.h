#pragma once

#include "Component.h"
#include "FVector2.h"

/// <summary> 
/// Represets 2d box shape that can be rendered
/// </summary>
class BoxShape2D : public RendererComponent
{
	public:

	BoxShape2D();
	~BoxShape2D();

	virtual void BeginLoad() override;
	virtual void Tick() override;
	virtual void Destroy() override;
	virtual void Render() override;

	/// <summary> 
	/// Set Unit size of this shape
	/// </summary>
	void SetSize(FVector2 size);

	/// <summary> 
	/// Transformed bounds/vertices of this shape
	/// </summary>
	FBounds transformedBounds;

	private:

	FVector2 size;
};