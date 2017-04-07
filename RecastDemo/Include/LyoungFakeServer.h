#pragma once
#include <chrono>
class LyoungFakeServer
{
public:
	const long TickTimeMS = 3000;//0.33f;

	LyoungFakeServer();
	~LyoungFakeServer();

	void Start();
	void Tick(float deltaTimeMs);
};

