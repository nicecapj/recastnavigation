#pragma once
#include "LyoungVector.h"
#include <string>

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
	std::string GetUIDString()
	{
		return uniqueIDCacheStr_;
	}
	void SetUID(unsigned int uid);

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

	vec3f GetTargetPosition()
	{
		return targetPosition_;
	}

	void SetTargetPosition(float x, float y, float z)
	{
		targetPosition_.X = x;
		targetPosition_.Y = y;
		targetPosition_.Z = z;
	}

	bool IsSetTargetPosition()
	{
		return isSetTargetPosition_;
	}

	void ProcessBasicStateMachine(float deltaTimeMs);

private:
	void SetTargetPosition(bool isSet)
	{
		isSetTargetPosition_ = isSet;
	}

	unsigned int uniqueID = 0;
	std::string uniqueIDCacheStr_ = "";

	vec3f position_;
	vec3f targetPosition_;
	bool isSetTargetPosition_ = false;	

	State currentState_ = State::IDLE;
	float Speed = 300.0f;

	float currentStateWaitTimeMS_ = 0.0f;

	LyoungFakeServer* worldServer_;
};

