#pragma once
#include "PlanistaProcesora.hpp"
#include "ProcessManager.hpp"
#include "Includes.hpp"
#include "Event.hpp"

class Synch
{
private:
	bool lockValue = false;
	bool test_and_set();
	int pid;
	std::shared_ptr <Event> event;
	//std::shared_ptr<PlanistaProcesora> planistaProcesora;
	//std::shared_ptr<ProcessManager> processManager;
public:
	Synch();
	Synch(std::shared_ptr < Event> event);
	//Synch(std::shared_ptr<PlanistaProcesora> planistaProcesora);
	//void DodajPlaniste(std::shared_ptr<PlanistaProcesora> planistaProcesora);
	void lock();
	void unlock();
	~Synch();
};