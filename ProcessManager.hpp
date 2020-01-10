#pragma once

#include "Includes.hpp"
#include "PCB.hpp"
#include "VirtualMemory.hpp"
#include "Event.hpp"
#include "Pamiec.hpp"

class Event;

class ManagerOfProcesses
{
private:
	std::shared_ptr<VirtualMemory> VM;
	std::shared_ptr<Event> e;
	std::shared_ptr<Pamiec> pamiec;

public:
	ManagerOfProcesses(std::shared_ptr<VirtualMemory> VM, std::shared_ptr<Event> e, std::shared_ptr<Pamiec> pamiec);
	~ManagerOfProcesses();

	std::vector<std::shared_ptr<PCB>> listOfReadyProcesses;
	std::vector<std::shared_ptr<PCB>> listOfProcesses;
	std::vector<std::shared_ptr<PCB>> listOfWaitingProcesses;
	int gettingID();
	bool createProcess(std::string processName, std::string filename);
	void createInactivityProcess();
	void deleteProcess(int processID);
	void addProcess(std::shared_ptr<PCB> Proces); //dodawanie procesu do listy procesow
	void addToQueue(std::shared_ptr<PCB> Proces); //dodawanie procesu do gotowych
	void addToWaiting(std::shared_ptr<PCB> Proces); //dodawanie do oczekujacychg
	void changeStateProcess(int processID, State state); //zmiana stanu procesu o danym PID
	std::shared_ptr<PCB> getProcessPointer(std::string processName); //zwrot referencji na proces o danej nazwie
	void displayQueue(); //wyswietlanie kolejki procesow gotowych
	void displayAllProcesses(); //wyswietlanie wszystkich procesow
	void displayWaitingProcesses();  //wyswietlanie oczekujacych
};