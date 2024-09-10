#pragma once
#include <nFramework/BaseManager.h>
#include <nFramework/mec/MECComponent.h>
#include <nFramework/nom/NOMMain.h>

using namespace nframework;
using namespace nom;

class BASEMGRDLL_API CommandManager : public BaseManager
{
public:
	CommandManager(void);
	~CommandManager(void);

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

private:
	IMEBComponent* meb;
	MECComponent* mec;
	tstring name;
	map<unsigned int, shared_ptr<NOM>> registeredMsg;
	map<unsigned int, shared_ptr<NOM>> discoveredMsg;

	// you can change the code, if necessary
	shared_ptr<NOM> ICD_TestNOM;
};

