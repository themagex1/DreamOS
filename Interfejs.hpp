#pragma once

#include "FileManager.hpp"
#include "ProcessManager.hpp"
#include "Interpreter.hpp"
#include "PlanistaProcesora.hpp"
#include "Pamiec.hpp"
#include "Includes.hpp"

class Interfejs
{
private:
	bool czy_system_ma_dzialac = true;
	std::array<std::string, 28> moje_polecenia = { "help","cp","tl","go","tk","dr","tp","cf","re","et","cy","fs","rf",
		"dl","showregs","bitvector","countchar","showblock","showdisc","sibof","showopenfiles","showtimes","showram","showexspace","showpagetable","showlru","cls","ex" };
	std::shared_ptr<FileManager> dysk;
	std::shared_ptr<ManagerOfProcesses> processes_manager;
	std::shared_ptr<Interpreter> interpreter;
	std::shared_ptr<PlanistaProcesora> planista;
	std::shared_ptr<Pamiec> pamiec;
	std::shared_ptr<VirtualMemory> virtualMemory;

	void blad_polecenia();
	void split_do_stringa(const std::string& str, std::vector<std::string>& cont);
	void switch_do_polecen(int& nr_polecenia, std::vector<std::string>& tablica_pol);
	void help_commands();
	void help_command_name(std::string& command_name);
	std::vector<std::string> typing();

	void create_process(std::string& nazwa_procesu, std::string& nazwa_programu);
	void tasklist();
	void go(int ile);
	void taskkill(std::string& nazwa_procesu);
	void show_regs();
	void dir();
	void type(std::string& nazwa_pliku);
	void create_file(std::string& nazwa_pliku);
	void rename_file(std::string& stara_nazwa_pliku, std::string& nowa_nazwa_pliku);
	void edit_file(std::string& nazwa_pliku);
	void copy(std::string& nazwa_pliku_1, std::string& nazwa_pliku_2);
	void file_size(std::string& nazwa_pliku);
	void clear_file(std::string& nazwa_pliku);
	void del(std::string& nazwa_pliku);
	void bv();
	void count_char(std::string& nazwa_pliku);
	void show_block(std::string& dane);
	void show_open_files();
	void show_exspace(std::string i);
	void exit();

public:
	//Interfejs() = default;
	Interfejs(std::shared_ptr<FileManager> dysk, std::shared_ptr<ManagerOfProcesses> processes_manager, std::shared_ptr<Interpreter> interpreter,
		std::shared_ptr<PlanistaProcesora> planista, std::shared_ptr<Pamiec> pamiec, std::shared_ptr<VirtualMemory> virtualMemory);
	void command_serv();
};