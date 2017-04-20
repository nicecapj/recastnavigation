#pragma once
#include <String>

template<typename T>
struct LyoungVector2
{
public:
	LyoungVector2();
	LyoungVector2(T x, T y) :X(x), Y(y) {};
	~LyoungVector2();

	std::string ToString()
	{
		std::string temp;

		char buff[64];
		sprintf(buff, "%4.2f", X);
		memset(buff, 0, sizeof(buff));
		temp.assign(buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, ",%4.2f", Y);
		temp.append(buff);

		return temp;
	}

	static LyoungVector2 Zero()
	{
		return LyoungVector2(0, 0, 0);
	}

	static float GetDistance(LyoungVector2& lhs, LyoungVector2& rhs)
	{
		return sqrtf((rhs.X - lhs.X)*(rhs.X - lhs.X) + (rhs.Y - lhs.Y)*(rhs.Y - lhs.Y));
	}

	T X;
	T Y;
};

template<typename T>
LyoungVector2<T>::~LyoungVector2()
{

}

template<typename T>
LyoungVector2<T>::LyoungVector2()
{

}
//-------------------------------------------------------------------------------------------------
template<typename T>
struct LyoungVector3
{
public:
	LyoungVector3();
	LyoungVector3(T x, T y, T z):X(x), Y(y), Z(z) {};
	LyoungVector3(const LyoungVector3& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;
	}
	void operator=(const LyoungVector3& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;		
	}

	LyoungVector3 operator+(const LyoungVector3& other)
	{
		return LyoungVector3(X + other.X, Y + other.Y, Z + other.Z);
	}

	LyoungVector3 operator-(const LyoungVector3& other)
	{
		return LyoungVector3(X - other.X, Y - other.Y, Z - other.Z);
	}
	
	LyoungVector3 operator*(const LyoungVector3& other)
	{
		return LyoungVector3(X * other.X, Y * other.Y, Z * other.Z);
	}

	LyoungVector3 operator*(float value)
	{
		return LyoungVector3(X * value, Y * value, Z * value);
	}

	LyoungVector3& operator+=(const LyoungVector3& other)
	{
		this->X += other.X;
		this->Y += other.Y;
		this->Z += other.Z;
		return *this;
	}

	~LyoungVector3();	

	float Length()
	{
		return sqrt(X*X + Y*Y + Z*Z);
	}

	float Dot(const LyoungVector3& other)
	{
		return X * other.X + Y * other.Y + Z * other.Z;
	}

	LyoungVector3 Scale(float value)
	{
		return LyoungVector3(X * value, Y * value, Z * value);
	}

	LyoungVector3 GetNormalizedVector()
	{
		//float length = Length();
		// LyoungVector3(X/length, Y/length, Z/length);
		return Scale(1.00f / Length());		
	}
	
	std::string ToString()
	{		
		std::string temp;		

		char buff[64];
		sprintf(buff, "%4.2f", X);
		memset(buff, 0, sizeof(buff));
		temp.assign(buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, ",%4.2f", Y);		
		temp.append(buff);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, ",%4.4f", Z);		
		temp.append(buff);
		
		return temp;
	}

	static LyoungVector3 Zero()
	{
		return LyoungVector3(0, 0, 0);
	}
	
	static float GetDistance(const LyoungVector3& lhs, const LyoungVector3& rhs)
	{
		return sqrtf((lhs.X - rhs.X)*(lhs.X - rhs.X) + (lhs.Y - rhs.Y)*(lhs.Y - rhs.Y) + (lhs.Z - rhs.Z)*(lhs.Z - rhs.Z));
	}
	
	T X;
	T Y;
	T Z;
};

template<typename T>
LyoungVector3<T>::~LyoungVector3()
{

}

template<typename T>
LyoungVector3<T>::LyoungVector3()
{

}

#define vec2f LyoungVector2<float>
#define vec3f LyoungVector3<float>

