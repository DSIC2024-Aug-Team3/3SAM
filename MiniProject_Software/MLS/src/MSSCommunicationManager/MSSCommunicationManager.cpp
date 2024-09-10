#include "MSSCommunicationManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
MSSCommunicationManager::MSSCommunicationManager(void)
{
	init();
}

MSSCommunicationManager::~MSSCommunicationManager(void)
{
	release();
}

/************************************************************************
	initialize / release
************************************************************************/
void
MSSCommunicationManager::init()
{
	setUserName(_T("MSSCommunicationManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);

	commConfig = new CommunicationConfig;
	commConfig->setIni(_T("MSSCommLinkInfo.ini"));
	commInterface = new NCommInterface(this);
}

void
MSSCommunicationManager::release()
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
MSSCommunicationManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
MSSCommunicationManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
MSSCommunicationManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
MSSCommunicationManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code

	commInterface->sendCommMsg(nomMsg);
}

void
MSSCommunicationManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
MSSCommunicationManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
MSSCommunicationManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
MSSCommunicationManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
	commInterface->sendCommMsg(nomMsg);
}

void
MSSCommunicationManager::setUserName(tstring userName)
{
	name = userName; 
}

tstring
MSSCommunicationManager::getUserName()
{
	return name;
}

void
MSSCommunicationManager::setData(void* data)
{
	// if need be, write your code
}

bool
MSSCommunicationManager::start()
{
	commInterface->setMEBComponent(meb);
	MessageProcessor msgProcessor = bind(&MSSCommunicationManager::processRecvMessage, this, placeholders::_1, placeholders::_2);
	commConfig->setMsgProcessor(msgProcessor);
	commInterface->initNetEnv(commConfig);
	tcout << _T("MSS Communication connect") << endl;
	return true;
}

bool
MSSCommunicationManager::stop()
{
	commInterface->releaseNetEnv(commConfig);
	return true;
}

void
MSSCommunicationManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

void
MSSCommunicationManager::processRecvMessage(unsigned char* data, int size)
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
	return new MSSCommunicationManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager* userManager)
{
	delete userManager;
}

