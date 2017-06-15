#pragma once

#include <math.h>

/// <summary> 
/// Represents two dimensional vector
/// </summary>
class FVector2
{
public:
	FVector2(const FVector2& other) = default;
	FVector2(FVector2&& other) = default;
	
	FVector2(const float& x, const float& y) : x(x), y(y)
	{
	}

	FVector2() : FVector2(0.0F, 0.0F)
	{
	}

	~FVector2()
	{
	}

	inline FVector2& operator=(const FVector2& b)
	{
		x = b.x;
		y = b.y;

		return *this;
	}

	inline FVector2& operator=(FVector2&& b)
	{
		x = b.x;
		y = b.y;

		b.x = 0;
		b.y = 0;

		return *this;
	}

	// Product
	//

	inline friend FVector2 operator *(const FVector2& a, const float& b)
	{
		return FVector2(a.x*b, a.y*b);
	}

	inline friend FVector2 operator *(const float& a, const FVector2& b)
	{
		return FVector2(b.x*a, b.y*a);
	}

	inline friend FVector2& operator *=(FVector2& a, const float& b)
	{
		a.x *= b;
		a.y *= b;

		return a;
	}

	inline friend float& operator *=(float& a, const FVector2& b)
	{
		a *= b.x;
		a *= b.y;

		return a;
	}


	// Substract
	//

	inline FVector2 operator -(const FVector2& b)
	{
		return FVector2(x - b.x, y - b.y);
	}

	inline FVector2& operator -=(const FVector2& b)
	{
		x -= b.x;
		y -= b.y;

		return *this;
	}


	// Sum
	//

	inline FVector2 operator +(const FVector2& b)
	{
		return FVector2(x + b.x, y + b.y);
	}

	inline FVector2& operator +=(const FVector2& b)
	{
		x += b.x;
		y += b.y;

		return *this;
	}


	// Division
	//

	inline friend FVector2 operator /(const float& a, const FVector2& b)
	{
		return FVector2(b.x / a, b.y / a);
	}

	inline friend FVector2 operator /(const FVector2& a, const float& b)
	{
		return FVector2(a.x / b, a.y / b);
	}

	inline FVector2 operator /=(const float& b)
	{
		x /= b; 
		y /= b; 
		
		return *this;
	}


	// Conditional
	//

	inline friend bool operator ==(const FVector2& a, const FVector2& b)
	{
		//TODO: Appr
		return a.x == b.x && a.y == b.y;
	}

	inline friend bool operator !=(const FVector2& a, const FVector2& b)
	{
		return !(a == b);
	}


	// Other
	//

	inline float& operator [](unsigned int i)
	{
		return i == 0 ? x : y;
	}

	inline float operator [](unsigned int i) const
	{
		return i == 0 ? x : y;
	}

	inline float Magnitude() const
	{
		return sqrtf(x*x + y*y);
	}

	inline FVector2 Normalized() const
	{
		return *this / this->Magnitude();
	}

	inline void Normalize()
	{
		*this /= this->Magnitude();
	}

	void FVector2::OrthoNormalize(FVector2& u, FVector2& v, FVector2& w)
	{
		u.Normalize();

		float dot0 = Dot(u, v);

		v -= u*dot0;
		v.Normalize();

		float dot1 = Dot(v, w);
		dot0 = Dot(u, w);

		w -= u*dot0 + v*dot1;
		w.Normalize();
	}

	// Global
	//
	inline static float Distance(FVector2 a, FVector2 b)
	{
		return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.x - b.x));
	}

	inline static float AngleBetween(const FVector2& a, const FVector2 &b)
	{
		return acosf(Dot(a,b) / a.Magnitude() / b.Magnitude());
	}

	inline static FVector2 Projection(const FVector2 &a, const FVector2& b)
	{
		return b*((Dot(a,b)) / (b.Magnitude()*b.Magnitude()));
	}

	inline static float Dot(const FVector2& a, const FVector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	inline static FVector2 Up()
	{
		return FVector2(0, 1);
	}

	inline static FVector2 Down()
	{
		return FVector2(0, -1);
	}

	inline static FVector2 Right()
	{
		return FVector2(1, 0);
	}

	inline static FVector2 Left()
	{
		return FVector2(-1, 0);
	}

	inline static FVector2 Zero()
	{
		return FVector2(0, 0);
	}

	inline static FVector2 One()
	{
		return FVector2(1, 1);
	}

	float x, y;
private:
};