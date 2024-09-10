#include "RSSModelManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
RSSModelManager::RSSModelManager(void)
{
	init();
}

RSSModelManager::~RSSModelManager(void)
{
	release();
}

void
RSSModelManager::init()
{
	setUserName(_T("RSSModelManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
RSSModelManager::release()
{
	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
RSSModelManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
RSSModelManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
RSSModelManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
RSSModelManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
RSSModelManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
RSSModelManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
RSSModelManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
RSSModelManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
RSSModelManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
RSSModelManager::getUserName()
{
	return name;
}

void
RSSModelManager::setData(void* data)
{
	// if need be, write your code
}

bool
RSSModelManager::start()
{
	// if need be, write your code
	airThreat = new AirthreatModel;
	airThreat->setThreatPosition(1.0, 1.0);
	airThreat->setThreatTargetPosition(10.0, 10.0);

	// binding message
	function<void(shared_ptr<NOM>)> msgProcessor;
	msgProcessor = bind(&RSSModelManager::processStartMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("Start"), msgProcessor));

	return true;
}

bool
RSSModelManager::stop()
{
	// if need be, write your code
	delete airThreat;

	return true;
}

void
RSSModelManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

/************************************************************************
	message processor
************************************************************************/
void
RSSModelManager::processStartMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager * createObject()
{
	return new RSSModelManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager * userManager)
{
	delete userManager;
}

