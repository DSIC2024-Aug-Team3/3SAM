#pragma once
#include <thread>
#include <chrono>
#include <functional>

class MissileModel
{
public:
	MissileModel();
	~MissileModel();

	void setMissilePosition(double x, double y);
	void setThreatTargetPosition(double x, double y);
	void start();
	void stop();

private:
	std::thread* simThread = nullptr;

	double initX, initY;
	double targetX, targetY;
	double currentX, currentY;

	bool isThreadRunning = false;

private:
	void threatSimulationThread();
	void updateThreatPosition();
};

