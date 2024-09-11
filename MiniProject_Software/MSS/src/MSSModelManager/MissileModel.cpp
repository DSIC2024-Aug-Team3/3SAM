#include "MissileModel.h"
#include "MSSModelManager.h"
#include <math.h>
#include <iostream>

MissileModel::MissileModel()
{

}

MissileModel::~MissileModel()
{
	// if need be, write your code
}

void MissileModel::setMissilePosition(double x, double y)
{
	initX = x;
	initY = y;
}

void MissileModel::setMissileTargetPosition(double x, double y)
{
	targetX = x;
	targetY = y;

	// simple model
}

void MissileModel::start(double x, double y)
{
	/*currentX = initX;
	currentY = initY;

	isThreadRunning = true;
	simThread = new std::thread(std::bind(&MissileModel::threatSimulationThread, this));*/

	// 미사일이 기동하는 함수 -> 좌표 변경
		//초기 위치 설정하고
		//공중위협 위치를 레이다한테 받을 때 -> 나도 똑같이 업데이트
	currentX = x;
	currentY = y;
}

/*새로 추가된 method
미사일 위치 정보 업데이트 함수*/
void MissileModel::calculator(double antiX, double antiY, shared_ptr<NOM> nomMsg, double s)
{
	double speed = s;
	//공중위협의 xy
	double deltaX, deltaY;
	double theta;
	deltaX = (antiX - currentX);
	deltaY = (antiY - currentY);

	deltaX *= 110;
	deltaY *= 86;

	theta = abs(atan(deltaY / deltaX));

	if (currentX < antiX)
		currentX = currentX + speed * cos(theta) / 110000;
	else
		currentX = currentX - speed * cos(theta) / 110000;

	if (currentY < antiY)
		currentY = currentY + speed * sin(theta) / 86000;
	else
		currentY = currentY - speed * sin(theta) / 86000;

	//********레이다로 currentX, currentY 전송해야함
	nomMsg->setValue(_T("MessageHeader.ID"), &NUShort(9));
	nomMsg->setValue(_T("MessageHeader.SIZE"), &NUShort(20));
	nomMsg->setValue(_T("MissileInfo.MissileLat"), &NDouble(currentX));
	nomMsg->setValue(_T("MissileInfo.MissileLon"), &NDouble(currentY));

	userManager->sendMsg(nomMsg);
}

void MissileModel::stop()
{
	isThreadRunning = false;

	if (simThread)
	{
		simThread->join();
		delete simThread;
		simThread = nullptr;
	}
}