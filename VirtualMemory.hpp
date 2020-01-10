//Made by Gregory Matczak

#pragma once

#include "PCB.hpp"
#include "Includes.hpp"
#include "PageData.hpp"

//Tablica stron - znajdowac sie b?dzie w PCB w postaci std::vector<PageData>
//Frame to numer ramki w ramie, isFree to validation bit.

class VirtualMemory
{

private:

	/*========================================================/Struktura Strony/========================================================*/

	//struktura pojedynczej strony w przestrzeni wymiany
	struct Page {
		char data[8]{ ' ' };
		Page();
		Page(std::string temp);
		void print();
	};

	/*========================================================/Przestrzen Wymiany/========================================================*/



	/*========================================================/ALGORYTM WYMIANY/========================================================*/

	//Stos algorytmu wymiany (Least Recently Used)
	//Z kazdym uzyciem ramki,ulozenie stosu bedzie sie zmieniac
	std::list<int> LRU{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };



	/*========================================================//========================================================*/




	//Metoda zwalniajaca miejsce w RAM-ie dla nowej strony oraz odkladajaca stara do przestrzeni wymiany( jest wywolywana wewnatrz metody load_to_ram)
	int Exchange_page(std::shared_ptr<std::array<char, 128>> RAM, int pageID, int PID, std::shared_ptr<std::vector<PageData>> PageTable);

public:
	//Przestrzen wymiany stron <PID,stronice danego procesu>
	std::map<int, std::vector<Page>>ExchangeSpace;

	struct FrameData
	{
		int PID;
		std::shared_ptr<std::vector<PageData>> tablicaStron;
		int pageID;

		FrameData()
		{
			PID = -1;
			tablicaStron = nullptr;
			pageID = -1;
		}
	};

	std::array<FrameData, 16> frameTable = { FrameData() };

	//Tablica wolnych ramek

	/*========================================================/Konstruktor i Dekonstruktor/========================================================*/
	//Konstruktor
	VirtualMemory();

	//Destruktor
	~VirtualMemory();

	//Wyswietla zawartosc Przestrzeni wymiany
	void show_ExSpace();

	//Wyswietla tylko strony danego procesu
	void show_ExSpace(int PID);

	//Wyswietla zawartosc danej tablicy stronic
	void show_PageTable(std::shared_ptr<std::vector<PageData>> PageTable);

	//Wyswietla zawartosc stosu LRU
	void show_LRU();

	//Aktualizuje stos LRU po uzyciu ramki
	void LRU_update(int frameID);

	//Metoda ladujaca program do przestrzeni wymiany, gdzie:
	// &path- ?ciezka do pliku zawierajacego program
	// PID- numer procesu
	int load_program(const std::string path, int PID);

	//Tworzy tablice stronic 
	std::shared_ptr<std::vector<PageData>> create_PageTable(int bytes, int PID);

	//Szuka wolnej ramki w pami?ci fizycznej i zwraca jej indeks(w momencie nie znalezienia takiej ramki, zwraca -1)
	int FindFreeFrame(std::shared_ptr<std::array<char, 128>> RAM);

	//Metoda ladujaca strone do pamieci fizycznej gdzie:
	// *RAM- wskaznik do p.fizycznej
	// page -strona do zaladowania
	// pageID - numer strony
	// PID - numer procesu, do ktorego nalezy strona
	// *pageTable - wkaznik do tablicy stron tego procesu
	int load_to_ram(std::shared_ptr<std::array<char, 128>> RAM, int pageID, int PID, std::shared_ptr<std::vector<PageData>> PageTable);

	//Usuwa strony danego procesu z Przestrzeni wymiany
	void erase(int PID);

	//Zwieksza rozmiar tablicy stronic
	void make_bigger(int size, std::shared_ptr<PCB> pcb);

	//Dopisuje bajt do Przestrzeni wymiany
	void write_to_virtual(std::shared_ptr<PCB> proc, std::string data, int address);
};