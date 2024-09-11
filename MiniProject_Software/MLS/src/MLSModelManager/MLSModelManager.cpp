#include "MLSModelManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
MLSModelManager::MLSModelManager(void)
{
	init();
}

MLSModelManager::~MLSModelManager(void)
{
	release();
}

void
MLSModelManager::init()
{
	setUserName(_T("MLSModelManager"));
	// by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
MLSModelManager::release()
{
	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
MLSModelManager::registerMsg(tstring msgName)
{


	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
MLSModelManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
MLSModelManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
MLSModelManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
MLSModelManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
MLSModelManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
MLSModelManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
MLSModelManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	for (auto it = msgMethodMap.begin(); it != msgMethodMap.end(); ++it)
	{
		if (it->first == nomMsg->getName())
		{
			tcout << _T("해당 메시지는 ") << nomMsg->getName() << _T("입니다.") << endl;
			it->second(nomMsg);
		}
	}
}

void
MLSModelManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
MLSModelManager::getUserName()
{
	return name;
}

void
MLSModelManager::setData(void* data)
{
	// if need be, write your code
}

bool
MLSModelManager::start()
{
	// if need be, write your code
	tcout << _T("Missile Launcher Simulator Start!") << endl;
	function<void(shared_ptr<NOM>)> msgProcessor;
	msgProcessor = bind(&MLSModelManager::DeployMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("deployMsg"), msgProcessor));

	function<void(shared_ptr<NOM>)> msgProcessor1;
	msgProcessor1 = bind(&MLSModelManager::StartMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("startMsg"), msgProcessor1));

	function<void(shared_ptr<NOM>)> msgProcessor2;
	msgProcessor2 = bind(&MLSModelManager::StopMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("stopMsg"), msgProcessor2));

	function<void(shared_ptr<NOM>)> msgProcessor3;
	msgProcessor3 = bind(&MLSModelManager::LaunchingMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("sendfireMsg"), msgProcessor3));

	return true;
}

bool
MLSModelManager::stop()
{
	// if need be, write your code
	delete missileLauncher;

	return true;
}

void
MLSModelManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}

/************************************************************************
	message processor
************************************************************************/

// 시나리오 배포 수신 시 실행할 함수
void
MLSModelManager::DeployMsg(shared_ptr<NOM> nomMsg)
{
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("deployChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(2));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(4));
	sendMsg(Msg);

	double lat = nomMsg->getValue(_T("LauncherInitInfo.LauncherLat"))->toDouble();
	double lon = nomMsg->getValue(_T("LauncherInitInfo.LauncherLon"))->toDouble();
}

// 모의 시작 신호를 수신 시 실행할 함수
void
MLSModelManager::StartMsg(shared_ptr<NOM> nomMsg)
{
	// 모의 시작 신호를 잘 받았다고 운통기로 답 신호 송신
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("startChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(4));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(4));
	sendMsg(Msg);
}

// 모의 중지 신호 수신 시 실행할 함수
void
MLSModelManager::StopMsg(shared_ptr<NOM> nomMsg)
{
	// 모의 중지 신호를 잘 받았다고 운통기로 답 신호 송신
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("stopChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(13));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(4));
	sendMsg(Msg);
}

// 
//  신호 수신 시 실행할 함수
void
MLSModelManager::LaunchingMsg(shared_ptr<NOM> nomMsg)
{
	misileNum = misileNum - 1;
	if (misileNum < 0)
	{
		tcout << _T("재고 부족") << endl;
		//shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("fireMsg"));
		//Msg->setValue(_T("MessageHeader.ID"), &NUShort(8));
		//Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(4));
	}
	else {
		tcout << _T("미사일 발사") << endl;
		shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("fireMsg"));
		Msg->setValue(_T("MessageHeader.ID"), &NUShort(8));
		Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(4));
		sendMsg(Msg);
	}


}

/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager* createObject()
{
	return new MLSModelManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager* userManager)
{
	delete userManager;
}

