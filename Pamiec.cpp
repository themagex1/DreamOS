//Made by Gregory Matczak 

#include "Pamiec.hpp"
#include "PCB.hpp"
#include "Includes.hpp"

//funkcja koduj�ca adres
unsigned int Pamiec::zakoduj_adres(unsigned int index_strony, unsigned int offset)
{
	return (index_strony << WIELKOSC_STRONY_WYKLADNIK) + offset;
}

//funkcja dekoduj�ca adres
void Pamiec::dekoduj_adres(int& adres, unsigned int* index_strony, unsigned int* offset)
{
	*offset = adres & MASKA_OFFSET;
	*index_strony = (adres >> WIELKOSC_STRONY_WYKLADNIK);
}

//konstruktor klasy pami��
Pamiec::Pamiec(std::shared_ptr<VirtualMemory> virtualMemory)
{
	for (int i = 0; i < 128; i++) ram[i] = 126; //~
	pamiec_ram = std::make_shared<std::array<char, 128>>(ram);
	this->virtualMemory = virtualMemory;
}

//funkcja zapisuj�ca pojedynczy bajt w pami�ci pod podanym adresem, w przypadku blednego adresu wyrzucany jest wyjatek
void Pamiec::zapisz_do_ramu(int adres, std::string wartosc, std::shared_ptr<PCB> pcb)
{
	//sprawdz_adres(adres, pcb->tablicaStron);
	unsigned int index, offset;
	int space = adres - pcb->tablicaStron->size() * 8 + wartosc.length() + 1;
	int pages = ceil((double)wartosc.length() / 8);
	dekoduj_adres(adres, &index, &offset);
	if (space <= 0)
	{
		if (!pcb->tablicaStron->at(index).isFree)
		{
			virtualMemory->write_to_virtual(pcb, wartosc, adres);
			for (int i = 0; i < 8; i++)
			{
				pamiec_ram->at(pcb->tablicaStron->at(index).Frame * 8 + i) = virtualMemory->ExchangeSpace[pcb->GetPID()][index].data[i];
			}
			return;
		}
	}
	else if (space > 0)
	{
		virtualMemory->make_bigger(space, pcb);
	}

	virtualMemory->write_to_virtual(pcb, wartosc, adres);

	for (int i = pages; i >= 0; i--)
	{
		virtualMemory->load_to_ram(pamiec_ram, pcb->tablicaStron->size() - (i + 1), pcb->GetPID(), pcb->tablicaStron);
	}
}

//funkcja odczytujaca bajt z poda danego adresu, w przypadku blednego adresu wyrzucany jest wyjatek
unsigned char Pamiec::czytaj_bajt(int adres, std::shared_ptr<PCB> pcb)
{
	sprawdz_adres(adres, pcb->tablicaStron);
	unsigned int index, offset;
	index = adres / 8;
	offset = adres % 8;
	int numer_ramki;

	if (!pcb->tablicaStron->at(index).isFree)
	{
		numer_ramki = pcb->tablicaStron->at(index).Frame;
		virtualMemory->LRU_update(numer_ramki);
	}
	else
	{
		numer_ramki = virtualMemory->load_to_ram(pamiec_ram, index, pcb->GetPID(), pcb->tablicaStron);
	}

	unsigned int pierwszy_bajt_ramki = numer_ramki * WIELKOSC_RAMKI;

	return pamiec_ram->at(pierwszy_bajt_ramki + offset);
}

//funkcja zwalniaj�ca podany blok pami�ci, adres musi wskazywa� na pocz�tek strony(offset=0), w przypadku blednego adresu wyrzucany jest wyjatek
void Pamiec::zwolnij_pamiec(std::shared_ptr<PCB> pcb)
{
	int frame;

	for (int i = 0; i < pcb->tablicaStron->size(); i++)
	{
		if (!pcb->tablicaStron->at(i).isFree)
		{
			frame = pcb->tablicaStron->at(i).Frame;
			for (int j = 0; j < 8; j++)
			{
				pamiec_ram->at((frame * 8) + j) = '~';
			}
		}
	}
	virtualMemory->erase(pcb->GetPID());
}

void Pamiec::pokaz_ram()
{
	std::cout << "|------------------|Pamiec RAM|------------------|\n" << std::endl;
	for (int i = 1; i <= 128; i++)
	{
		if (i % 8 == 0)std::cout << "\n";
		if (pamiec_ram->at(i - 1) == '~') std::cout << i - 1 << ":[ ] ";
		else std::cout << i - 1 << ":[" << pamiec_ram->at(i - 1) << "] ";
	}
	std::cout << std::endl << "|------------------------------------------------|\n";
}

//metoda sprawdzaj�ca poprawno�� adresu(na u�ytek wewn�trzny)
void Pamiec::sprawdz_adres(int& adres, std::shared_ptr<std::vector<PageData>> pageTable)
{
	unsigned int index_strony;
	unsigned int offset;

	dekoduj_adres(adres, &index_strony, &offset);

	if (index_strony >= pageTable->size())
	{
		throw std::invalid_argument("nieprawidlowy numer strony");
	}
	if (offset > WIELKOSC_STRONY)
	{
		throw std::invalid_argument("offset wykracza poza strone");
	}
}