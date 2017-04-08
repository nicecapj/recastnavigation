#pragma once
#include <map>
#include "Sample.h"
#include "LyoungVector.h"

//class Sample;

class LyoungClient;
class LyoungFakeServer : public SampleTool
{
public:
	const long TickTimeMS = 3000;//0.33f;
	const int DummyClientSettingCount = 50;
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
	
	virtual void handleRender();
	virtual void handleRenderOverlay(double* proj, double* model, int* view);

	void handleRenderOverlayClient(double* proj, double* model, int* view, LyoungClient* client);

	typedef std::map<unsigned int, LyoungClient*> clientMap;
	typedef clientMap::iterator clientMapIter;

private:
	clientMap clients;
	unsigned int nextUniqueId = 0;

	Sample* navSample_;
	float agentRadius_;
	float agentHeight_;
	float agentClimb_;

	// Inherited via SampleTool
	virtual int type() override;
	virtual void init(Sample * sample) override;
	virtual void reset() override;
	virtual void handleMenu() override;
	virtual void handleClick(const float * s, const float * p, bool shift) override;
	virtual void handleToggle() override;
	virtual void handleStep() override;
	virtual void handleUpdate(const float dt) override;
};