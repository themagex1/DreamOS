
#include "PlanistaProcesora.hpp"
#include "ProcessManager.hpp"
#include "Includes.hpp"

PlanistaProcesora::PlanistaProcesora(std::shared_ptr<ManagerOfProcesses> MoP)
{
    managerOfProcesses = MoP;
    SprawdzProcesy();
}

double PlanistaProcesora::ObliczCzas(std::shared_ptr<PCB> proces)
{
    double i = proces.get()->tn * alfa;
    double j = (1 - alfa) * proces.get()->Tn;
    return  i + j;
}

void PlanistaProcesora::ZmienProces(std::shared_ptr<PCB> proces)
{
    if (liczy)
    {
        Stop();
    }

    if (AKTUALNY != nullptr && AKTUALNY->GetState() != State::Dummy)
    {
        managerOfProcesses->changeStateProcess(AKTUALNY->GetPID(), State::Ready);
    }
    AKTUALNY = proces;
    managerOfProcesses->changeStateProcess(AKTUALNY->GetPID(), State::Running);
}

void PlanistaProcesora::SprawdzProcesy()
{
    if (managerOfProcesses->listOfReadyProcesses.empty())
    {
        AKTUALNY = managerOfProcesses->listOfProcesses[0];
        return;
    }

    double min = ObliczCzas(managerOfProcesses->listOfReadyProcesses[0]);
    int najlepszy = 0;
    for (int i = 1; i < managerOfProcesses->listOfReadyProcesses.size(); i++)
    {
        // if(managerOfProcesses->listOfProcesses[i]->)
        managerOfProcesses->listOfReadyProcesses[i].get()->Tn = ObliczCzas(managerOfProcesses->listOfReadyProcesses[i]);
        if (managerOfProcesses->listOfReadyProcesses[i]->Tn < min)
        {
            min = managerOfProcesses->listOfReadyProcesses[i].get()->Tn;
            najlepszy = i;
        }
    }

    if (managerOfProcesses->listOfReadyProcesses[najlepszy] != AKTUALNY)
        ZmienProces(managerOfProcesses->listOfReadyProcesses[najlepszy]);
}

void PlanistaProcesora::Start()
{
    startTime = clock();
    liczy = true;
}

void PlanistaProcesora::Stop()
{
    double czas = clock() - startTime;
    czas = czas / CLOCKS_PER_SEC;
    czas *= 1000;
    AKTUALNY->tn = czas;
    //std::cout << "Czas = " << czas << " s" << std::endl;
    liczy = false;

    SprawdzProcesy();
}

void PlanistaProcesora::PokazCzasy()
{
    for (int i = 0; i < managerOfProcesses->listOfReadyProcesses.size(); i++)
    {
        std::cout << managerOfProcesses->listOfReadyProcesses[i].get()->processName << " ID = " << managerOfProcesses->listOfReadyProcesses[i].get()->processID
            << " Tn = " << managerOfProcesses->listOfReadyProcesses[i].get()->Tn << " tn = " << managerOfProcesses->listOfReadyProcesses[i].get()->tn << std::endl;
    }
}