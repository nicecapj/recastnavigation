#include "LyoungClient.h"
#include "LyoungFakeServer.h"
#include "LyoungVector.h"
#include "Sample.h"
#include <SDL.h>


LyoungClient::LyoungClient(LyoungFakeServer* worldServer)
{
	//position_ = LyoungVector3<float>::Zero();
	position_ = vec3f(40, 30, 40);	//valid pos in dungeon map
}


LyoungClient::~LyoungClient()
{	
}

void LyoungClient::Tick(float deltaTimeMs)
{
	ProcessBasicStateMachine(deltaTimeMs);
}

vec3f LyoungClient::FindDestination()
{
	//float forcastPosition[3];	

	auto navSample = worldServer_->GetNavigationSample();
	if (navSample != nullptr)
	{
		auto query = navSample->getNavMeshQuery();
		if (query != nullptr)
		{
			//query->findStraightPath(&position_, )
		}
	}
	
	return vec3f::Zero();
}

void LyoungClient::ProcessBasicStateMachine(float deltaTimeMs)
{
	currentStateWaitTimeMS_ += deltaTimeMs;

	switch (currentState_)
	{
	case IDLE:
	{
		if (currentStateWaitTimeMS_ > waitTime[IDLE])
		{
			currentStateWaitTimeMS_ = 0.0;

			//vec3f dest = FindDestination();			
		}
	}break;

	case MOVE:
	{
		if (currentStateWaitTimeMS_ > waitTime[MOVE])
		{
			currentStateWaitTimeMS_ = 0.0;
		}
	}
	break;
	}
}