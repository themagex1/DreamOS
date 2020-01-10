#pragma once

#include "Includes.hpp"
#include "PageData.hpp"
#include "PCB.hpp"
#include "VirtualMemory.hpp"

const unsigned int WIELKOSC_STRONY_WYKLADNIK = 3;
const unsigned int WIELKOSC_STRONY = (1 << WIELKOSC_STRONY_WYKLADNIK);
const unsigned int MASKA_OFFSET = WIELKOSC_STRONY - 1;
const unsigned int WIELKOSC_RAMKI = WIELKOSC_STRONY;

class Pamiec
{
private:
	std::shared_ptr<VirtualMemory> virtualMemory;
	void sprawdz_adres(int& adres, std::shared_ptr<std::vector<PageData>> pageTable);
	std::array<char, 128> ram;

public:
	std::shared_ptr<std::array<char, 128>> pamiec_ram;

	Pamiec(std::shared_ptr<VirtualMemory> virtualMemory);
	unsigned int zakoduj_adres(unsigned int index_strony, unsigned int offset);
	void dekoduj_adres(int& adres, unsigned int* index_strony, unsigned int* offset);
	void zapisz_do_ramu(int adres, std::string wartosc, std::shared_ptr<PCB> pcb);
	unsigned char czytaj_bajt(int adres, std::shared_ptr<PCB> pcb);
	void zwolnij_pamiec(std::shared_ptr<PCB> pcb);
	void pokaz_ram();
};