#pragma once

#include "Includes.hpp"
#include "Synch.hpp"
#include "Pamiec.hpp"
#include "Event.hpp"

#define BlockSize 16		//wielkosc jednego bloku w bajtach
#define DiskSize 256		//wielkosc dysku w bajtach

#define Closed_error 1;        //plik jest zamkniety a ktos probuje cos na nim robic
#define NoSpace_error -1;    // brak miejsca na dysku 
#define NameTaken_error 3;    // nazwa pliku jest juz w uzyciu
#define NotClosed_error 4;    //proba usuniecia otwartego pliku
#define InUse_error 5;        //plik jest uzywany juz przez kogos
#define AlreadyOpened_error 6;
#define NoFile_error -2;        //nie ma pliku o takiej nazwie
#define OutofRange_error 8;
#define MaxFileSizeLimit_error 10;
#define Noblock_error "9"; // nie ma takiego bloku
#define NoFile_error2 "-2";
#define FileNameSize 2;
#define WrongFileName_error 13;

struct File
{
	std::string Name;
	int Index_Block_Number;
	int Size = 0;
	int Read_pointer;
	Synch zamek;
	int Write_pointer;

	File()
	{
		this->Read_pointer = 0;
		this->Write_pointer = 0;
	}

	File(std::string name, std::shared_ptr < Event> event)
	{
		this->Name = name;
		this->Read_pointer = 0;
		this->Write_pointer = 0;
		zamek = Synch(event);
	}
};

struct world
{
	static std::array<char, int(DiskSize)> disk; //tablica reprezentujaca dysk
	static std::vector<int> OpenFiles;		//tablica otwartych plikow
	static std::array<int, DiskSize / BlockSize> BitMap;		//wektor bitow pokazujacy ktore bloki s� wolne
	static std::vector<File> MainCatalog;				//glowny katalog
};

class FileManager
{
private:
	std::shared_ptr<Pamiec> pamiec;
	//std::shared_ptr<PlanistaProcesora> planistaProcesora;
	std::shared_ptr<Event> event;

public:

	FileManager(std::shared_ptr<Pamiec> pamiec, std::shared_ptr<Event> event);

	int CreateFile(std::string name);	 //tworzenie pliku

	int DeleteFile(std::string name);	 //usuwanie pliku

	//int OpenFile(std::string name);		//otwieranie pliku
	int OpenFile(std::string name, int pointer);

	int CloseFile(std::string name);	//zamykanie pliku

	int WriteToFile(std::string name, std::string data);	//dopisywanie do konca pliku

	int SearchIndexBlock(int i);

	int ReadFile(std::string name, int ammount, int adres, std::shared_ptr<PCB> pcb);	//czytanie z pliku danej ilosci bajtow/ zapisuje wynik do miejsca w pamieci wskazywanego przez adres.

	int RenameFile(std::string name, std::string newName);		//zmiana nazwy pliku na inn�

	//int ClearFile(std::string name);			//usuwanie zawartosci pliku bez usuwania pliku
	std::vector<std::string> ShowCatalog();		// zwraca wektor z nazwami plikow

	int FindSpace();					//szukanie wolnych blokow

	bool CheckNames(std::string name);		//sprawdzanko czy nie ma juz innego pliku o tej samej nazwie

	bool CheckIfOpen(std::string name);		//sprawdzanie czy plik jest otwarty

	void PrintBitMap();				//wypisywanie wektora odpowiedzialnego za wolne bloki
	int PrintMemoryLeft();			//wypisywanie wolnego miejsca na dysku

	int FileSize(std::string name);

	int ClearFile(std::string name);

	int CountCharacters(std::string name);

	std::string ShowBlock(int i);

	std::string ShowDisc();

	std::string ShowIndexBlockOfFile(std::string name);

	std::string ReadFullFile(std::string name); //przekazanie calego pliku do inferfejs

	std::vector<std::string> ShowOpenFiles();
};