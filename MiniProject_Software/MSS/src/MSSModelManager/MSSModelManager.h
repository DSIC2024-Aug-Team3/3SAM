#pragma once
#include <nFramework/mec/MECComponent.h>
#include <nFramework/nom/NOMMain.h>
#include "MissileModel.h"

using namespace nframework;
using namespace nom;

class BASEMGRDLL_API MSSModelManager : public BaseManager
{
public:
	MSSModelManager(void);
	~MSSModelManager(void);

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
	void DeployMsg(shared_ptr<NOM>);
	void StartMsg(shared_ptr<NOM>);
	void FireMsg(shared_ptr<NOM>);
	void StopMsg(shared_ptr<NOM>);
	void InterceptResMsg(shared_ptr<NOM>);
	void getAntiLocation(shared_ptr<NOM>);



private:
	IMEBComponent* meb;
	MECComponent* mec;
	tstring name;
	map<unsigned int, shared_ptr<NOM>> registeredMsg;
	map<unsigned int, shared_ptr<NOM>> discoveredMsg;
	map<tstring, function<void(shared_ptr<NOM>)>> msgMethodMap;

	// Air Threat Model
	MissileModel* missile;
	shared_ptr<NOM> ICD_TestNOM;
	double initLat;
	double initLon;
	double antiLat;
	double antiLon;
	bool fireflag;
	double currentX;
	double currentY;
};