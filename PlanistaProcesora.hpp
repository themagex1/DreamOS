#pragma once

#include "PCB.hpp"
#include "ProcessManager.hpp"
#include "Includes.hpp"

class ManagerOfProcesses;

class PlanistaProcesora
{
private:
    double alfa = 0.5;
    clock_t startTime;
    bool liczy = false;

    double ObliczCzas(std::shared_ptr<PCB> proces);
    void ZmienProces(std::shared_ptr<PCB> proces);

public:
    PlanistaProcesora(std::shared_ptr<ManagerOfProcesses> MoP);
    std::shared_ptr<PCB> AKTUALNY;
    std::shared_ptr<ManagerOfProcesses> managerOfProcesses;

    void SprawdzProcesy();
    void Start();
    void Stop();

    void PokazCzasy();
};