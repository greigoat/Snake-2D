#include "FBounds.h"

FVector2 FBounds::Min()
{
	return FVector2(rect.left, rect.top);
}

FVector2 FBounds::Max()
{
	return FVector2(rect.right, rect.bottom);
}

bool FBounds::Interesects(FBounds & b)
{
	if (Max().x < b.Min().x || Min().x > b.Max().x) return false;
	if (Max().y < b.Min().y || Min().y > b.Max().y) return false;

	return true;
}
