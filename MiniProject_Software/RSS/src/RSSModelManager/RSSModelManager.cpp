#include "RSSModelManager.h"
#include <math.h>
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
	Radar = new RadarModel();
	// airThreat->setThreatPosition(1.0, 1.0);
	// airThreat->setThreatTargetPosition(10.0, 10.0);
	tcout << _T("Radar System Simulator Start!") << endl;
	// binding message
	function<void(shared_ptr<NOM>)> msgProcessor;
	msgProcessor = bind(&RSSModelManager::DeployMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("deployMsg"), msgProcessor));
	
	function<void(shared_ptr<NOM>)> msgProcessor1;
	msgProcessor1 = bind(&RSSModelManager::StartMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("startMsg"), msgProcessor1));

	function<void(shared_ptr<NOM>)> msgProcessor2;
	msgProcessor2 = bind(&RSSModelManager::StopMsg, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("stopMsg"), msgProcessor2));

	function<void(shared_ptr<NOM>)> msgProcessor3;
	msgProcessor3 = bind(&RSSModelManager::GetAirThreatLocation, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("R_antiInfoMsg"), msgProcessor3));

	function<void(shared_ptr<NOM>)> msgProcessor4;
	msgProcessor4 = bind(&RSSModelManager::GetMissileLocation, this, placeholders::_1);
	msgMethodMap.insert(make_pair(_T("R_missileInfoMsg"), msgProcessor4));

	return true;
}

bool
RSSModelManager::stop()
{
	// if need be, write your code
	delete Radar;

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
// �ó����� ���� ���� �� ������ �Լ�
void
RSSModelManager::DeployMsg(shared_ptr<NOM> nomMsg)
{
	firecommand = true;
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("deployChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(2));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(3));
	sendMsg(Msg);

	radarLat = nomMsg->getValue(_T("RadarInitInfo.RadarLat"))->toDouble();
	radarLon = nomMsg->getValue(_T("RadarInitInfo.RadarLon"))->toDouble();
	missileLat = nomMsg->getValue(_T("LauncherInitInfo.LauncherLat"))->toDouble();
	missileLon = nomMsg->getValue(_T("LauncherInitInfo.LauncherLon"))->toDouble();
}

// ���� ���� ��ȣ ���� �� ������ �Լ�
void
RSSModelManager::StartMsg(shared_ptr<NOM> nomMsg)
{
	// ���� ���� ��ȣ�� �� �޾Ҵٰ� ������ �� ��ȣ �۽�
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("startChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(4));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(3));
	sendMsg(Msg);
}

// ���� ���� ��ȣ ���� �� ������ �Լ�
void
RSSModelManager::StopMsg(shared_ptr<NOM> nomMsg)
{
	// ���� ���� ��ȣ�� �� �޾Ҵٰ� ������ �� ��ȣ �۽�
	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("stopChkMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(13));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(8));
	Msg->setValue(_T("SrcId"), &NUInteger(3));
	sendMsg(Msg);
}

// �������� ��ġ ���� �� ������ �Լ�
void
RSSModelManager::GetAirThreatLocation(shared_ptr<NOM> nomMsg)
{
	// ��������⿡�� ��ġ ����
	antiLat = nomMsg->getValue(_T("AntiInfo.AntiLat"))->toDouble();
	antiLon = nomMsg->getValue(_T("AntiInfo.AntiLon"))->toDouble();
	// ���������� �� ��ǥ�� �۽�, ����ź���Ǳ�ε� �� ��ǥ�� �۽�
	// ******************************************************************************
	// if �����Ÿ� �̳��� �����߰� (���̴ٸ��Ǳ� ��ġ ���� 200km �̳�)
	double radarAntiDiffLat = radarLat - antiLat >= 0 ? radarLat - antiLat : -(radarLat - antiLat);
	double radarAntiDiffLon = radarLon - antiLon >= 0 ? radarLon - antiLon : -(radarLon - antiLon);
	radarAntiDiffLat *= 110;
	radarAntiDiffLon *= 86;
	double res = sqrt(radarAntiDiffLat * radarAntiDiffLat + radarAntiDiffLon * radarAntiDiffLon);
	if (res < 200)
	{
		auto missileNOM = meb->getNOMInstance(name, _T("sendfireMsg"));
		shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("O_antiInfoMsg"));
		Msg->setValue(_T("MessageHeader.ID"), &NUShort(6));
		Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(20));
		Msg->setValue(_T("AntiInfo.AntiLat"), &NDouble(antiLat)); //** Lat > antiLat
		Msg->setValue(_T("AntiInfo.AntiLon"), &NDouble(antiLon)); //** Lon > antiLon
		sendMsg(Msg);

		double missileAntiDiffLat = missileLat - antiLat >= 0 ? missileLat - antiLat : -(missileLat - antiLat);
		double missileAntiDiffLon = missileLon - antiLon >= 0 ? missileLon - antiLon : -(missileLon - antiLon);
		missileAntiDiffLat *= 110;
		missileAntiDiffLon *= 86;
		res = sqrt(missileAntiDiffLat * missileAntiDiffLat + missileAntiDiffLon * missileAntiDiffLon);
		if (res < 50)
		{
			//����ź �߻� ����
			//��������⿡�� �߻� ��� ������********
			if (firecommand)
			{
				tcout << _T("����ź ���� �� Ž��. ����ź �߻� ���") << endl;
				Msg = meb->getNOMInstance(name, _T("sendfireMsg"));
				Msg->setValue(_T("MessageHeader.ID"), &NUShort(14));
				Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(4));

				sendMsg(Msg);
				firecommand = false;
			}
		}
	}
	// ******************************************************************************

}

// ����ź ��ġ ���� �� ������ �Լ�
void
RSSModelManager::GetMissileLocation(shared_ptr<NOM> nomMsg)
{
	// ��������⿡�� ��ġ ����
	missileLat = nomMsg->getValue(_T("MissileInfo.MissileLat"))->toDouble();
	missileLon = nomMsg->getValue(_T("MissileInfo.MissileLon"))->toDouble();
	// ���������� �� ��ǥ�� �۽�
	// ******************************************************************************

	shared_ptr<NOM> Msg = meb->getNOMInstance(name, _T("O_missileInfoMsg"));
	Msg->setValue(_T("MessageHeader.ID"), &NUShort(10));
	Msg->setValue(_T("MessageHeader.SIZE"), &NUShort(20));
	Msg->setValue(_T("MissileInfo.MissileLat"), &NDouble(missileLat));
	Msg->setValue(_T("MissileInfo.MissileLon"), &NDouble(missileLon));
	sendMsg(Msg);
	// ******************************************************************************

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

