#include "LyoungClient.h"
#include "LyoungFakeServer.h"
#include "LyoungVector.h"
#include "Sample.h"
#include <SDL.h>
#include <random>	//c++11 random

LyoungClient::LyoungClient(LyoungFakeServer* worldServer)
{
	//position_ = LyoungVector3<float>::Zero();

	std::random_device alwaysRandomSeed;	
	std::uniform_real<float> floatRandom(0.0f, 10.0f) ;

	position_ = vec3f(54.74, -0.75, 16.75);	//valid pos in navi test map					
	position_.X += floatRandom(alwaysRandomSeed);
	position_.Z += floatRandom(alwaysRandomSeed);

	targetPosition_ = position_;		
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

void LyoungClient::SetUID(unsigned int uid)
{
	uniqueID = uid;

	char buff[64] = { 0, };
	_itoa(uid, buff, 10);

	uniqueIDCacheStr_.assign(buff);
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