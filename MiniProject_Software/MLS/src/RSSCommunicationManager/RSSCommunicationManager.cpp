#include "RSSCommunicationManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
RSSCommunicationManager::RSSCommunicationManager(void)
{
	init();
}

RSSCommunicationManager::~RSSCommunicationManager(void)
{
	release();
}

/************************************************************************
	initialize / release
************************************************************************/
void
RSSCommunicationManager::init()
{
	setUserName(_T("RSSCommunicationManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);

	commConfig = new CommunicationConfig;
	commConfig->setIni(_T("RSSCommLinkInfo.ini"));
	commInterface = new NCommInterface(this);
}

void
RSSCommunicationManager::release()
{
	delete commConfig;
	delete commInterface;

	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
RSSCommunicationManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
RSSCommunicationManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
RSSCommunicationManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
RSSCommunicationManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code

	commInterface->sendCommMsg(nomMsg);
}

void
RSSCommunicationManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
RSSCommunicationManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
RSSCommunicationManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
RSSCommunicationManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code

	commInterface->sendCommMsg(nomMsg);
}

void
RSSCommunicationManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
RSSCommunicationManager::getUserName()
{
	return name;
}

void
RSSCommunicationManager::setData(void* data)
{
	// if need be, write your code
}

bool
RSSCommunicationManager::start()
{
	commInterface->setMEBComponent(meb);
	MessageProcessor msgProcessor = bind(&RSSCommunicationManager::processRecvMessage, this, placeholders::_1, placeholders::_2);
	commConfig->setMsgProcessor(msgProcessor);
	commInterface->initNetEnv(commConfig);
	tcout << _T("RSS Communication connect") << endl;
	return true;
}

bool
RSSCommunicationManager::stop()
{
	commInterface->releaseNetEnv(commConfig);
	return true;
}

void
RSSCommunicationManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

void
RSSCommunicationManager::processRecvMessage(unsigned char* data, int size)
{
	auto IDSize = commConfig->getHeaderIDSize();
	auto IDPos = commConfig->getHeaderIDPos();

	unsigned short tmpMsgID;
	memcpy(&tmpMsgID, data + IDPos, IDSize);
	auto msgID = ntohs(tmpMsgID);

	auto nomMsg = meb->getNOMInstance(name, commMsgHandler.getMsgName(msgID));

	if (nomMsg.get())
	{
		if (nomMsg->getType() == NOM_TYPE_OBJECT)
		{
			nomMsg->deserialize(data, size);
			this->updateMsg(nomMsg);
		}
		else
		{
			auto nomMsgCP = nomMsg->clone();
			nomMsgCP->deserialize(data, size);
			this->sendMsg(nomMsgCP);
		}
	}
	else
	{
		tcerr << _T("not defined message") << endl;
	}
}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager* createObject()
{
	return new RSSCommunicationManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager* userManager)
{
	delete userManager;
}

