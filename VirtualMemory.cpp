//Made by Gregory Matczak 

#include "VirtualMemory.hpp"
#include "PCB.hpp"
#include "Includes.hpp"

/*==================================/Struktura Strony/==================================*/
VirtualMemory::Page::Page(std::string temp)//Konstruktor strony z zawartoscia
{
	while (temp.length() != 8) temp += ' ';//Wypelnienie reszty strony spacjami
	for (int i = 0; i < temp.length(); i++) { this->data[i] = temp[i]; }
}

VirtualMemory::Page::Page() //Konstruktor pustej strony 
{
	for (int i = 0; i < 8; i++) { this->data[i] = ' '; }
}

void VirtualMemory::Page::print()//Funkcja wyswietlajaca strone
{
	for (auto x : data)
	{
		if (x == ' ') std::cout << '#';
		else std::cout << x;
	}
}
/*==================================/Struktura tabeli stronic/==================================*/
PageData::PageData()
{
	this->Frame = -1;
	this->isFree = true;
}

PageData::PageData(int Frame, bool isFree) :Frame(Frame), isFree(isFree) {}

//Funkcja tworz?ca Tablic? stronic
std::shared_ptr<std::vector<PageData>> VirtualMemory::create_PageTable(int bytes, int PID)
{
	const double pages = ceil(static_cast<double>(bytes) / 8);
	std::shared_ptr<std::vector<PageData>> pagetable = std::make_shared<std::vector<PageData>>();

	for (int i = 0; i < pages; i++)
	{
		pagetable->push_back(PageData(-1, true));
	}

	return pagetable;
}

//Funkcja zwiekszajaca tablice stronic
void VirtualMemory::make_bigger(int bytes, std::shared_ptr<PCB> pcb)
{
	const double pages = ceil(static_cast<double>(bytes) / 8);

	for (int i = 0; i < pages; i++)
	{
		pcb->tablicaStron->push_back(PageData(-1, true));
		ExchangeSpace[pcb->GetPID()].push_back(Page());
	}
}

//Funkcja wyswietlajaca zawartosc tablicy stronic
void VirtualMemory::show_PageTable(std::shared_ptr<std::vector<PageData>> PageTable)
{
	int i = 0;
	std::cout << "|--| TablicaStronic |--|" << std::endl;
	std::cout << "| Strona | Ramka | Bit |" << std::endl;
	for (auto x : *PageTable)
		std::cout << "|   " << i++ << "    |   " << x.Frame << "   |  " << x.isFree << "  |" << std::endl;
}
/*==================================/Konstruktor i destruktor calego Virtual Memory/==================================*/
VirtualMemory::VirtualMemory() = default;
VirtualMemory::~VirtualMemory() = default;

/*==================================/Znajdowanie wolnej ramki/==================================*/
int VirtualMemory::FindFreeFrame(std::shared_ptr<std::array<char, 128>> RAM)
{
	for (int i = 0; i <= 120; i += 8)
	{
		if (RAM->at(i) == '~')
		{
			return i;
		}
	}
	return -1;
}
/*========================================/Funkcje LRU/========================================*/

//Aktualizacja stosu
void VirtualMemory::LRU_update(int frameID)
{
	if (frameID > 15 || frameID < 0) return;
	std::list<int>::iterator it = std::find(LRU.begin(), LRU.end(), frameID);
	LRU.erase(it);
	LRU.emplace_front(frameID);
}

//Wyswietlanie stosu
void VirtualMemory::show_LRU()
{
	std::cout << "Zawartosc stosu algorytmu wymiany (LRU):" << std::endl << std::endl;
	for (auto x : LRU) std::cout << x << " ";
	std::cout << std::endl;
}

/*=================================/Funkcje Ladowania do pamieci fizycznej/=================================*/
int VirtualMemory::Exchange_page(std::shared_ptr<std::array<char, 128>> RAM, int pageID, int PID, std::shared_ptr<std::vector<PageData>> PageTable)
{
	int frame = LRU.back();
	LRU_update(frame);

	for (int i = frame * 8; i < frame * 8 + 8; i++)
	{
		ExchangeSpace[PID][pageID].data[i - frame * 8] = RAM->at(i);
	}

	if (frameTable[frame].PID != PID)
	{
		PageTable = frameTable[frame].tablicaStron;
	}

	for (int i = 0; i < PageTable->size(); i++)
	{
		if (PageTable->at(i).Frame == frame)
		{
			pageID = i;
			break;
		}
	}

	PageTable->at(pageID).Frame = -1;
	PageTable->at(pageID).isFree = true;

	return frame;
}

int VirtualMemory::load_to_ram(std::shared_ptr<std::array<char, 128>> RAM, int pageID, int PID, std::shared_ptr<std::vector<PageData>> PageTable)
{
	Page page = ExchangeSpace[PID][pageID];
	int frame = FindFreeFrame(RAM);
	if (frame == -1) { frame = Exchange_page(RAM, pageID, PID, PageTable); }
	else frame = frame / 8;
	for (int i = frame * 8; i < frame * 8 + 8; i++) RAM->at(i) = page.data[i - frame * 8];

	frameTable[frame].pageID = pageID;
	frameTable[frame].PID = PID;
	frameTable[frame].tablicaStron = PageTable;

	PageTable->at(pageID).Frame = frame;
	PageTable->at(pageID).isFree = false;

	LRU_update(frame);

	return frame;
}
/*=================================/Funkcje przestrzeni wymiany/=================================*/

//Funkcja ladowania programu do przestrzeni wymiany
int VirtualMemory::load_program(const std::string path, int PID)
{
	std::ifstream file(path);
	std::string temp;
	std::string program;
	std::vector<Page> tempVector;

	if (!file.is_open())
	{
		std::cout << "Blad! : Nie mozna otworzyc pliku." << std::endl;
		return -1;
	}

	while (std::getline(file, temp))//Zapis zawartosci pliku do zmiennej
	{
		if (!temp.empty())
		{
			program += temp;
		}
	}

	int pages_num = program.length() / 8;
	temp.clear();

	for (auto x : program)//Przepisanie zawartosci zmiennej na strony
	{
		temp += x;
		if (temp.size() == 8)
		{
			tempVector.push_back(Page(temp));
			temp.clear();
		}
	}

	if (!temp.empty())  tempVector.push_back(Page(temp));
	temp.clear();

	ExchangeSpace.insert(std::make_pair(PID, tempVector));// Przekazanie wektora ze stronami do przestrzeni

	return program.length();
}

//Dopisuje fragment do Przestrzeni wymiany
void VirtualMemory::write_to_virtual(std::shared_ptr<PCB> proc, std::string data, int address)
{
	int page = floor((address) / 8);
	for (int i = 0; i < data.length(); i++)
	{
		ExchangeSpace[proc->GetPID()][page].data[(address + i) % 8] = data[i];
	}
}

//Funkcja usuwajaca strony zawierajace program wybranego procesu z przestrzeni wymiany
void VirtualMemory::erase(int PID)
{
	ExchangeSpace.erase(PID);
}

//Funkcja wyswietlajaca zawartosc przestrzeni wymiany
void VirtualMemory::show_ExSpace()
{
	std::cout << "/===============/Przestrzen wymiany/===============/" << std::endl << std::endl;
	for (auto x : ExchangeSpace)
	{
		std::cout << "/---------/ID Procesu: " << x.first << "/---------/" << std::endl;
		for (int i = 0; i < x.second.size(); i++)
		{
			std::cout << "Strona " << i << ".: ";
			x.second[i].print();
			std::cout << std::endl;
		}
	}
}

void VirtualMemory::show_ExSpace(int PID)
{
	int i = 0;
	std::map<int, std::vector<VirtualMemory::Page>>::iterator it = ExchangeSpace.find(PID);

	std::cout << "/---------/ID Procesu: " << PID << "/---------/" << std::endl;

	for (auto x : it->second)
	{
		std::cout << "Strona " << i++ << ".: ";
		x.print();
		std::cout << std::endl;
	}
}