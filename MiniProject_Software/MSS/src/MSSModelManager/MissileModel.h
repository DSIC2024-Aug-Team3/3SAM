#pragma once
#include <thread>
#include <chrono>
#include <functional>
#include <nFramework/BaseManager.h>

using namespace nframework;

class MissileModel
{
public:
	MissileModel();
	~MissileModel();

	void setMissilePosition(double x, double y);
	void setMissileTargetPosition(double x, double y);
	void start(double x, double y);
	void calculator(double x, double y, shared_ptr<NOM> nomMsg, double s);
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
	shared_ptr<NOM> missileNOM;
	BaseManager* userManager;
};

