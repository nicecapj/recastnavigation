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
unsigned int LyoungFakeServer::ConnectionColor = duRGBA(102, 211, 0, 64);

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
				client->handleRender();
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