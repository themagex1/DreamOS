#include "FileManager.hpp"

std::array<char, int(DiskSize)> world::disk;
std::array<int, DiskSize / BlockSize> world::BitMap;
std::vector<File> world::MainCatalog;
std::vector<int> world::OpenFiles;

FileManager::FileManager(std::shared_ptr<Pamiec> pamiec, std::shared_ptr<Event> event)
{
	world::disk.fill('@');
	world::BitMap.fill(0);
	this->pamiec = pamiec;
	this->event = event;
}

int FileManager::RenameFile(std::string name, std::string newName)
{
	if (CheckIfOpen(name))
	{
		return NotClosed_error;
	}
	else
	{
		for (int i = 0; i < world::MainCatalog.size(); i++)
		{
			if (world::MainCatalog[i].Name == name)
			{
				world::MainCatalog[i].Name = newName;
				return 0;
			}

		}
		return NoFile_error;
	}
}

int FileManager::ClearFile(std::string name)
{
	for (int i = 0; i < world::MainCatalog.size(); i++)
	{
		if (world::MainCatalog[i].Name == name)
		{
			if (CheckIfOpen(name))
				CloseFile(name);

			DeleteFile(name);

			CreateFile(name);
			return 0;
		}
	}
	return NoFile_error;
}

int FileManager::CountCharacters(std::string name)
{
	for (auto i : world::MainCatalog)
	{
		if (i.Name == name)
		{
			return i.Write_pointer;
		}
	}
	return NoFile_error;
}

std::string FileManager::ShowBlock(int i)
{
	std::string wynik;
	if (i >= DiskSize / BlockSize)
	{
		return Noblock_error;
	}
	for (int j = i * BlockSize; j < i * BlockSize + BlockSize; j++)
	{
		wynik.push_back(world::disk[j]);
	}
	return wynik;
}

std::string FileManager::ShowDisc()
{
	std::string wynik;
	for (int i = 0; i < world::disk.size(); i++)
	{
		wynik.push_back(world::disk[i]);
	}
	return wynik;
}

std::string FileManager::ShowIndexBlockOfFile(std::string name)
{
	std::string wynik;
	for (auto i : world::MainCatalog)
	{
		if (i.Name == name)
		{
			wynik = ShowBlock(i.Index_Block_Number);
			return wynik;
		}
	}
	return NoFile_error2;

}

std::vector<std::string> FileManager::ShowOpenFiles()
{
	std::vector<std::string> names;
	for (int i = 0; i < world::OpenFiles.size(); i++)
	{
		names.push_back(world::MainCatalog[world::OpenFiles[i]].Name);
	}
	return names;
}

std::vector<std::string> FileManager::ShowCatalog()		//zwraca wszystkie nazwy plikow w glownym katalogu jako wektor stringow
{
	std::vector<std::string> Nazwy_Plikow;
	for (auto a : world::MainCatalog)
	{
		Nazwy_Plikow.push_back(a.Name);
	}
	return Nazwy_Plikow;
}

int FileManager::FindSpace()
{
	for (int i = 0; i < world::BitMap.size(); i++)
	{
		if (world::BitMap[i] == 0)
		{
			world::BitMap[i] = 1;
			return i;
		}

	}
	return NoSpace_error;
}

bool FileManager::CheckNames(std::string name)
{
	for (auto x : world::MainCatalog)
	{
		if (x.Name == name) return 1;
	}
	return 0;
}

bool FileManager::CheckIfOpen(std::string name)
{
	for (auto x : world::OpenFiles)
	{
		if (world::MainCatalog[x].Name == name) return 1;
	}
	return 0;
}

void FileManager::PrintBitMap()
{
	for (auto x : world::BitMap)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
}

int FileManager::PrintMemoryLeft()
{
	int wolne = 0;
	for (auto x : world::BitMap)
	{
		if (x == 0)
		{
			wolne++;
		}
	}
	//	std::cout << "ilosc wolnego miejsca" << wolne * BlockSize << std::endl; // do usuniecia
	return wolne * BlockSize;
}

int FileManager::CreateFile(std::string name)
{
	int check = FileNameSize;
	if (check != name.length())
	{
		return WrongFileName_error;
	}
	if (CheckNames(name) == 1)			//czy jest juz taki plik
	{
		//std::cout << "nazwa  pliku zajeta" << std::endl; //do usuniecia
		return NameTaken_error;		//jesli tak to error
	}
	else
	{
		int Blok_Indeksowy = FindSpace();
		if (Blok_Indeksowy != -1)
		{
			//	std::cout << "Stworzono nowy plik" << std::endl; //do usuniecia
			File New(name, event);
			New.Index_Block_Number = Blok_Indeksowy;
			for (int z = Blok_Indeksowy * BlockSize; z < Blok_Indeksowy * BlockSize + BlockSize; z += 2)
			{
				world::disk[z] = '-';
				world::disk[z + 1] = '1';
			}
			world::MainCatalog.push_back(New);
			return 0;
		}
		else return NoSpace_error;
	}
}

int FileManager::FileSize(std::string name)
{
	for (auto a : world::MainCatalog)
	{
		if (a.Name == name)
		{
			return (std::ceil((double)a.Write_pointer / BlockSize) + 1) * BlockSize;
		}
	}
	return NoFile_error;
}

int FileManager::DeleteFile(std::string name)
{
	std::string temp = "-1";
	int wynik;
	for (int i = 0; i < world::MainCatalog.size(); i++)
	{
		if (world::MainCatalog[i].Name == name)
		{
			if (CheckIfOpen(name))
			{
				return NotClosed_error;
			}
			else
			{
				int a = world::MainCatalog[i].Index_Block_Number * BlockSize;	//poczatek bloku indeksowego na dysku
				for (int x = 0; x < world::OpenFiles.size(); x++)
				{
					if (world::OpenFiles[x] > i)
					{
						world::OpenFiles[x]--;
					}
				}
				for (int j = 0; j < BlockSize; j += 2)
				{
					if (world::disk[a + j] == '-')		//sprawdzanie czy napotkano pusty blok bo jesli tak to mozna juz przestac
					{
						break;
					}
					else
					{
						temp[0] = world::disk[a + j];					//ustawianie kolejnych blokow zeby mozna je bylo ustawic na wolne w bitmapie
						temp[1] = world::disk[a + j + 1];
					}
					wynik = std::stoi(temp);
					world::BitMap[wynik] = 0;			//ustawianie bloku na wolny
				}
			}
			world::BitMap[world::MainCatalog[i].Index_Block_Number] = 0;
			world::MainCatalog.erase(world::MainCatalog.begin() + i);
			return 0;
		}


	}
	return NoFile_error;

}

int FileManager::OpenFile(std::string name, int pointer)
{
	for (int i = 0; i < world::MainCatalog.size(); i++)
	{
		if (world::MainCatalog[i].Name == name)
		{
			if (CheckIfOpen(name))
			{
				//	std::cout << "blad plik jest juz otwarty" << std::endl; //do usuniecia
				return AlreadyOpened_error;
			}
			else
			{
				//	std::cout << "otworzono plik" << std::endl; //do usuniecia
				world::MainCatalog[i].zamek.lock();
				world::MainCatalog[i].Read_pointer = pointer;
				world::OpenFiles.push_back(i);
				return 0;
			}
		}
	}
	//	std::cout << "nie ma takiej nazwy" << std::endl; //do usuniecia
	return NoFile_error;
}

int FileManager::CloseFile(std::string name)
{
	for (int i = 0; i < world::OpenFiles.size(); i++)
	{
		if (world::MainCatalog[world::OpenFiles[i]].Name == name)
		{
			//	std::cout << "zamknieto plik" << std::endl; //do usuniecia
			world::OpenFiles.erase(world::OpenFiles.begin() + i);
			return 0;
		}

	}
	return Closed_error; //std::cout << "error juz jest zamkniety plik" << std::endl; //do usuniecia
}

int FileManager::WriteToFile(std::string name, std::string data)
{
	int blok;
	char temp;

	for (auto i : world::OpenFiles)
	{
		if (world::MainCatalog[i].Name == name)			//szukamy z otwartych plikow 
		{
			for (int j = 0; j < data.size(); j++)		//zapisywanie do pliku
			{
				if (world::MainCatalog[i].Write_pointer < BlockSize * BlockSize / 2)
				{
					blok = SearchIndexBlock(i);		//funkcja przeszukujaca blok indeksowy po indeksy blokow / jesli wszystkie zajete to tworzy nowe
					if (blok == -1) break;
					temp = data[j];
					world::disk[blok * BlockSize + world::MainCatalog[i].Write_pointer % BlockSize] = temp;
					world::MainCatalog[i].Write_pointer++;
				}
				else return MaxFileSizeLimit_error;
			}
			return 0;
		}
	}
	return Closed_error;
}

int FileManager::SearchIndexBlock(int i)
{
	int ktoryblok;
	int miejsce;
	std::string wynik = "-1";
	char temp;

	ktoryblok = world::MainCatalog[i].Write_pointer / BlockSize;		//sprawdzamy w ktorym bloku znajduej sie aktualnie write pointer (tzn nie jego wartosc tylko to tak powiedzialem xd)

	int a = world::MainCatalog[i].Index_Block_Number * BlockSize;	//poczatek bloku indeksowego na dysku
	a += ktoryblok * 2;		//ustawiamy a, na miejsce gdzie znajduje sie indeks bloku w kt�rym chcemy cos zapisywac (indeksy blokow sa 2 znakami, dlatego razy 2)
	wynik[0] = world::disk[a];
	wynik[1] = world::disk[a + 1];
	miejsce = stoi(wynik);			//konwertujemy odczytany indeks bloku na inta
	if (miejsce == -1)			//jesli -1 to znaczy ze nie ma jescze przypisanego bloku
	{
		miejsce = FindSpace();		//w takim wypadku znajdujemy miejsce na ten blok

		if (miejsce == -1)
			return NoSpace_error;		//jesli nie ma miejsca to error

		if (miejsce < 10)			//sprawdzanie czy ideks bloku jest jedno czy dwu cyfrowy  jesli jedno to zamieniamy go na 0X, gddzie X = miejsce, wiec np z 1 zrobi sie 01.
		{
			wynik[0] = '0';
			wynik[1] = miejsce + '0';
			world::disk[a] = wynik[0];		//wrzucamy ten blok zamiast -1 do bloku indeksowego
			world::disk[a + 1] = wynik[1];
		}
		else                               //jesli blok jest dwucyfrowy to wrzucamy go poprostu do stringa i potem na dysk
		{
			wynik = std::to_string(miejsce);
			world::disk[a] = wynik[0];			//wrzucamy ten blok zamiast -1 do bloku indeksowego
			world::disk[a + 1] = wynik[1];
		}
		return stoi(wynik);	//zwraca indeks bloku ktorego bedziemy uzywac
	}
	else return miejsce;
}

int FileManager::ReadFile(std::string name, int ammount, int adres, std::shared_ptr<PCB> pcb)
{
	int ktoryblok;
	std::string temp = "-1";
	int wynik;
	int a;
	for (auto i : world::OpenFiles)
	{
		if (world::MainCatalog[i].Name == name)			//szukamy z otwartych plikow 
		{
			if (world::MainCatalog[i].Write_pointer >= ammount)
			{
				for (int j = 0; j < ammount; j++)
				{
					a = world::MainCatalog[i].Index_Block_Number * BlockSize;	//poczatek bloku indeksowego na dysku
					ktoryblok = world::MainCatalog[i].Read_pointer / BlockSize;	//sprawdzamy z ktorego bloku nalezy teraz skorzystac
					temp[0] = world::disk[a + ktoryblok * 2];
					temp[1] = world::disk[a + ktoryblok * 2 + 1];		//pobieranie tego bloku z bloku indeksowego *2, bo bloki maja dwa znaki
					wynik = std::stoi(temp);		//konwersja na int'a
					wynik = wynik * BlockSize;		//ustawianie zmiennej wynik, na miejsce na dysku z ktorego zacznie sie pobieranie danych
					std::string temp1(1, world::disk[wynik + world::MainCatalog[i].Read_pointer % BlockSize]);
					pamiec->zapisz_do_ramu(adres++, temp1, pcb);
					//std::cout << world::disk[wynik + world::MainCatalog[i].Read_pointer % BlockSize];	//wynik to blok z ktorego pobieramy a readpointer to offset
					world::MainCatalog[i].Read_pointer++;
				}
			}
			else return OutofRange_error;
			return 0;
		}
	}
	return NoFile_error;				//do zrobienia sprawdzanie z wielkoscia pliku czy nie wychodzi za zakres i zeby max bylo 8 blokow w bloku indeksowym
}

std::string FileManager::ReadFullFile(std::string name)
{
	int a;
	int counter = 0;
	std::string temp = "-1";
	int wynik;
	std::string WholeFile;
	for (int i = 0; i < world::MainCatalog.size(); i++)
	{
		if (world::MainCatalog[i].Name == name)
		{
			a = world::MainCatalog[i].Index_Block_Number * BlockSize;
			for (int j = 0; j <= world::MainCatalog[i].Write_pointer / BlockSize * 2; j += 2)
			{
				temp[0] = world::disk[a + j];
				temp[1] = world::disk[a + 1 + j];
				wynik = stoi(temp);
				wynik *= BlockSize;
				for (int z = 0; z < BlockSize; z++)
				{
					if (counter < world::MainCatalog[i].Write_pointer)
					{
						WholeFile.push_back(world::disk[wynik + z]);
						counter++;
					}
				}
			}
		}
	}
	return WholeFile;
}