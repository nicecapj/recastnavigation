#include "LyoungFakeServer.h"
#include <iostream>

#include "LyoungClient.h"

#include "Sample.h"
#include "DebugDraw.h"

unsigned int LyoungFakeServer::AgentColor = duRGBA(128, 25, 0, 192);
unsigned int LyoungFakeServer::PathColor = duRGBA(51, 102, 0, 129);
unsigned int LyoungFakeServer::TargetColor = duRGBA(0, 0, 0, 64);

LyoungFakeServer::LyoungFakeServer()
{	
	navSample_ = nullptr;	
}


LyoungFakeServer::~LyoungFakeServer()
{
	clientMapIter it = clients.begin();
	clientMapIter endIter = clients.end();
	for (it ; it != endIter; ++it)
	{
		delete(it->second);
		it->second = nullptr;
	}
	clients.clear();
}


void LyoungFakeServer::Start()
{
	CreateDummyClients();
}

void LyoungFakeServer::Tick(float deltaTimeMs)
{	
	clientMapIter it = clients.begin();
	clientMapIter endIter = clients.end();
	for (it = clients.begin() ; it != endIter; ++it)
	{
		LyoungClient* client = it->second;
		if (client != nullptr)
		{
			client->Tick(deltaTimeMs);

			if (navSample_ != nullptr)
			{
				DrawClient(client->GetUID(), client->GetPosition(), agentRadius_, agentHeight_, agentClimb_, LyoungFakeServer::AgentColor);
			}			
		}
	}
}

void LyoungFakeServer::SetNavigationSample(Sample* navSample)
{
	navSample_ = navSample;

	if (navSample_ != nullptr)
	{
		agentRadius_ = navSample_->getAgentRadius();
		agentHeight_ = navSample_->getAgentHeight();
		agentClimb_ = navSample_->getAgentClimb();
	}
}

void LyoungFakeServer::CreateDummyClients()
{
	clients.insert(std::make_pair(++nextUniqueId, new LyoungClient(this)));
}


void LyoungFakeServer::DrawClient(unsigned int uid, vec3f pos, float r, float h, float c, const unsigned int col)
{
	if (navSample_ == nullptr)
		return;

	duDebugDraw& dd = navSample_->getDebugDraw();

	dd.depthMask(false);

	// Agent dimensions.	
	duDebugDrawCylinderWire(&dd, pos.X - r, pos.Y + 0.02f, pos.Z - r, pos.X + r, pos.Y + h, pos.Z + r, col, 2.0f);

	duDebugDrawCircle(&dd, pos.X, pos.Y + c, pos.Z, r, duRGBA(0, 0, 0, 64), 1.0f);

	unsigned int colb = duRGBA(0, 0, 0, 196);
	dd.begin(DU_DRAW_LINES);
	dd.vertex(pos.X, pos.Y - c, pos.Z, colb);
	dd.vertex(pos.X, pos.Y + c, pos.Z, colb);
	dd.vertex(pos.X - r / 2, pos.Y + 0.02f, pos.Z, colb);
	dd.vertex(pos.X + r / 2, pos.Y + 0.02f, pos.Z, colb);
	dd.vertex(pos.X, pos.Y + 0.02f, pos.Z - r / 2, colb);
	dd.vertex(pos.X, pos.Y + 0.02f, pos.Z + r / 2, colb);
	dd.end();

	dd.depthMask(true);
}