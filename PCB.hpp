#pragma once

#include "PageData.hpp"
#include "Includes.hpp"

enum class State
{
	Dummy,
	New,
	Ready,
	Waiting,
	Running,
	Terminated //useless
};

class PCB
{
public:
	int processID;
	std::string processName;
	State processState;

	//           |||| BLOK PAMIECI ||||
	std::shared_ptr<std::vector<PageData>> tablicaStron;
	//           |||| BLOK PROCESORA ||||
	double tn;
	double Tn;
	//           |||| INTERPRETER ||||
	std::array<int, 4> registers;
	unsigned int licznikRozkazow;

	//            ||| KONSTRUKTORY |||
	PCB();
	PCB(int, std::string, State); //id, filename, stan
	~PCB();

	// ======GETTERY, SETTERY=======
	int GetPID();
	std::string GetNameProcess();
	State GetState();

	void ChangeState(State processState); //zmiana stanu procesu
	std::string DisplayProcess();
};