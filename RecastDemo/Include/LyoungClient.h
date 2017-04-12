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

	std::string StateStr[State::COUNT] =
	{
		"IDLE",
		"MOVE",
	};

	float waitTime[State::COUNT] = { 2,4 };

	LyoungClient(LyoungFakeServer* worldServer);
	~LyoungClient();
	
	void Initialze();	//if server has navigation mesh, it is set to true, not duplicated call.

	void Tick(float deltaTimeMs);


	unsigned int GetUID() 
	{
		return uniqueID; 
	}
	std::string GetUIDString()
	{
		return uniqueIDCacheStr_;
	}
	void SetUID(unsigned int uid);

	std::string GetUidAndPositionString()
	{
		std::string temp;
		temp.assign(uniqueIDCacheStr_);
		temp.append(" : ");
		temp.append(position_.ToString());
		return temp;
	}

	std::string GetUidAndTargetPositionString()
	{
		std::string temp;
		temp.assign(uniqueIDCacheStr_);
		temp.append(" : ");
		temp.append(targetPosition_.ToString());
		return temp;
	}

	std::string GetStateString()
	{
		return StateStr[currentState_];
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


	float m_straightPath[MAX_POLYS * 3];
	unsigned char m_straightPathFlags[MAX_POLYS];
	dtPolyRef m_straightPathPolys[MAX_POLYS];
	int m_nstraightPath;
	int m_straightPathOptions;

	float* GetStraightPath() {
		return m_straightPath; 
	}
	int GetStraightPathCount() { return m_nstraightPath; }

	void handleRender();
	void RenderPath();
	
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
	float agentRadius_;
	float agentHeight_;
	float agentClimb_;

	State currentState_ = State::IDLE;
	float Speed = 300.0f;

	float currentStateWaitTimeMS_ = 0.0f;

	LyoungFakeServer* worldServer_;
};

