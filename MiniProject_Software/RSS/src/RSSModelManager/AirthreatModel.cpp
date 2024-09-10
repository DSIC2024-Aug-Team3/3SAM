#include "AirthreatModel.h"
#include <math.h>
#include <iostream>

AirthreatModel::AirthreatModel()
{
	initX = 0;
	initY = 0;
	targetX = 0;
	targetY = 0;
	speed = 0;
}

AirthreatModel::~AirthreatModel()
{
	// if need be, write your code
}

void AirthreatModel::setThreatPosition(double x, double y)
{
	initX = x;
	initY = y;
}

void AirthreatModel::setThreatTargetPosition(double x, double y)
{
	targetX = x;
	targetY = y;

	// simple model
	speed = sqrt((targetX - initX) * (targetX - initX) + (targetY - initY)) / 30.0;
}

void AirthreatModel::start()
{
	currentX = initX;
	currentY = initY;

	isThreadRunning = true;
	simThread = new std::thread(std::bind(&AirthreatModel::threatSimulationThread, this));
}

void AirthreatModel::stop()
{
	isThreadRunning = false;

	if (simThread)
	{
		simThread->join();
		delete simThread;
		simThread = nullptr;
	}
}

void AirthreatModel::threatSimulationThread()
{
	while (isThreadRunning)
	{
		updateThreatPosition();

		// the code below may be done by updating the target destination
		// currentX, currentY

		// check the range and end condition
		if ((initX < targetX && currentX > targetX) || (initX > targetX&& currentX < targetX))
		{
			isThreadRunning = false;
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void AirthreatModel::updateThreatPosition()
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

	std::cout << "Threat position : (" << currentX << ", " << currentY << ")" << std::endl;
}
