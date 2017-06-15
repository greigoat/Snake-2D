#include "FMatrix3x3.h"

FMatrix3x3::FMatrix3x3()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}

FMatrix3x3::~FMatrix3x3()
{
}

void FMatrix3x3::SetIdentity()
{
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1;
}

FMatrix3x3 FMatrix3x3::operator*(const FMatrix3x3 & other)
{
	FMatrix3x3 product;

	for (int x = 0; x<3; ++x)
		for (int y = 0; y<3; ++y)
		{
			float sum = 0;
			for (int z = 0; z<3; ++z)
				sum += m[x][z] * other.m[z][y];
			product.m[x][y] = sum;
		}

	return product;
}

void FMatrix3x3::Transform(std::vector<FVector2>& outVertices)
{
	float transformedX, transformedY;

	for (size_t x = 0; x < outVertices.size(); ++x)
	{
		transformedX = (float)(outVertices[x].x * m[0][0] +
			outVertices[x].y * m[1][0] + m[2][0]);

		transformedY = (float)(outVertices[x].x * m[0][1] +
			outVertices[x].y * m[1][1] + m[2][1]);

		outVertices[x].x = transformedX;
		outVertices[x].y = transformedY;
	}
}

void FMatrix3x3::Translate(const FVector2 & translation)
{
	FMatrix3x3 m1;
	auto x = translation.x;
	auto y = translation.y;

	m1.m[0][0] = 1;	m1.m[0][1] = 0; m1.m[0][2] = 0;
	m1.m[1][0] = 0;	m1.m[1][1] = 1; m1.m[1][2] = 0;
	m1.m[2][0] = x;	m1.m[2][1] = y; m1.m[2][2] = 1;

	*this = *this * m1;
}

void FMatrix3x3::Rotate(float theta)
{
	if (theta == 0) return;

	FMatrix3x3 m1, mid;

	float radians = 6.283185308f / (360.0F / theta);
	float c = cosf(radians);
	float s = sinf(radians);

	m1.m[0][0] = c; m1.m[0][1] = s; m1.m[0][2] = 0;
	m1.m[1][0] = -s; m1.m[1][1] = c; m1.m[1][2] = 0;
	m1.m[2][0] = 0; m1.m[2][1] = 0; m1.m[2][2] = 1;

	*this = *this * m1;
}

void FMatrix3x3::Scale(const FVector2 & scale)
{
	FMatrix3x3 m1;
	m1.m[0][0] = scale.x; m1.m[0][1] = 0;   m1.m[0][2] = 0;
	m1.m[1][0] = 0;   m1.m[1][1] = scale.y; m1.m[1][2] = 0;
	m1.m[2][0] = 0;   m1.m[2][1] = 0;   m1.m[2][2] = 1;

	*this = *this * m1;
}
