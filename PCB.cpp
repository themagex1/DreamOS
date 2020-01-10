//made by DR
#include "PCB.hpp"
#include "Includes.hpp"

PCB::PCB()
{
	this->tablicaStron = std::make_shared<std::vector<PageData>>();
	this->processID = -1;
	this->processName = "BLAD";
	this->processState = State::Dummy;

	this->registers[0] = 0;
	this->registers[1] = 0;
	this->registers[2] = 0;
	this->registers[3] = 0;
	this->licznikRozkazow = 0;

	this->Tn = 1.0;
	this->tn = 1.0;
}

PCB::PCB(int ID, std::string nazwaProcesu, State stanProcesu)
{
	this->processID = ID;
	this->processName = nazwaProcesu;
	this->processState = stanProcesu;

	this->registers[0] = 0;
	this->registers[1] = 0;
	this->registers[2] = 0;
	this->registers[3] = 0;
	this->licznikRozkazow = 0;

	this->Tn = 1.0;
	this->tn = 1.0;
}

PCB::~PCB() = default;

//=====GETTERY=======
int PCB::GetPID()
{
	return processID;
}

std::string PCB::GetNameProcess()
{
	return processName;
}

State PCB::GetState()
{
	return processState;
}

//=========METODY=========
void PCB::ChangeState(State state)
{
	this->processState = state;
}

std::string PCB::DisplayProcess()
{
	std::string chosenProces = "Nazwa Procesu: " + processName + " ID: " + std::to_string(processID) + " Status: ";

	if (processState == State::New) chosenProces += "NEW";
	else if (processState == State::Ready) chosenProces += "READY";
	else if (processState == State::Waiting) chosenProces += "WAITING";
	else if (processState == State::Running) chosenProces += "RUNNING";
	else if (processState == State::Terminated) chosenProces += "TERMINATED";
	else if (processState == State::Dummy) chosenProces += "DUMMY";

	return chosenProces;
}