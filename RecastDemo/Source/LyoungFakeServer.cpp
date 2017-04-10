#include "LyoungFakeServer.h"
#include <iostream>

#include "SDL_opengl.h"
#ifdef __APPLE__
#	include <OpenGL/glu.h>
#else
#	include <GL/glu.h>
#endif
#include "imgui.h"
#include "Sample.h"
#include "DebugDraw.h"
#include "imguiRenderGL.h"

#include "LyoungClient.h"

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
	for (int i = 0; i < DummyClientSettingCount; ++i)
	{
		LyoungClient* client = new LyoungClient(this);
		client->SetUID(++nextUniqueId);

		clients.insert(clientMap::value_type(client->GetUID(), client));
	}
}


void LyoungFakeServer::handleRender()
{
	clientMapIter it = clients.begin();
	clientMapIter endIter = clients.end();
	for (it = clients.begin(); it != endIter; ++it)
	{
		LyoungClient* client = it->second;
		if (client != nullptr)
		{
			if (navSample_ != nullptr)
			{
				DrawClient(client->GetUID(), client->GetPosition(), agentRadius_, agentHeight_, agentClimb_, LyoungFakeServer::AgentColor);
				DrawPath(client);
			}
		}
	}
}

void LyoungFakeServer::handleRenderOverlay(double * proj, double * model, int * view)
{
	clientMapIter it = clients.begin();
	clientMapIter endIter = clients.end();
	for (it = clients.begin(); it != endIter; ++it)
	{
		LyoungClient* client = it->second;
		if (client != nullptr)
		{
			if (navSample_ != nullptr)
			{
				handleRenderOverlayClient(proj, model, view, client);
			}
		}
	}	
}

void LyoungFakeServer::handleRenderOverlayClient(double* proj, double* model, int* view, LyoungClient* client)
{
	GLdouble x, y, z;

	auto currentPosition = client->GetPosition();
	auto targetPosition = client->GetTargetPosition();

	// Draw start and end point labels
	if (gluProject((GLdouble)currentPosition.X, (GLdouble)currentPosition.Y, (GLdouble)currentPosition.Z,
		model, proj, view, &x, &y, &z))
	{
		imguiDrawText((int)x, (int)(y - 25), IMGUI_ALIGN_CENTER, client->GetUidAndPositionString().c_str(), imguiRGBA(0, 0, 0, 220));
	}
	if (client->IsSetTargetPosition() && gluProject((GLdouble)targetPosition.X, (GLdouble)targetPosition.Y, (GLdouble)targetPosition.Z,
		model, proj, view, &x, &y, &z))
	{
		imguiDrawText((int)x, (int)(y - 25), IMGUI_ALIGN_CENTER, client->GetUidAndTargetPositionString().c_str(), imguiRGBA(0, 0, 0, 220));
	}	
}

int LyoungFakeServer::type()
{
	return 0;
}

void LyoungFakeServer::init(Sample * sample)
{
}

void LyoungFakeServer::reset()
{
}

void LyoungFakeServer::handleMenu()
{
}

void LyoungFakeServer::handleClick(const float * s, const float * p, bool shift)
{
}

void LyoungFakeServer::handleToggle()
{
}

void LyoungFakeServer::handleStep()
{
}

void LyoungFakeServer::handleUpdate(const float dt)
{
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

void LyoungFakeServer::DrawPath(LyoungClient* client)
{

	duDebugDraw& dd = navSample_->getDebugDraw();
	int straightPathCount = client->GetStraightPathCount();
	if (straightPathCount)
	{
		dd.depthMask(false);
		const unsigned int spathCol = duRGBA(64, 16, 0, 220);
		const unsigned int offMeshCol = duRGBA(128, 96, 0, 220);
		dd.begin(DU_DRAW_LINES, 2.0f);
		for (int i = 0; i < straightPathCount - 1; ++i)
		{
			unsigned int col;
			if (client->m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
				col = offMeshCol;
			else
				col = spathCol;

			dd.vertex(client->m_straightPath[i * 3], client->m_straightPath[i * 3 + 1] + 0.4f, client->m_straightPath[i * 3 + 2], col);
			dd.vertex(client->m_straightPath[(i + 1) * 3], client->m_straightPath[(i + 1) * 3 + 1] + 0.4f, client->m_straightPath[(i + 1) * 3 + 2], col);
		}
		dd.end();
		dd.begin(DU_DRAW_POINTS, 6.0f);
		for (int i = 0; i < straightPathCount; ++i)
		{
			unsigned int col;
			if (client->m_straightPathFlags[i] & DT_STRAIGHTPATH_START)
				col = PathColor;
			else if (client->m_straightPathFlags[i] & DT_STRAIGHTPATH_END)
				col = TargetColor;
			else if (client->m_straightPathFlags[i] & DT_STRAIGHTPATH_OFFMESH_CONNECTION)
				col = offMeshCol;
			else
				col = spathCol;
			dd.vertex(client->m_straightPath[i * 3], client->m_straightPath[i * 3 + 1] + 0.4f, client->m_straightPath[i * 3 + 2], col);
		}
		dd.end();
		dd.depthMask(true);
	}
}