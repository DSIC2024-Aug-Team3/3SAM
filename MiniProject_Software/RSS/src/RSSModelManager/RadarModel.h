#pragma once
#include <thread>
#include <chrono>
#include <functional>
#include <nFramework/BaseManager.h>

using namespace nframework;

class RadarModel
{
public:
	RadarModel();
	~RadarModel();

	void setThreatPosition(double x, double y);
	void setThreatTargetPosition(double x, double y);
	void start();
	void stop();
	void FireMissile(double antiLat, double antiLon, double missileLat, double missileLon);

private:
	std::thread* simThread = nullptr;

	double initX, initY;
	double targetX, targetY;
	double speed;
	double currentX, currentY;
	bool firecommand;
	shared_ptr<NOM> missileNOM;
	bool isThreadRunning = false;
	void setManager(BaseManager* mgr);
	void setNOM(shared_ptr<NOM>);

private:
	void threatSimulationThread();
	void updateThreatPosition();
	double antiLat, antiLon, missileLat, missileLon;
	shared_ptr<NOM> RadNOM;
	BaseManager* userManager;

};

