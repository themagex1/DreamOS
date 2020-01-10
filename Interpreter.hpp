#pragma once

#include "Includes.hpp"
#include "Pamiec.hpp"
#include "FileManager.hpp"
#include "PlanistaProcesora.hpp"
#include "ProcessManager.hpp"

class Interpreter
{
private:
	unsigned int AX = 0, BX = 0, CX = 0, DX = 0;
	unsigned int licznikRozkazow = 0, iloscRozkazow = 0, licznik = 0, adres = -1, linijka = 0;
	std::string dane1, dane2;
	std::shared_ptr<Pamiec> pamiec;
	std::shared_ptr<FileManager> fileManager;
	std::shared_ptr<PlanistaProcesora> planistaProcesora;
	std::shared_ptr<ManagerOfProcesses> managerOfProcesses;

	void PobierzAktualneRejestry();
	void AktualizujRejestryProcesu();
	void AktualizujRejestry(bool* rejestr, int dane);
	void KtoryRejestr(bool* rejestr, int numer);
	void PokazRejestry();
	int WykonajInstrukcje(const std::string& rozkaz, const std::string& nazwaProcesu);
	std::array<std::string, 4> RozdzielRozkaz(const std::string& rozkaz);
	unsigned int PodajAdres(const std::string& adres);
	std::string PodajNazwe(const std::string& nazwa);
	std::string PodajRozkaz(bool symulacja);

public:
	void PokazRejestryProcesu();
	void Symuluj(const std::string& nazwaProcesu);
	int WykonajLinie();
	std::string PokazNastepnyRozkaz();
	Interpreter(std::shared_ptr<Pamiec> pamiec, std::shared_ptr<FileManager> fileManager,
		std::shared_ptr<ManagerOfProcesses> managerOfProcesses, std::shared_ptr<PlanistaProcesora> planistaProcesa);
	~Interpreter();
};