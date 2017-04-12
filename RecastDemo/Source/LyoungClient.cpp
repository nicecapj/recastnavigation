#include "LyoungClient.h"
#include "LyoungFakeServer.h"
#include "LyoungVector.h"
#include "Sample.h"
#include <SDL.h>
#include <random>	//c++11 random

#include "DetourCommon.h"

//return 0~1
static float frand()
{
	return (float)rand() / (float)RAND_MAX;
}

LyoungClient::LyoungClient(LyoungFakeServer* worldServer)
{
	worldServer_ = worldServer;
	position_ = LyoungVector3<float>::Zero();	
	targetPosition_ = position_;	
	
	filter_.setIncludeFlags(SAMPLE_POLYFLAGS_ALL ^ SAMPLE_POLYFLAGS_DISABLED);
	filter_.setExcludeFlags(0);

	isInitialize = false;
}


LyoungClient::~LyoungClient()
{	
}

void LyoungClient::Initialze()
{
	if (!isInitialize)
	{
		auto navSample = worldServer_->GetNavigationSample();
		if (navSample != nullptr)
		{
			auto query = navSample->getNavMeshQuery();
			if (query != nullptr)
			{
				auto buildedNavMesh = query->getAttachedNavMesh();
				if (buildedNavMesh != nullptr)
				{					
					agentRadius_ = navSample->getAgentRadius();
					agentHeight_ = navSample->getAgentHeight();
					agentClimb_ = navSample->getAgentClimb();

					query->findRandomPoint(&filter_, frand, &startRef, (float*)&position_);
					
					isInitialize = true;
				}
			}
		}
	}
}


void LyoungClient::Tick(float deltaTimeMs)
{
	Initialze();
	ProcessBasicStateMachine(deltaTimeMs);
}

bool LyoungClient::FindValidDestination(vec3f& foundPosition)
{
	//std::random_device alwaysRandomSeed;
	//std::uniform_real<float> floatRandom(0.0f, 10.0f);
	//float rndValue = floatRandom(alwaysRandomSeed);


	auto navSample = worldServer_->GetNavigationSample();
	if (navSample != nullptr)
	{
		auto query = navSample->getNavMeshQuery();
		if (query != nullptr)
		{
			auto buildedNavMesh = query->getAttachedNavMesh();
			if (buildedNavMesh == nullptr)
				return false;

			if (query->findRandomPoint(&filter_, frand, &endRef, (float*)&targetPosition_) == DT_SUCCESS)
			{
				if (query->findPath(startRef, endRef, (const float*)&position_, (const float*)&targetPosition_, &filter_, path_, &pathSize, MAX_POLYS) == DT_SUCCESS)
				{
					//nicecapj_20170411-if found poly ids, change to straight position.
					m_nstraightPath = 0;
					if (pathSize)
					{
						// In case of partial path, make sure the end point is clamped to the last polygon.
						float epos[3];
						dtVcopy(epos, (const float*)&targetPosition_);
						if (path_[pathSize - 1] != endRef)
							query->closestPointOnPoly(path_[pathSize - 1], (const float*)&targetPosition_, epos, 0);

						if (query->findStraightPath((const float*)&position_, epos, path_, pathSize,
							m_straightPath,
							m_straightPathFlags,
							m_straightPathPolys,
							&m_nstraightPath,
							MAX_POLYS,
							m_straightPathOptions) == DT_SUCCESS)
						{
							return true;
						}						
					}
				}
			}
		}
	}
	
	return false;
}

void LyoungClient::handleRender()
{
	auto navSample = worldServer_->GetNavigationSample();
	if (navSample != nullptr)
	{
		auto query = navSample->getNavMeshQuery();
		if (query != nullptr)
		{
			auto buildedNavMesh = query->getAttachedNavMesh();
			if (buildedNavMesh == nullptr)
				return;

			duDebugDraw& dd = navSample->getDebugDraw();

			dd.depthMask(false);

			// Agent dimensions.	
			duDebugDrawCylinderWire(&dd,
				position_.X - agentRadius_,
				position_.Y + 0.02f,
				position_.Z - agentRadius_,
				position_.X + agentRadius_,
				position_.Y + agentHeight_,
				position_.Z + agentRadius_,
				LyoungFakeServer::AgentColor,
				2.0f);

			duDebugDrawCircle(&dd, position_.X, position_.Y + agentClimb_, position_.Z, agentRadius_, duRGBA(0, 0, 0, 64), 1.0f);

			unsigned int colb = duRGBA(0, 0, 0, 196);
			dd.begin(DU_DRAW_LINES);
			dd.vertex(position_.X, position_.Y - agentClimb_, position_.Z, colb);
			dd.vertex(position_.X, position_.Y + agentClimb_, position_.Z, colb);
			dd.vertex(position_.X - agentRadius_ / 2, position_.Y + 0.02f, position_.Z, colb);
			dd.vertex(position_.X + agentRadius_ / 2, position_.Y + 0.02f, position_.Z, colb);
			dd.vertex(position_.X, position_.Y + 0.02f, position_.Z - agentRadius_ / 2, colb);
			dd.vertex(position_.X, position_.Y + 0.02f, position_.Z + agentRadius_ / 2, colb);
			dd.end();

			dd.depthMask(true);
		}
	}
	
	RenderPath();
}

void LyoungClient::RenderPath()
{
	auto navSample = worldServer_->GetNavigationSample();
	if (navSample != nullptr)
	{
		auto query = navSample->getNavMeshQuery();
		if (query != nullptr)
		{
			auto buildedNavMesh = query->getAttachedNavMesh();
			if (buildedNavMesh == nullptr)
				return;

			duDebugDraw& dd = navSample->getDebugDraw();
			int straightPathCount = GetStraightPathCount();
			if (straightPathCount)
			{
				dd.depthMask(false);
				const unsigned int spathCol = duRGBA(64, 16, 0, 220);
				const unsigned int offMeshCol = duRGBA(128, 96, 0, 220);
				dd.begin(DU_DRAW_LINES, 2.0f);
				for (int i = 0; i < straightPathCount - 1; ++i)
				{
					unsigned int col;
					if (m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
						col = offMeshCol;
					else
						col = spathCol;

					dd.vertex(m_straightPath[i * 3], m_straightPath[i * 3 + 1] + 0.4f, m_straightPath[i * 3 + 2], col);
					dd.vertex(m_straightPath[(i + 1) * 3], m_straightPath[(i + 1) * 3 + 1] + 0.4f, m_straightPath[(i + 1) * 3 + 2], col);
				}
				dd.end();
				dd.begin(DU_DRAW_POINTS, 6.0f);
				for (int i = 0; i < straightPathCount; ++i)
				{
					unsigned int col;
					if (m_straightPathFlags[i] & DT_STRAIGHTPATH_START)
						col = LyoungFakeServer::AgentColor;
					else if (m_straightPathFlags[i] & DT_STRAIGHTPATH_END)
						col = LyoungFakeServer::TargetColor;
					else if (m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
						col = LyoungFakeServer::ConnectionColor;
					else
						col = spathCol;
					dd.vertex(m_straightPath[i * 3], m_straightPath[i * 3 + 1] + 0.4f, m_straightPath[i * 3 + 2], col);
				}
				dd.end();
				dd.depthMask(true);
			}
		}
	}
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

			vec3f dest = vec3f::Zero();
			if (FindValidDestination(dest))
			{
				currentState_ = MOVE;
				targetPosition_ = dest;
			}
		}
	}break;

	case MOVE:
	{
		if (currentStateWaitTimeMS_ > waitTime[MOVE])
		{
			currentStateWaitTimeMS_ = 0.0;
			currentState_ = IDLE;
		}
		else
		{
			if (vec3f::GetDistance(position_, targetPosition_) > 0.05f)
			{
				
			}
			else
			{
				targetPosition_ = position_;	//force position and target pogition to be synchroized.
			}
		}
	}
	break;
	}
}