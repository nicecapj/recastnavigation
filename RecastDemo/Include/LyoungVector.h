#pragma once
template<typename T>
struct LyoungVector2
{
public:
	LyoungVector2();
	LyoungVector2(T x, T y) :X(x), Y(y) {};
	~LyoungVector2();

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
	LyoungVector3& operator=(const LyoungVector3& other)
	{
		X = other.X;
		Y = other.Y;
		Z = other.Z;

		return *this;
	}

	~LyoungVector3();	

	static LyoungVector3 Zero()
	{
		return LyoungVector3(0, 0, 0);
	}
	
	static float GetDistance(const LyoungVector3& lhs, const LyoungVector3& rhs)
	{
		return sqrtf((rhs.X - lhs.X)*(rhs.X - lhs.X) + (rhs.Y - lhs.Y)*(rhs.Y - lhs.Y) + (rhs.Z - lhs.Z)*(rhs.Z - lhs.Z));
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

