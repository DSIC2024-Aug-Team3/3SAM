#pragma once
#include <thread>
#include <chrono>
#include <functional>

class AirthreatModel
{
public:
	AirthreatModel();
	~AirthreatModel();

	void setThreatPosition(double x, double y);
	void setThreatTargetPosition(double x, double y);
	void start();
	void stop();

private:
	std::thread* simThread = nullptr;

	double initX, initY;
	double targetX, targetY;
	double speed;
	double currentX, currentY;

	bool isThreadRunning = false;

private:
	void threatSimulationThread();
	void updateThreatPosition();
};

