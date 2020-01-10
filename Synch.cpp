#include "Synch.hpp"

Synch::~Synch() = default;

Synch::Synch()
{

}

Synch::Synch(std::shared_ptr <Event> event)
{
	this->event = event;
}

bool Synch::test_and_set()
{
	bool val = this->lockValue;
	this->lockValue = true;
	return val;
}

void Synch::lock()
{
	//while (this->test_and_set()) { planistaProcesora->addToWaiting(std::shared_ptr<PCB> process); }
	if (this->test_and_set()) { event->SygnalWaiting(); }
	else {
		this->lockValue = true;
		this->pid = event->SygnalPID();
	}
}

void Synch::unlock()
{
	this->lockValue = false;
	this->pid = -1;
}