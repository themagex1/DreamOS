#pragma once

#include "Interpreter.hpp"

Interpreter::Interpreter(std::shared_ptr<Pamiec> pamiec, std::shared_ptr<FileManager> fileManager,
	std::shared_ptr<ManagerOfProcesses> managerOfProcesses, std::shared_ptr<PlanistaProcesora> planistaProcesora)
{
	this->pamiec = pamiec;
	this->fileManager = fileManager;
	this->managerOfProcesses = managerOfProcesses;
	this->planistaProcesora = planistaProcesora;
}

Interpreter::~Interpreter() = default;

std::string Interpreter::PokazNastepnyRozkaz()
{
	return PodajRozkaz(true);
}

void Interpreter::PokazRejestry()
{
	std::cout << "||  AX: " << AX;
	std::cout << "  BX: " << BX;
	std::cout << "  CX: " << CX;
	std::cout << "  DX: " << DX << "  ||" << std::endl;
}

void Interpreter::PokazRejestryProcesu()
{
	std::cout << "\tZawartosc rejestrow procesu: " << std::endl;
	std::cout << "AX: " << planistaProcesora->AKTUALNY->registers[0] << std::endl;
	std::cout << "BX: " << planistaProcesora->AKTUALNY->registers[1] << std::endl;
	std::cout << "CX: " << planistaProcesora->AKTUALNY->registers[2] << std::endl;
	std::cout << "DX: " << planistaProcesora->AKTUALNY->registers[3] << std::endl;
	std::cout << "Wartosc licznika rozkazow: " << planistaProcesora->AKTUALNY->licznikRozkazow << std::endl << std::endl;
}

void Interpreter::PobierzAktualneRejestry()
{
	AX = planistaProcesora->AKTUALNY->registers[0];
	BX = planistaProcesora->AKTUALNY->registers[1];
	CX = planistaProcesora->AKTUALNY->registers[2];
	DX = planistaProcesora->AKTUALNY->registers[3];
	licznikRozkazow = planistaProcesora->AKTUALNY->licznikRozkazow;
}

void Interpreter::AktualizujRejestryProcesu()
{
	planistaProcesora->AKTUALNY->registers[0] = AX;
	planistaProcesora->AKTUALNY->registers[1] = BX;
	planistaProcesora->AKTUALNY->registers[2] = CX;
	planistaProcesora->AKTUALNY->registers[3] = DX;
	planistaProcesora->AKTUALNY->licznikRozkazow = this->licznikRozkazow;
}

void Interpreter::AktualizujRejestry(bool* rejestr, int dane)
{
	if (rejestr[0]) { AX = dane; }
	else if (rejestr[1]) { BX = dane; }
	else if (rejestr[2]) { CX = dane; }
	else if (rejestr[3]) { DX = dane; }
}

void Interpreter::KtoryRejestr(bool* rejestr, int numer)
{
	for (int i = 0; i < 4; i++)
	{
		if (i == numer)
		{
			rejestr[i] = true;
		}
		else
		{
			rejestr[i] = false;
		}
	}
}

std::array<std::string, 4> Interpreter::RozdzielRozkaz(const std::string & rozkaz)
{
	int poczatek = 0, koniec = 0, licznik = 0;
	std::array<std::string, 4> rozkazWynik;

	while ((koniec = rozkaz.find(' ', poczatek)) != std::string::npos)
	{
		rozkazWynik[licznik] = rozkaz.substr(poczatek, koniec - poczatek);
		poczatek = koniec + 1;
		licznik++;
	}
	rozkazWynik[licznik] = rozkaz.substr(poczatek);
	return rozkazWynik;
}

unsigned int Interpreter::PodajAdres(const std::string & adres)
{
	std::string pom;
	for (const char& c : adres)
	{
		if (c != '[' && c != ']') { pom += c; }
	}
	return stoi(pom);
}

std::string Interpreter::PodajNazwe(const std::string & nazwa)
{
	std::string pom;
	for (const char& c : nazwa)
	{
		if (c != '"') { pom += c; }
	}
	return pom;
}

int Interpreter::WykonajInstrukcje(const std::string & instrukcja, const std::string & nazwaProcesu)
{
	std::array<std::string, 4> rozkaz = RozdzielRozkaz(instrukcja);
	const std::string instrukcjaMem = rozkaz[0];
	int roz1 = 0, roz2 = 0, iloscBajtow = 0;
	bool rejestr[4] = { false };

	if (!rozkaz[1].empty())
	{
		if (rozkaz[1] == "AX")
		{
			roz1 = AX;
			KtoryRejestr(rejestr, 0);
		}
		else if (rozkaz[1] == "BX")
		{
			roz1 = BX;
			KtoryRejestr(rejestr, 1);
		}
		else if (rozkaz[1] == "CX")
		{
			roz1 = CX;
			KtoryRejestr(rejestr, 2);
		}
		else if (rozkaz[1] == "DX")
		{
			roz1 = DX;
			KtoryRejestr(rejestr, 3);
		}

		else if (rozkaz[1][0] == '[')
		{
			adres = PodajAdres(rozkaz[1]);
			if (instrukcjaMem != "SAV")
			{
				roz1 = pamiec->czytaj_bajt(adres, planistaProcesora->AKTUALNY);
			}
		}
		else if (rozkaz[1][0] == '"')
		{
			dane1 = PodajNazwe(rozkaz[1]);
		}
		else
		{
			roz1 = std::stoi(rozkaz[1]);
		}
	}

	if (!rozkaz[2].empty())
	{
		if (rozkaz[2] == "AX")
		{
			roz2 = AX;
		}
		else if (rozkaz[2] == "BX")
		{
			roz2 = BX;
		}
		else if (rozkaz[2] == "CX")
		{
			roz2 = CX;
		}
		else if (rozkaz[2] == "DX")
		{
			roz2 = DX;
		}
		else if (rozkaz[2][0] == '[')
		{
			adres = PodajAdres(rozkaz[2]);
			if (instrukcjaMem != "SAV")
			{
				roz2 = pamiec->czytaj_bajt(adres, planistaProcesora->AKTUALNY) - 48;
			}
		}
		else if (rozkaz[2][0] == '"')
		{
			dane2 = PodajNazwe(rozkaz[2]);
		}
		else
		{
			roz2 = std::stoi(rozkaz[2]);
		}
	}

	if (!rozkaz[3].empty())
	{
		iloscBajtow = std::stoi(rozkaz[3]);
	}

	//Rozkazy dotyczace operacji arytmetycznych
	if (instrukcjaMem == "ADD")
	{
		roz1 += roz2;
	}
	else if (instrukcjaMem == "SUB")
	{
		roz1 -= roz2;
	}
	else if (instrukcjaMem == "MUL")
	{
		roz1 *= roz2;
	}
	else if (instrukcjaMem == "DIV")
	{
		if (roz2 != 0)
		{
			roz1 /= roz2;
		}
		else
		{
			std::cout << "Nie mozna dzielic przez zero!" << std::endl;
			std::cout << "Proces " << nazwaProcesu << " zostaje zabity" << std::endl;
			return -1;
		}
	}
	else if (instrukcjaMem == "MOD")
	{
		if (roz2 != 0)
		{
			roz1 %= roz2;
		}
		else
		{
			std::cout << "Nie mozna dzielic przez zero!" << std::endl;
			std::cout << "Proces " << nazwaProcesu << "Zostaje zabity" << std::endl;
			return -1;
		}
	}
	else if (instrukcjaMem == "DEC")
	{
		roz1--;
	}
	else if (instrukcjaMem == "INC")
	{
		roz1++;
	}
	else if (instrukcjaMem == "SAV")
	{
		std::string temp = std::to_string(roz1);
		temp += ' ';

		pamiec->zapisz_do_ramu(adres, temp, planistaProcesora->AKTUALNY);
	}
	else if (instrukcjaMem == "MOV")
	{
		roz1 = roz2;
	}
	else if (instrukcjaMem == "JMP")
	{
		licznikRozkazow = adres;
		iloscRozkazow = 0;
	}
	else if (instrukcjaMem == "JNZ")
	{
		if (roz1 != 0)
		{
			licznikRozkazow = adres;
			iloscRozkazow = 0;
		}
	}
	else if (instrukcjaMem == "JIZ")
	{
		if (roz1 == 0)
		{
			licznikRozkazow = adres;
		}
	}
	else if (instrukcjaMem == "NOP")
	{
		return 0;
	}
	else if (instrukcjaMem == "HLT")
	{
		return -2;
	}

	//Rozkazy dotyczace procesow
	else if (instrukcjaMem == "CP")
	{
		managerOfProcesses->createProcess(dane1, dane2);
	}
	else if (instrukcjaMem == "KP")
	{
		managerOfProcesses->deleteProcess(managerOfProcesses->getProcessPointer(dane1)->GetPID());
	}

	//Rozkazy dotyczace plikow
	else if (instrukcjaMem == "CRF")
	{
		return fileManager->CreateFile(dane1);
	}
	else if (instrukcjaMem == "OPF")
	{
		return fileManager->OpenFile(dane1, roz2);
	}
	else if (instrukcjaMem == "CF")
	{
		return fileManager->CloseFile(dane1);
	}
	else if (instrukcjaMem == "WF")
	{
		if (!dane2.empty())
		{
			return fileManager->WriteToFile(dane1, std::to_string(roz2) + ' ');
		}
		else
		{
			char temp;
			while ((temp = pamiec->czytaj_bajt(adres++, planistaProcesora->AKTUALNY)) != ' ')
			{
				dane2 += temp;
			}
			dane2 += ' ';

			return fileManager->WriteToFile(dane1, dane2);
		}
	}
	else if (instrukcjaMem == "RF")
	{
		int czytajWynik = fileManager->ReadFile(dane1, iloscBajtow, adres, planistaProcesora->AKTUALNY);

		if (czytajWynik == 0)
		{
			char temp;

			dane2 = "";

			while ((temp = pamiec->czytaj_bajt(adres++, planistaProcesora->AKTUALNY)) != ' ')
			{
				dane2 += temp;
			}

			roz2 = std::stoi(dane2);
		}

		return czytajWynik;
	}

	else
	{
		std::cout << "Nierozpoznana instrukcja (" << instrukcjaMem << ")" << std::endl;
		return -1;
	}

	AktualizujRejestry(rejestr, roz1);

	return 0;
}

std::string Interpreter::PodajRozkaz(bool symulacja)
{
	unsigned int tempLicznik = licznikRozkazow, it = 2;
	std::string instrukcja = "";
	char temp;

	for (int i = 0; i < it - 1; i++)
	{
		while ((temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY)) != ' ')
		{
			instrukcja += temp;

			if (instrukcja == "HLT" || instrukcja == "NOP")
			{
				return instrukcja;
			}
		}

		if (instrukcja == "MOV") it = 3;
		else if (instrukcja == "ADD") it = 3;
		else if (instrukcja == "SUB") it = 3;
		else if (instrukcja == "DIV") it = 3;
		else if (instrukcja == "MUL") it = 3;
		else if (instrukcja == "MOD") it = 3;
		else if (instrukcja == "DEC") it = 2;
		else if (instrukcja == "INC") it = 2;
		else if (instrukcja == "SAV") it = 3;
		else if (instrukcja == "JMP") it = 2;
		else if (instrukcja == "JNZ") it = 3;
		else if (instrukcja == "JIZ") it = 3;
		else if (instrukcja == "NOP") it = 1;
		else if (instrukcja == "HLT") it = 0;
		else if (instrukcja == "CP") it = 3;
		else if (instrukcja == "KP") it = 2;
		else if (instrukcja == "CRF") it = 2;
		else if (instrukcja == "OPF") it = 3;
		else if (instrukcja == "CF") it = 2;
		else if (instrukcja == "WF") it = 3;
		else if (instrukcja == "RF") it = 4;

		instrukcja += ' ';
	}

	temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY);
	instrukcja += temp;

	if (temp == '[')
	{
		while ((temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY)) != ']')
		{
			instrukcja += temp;
		}
		instrukcja += temp;
	}
	else if (temp == '"')
	{
		while ((temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY)) != '"')
		{
			instrukcja += temp;
		}
		instrukcja += temp;
	}
	else if (temp >= 48 && temp <= 57)
	{
		while (true)
		{
			temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY);
			if (temp >= 48 && temp <= 57)
			{
				instrukcja += temp;
			}
			else
			{
				licznikRozkazow--;
				break;
			}
		}
	}
	else
	{
		temp = pamiec->czytaj_bajt(licznikRozkazow++, planistaProcesora->AKTUALNY);
		instrukcja += temp;
	}

	if (symulacja)
	{
		licznikRozkazow = tempLicznik;
	}

	return instrukcja;
}

int Interpreter::WykonajLinie()
{
	PobierzAktualneRejestry();

	int wynik = 0;
	std::string instrukcja = PodajRozkaz(false);

	std::cout << std::endl << "Rozkaz: " << instrukcja << std::endl;
	std::cout << "PID procesu: " << planistaProcesora->AKTUALNY->GetPID() << std::endl;
	std::cout << "Nazwa procesu: " << planistaProcesora->AKTUALNY->GetNameProcess() << std::endl;
	std::cout << std::endl << "\tStan przed wykonaniem instrukcji: " << std::endl << std::endl;
	std::cout << "Licznik rozkazow:\t" << planistaProcesora->AKTUALNY->licznikRozkazow << std::endl;
	std::cout << "Stan rejestrow: \t";

	PokazRejestry();

	planistaProcesora->Start();

	if ((wynik = WykonajInstrukcje(instrukcja, planistaProcesora->AKTUALNY->GetNameProcess())) != -1)
	{
		std::cout << std::endl << "\tStan po wykonaniu instrukcji: " << std::endl << std::endl;

		std::cout << "Licznik rozkazow:\t" << licznikRozkazow << std::endl;
		std::cout << "Stan rejestrow: ";

		PokazRejestry();
	}

	AktualizujRejestryProcesu();

	planistaProcesora->Stop();

	return wynik;
}

void Interpreter::Symuluj(const std::string & nazwaProcesu)
{
	int tempAX = AX, tempBX = BX, tempCX = CX, tempDX = DX, tempLicznik = licznikRozkazow;
	std::string instrukcja = "";

	std::cout << "Po wykonaniu programu, wartosc rejestrow i licznika rozkazow nie ulegnie zmianie" << std::endl;

	AX = BX = CX = DX = licznikRozkazow = 0;

	while (true)
	{
		if ((instrukcja = PodajRozkaz(true)) != "HLT")
		{
			if (WykonajLinie() != 0)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	std::cout << "Wykonywanie instrukcji programu zostalo zakonczone" << std::endl;

	AX = tempAX;
	BX = tempBX;
	CX = tempCX;
	DX = tempDX;
	licznikRozkazow = tempLicznik;

	AktualizujRejestryProcesu();
}