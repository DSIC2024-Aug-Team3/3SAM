#include "MSSModelManager.h"

/************************************************************************
	Constructor / Destructor
************************************************************************/
MSSModelManager::MSSModelManager(void)
{
	init();
}

MSSModelManager::~MSSModelManager(void)
{
	release();
}

void
MSSModelManager::init()
{
	setUserName(_T("MSSModelManager"));

	// by contract
	mec = new MECComponent;
	mec->setUser(this);
}

void
MSSModelManager::release()
{
	meb = nullptr;
	delete mec;
	mec = nullptr;
}

/************************************************************************
	Inherit Function
************************************************************************/
shared_ptr<NOM>
MSSModelManager::registerMsg(tstring msgName)
{
	shared_ptr<NOM> nomMsg = mec->registerMsg(msgName);
	registeredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));

	return nomMsg;
}

void
MSSModelManager::discoverMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.insert(pair<unsigned int, shared_ptr<NOM>>(nomMsg->getInstanceID(), nomMsg));
}

void
MSSModelManager::updateMsg(shared_ptr<NOM> nomMsg)
{
	mec->updateMsg(nomMsg);
}

void
MSSModelManager::reflectMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
}

void
MSSModelManager::deleteMsg(shared_ptr<NOM> nomMsg)
{
	mec->deleteMsg(nomMsg);
	registeredMsg.erase(nomMsg->getInstanceID());
}

void
MSSModelManager::removeMsg(shared_ptr<NOM> nomMsg)
{
	discoveredMsg.erase(nomMsg->getInstanceID());
}

void
MSSModelManager::sendMsg(shared_ptr<NOM> nomMsg)
{
	mec->sendMsg(nomMsg);
}

void
MSSModelManager::recvMsg(shared_ptr<NOM> nomMsg)
{
	// if need be, write your code
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
MSSModelManager::setUserName(tstring userName)
{
	name = userName;
}

tstring
MSSModelManager::getUserName()
{
	return name;
}

void
MSSModelManager::setData(void* data)
{
	// if need be, write your code
}

bool
MSSModelManager::start()
{
	// if need be, write your code
	tcout << _T("Missile System Simulator Start!") << endl;
	fireflag = false;
	function<void(shared_ptr<NOM>)> msgProcessor;
	msgProcessor = bind(&MSSModelManager::StartMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("startMsg"), msgProcessor));

	function<void(shared_ptr<NOM>)> msgProcessor1;
	msgProcessor1 = bind(&MSSModelManager::DeployMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("deployMsg"), msgProcessor1));

	function<void(shared_ptr<NOM>)> msgProcessor2;
	msgProcessor2 = bind(&MSSModelManager::StopMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("stopMsg"), msgProcessor2));

	function<void(shared_ptr<NOM>)> msgProcessor3;
	msgProcessor3 = bind(&MSSModelManager::InterceptResMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("interceptResMsg"), msgProcessor3));
	
	function<void(shared_ptr<NOM>)> msgProcessor4;
	msgProcessor4 = bind(&MSSModelManager::FireMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("fireMsg"), msgProcessor4));

	function<void(shared_ptr<NOM>)> msgProcessor5;
	msgProcessor5 = bind(&MSSModelManager::getAntiLocation, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("O_antiInfoMsg"), msgProcessor5));


	return true;

	//// binding message
	//function<void(shared_ptr<NOM>)> msgProcessor;
	//msgProcessor = bind(&MSSModelManager::processStartMsg, this, placeholders::_1);
	//msgMethodMap.insert(make_pair(_T("Start"), msgProcessor));

	//return true;
}

bool
MSSModelManager::stop()
{
	// if need be, write your code
	delete missile;

	return true;
}

void
MSSModelManager::setMEBComponent(IMEBComponent* realMEB)
{
	meb = realMEB;
	mec->setMEB(meb);
}


void
MSSModelManager::DeployMsg(shared_ptr<NOM> nomMsg)
{
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("deployChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(2));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(2));
	sendMsg(Msg);

	initLat = nomMsg->getValue(_T("LauncherInitInfo.LauncherLat"))->toDouble();
	initLon = nomMsg->getValue(_T("LauncherInitInfo.LauncherLon"))->toDouble();
}

// 모의 시작 신호 수신 시 실행할 함수
void
MSSModelManager::StartMsg(shared_ptr<NOM> nomMsg)
{
	// 모의 시작 신호를 잘 받았다고 운통기로 답 신호 송신
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("startChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(4));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(2));
	sendMsg(Msg);
}

// 이탈 명령 수신 시 실행할 함수
void
MSSModelManager::FireMsg(shared_ptr<NOM> nomMsg)
{
	// 유도탄 동작 시작
	// MLS에게 MSS의 초기위치, 목적위치, 속도를 받음
	currentX = initLat;
	currentY = initLon;
	fireflag = true;

}

// 모의 중지 신호 수신 시 실행할 함수
void
MSSModelManager::StopMsg(shared_ptr<NOM> nomMsg)
{
	// 모의 중지 신호를 잘 받았다고 운통기로 답 신호 송신
	fireflag = false;
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("stopChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(13));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(2));
	sendMsg(Msg);
}

// 요격 결과 수신 시 실행할 함수
void
MSSModelManager::InterceptResMsg(shared_ptr<NOM> nomMsg)
{
	// 요격 결과를 확인하고 만약 요
	int result;
	result = nomMsg->getValue(_T("interceptRes"))->toUInt();
	if (result == 1) // 요격 성공
	{
		return; // ATSMain 함수로 돌아가기
	}
	else // 요격 실패
	{
		return; // ATSMain 함수로 돌아가기
	}
}

// RSS에게 공중위협의 위치를 수신하면
void
MSSModelManager::getAntiLocation(shared_ptr<NOM> nomMsg)
{
	if (fireflag)
	{
		antiLat = nomMsg->getValue(_T("AntiInfo.AntiLat"))->toDouble();
		antiLon = nomMsg->getValue(_T("AntiInfo.AntiLon"))->toDouble();
		auto missileNOM = meb->getNOMInstance(name, _T("R_missileInfoMsg")); // 레이다로 유도탄의 위치 계속 전송

		double speed = 1700.0;
		//공중위협의 xy
		double deltaX, deltaY;
		double theta;
		deltaX = (antiLat - currentX);
		deltaY = (antiLon - currentY);

		deltaX *= 110;
		deltaY *= 86;

		theta = abs(atan(deltaY / deltaX));

		if (currentX < antiLat)
			currentX = currentX + speed * cos(theta) / 110000;
		else
			currentX = currentX - speed * cos(theta) / 110000;

		if (currentY < antiLon)
			currentY = currentY + speed * sin(theta) / 86000;
		else
			currentY = currentY - speed * sin(theta) / 86000;

		//********레이다로 currentX, currentY 전송해야함
		missileNOM->setValue(_T("MessageHeader.ID"), &NUShort(9));
		missileNOM->setValue(_T("MessageHeader.SIZE"), &NUShort(20));
		missileNOM->setValue(_T("MissileInfo.MissileLat"), &NDouble(currentX));
		missileNOM->setValue(_T("MissileInfo.MissileLon"), &NDouble(currentY));

		sendMsg(missileNOM);
	}
	// 만약 initLat과 antiLat, initLon과 antiLon을 계산했을 때 유도탄 발사 범위 이내인 경우
	/*
	if ()
	{
		missile->setMissilePosition(initLat, initLon)
		missile->setMissileTargetPosition(antiLat, antiLon)
		// missile 객체 내의 함수들에서 미사일 격추하기
		// 그리고 격추 결과를 return 받기??
	}
	*/
}
/************************************************************************
	Export Function
************************************************************************/
extern "C" BASEMGRDLL_API
BaseManager * createObject()
{
	return new MSSModelManager;
}

extern "C" BASEMGRDLL_API
void deleteObject(BaseManager * userManager)
{
	delete userManager;
}

