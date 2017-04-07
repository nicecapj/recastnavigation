#pragma once
template<typename T>
struct LyoungVector2
{
public:
	LyoungVector2();
	static LyoungVector2 Zero()
	{
		return LyoungVector2(0, 0, 0);
	}
	LyoungVector2(T x, T y) :X(x), Y(y) {};
	~LyoungVector2();

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
	static LyoungVector3 Zero()
	{
		return LyoungVector3(0, 0, 0);
	}

	LyoungVector3();
	LyoungVector3(T x, T y, T z):X(x), Y(y), Z(z) {};
	~LyoungVector3();

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

