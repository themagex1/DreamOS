#pragma once

#include "interfejs.hpp"
#include "Includes.hpp"
#include "Interpreter.hpp"
#include "FileManager.hpp"
#include "ProcessManager.hpp"
#include "Pamiec.hpp"

Interfejs::Interfejs(std::shared_ptr<FileManager> dysk, std::shared_ptr<ManagerOfProcesses> processes_manager,
	std::shared_ptr<Interpreter> interpreter, std::shared_ptr<PlanistaProcesora> planista, std::shared_ptr<Pamiec> pamiec,
	std::shared_ptr<VirtualMemory> virtualMemory)
{
	this->dysk = dysk;
	this->processes_manager = processes_manager;
	this->interpreter = interpreter;
	this->planista = planista;
	this->pamiec = pamiec;
	this->virtualMemory = virtualMemory;
}

void Interfejs::command_serv()
{
	std::cout << "DreamOS" << std::endl;
	std::cout << "(c) 2020 DreamOS Corporation. Wszelkie prawa zastrzezone." << std::endl;
	while (czy_system_ma_dzialac)
	{
		std::cout << std::endl;
		std::string polecenie;
		int nr_polecenia;
		do
		{
			std::cout << "C:\\>";
			std::getline(std::cin, polecenie);
		} while (polecenie.empty());
		std::vector<std::string> tablica_pol;
		split_do_stringa(polecenie, tablica_pol);
		std::transform(tablica_pol[0].begin(), tablica_pol[0].end(), tablica_pol[0].begin(),
			[](unsigned char c) { return std::tolower(c); });
		bool czy_znaleziono = false;
		for (int i = 0; i < moje_polecenia.size(); i++)
		{
			if (tablica_pol[0] == moje_polecenia[i])
			{
				nr_polecenia = i;
				czy_znaleziono = true;
			}
		}
		if (czy_znaleziono)
		{
			switch_do_polecen(nr_polecenia, tablica_pol);
		}
		else
		{
			blad_polecenia();
		}
	}
}

void Interfejs::blad_polecenia()
{
	std::cout << "Nie rozpoznano polecenia. Wpisz \"help\" zeby wyswietlic mozliwe polecenia." << std::endl;
}

void Interfejs::split_do_stringa(const std::string& str, std::vector<std::string>& cont)
{
	char delim = ' ';
	std::size_t current, previous = 0;
	current = str.find(delim);
	while (current != std::string::npos) {
		std::string pomocniczy;
		pomocniczy = str.substr(previous, current - previous);
		if (pomocniczy.find_first_not_of(' ') != std::string::npos)
			cont.push_back(pomocniczy);
		previous = current + 1;
		current = str.find(delim, previous);
	}
	std::string pomocniczy2 = str.substr(previous, current - previous);
	if (pomocniczy2.find_first_not_of(' ') != std::string::npos)
		cont.push_back(pomocniczy2);
}

void Interfejs::switch_do_polecen(int& nr_polecenia, std::vector<std::string>& tablica_pol)
{
	switch (nr_polecenia)
	{
	case 0:
	{
		if (tablica_pol.size() == 1)
		{
			help_commands();
		}
		else if (tablica_pol.size() == 2)
		{
			std::transform(tablica_pol[1].begin(), tablica_pol[1].end(), tablica_pol[1].begin(),
				[](unsigned char c) { return std::tolower(c); });
			help_command_name(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 1:
	{
		if (tablica_pol.size() == 3)
		{
			//wykonywanie CP - Create Process
			create_process(tablica_pol[1], tablica_pol[2]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 2:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie TL - Tasklist
			tasklist();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 3:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie GO
			go(1);
		}
		else if (tablica_pol.size() == 2)
		{
			std::stringstream pomoc(tablica_pol[1]);
			int ile = 0;
			pomoc >> ile;
			if (ile == 0)
			{
				blad_polecenia();
			}
			else
			{
				go(ile);
			}
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 4:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie TK - Taskkill
			taskkill(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 5:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie DR - Dir
			dir();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 6:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie TP - Type
			type(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 7:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie CF - Create File
			create_file(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 8:
	{
		if (tablica_pol.size() == 3)
		{
			//wykonywanie RE - Rename File
			rename_file(tablica_pol[1], tablica_pol[2]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 9:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie ET - Edit File
			edit_file(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 10:
	{
		if (tablica_pol.size() == 3)
		{
			//wykonywanie CY - Copy
			copy(tablica_pol[1], tablica_pol[2]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 11:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie FS - File Size
			file_size(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 12:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie RF - Clear File
			clear_file(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 13:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie DL - Delete
			del(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 14:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWREGS
			show_regs();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 15:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie BV - Bitvector
			bv();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 16:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie COUNTCHAR
			count_char(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 17:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie SHOWBLOCK
			show_block(tablica_pol[1]);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 18:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWDISC
			std::cout << dysk->ShowDisc() << std::endl;
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 19:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie SIBOF
			std::string wynik_funkcji = dysk->ShowIndexBlockOfFile(tablica_pol[1]);
			if (wynik_funkcji != "-2")
				std::cout << wynik_funkcji << std::endl;
			else
				std::cout << "Nie ma pliku o takiej nazwie." << std::endl;
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 20:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWOPENFILES
			show_open_files();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 21:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWTIMES
			planista->PokazCzasy();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 22:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWRAM
			pamiec->pokaz_ram();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 23:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWEXSPACE
			virtualMemory->show_ExSpace();
		}
		/*else if (tablica_pol.size() == 2)
		{
			show_exspace(tablica_pol[1]);
		}*/
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 24:
	{
		if (tablica_pol.size() == 2)
		{
			//wykonywanie SHOWPAGETABLE
			std::shared_ptr<PCB>pcb = processes_manager->getProcessPointer(tablica_pol[1]);
			virtualMemory->show_PageTable(pcb->tablicaStron);
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 25:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie SHOWLRU
			virtualMemory->show_LRU();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 26:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie CLS - Cls
			system("cls");
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	case 27:
	{
		if (tablica_pol.size() == 1)
		{
			//wykonywanie EX - Exit
			exit();
		}
		else
		{
			blad_polecenia();
		}
		break;
	}
	default:
		blad_polecenia();
	}
}

void Interfejs::help_commands()
{
	std::cout << "Zeby uzyskac wiecej informacji wpisz HELP command-name" << std::endl;
	std::cout << "CP\tTworzenie procesu." << std::endl;
	std::cout << "TL\tWyswietlenie listy procesow." << std::endl;
	std::cout << "GO\tKontynuowanie wykonywania procesu." << std::endl;
	std::cout << "TK\tZabijanie procesu." << std::endl;
	std::cout << "DR\tWyswietlenie listy plikow w katalogu glownym oraz ilosc dostepnego miejsca." << std::endl;
	std::cout << "TP\tWyswietlenie zawartosci pliku." << std::endl;
	std::cout << "CF\tTworzenie nowego, pustego pliku." << std::endl;
	std::cout << "RE\tZmienianie nazwy pliku." << std::endl;
	std::cout << "ET\tEdytowanie pliku." << std::endl;
	std::cout << "CY\tKopiowanie plikow." << std::endl;
	std::cout << "FS\tWyswietlenie rozmiaru pliku." << std::endl;
	std::cout << "RF\tCzyszczenie pliku." << std::endl;
	std::cout << "DL\tUsuniecie pliku." << std::endl;
	std::cout << "SHOWREGS\tWyswietlenie zawartosci rejestrow." << std::endl;
	std::cout << "BITVECTOR\tWyswietlenie wektora bitowego." << std::endl;
	std::cout << "COUNTCHAR\tWyswietlenie ilosci znakow jakie sa w pliku." << std::endl;
	std::cout << "SHOWBLOCK\tWyswietlenie zawartosci konkretnego bloku dyskowego." << std::endl;
	std::cout << "SHOWDISC\tWyswietlenie zawartosci dysku." << std::endl;
	std::cout << "SIBOF\tWyswietlenie zawartosci bloku indeksowego pliku." << std::endl;
	std::cout << "SHOWOPENFILES\tWyswietlenie listy otwartych plikow." << std::endl;
	std::cout << "SHOWTIMES\tWyswietlenie czasow Tn i tn dla gotowych procesow w kolejce." << std::endl;
	std::cout << "SHOWRAM\tWyswietlenie zawartosci pamieci RAM." << std::endl;
	std::cout << "SHOWEXSPACE\tWyswietla zawartosc Przestrzeni wymiany lub tylko strony danego procesu." << std::endl;
	std::cout << "SHOWPAGETABLE\tWyswietla zawartosc danej tablicy stronic." << std::endl;
	std::cout << "SHOWLRU\tWyswietla zawartosc stosu LRU." << std::endl;
	std::cout << "CLS\tCzyszczenie ekranu." << std::endl;
	std::cout << "EX\tZamykanie systemu." << std::endl;
}

void Interfejs::help_command_name(std::string& command_name)
{
	bool czy_znaleziono = false;
	int nr_polecenia;
	for (int i = 0; i < moje_polecenia.size(); i++)
	{
		if (command_name == moje_polecenia[i])
		{
			nr_polecenia = i;
			czy_znaleziono = true;
		}
	}
	if (czy_znaleziono)
	{
		switch (nr_polecenia)
		{
		case 1:
		{
			std::cout << "Tworzenie procesu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "CP [nazwa_procesu] [nazwa_programu]" << std::endl;
			break;
		}
		case 2:
		{
			std::cout << "Wyswietlenie listy procesow." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "TL" << std::endl;
			break;
		}
		case 3:
		{
			std::cout << "Kontynuowanie wykonywania procesu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "GO" << std::endl;
			std::cout << "lub" << std::endl;
			std::cout << "GO [ile_rozkazow]" << std::endl;
			break;
		}
		case 4:
		{
			std::cout << "Zabijanie procesu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "TK [nazwa_procesu]" << std::endl;
			break;
		}
		case 5:
		{
			std::cout << "Wyswietlenie listy plikow w katalogu glownym oraz ilosc dostepnego miejsca." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "DR" << std::endl;
			break;
		}
		case 6:
		{
			std::cout << "Wyswietlenie zawartosci pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "TP [nazwa_pliku]" << std::endl;
			break;
		}
		case 7:
		{
			std::cout << "Tworzenie nowego, pustego pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "CF [nazwa_pliku]" << std::endl;
			break;
		}
		case 8:
		{
			std::cout << "Zmienianie nazwy pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "RE [stara_nazwa_pliku] [nowa_nazwa_pliku]" << std::endl;
			break;
		}
		case 9:
		{
			std::cout << "Edytowanie pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "ET [nazwa_pliku]" << std::endl;
			std::cout << "Po wcisniecu enter mozemy zaczac wpisywac tresc jaka chcemy dodac do pliku." << std::endl;
			break;
		}
		case 10:
		{
			std::cout << "Kopiowanie plikow." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "CY [nazwa_pliku_1] [nazwa_pliku_2]" << std::endl;
			break;
		}
		case 11:
		{
			std::cout << "Wyswietlenie rozmiaru pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "FS [nazwa_pliku]" << std::endl;
			break;
		}
		case 12:
		{
			std::cout << "Czyszczenie pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "RF [nazwa_pliku]" << std::endl;
			break;
		}
		case 13:
		{
			std::cout << "Usuniecie pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "DL [nazwa_pliku]" << std::endl;
			break;
		}
		case 14:
		{
			std::cout << "Wyswietlenie zawartosci rejestrow." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWREGS" << std::endl;
			break;
		}
		case 15:
		{
			std::cout << "Wyswietlenie wektora bitowego." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "BITVECTOR" << std::endl;
			break;
		}
		case 16:
		{
			std::cout << "Wyswietlenie ilosci znakow jakie sa w pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "COUNTCHAR [nazwa_pliku]" << std::endl;
			break;
		}
		case 17:
		{
			std::cout << "Wyswietlenie zawartosci bloku dyskowego." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWBLOCK [nr_bloku]" << std::endl;
			break;
		}
		case 18:
		{
			std::cout << "Wyswietlenie zawartosci dysku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWDISC" << std::endl;
			break;
		}
		case 19:
		{
			std::cout << "Wyswietlenie zawartosci bloku indeksowego pliku." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SIBOF [nazwa_pliku]" << std::endl;
			break;
		}
		case 20:
		{
			std::cout << "Wyswietlenie listy otwartych plikow." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWOPENFILES" << std::endl;
			break;
		}
		case 21:
		{
			std::cout << "Wyswietlenie czasow Tn i tn dla gotowych procesow w kolejce." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWTIMES" << std::endl;
			break;
		}
		case 22:
		{
			std::cout << "Wyswietlenie zawartosci pamieci RAM." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWRAM" << std::endl;
			break;
		}
		case 23:
		{
			std::cout << "Wyswietla zawartosc Przestrzeni wymiany lub tylko strony danego procesu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWEXSPACE" << std::endl;
			//std::cout << "lub" << std::endl;
			//std::cout << "SHOWEXSPACE [PID]" << std::endl;
			break;
		}
		case 24:
		{
			std::cout << "Wyswietla zawartosc danej tablicy stronic." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWPAGETABLE" << std::endl;
			break;
		}
		case 25:
		{
			std::cout << "Wyswietla zawartosc stosu LRU." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "SHOWLRU" << std::endl;
			break;
		}
		case 26:
		{
			std::cout << "Czyszczenie ekranu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "CLS" << std::endl;
			break;
		}
		case 27:
		{
			std::cout << "Zamykanie systemu." << std::endl;
			std::cout << "Skladnia:" << std::endl;
			std::cout << "EX" << std::endl;
			break;
		}
		default: blad_polecenia();
		}
	}
	else
	{
		blad_polecenia();
	}
}

void Interfejs::create_process(std::string& nazwa_procesu, std::string& nazwa_programu)
{
	if (processes_manager->getProcessPointer(nazwa_procesu) != nullptr)
	{
		std::cout << "Proces o podanej nazwie juz istnieje." << std::endl;
	}
	else
	{
		if (processes_manager->createProcess(nazwa_procesu, nazwa_programu))
		{
			std::cout << "Proces o nazwie " << nazwa_procesu << " zostal utworzony." << std::endl;
		}
		else
		{
			std::cout << "Proces o nazwie " << nazwa_procesu << " nie zostal utworzony." << std::endl;
		}
	}
}

void Interfejs::tasklist()
{
	processes_manager->displayAllProcesses();
}

void Interfejs::go(int ile)
{
	for (int i = 0; i < ile; i++)
	{
		if (planista->AKTUALNY != nullptr)
		{
			std::string nazwa_aktualnego_procesu = planista->AKTUALNY->processName;
			int wynik_funkcji = interpreter->WykonajLinie();
			if (wynik_funkcji == -1 || wynik_funkcji == -2)
			{
				taskkill(nazwa_aktualnego_procesu);
				break;
			}
		}
		else
		{
			std::cout << "Nie ma zadnego dostepnego procesu." << std::endl;
			break;
		}
	}
}

void Interfejs::taskkill(std::string& nazwa_procesu)
{
	std::shared_ptr<PCB> szukany = processes_manager->getProcessPointer(nazwa_procesu);
	if (szukany != nullptr)
	{
		processes_manager->deleteProcess(szukany->GetPID());
		std::cout << "Proces " << nazwa_procesu << " zostal usuniety." << std::endl;
	}
	else
	{
		std::cout << "Nie ma takiego procesu." << std::endl;
	}
}

void Interfejs::show_regs()
{
	interpreter->PokazRejestryProcesu();
}

void Interfejs::dir()
{
	std::vector<std::string> v = dysk->ShowCatalog();

	for (auto i : v)
	{
		std::cout << i << std::endl;
	}
	std::cout << "Miejsce wolne: " << dysk->PrintMemoryLeft() << " bajtow" << std::endl;
}

void Interfejs::type(std::string& nazwa_pliku)
{
	if (dysk->CheckNames(nazwa_pliku))
		std::cout << dysk->ReadFullFile(nazwa_pliku) << std::endl;
	else
		std::cout << "Nie ma takiego pliku." << std::endl;
}

void Interfejs::create_file(std::string& nazwa_pliku)
{
	int wynik = dysk->CreateFile(nazwa_pliku);
	if (wynik == -1)
		std::cout << "Brak miejsca na dysku." << std::endl;
	else if (wynik == 3)
		std::cout << "Juz istnieje plik o podanej nazwie." << std::endl;
	else if (wynik == 13)
		std::cout << "Nieprawidlowa nazwa pliku. Nazwa musi miec dwie litery." << std::endl;
	else
		std::cout << "Stworzono plik." << std::endl;
}

void Interfejs::rename_file(std::string& stara_nazwa_pliku, std::string& nowa_nazwa_pliku)
{
	int wynik_funkcji = dysk->RenameFile(stara_nazwa_pliku, nowa_nazwa_pliku);
	if (wynik_funkcji == 4)
		std::cout << "Plik nie zostal zamkniety." << std::endl;
	else if (wynik_funkcji == -2)
		std::cout << "Nie ma takiego pliku." << std::endl;
	else if (wynik_funkcji == 13)
		std::cout << "Nieprawidlowa nazwa pliku. Nazwa musi miec dwie litery." << std::endl;
	else
		std::cout << "Nazwa pliku zostala zmieniona." << std::endl;
}

void Interfejs::edit_file(std::string& nazwa_pliku)
{
	if (dysk->CheckNames(nazwa_pliku))
	{
		std::string dane_do_zapisu;
		getline(std::cin, dane_do_zapisu);
		int wynik1 = dysk->OpenFile(nazwa_pliku, 0);
		if (wynik1 == -2)
			std::cout << "Nie znaleziono pliku." << std::endl;
		else if (!dane_do_zapisu.empty())
		{
			int wynik2 = dysk->WriteToFile(nazwa_pliku, dane_do_zapisu);
			if (wynik2 == 10)
				std::cout << "Za malo pamieci." << std::endl;
			else if (wynik2 == 1)
				std::cout << "Plik jest zamkniety. Nie mozna do niego zapisac." << std::endl;
		}
		int wynik3 = dysk->CloseFile(nazwa_pliku);
		if (wynik3 == 1)
			std::cout << "Plik jest juz zamkniety. Nie mozna go zamknac ponownie." << std::endl;
	}
	else
	{
		std::cout << "Nie znaleziono pliku." << std::endl;
	}
}

void Interfejs::copy(std::string& nazwa_pliku_1, std::string& nazwa_pliku_2)
{
	if (!dysk->CheckNames(nazwa_pliku_1))
	{
		int wynik0 = -10;
		bool czy_nadpisac = false;
		if (!dysk->CheckNames(nazwa_pliku_2))
		{
			wynik0 = dysk->CreateFile(nazwa_pliku_2);
			if (wynik0 == -1)
				std::cout << "Brak miejsca na dysku." << std::endl;
			else if (wynik0 == 3)
				std::cout << "Juz istnieje plik o podanej nazwie." << std::endl;
			else if (wynik0 == 13)
				std::cout << "Nieprawidlowa nazwa pliku. Nazwa musi miec dwie litery." << std::endl;
		}
		else
		{
			std::cout << "Plik " << nazwa_pliku_2 << " juz istnieje na dysku. Czy go nadpisac?(Yes/No) ";
			std::string decyzja;
			getline(std::cin, decyzja);
			std::transform(decyzja.begin(), decyzja.end(), decyzja.begin(),
				[](unsigned char c) { return std::tolower(c); });
			decyzja.erase(remove_if(decyzja.begin(), decyzja.end(), isspace), decyzja.end());
			if (decyzja == "yes")
			{
				czy_nadpisac = true;
				int wynik_funkcji = dysk->ClearFile(nazwa_pliku_2);
			}
		}
		if ((wynik0 != -10 && wynik0 != -1 && wynik0 != 3 && wynik0 != 13 && czy_nadpisac == false) || czy_nadpisac == true)
		{
			std::string plik1 = dysk->ReadFullFile(nazwa_pliku_1);
			int wynik1 = dysk->OpenFile(nazwa_pliku_2, 0);
			if (wynik1 == -2)
				std::cout << "Nie znaleziono pliku." << std::endl;
			else
			{
				int wynik2 = dysk->WriteToFile(nazwa_pliku_2, plik1);
				if (wynik2 == 10)
					std::cout << "Za malo pamieci." << std::endl;
				else if (wynik2 == 1)
					std::cout << "Plik jest zamkniety. Nie mozna do niego zapisac." << std::endl;
				int wynik3 = dysk->CloseFile(nazwa_pliku_2);
				if (wynik3 == 1)
					std::cout << "Plik jest juz zamkniety. Nie mozna go zamknac ponownie." << std::endl;
			}
		}
	}
	else
		std::cout << "Plik, z ktorego chcemy kopiowac, nie istnieje." << std::endl;
}


void Interfejs::file_size(std::string& nazwa_pliku)
{
	int wynik_funkcji = dysk->FileSize(nazwa_pliku);
	if (wynik_funkcji == -2)
		std::cout << "Nie ma takiego pliku." << std::endl;
	else
		std::cout << "Rozmiar pliku " << nazwa_pliku << ": " << wynik_funkcji << std::endl;
}


void Interfejs::clear_file(std::string& nazwa_pliku)
{
	int wynik_funkcji = dysk->ClearFile(nazwa_pliku);
	if (wynik_funkcji == -2)
		std::cout << "Nie ma takiego pliku." << std::endl;
	else
		std::cout << "Plik zostal wyczyszczony." << std::endl;
}

void Interfejs::del(std::string& nazwa_pliku)
{
	int wynik_funkcji = dysk->DeleteFile(nazwa_pliku);
	if (wynik_funkcji == 4)
		std::cout << "Proba usuniecia otwartego pliku." << std::endl;
	else if (wynik_funkcji == -2)
		std::cout << "Nie ma takiego pliku." << std::endl;
	else
		std::cout << "Plik zostal usuniety." << std::endl;
}

void Interfejs::bv()
{
	dysk->PrintBitMap();
}

void Interfejs::count_char(std::string& nazwa_pliku)
{
	int liczba_znakow = dysk->CountCharacters(nazwa_pliku);
	if (liczba_znakow != -2)
		std::cout << "Plik " << nazwa_pliku << " ma: " << liczba_znakow << " znakow." << std::endl;
	else
		std::cout << "Nie ma takiego pliku." << std::endl;
}

void Interfejs::show_block(std::string& dane)
{
	std::stringstream pomoc(dane);
	int nr_bloku = -1;
	pomoc >> nr_bloku;
	if (nr_bloku == -1)
	{
		blad_polecenia();
	}
	else
	{
		std::string wynik_funkcji = dysk->ShowBlock(nr_bloku);
		if (wynik_funkcji != "9")
			std::cout << wynik_funkcji << std::endl;
		else
			std::cout << "Nie ma takiego bloku." << std::endl;
	}
}

void Interfejs::show_open_files()
{
	std::vector<std::string> pliki = dysk->ShowOpenFiles();
	for (auto e : pliki)
	{
		std::cout << e << std::endl;
	}
}

void Interfejs::show_exspace(std::string i)
{
	blad_polecenia();
	//std::stringstream pomoc(i);
	//int nr = -100;
	//pomoc >> nr;
	//if (nr == -100)
	//{
	//	blad_polecenia();
	//}
	//else
	//{
	//	//if(processes_manager->)
	//	//{
	//	//	std::cout << "ERROR" << std::endl;
	//	//}
	//	//else 
	//	virtualMemory->show_ExSpace(nr);
	//}
}

void Interfejs::exit()
{
	system("cls");
	std::cout << "Zamykanie systemu." << std::endl;
	Sleep(1000);
	czy_system_ma_dzialac = false;
}