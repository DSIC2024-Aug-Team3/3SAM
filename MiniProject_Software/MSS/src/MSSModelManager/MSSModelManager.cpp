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
			tcout << _T("�ش� �޽����� ") << nomMsg->getName() << _T("�Դϴ�.") << endl;
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

// ���� ���� ��ȣ ���� �� ������ �Լ�
void
MSSModelManager::StartMsg(shared_ptr<NOM> nomMsg)
{
	// ���� ���� ��ȣ�� �� �޾Ҵٰ� ������ �� ��ȣ �۽�
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("startChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(4));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(2));
	sendMsg(Msg);
}

// ��Ż ��� ���� �� ������ �Լ�
void
MSSModelManager::FireMsg(shared_ptr<NOM> nomMsg)
{
	// ����ź ���� ����
	// MLS���� MSS�� �ʱ���ġ, ������ġ, �ӵ��� ����
	currentX = initLat;
	currentY = initLon;
	fireflag = true;

}

// ���� ���� ��ȣ ���� �� ������ �Լ�
void
MSSModelManager::StopMsg(shared_ptr<NOM> nomMsg)
{
	// ���� ���� ��ȣ�� �� �޾Ҵٰ� ������ �� ��ȣ �۽�
	fireflag = false;
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("stopChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(13));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(2));
	sendMsg(Msg);
}

// ��� ��� ���� �� ������ �Լ�
void
MSSModelManager::InterceptResMsg(shared_ptr<NOM> nomMsg)
{
	// ��� ����� Ȯ���ϰ� ���� ��
	int result;
	result = nomMsg->getValue(_T("interceptRes"))->toUInt();
	if (result == 1) // ��� ����
	{
		return; // ATSMain �Լ��� ���ư���
	}
	else // ��� ����
	{
		return; // ATSMain �Լ��� ���ư���
	}
}

// RSS���� ���������� ��ġ�� �����ϸ�
void
MSSModelManager::getAntiLocation(shared_ptr<NOM> nomMsg)
{
	if (fireflag)
	{
		antiLat = nomMsg->getValue(_T("AntiInfo.AntiLat"))->toDouble();
		antiLon = nomMsg->getValue(_T("AntiInfo.AntiLon"))->toDouble();
		auto missileNOM = meb->getNOMInstance(name, _T("R_missileInfoMsg")); // ���̴ٷ� ����ź�� ��ġ ��� ����

		double speed = 1700.0;
		//���������� xy
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

		//********���̴ٷ� currentX, currentY �����ؾ���
		missileNOM->setValue(_T("MessageHeader.ID"), &NUShort(9));
		missileNOM->setValue(_T("MessageHeader.SIZE"), &NUShort(20));
		missileNOM->setValue(_T("MissileInfo.MissileLat"), &NDouble(currentX));
		missileNOM->setValue(_T("MissileInfo.MissileLon"), &NDouble(currentY));

		sendMsg(missileNOM);
	}
	// ���� initLat�� antiLat, initLon�� antiLon�� ������� �� ����ź �߻� ���� �̳��� ���
	/*
	if ()
	{
		missile->setMissilePosition(initLat, initLon)
		missile->setMissileTargetPosition(antiLat, antiLon)
		// missile ��ü ���� �Լ��鿡�� �̻��� �����ϱ�
		// �׸��� ���� ����� return �ޱ�??
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

