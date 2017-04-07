#pragma once
#include <map>
#include "LyoungVector.h"

class Sample;

class LyoungClient;
class LyoungFakeServer
{
public:
	const long TickTimeMS = 3000;//0.33f;
	static unsigned int AgentColor;
	static unsigned int PathColor;
	static unsigned int TargetColor;


	LyoungFakeServer();
	~LyoungFakeServer();

	void Start();
	void Tick(float deltaTimeMs);
		
	Sample* GetNavigationSample() {
		return navSample_;  
	}
	void SetNavigationSample(Sample* navSample);
	void CreateDummyClients();
	void DrawClient(unsigned int uid, vec3f pos, float r, float h, float c, const unsigned int col);

	typedef std::map<unsigned int, LyoungClient*> clientMap;
	typedef clientMap::iterator clientMapIter;

private:
	clientMap clients;
	unsigned int nextUniqueId = 0;

	Sample* navSample_;
	float agentRadius_;
	float agentHeight_;
	float agentClimb_;
};