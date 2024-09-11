#include "MissileLauncherModel.h"
#include <math.h>
#include <iostream>

MissileLauncherModel::MissileLauncherModel()
{
	initX = 0;
	initY = 0;
	targetX = 0;
	targetY = 0;
	speed = 0;
}

MissileLauncherModel::~MissileLauncherModel()
{
	// if need be, write your code
}

void MissileLauncherModel::setThreatPosition(double x, double y)
{
	initX = x;
	initY = y;
}

void MissileLauncherModel::setThreatTargetPosition(double x, double y)
{
	targetX = x;
	targetY = y;

	// simple model
	speed = sqrt((targetX - initX) * (targetX - initX) + (targetY - initY)) / 30.0;
}

void MissileLauncherModel::start()
{
	currentX = initX;
	currentY = initY;

	isThreadRunning = true;
	simThread = new std::thread(std::bind(&MissileLauncherModel::threatSimulationThread, this));
}

void MissileLauncherModel::stop()
{
	isThreadRunning = false;

	if (simThread)
	{
		simThread->join();
		delete simThread;
		simThread = nullptr;
	}
}

void MissileLauncherModel::threatSimulationThread()
{
	while (isThreadRunning)
	{
		updateThreatPosition();

		// the code below may be done by updating the target destination
		// currentX, currentY

		// check the range and end condition
		// NOM nomMsg = meb->getNOMInstance();

		// Nom 메시지 생성해서 X, Y 좌표 받고 그걸 mec->send 하면 UDP가 subscribe하다가 이걸 받아서 다시 레이다로 보내는 코드 적기
		if ((initX < targetX && currentX > targetX) || (initX > targetX && currentX < targetX))
		{
			isThreadRunning = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void MissileLauncherModel::updateThreatPosition()
{
	// you can change the model

	double deltaX, deltaY;
	double theta;

	deltaX = targetX - initX;
	deltaY = targetY - initY;

	if (deltaX == 0.0 || deltaY == 0.0)
		return;

	theta = abs(atan(deltaY / deltaX));

	if (initX < targetX)
		currentX = currentX + speed * cos(theta);
	else
		currentX = currentX - speed * cos(theta);

	if (initY < targetY)
		currentY = currentY + speed * sin(theta);
	else
		currentY = currentY - speed * sin(theta);

}
