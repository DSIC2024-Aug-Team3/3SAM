#include <math.h>
#include <iostream>
#include "RadarModel.h"
#include "RSSModelManager.h"

RadarModel::RadarModel()
{
	initX = 0;
	initY = 0;
	targetX = 0;
	targetY = 0;
	speed = 0;
	firecommand = true;
}

RadarModel::~RadarModel()
{
	// if need be, write your code
}

void RadarModel::setThreatPosition(double x, double y)
{
	initX = x;
	initY = y;
}

void RadarModel::setThreatTargetPosition(double x, double y)
{
	targetX = x;
	targetY = y;

	// simple model
	speed = sqrt((targetX - initX) * (targetX - initX) + (targetY - initY)) / 30.0;
}


void RadarModel::start()
{
	currentX = initX;
	currentY = initY;

	isThreadRunning = true;
	simThread = new std::thread(std::bind(&RadarModel::threatSimulationThread, this));
}

void RadarModel::stop()
{
	isThreadRunning = false;

	if (simThread)
	{
		simThread->join();
		delete simThread;
		simThread = nullptr;
	}
}

void RadarModel::threatSimulationThread()
{
	while (isThreadRunning)
	{
		updateThreatPosition();
		if ((initX < targetX && currentX > targetX) || (initX > targetX&& currentX < targetX))
		{
			isThreadRunning = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
void RadarModel::setManager(BaseManager* mgr)
{
	userManager = mgr;
}

void RadarModel::setNOM(shared_ptr<NOM> nomMsg)
{
	RadNOM = nomMsg;

}

void RadarModel::updateThreatPosition()
{
	// you can change the model

	float deltaX, deltaY;
	float theta;

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

	std::cout << "Threat position : (" << currentX << ", " << currentY << ")" << std::endl;
}
