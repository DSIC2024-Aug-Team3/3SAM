#pragma once
#include <nFramework/mec/MECComponent.h>
#include <nFramework/nom/NOMMain.h>
#include <nFramework/BaseManager.h>
#include "RadarModel.h"

using namespace nframework;
using namespace nom;

class BASEMGRDLL_API RSSModelManager : public BaseManager
{
public:
	RSSModelManager(void);
	~RSSModelManager(void);

public:
	// inherited from the BaseManager class
	virtual shared_ptr<NOM> registerMsg(tstring) override;
	virtual void discoverMsg(shared_ptr<NOM>) override;
	virtual void updateMsg(shared_ptr<NOM>) override;
	virtual void reflectMsg(shared_ptr<NOM>) override;
	virtual void deleteMsg(shared_ptr<NOM>) override;
	virtual void removeMsg(shared_ptr<NOM>) override;
	virtual void sendMsg(shared_ptr<NOM>) override;
	virtual void recvMsg(shared_ptr<NOM>) override;
	virtual void setUserName(tstring) override;
	virtual tstring getUserName() override;
	virtual void setData(void*) override;
	virtual bool start() override;
	virtual bool stop() override;
	virtual void setMEBComponent(IMEBComponent*) override;

private:
	void init();
	void release();
	void StartMsg(shared_ptr<NOM>);
	void DeployMsg(shared_ptr<NOM>);
	void StopMsg(shared_ptr<NOM>);
	void GetAirThreatLocation(shared_ptr<NOM>);
	void GetMissileLocation(shared_ptr<NOM>);

private:
	IMEBComponent* meb;
	MECComponent* mec;
	tstring name;
	map<unsigned int, shared_ptr<NOM>> registeredMsg;
	map<unsigned int, shared_ptr<NOM>> discoveredMsg;
	map<tstring, function<void(shared_ptr<NOM>)>> msgMethodMap;
	double antiLat, antiLon, missileLat, missileLon;
	double radarLat, radarLon; //* 레이다 위치
	// Air Threat Model
	RadarModel* Radar;
	shared_ptr<NOM> message;
	bool firecommand;
};