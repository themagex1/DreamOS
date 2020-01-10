//made by DR
#include "ProcessManager.hpp"

ManagerOfProcesses::ManagerOfProcesses(std::shared_ptr<VirtualMemory> VM, std::shared_ptr<Event> e, std::shared_ptr<Pamiec> pamiec)
{
	this->VM = VM;
	this->e = e;
	this->pamiec = pamiec;
	createInactivityProcess();
}

ManagerOfProcesses::~ManagerOfProcesses() = default;

int ManagerOfProcesses::gettingID()
{
	int id = 0;
	for (auto i : listOfProcesses)
	{
		if (i->processID == id)
		{
			id++;
			break;
		}
	}
	return id;
}

bool ManagerOfProcesses::createProcess(std::string processName, std::string filename)
{
	auto Proces = std::make_shared<PCB>(std::move(gettingID()), processName, State::Ready);

	int rozmiar = VM->load_program(filename, Proces->GetPID());

	if (rozmiar == -1)
	{
		return false;
	}

	if (listOfProcesses[0]->GetState() == State::Dummy)
	{
		listOfProcesses.clear();
	}

	listOfProcesses.push_back(Proces);
	listOfReadyProcesses.push_back(Proces);
	Proces->tablicaStron = VM->create_PageTable(rozmiar, Proces->GetPID());

	e->Sygnal();

	return true;
}

void ManagerOfProcesses::createInactivityProcess()
{
	auto Proces = std::make_shared<PCB>(-1, "PROCES DUMMY", State::Dummy);
	listOfProcesses.push_back(Proces);

	int rozmiar = VM->load_program("p0.txt", Proces->GetPID());

	Proces->tablicaStron = VM->create_PageTable(rozmiar, Proces->GetPID());

	e->Sygnal();
}

void ManagerOfProcesses::deleteProcess(int processID)
{
	//usuwanie z listy procesow
	std::vector<std::shared_ptr<PCB>>::iterator it = std::find_if(std::begin(listOfProcesses), std::end(listOfProcesses), [&processID](auto& Proces) {return Proces->processID == processID; });

	pamiec->zwolnij_pamiec(*it); //zwolnienie pamieci po usunietym procesie

	if (*it != nullptr)
	{
		listOfProcesses.erase(it);
	}
	//usuwanie z listy gotowych
	it = std::find_if(std::begin(listOfReadyProcesses), std::end(listOfReadyProcesses), [&processID](auto& Proces) {return Proces->processID == processID; });
	if (*it != nullptr)
	{
		listOfReadyProcesses.erase(it);
	}
	//usuwanie z oczekujacych
	if (!listOfWaitingProcesses.empty())
	{
		it = std::find_if(std::begin(listOfWaitingProcesses), std::end(listOfWaitingProcesses), [&processID](auto& Proces) {return Proces->processID == processID; });
		if (*it != nullptr)
			listOfWaitingProcesses.erase(it);
	}

	if (listOfProcesses.empty())
	{
		createInactivityProcess();
	}

	e->Sygnal();
}

void ManagerOfProcesses::addProcess(std::shared_ptr<PCB> process)
{
	listOfProcesses.push_back(process);
}

void ManagerOfProcesses::addToQueue(std::shared_ptr<PCB> process)
{
	listOfReadyProcesses.push_back(process);
	e->Sygnal();
}

void ManagerOfProcesses::addToWaiting(std::shared_ptr<PCB> process)
{
	listOfWaitingProcesses.push_back(process);
}

void ManagerOfProcesses::changeStateProcess(int processID, State state)
{
	for (int i = 0; i < listOfProcesses.size(); i++)
	{
		if (listOfProcesses[i]->processID == processID)
		{
			listOfProcesses[i]->ChangeState(state);
			return;
		}
	}
}

std::shared_ptr<PCB> ManagerOfProcesses::getProcessPointer(std::string processName)
{
	for (int i = 0; i < listOfProcesses.size(); i++)
	{
		if (listOfProcesses[i]->processName == processName)
		{
			return listOfProcesses.at(i);
		}
	}

	return nullptr;
}

void ManagerOfProcesses::displayQueue()
{
	std::cout << "######################################################" << std::endl;
	std::cout << "LISTA GOTOWYCH PROCESOW: " << std::endl;
	if (!listOfReadyProcesses.empty())
	{
		for (auto i : listOfReadyProcesses)
		{
			std::cout << i->DisplayProcess() << std::endl;
		}
	}
	else std::cout << "LISTA JEST PUSTA!" << std::endl;
	std::cout << "######################################################" << std::endl;
}

void ManagerOfProcesses::displayAllProcesses()
{
	std::cout << "######################################################" << std::endl;
	std::cout << "LISTA WSZYSTKICH PROCESOW: " << std::endl;
	if (!listOfProcesses.empty())
	{
		for (auto i : listOfProcesses)
		{
			std::cout << i->DisplayProcess() << std::endl;
		}
	}
	else std::cout << "LISTA JEST PUSTA!" << std::endl;
	std::cout << "######################################################" << std::endl;
}

void ManagerOfProcesses::displayWaitingProcesses()
{
	std::cout << "######################################################" << std::endl;
	std::cout << "LISTA PROCESOW OCZEKUJACYCH: " << std::endl;
	if (!listOfWaitingProcesses.empty())
	{
		for (auto i : listOfWaitingProcesses)
		{
			std::cout << i->DisplayProcess() << std::endl;
		}
	}
	else std::cout << "LISTA JEST PUSTA!" << std::endl;
	std::cout << "######################################################" << std::endl;
}