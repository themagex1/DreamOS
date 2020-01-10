#include "Includes.hpp"
#include "interfejs.hpp"
#include "Interpreter.hpp"
#include "Pamiec.hpp"
#include "PCB.hpp"
#include "PlanistaProcesora.hpp"
#include "ProcessManager.hpp"
#include "Synch.hpp"
#include "VirtualMemory.hpp"
#include "FileManager.hpp"
#include "Event.hpp"

std::shared_ptr<Event> e = std::make_shared<Event>();
std::shared_ptr<VirtualMemory> virtualMemory = std::make_shared<VirtualMemory>();
std::shared_ptr<Pamiec> pamiec = std::make_shared<Pamiec>(virtualMemory);
std::shared_ptr<ManagerOfProcesses> managerOfProcesses = std::make_shared<ManagerOfProcesses>(virtualMemory, e, pamiec);
std::shared_ptr<PlanistaProcesora> planistaProcesora = std::make_shared<PlanistaProcesora>(managerOfProcesses);
std::shared_ptr<FileManager> fileManager = std::make_shared<FileManager>(pamiec, e);
std::shared_ptr<Interpreter> interpreter = std::make_shared<Interpreter>(pamiec, fileManager, managerOfProcesses, planistaProcesora);
std::shared_ptr<Interfejs> interfejs = std::make_shared <Interfejs>(fileManager, managerOfProcesses, interpreter, planistaProcesora, pamiec, virtualMemory);

int main()
{
	e->DodajPlaniste(planistaProcesora);
	interfejs->command_serv();

	return 0;
}