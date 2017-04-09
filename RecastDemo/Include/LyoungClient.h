#pragma once
#include "LyoungVector.h"
#include <string>

#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"

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

	float waitTime[State::COUNT] = { 0.5/*2*/,4 };

	LyoungClient(LyoungFakeServer* worldServer);
	~LyoungClient();
	
	void Initialze();	//if server has navigation mesh, it is set to true, not duplicated call.

	void Tick(float deltaTimeMs);

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


	//path finding
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * 3];
	static const int MAX_POLYS = 256;
	static const int MAX_SMOOTH = 2048;	unsigned char steerPathFlags[MAX_STEER_POINTS];

	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	int pathSize = 10;

	dtQueryFilter filter_;

	bool FindValidDestination(vec3f& foundPosition);

private:
	bool isInitialize = false;
	void SetTargetPosition(bool isSet)
	{
		isSetTargetPosition_ = isSet;
	}

	unsigned int uniqueID = 0;
	std::string uniqueIDCacheStr_ = "";

	vec3f position_;
	vec3f targetPosition_;
	dtPolyRef path_[MAX_POLYS];
	dtPolyRef startRef;
	dtPolyRef endRef;
	bool isSetTargetPosition_ = false;	

	State currentState_ = State::IDLE;
	float Speed = 300.0f;

	float currentStateWaitTimeMS_ = 0.0f;

	LyoungFakeServer* worldServer_;
};

