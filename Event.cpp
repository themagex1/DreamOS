#include "Event.hpp"

void Event::DodajPlaniste(std::shared_ptr<PlanistaProcesora> planista)
{
	planistaProcesora = planista;
}

void Event::Sygnal()
{
	if (planistaProcesora != nullptr)
		planistaProcesora->SprawdzProcesy();
}

int Event::SygnalPID()
{
	return planistaProcesora->AKTUALNY->GetPID();
}

void Event::SygnalWaiting()
{
	planistaProcesora->managerOfProcesses->addToWaiting(planistaProcesora->AKTUALNY);
}
