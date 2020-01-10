#pragma once

#include "PlanistaProcesora.hpp"
#include "Includes.hpp"

class PlanistaProcesora;

class Event
{
private:
	std::shared_ptr<PlanistaProcesora> planistaProcesora;

public:
	void DodajPlaniste(std::shared_ptr<PlanistaProcesora> planista);
	void Sygnal();
	int SygnalPID();
	void SygnalWaiting();
};