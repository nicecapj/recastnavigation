#pragma once
#include "LyoungVector.h"

class LyoungFakeServer;
class LyoungClient
{
public:
	enum State
	{
		IDLE,
		MOVE,
		COUNT,
	};

	float waitTime[State::COUNT] = { 2,4 };

	LyoungClient(LyoungFakeServer* worldServer);
	~LyoungClient();
	
	void Tick(float deltaTimeMs);

	vec3f FindDestination();
	void BeginMove();
	void EndMove();	

	unsigned int GetUID() 
	{
		return uniqueID; 
	}
	void SetUID(unsigned int uid) {
		uniqueID = uid;
	}

	vec3f GetPosition()
	{
		return position_;
	}

	void SetPosition(float x, float y, float z)
	{
		position_.X = x;
		position_.Y = y;
		position_.Z = z;
	}

	void ProcessBasicStateMachine(float deltaTimeMs);

private:
	vec3f position_;
	unsigned int uniqueID = 0;

	State currentState_ = State::IDLE;
	float Speed = 300.0f;

	float currentStateWaitTimeMS_ = 0.0f;

	LyoungFakeServer* worldServer_;
};

