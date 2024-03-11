#pragma once
#include <filesystem>
#include <Windows.h>
#include "Menu.h"
#include <iostream>
#include <fstream>
namespace fs = std::filesystem;
class Manager
{
private:
	std::vector<fs::path> Files;
	std::vector<std::string> Filters;
	fs::path currentpath;
public:
	Manager(const fs::path& path)
	{
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (!Filters.empty())
				for (int i{}; i < Filters.size(); i++)
				{
					if (entry.path().extension() == Filters[i] or entry.is_directory())
					{
						Files.push_back(entry);
					}
				}
			Files.push_back(entry);
		}
	}
	Manager() {}
	void renew()
	{
		enterdir(currentpath);
	}
	void enterdir(const fs::path& path)
	{
		currentpath = path;
		Files.clear();
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (!Filters.empty())
			{
				for (const auto &i : Filters)
				{
					if (entry.path().extension() == i or entry.is_directory())
					{
						Files.push_back(entry);
						break;
					}
				}
			}
			else Files.push_back(entry);
		}
	}
	void readfile(const fs::path& path)
	{
		WCHAR key = -1;
		system("cls");
		std::string buffer;
		std::ifstream in(path, std::ios::in);
		if (fs::is_empty(path))
		{
			warnmessage("File is empty!");
		}
		else
		{
			while (std::getline(in, buffer))
			{
				std::cout << buffer << '\n';
			}
			while (true)
			{
				key = GetKey();
				if (key == ENTER or key == ESC) {
					SetColor(WHITE, BLACK);
					system("cls");
					return;
				}
			}
		}
		in.close();
	}
	void gotostartdir()
	{
		Files.clear();
		Files.push_back("C:\\");
		Files.push_back("D:\\");
	}
	void returntoprevdir()
	{
		if (currentpath == "C:\\" or currentpath == "D:\\")
		{
			gotostartdir();
		}
		else enterdir(currentpath.parent_path());
	}
	size_t findfileidx(const fs::path& path)
	{
		for (size_t i{}; i < Files.size(); i++)
		{
			if (Files[i] == path) { return i; }
		}
		return 0;
	}
	void deletefile(const fs::path& path)
	{
		fs::remove_all(path);
		enterdir(currentpath);
	}
	void deletefilter(const size_t& idx)
	{
		Filters.erase(Filters.begin() + idx);
	}
	void makecopy(const std::string& target, const fs::path& path)
	{
		const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
		if (fs::is_directory(path))
		{
			fs::create_directory(target + (target == "C:\\" or target == "D:\\" ? "" : "/") + path.filename().string());
			fs::copy(path, target + (target == "C:\\" or target == "D:\\" ? "" : "/") + path.filename().string(), copyOptions);
		}
		else
		{
			fs::copy(path, target, copyOptions);
		}
	}
	bool copyfile(const fs::path& path, const bool copymove)
	{
		std::string target;
		system("cls");
		bool type = copytable(copymove);
		if (fs::status(path).permissions() == fs::perms::all)
		{
			if (!type)
			{
				system("cls");
				drawnameframe(76, 11, 25, 8);
				SetCursorPosition(26, 9);
				SetColor(WHITE, BLACK);
				std::cout << "Enter path: ";
				std::getline(std::cin, target);
				makecopy(target, path);
				enterdir(currentpath);
			}
			else { currentpath = path; }
			system("cls");
			return type;
		}
		else
		{
			system("cls");
			warnmessage("you don't have permissions for this file!");
			return false;
		}
	}
	bool movefile(const fs::path& path)
	{
		std::string target;
		if (copyfile(path, false))
		{
			currentpath = path;
			return true;
		}
		else
		{
			fs::remove_all(path);
			system("cls");
		}
		return false;
	}
	void gotopath()
	{
		std::string target;
		system("cls");
		drawnameframe(76, 11, 25, 8);
		SetCursorPosition(26, 9);
		SetColor(WHITE, BLACK);
		std::cout << "Enter path: ";
		std::getline(std::cin, target);
		system("cls");
		if (fs::exists(target))
		{
			if (fs::is_directory(target))
			{
				enterdir(target);
			}
			else readfile(target);
		}
		else if (fs::exists(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + target))
		{
			if (fs::is_directory(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + target))
			{
				enterdir(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + target);
			}
			else readfile(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + target);
		}
		else { warnmessage("This path does not exist!"); }
		system("cls");
	}
	void createfile()
	{
		system("cls");
		bool type = createtable();
		system("cls");
		drawnameframe(76, 11, 25, 8);
		SetCursorPosition(26, 9);
		SetColor(WHITE, BLACK);
		std::string file;
		std::cout << "Enter name: ";
		std::getline(std::cin, file);
		if (type)
		{
			fs::create_directory(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + file);
		}
		else
		{
			std::ofstream(currentpath.string() + (currentpath == "C:\\" or currentpath == "D:\\" ? "" : "/") + file);
		}
	}
	void createfilter()
	{
		drawnameframe(76, 11, 25, 8);
		SetCursorPosition(26, 9);
		SetColor(WHITE, BLACK);
		std::string file;
		std::cout << "Enter name(with .): ";
		std::getline(std::cin, file);
		Filters.push_back(file);
		system("cls");
	}
	void renamefile(fs::path& path)
	{
		system("cls");
		drawnameframe(76, 11, 25, 8);
		SetCursorPosition(26, 9);
		SetColor(WHITE, BLACK);
		std::string file;
		std::cout << "Enter name: ";
		std::getline(std::cin, file);
		fs::rename(path, path.parent_path().string() + (path.parent_path() == "C:\\" or path.parent_path() == "D:\\" ? "" : "/") + file);
		system("cls");
		enterdir(currentpath);
	}
	fs::path getcurrentpath()
	{
		return currentpath;
	}
	std::vector<fs::path>getfiles() {
		return Files;
	}
	std::vector<std::string>getfilenames()
	{
		std::vector<std::string> filenames;
		for (const auto& i : Files)
		{
			filenames.push_back(i.filename().string());
		}
		return filenames;
	}
	std::vector<std::string>getfilters()
	{
		return this->Filters;
	}
};
class window {
protected:
	Manager Window;
	bool side;
	bool work = true;
public:
	window(const fs::path& path, const bool& side)
	{
		Window.enterdir(path);
		this->side = side;
	}
	window(const bool& side)
	{
		Window.enterdir("C:\\");
		this->side = side;
	}
	window()
	{}
	Manager& getmanager()
	{
		return Window;
	}
	void filtersmenu()
	{
		Menu filtersmenu(Window.getfilters());
		
		WCHAR key;
		while (true)
		{
			filtersmenu.drawOptions();
			filtersmenu.drawFrame();
			SetColor(WHITE, BLACK);
			key = GetKey();
			if (key == -1) {
				Sleep(10);
				continue;
			}
			switch (key)
			{
			case VK_UP: // вверх по меню
			{
				filtersmenu.up();
				filtersmenu.drawOptions();
				break;
			}
			case VK_DOWN: // вниз по меню
			{
				filtersmenu.down();
				filtersmenu.drawOptions();
				break;
			}
			case VK_DELETE: // видалити фільтр
			{
				system("cls");
				if (Sure("Delete" + Window.getfilters()[filtersmenu.getSelectedOption()]))
				{
					Window.deletefilter(filtersmenu.getSelectedOption());
				}
				filtersmenu.generate(Window.getfilters());
				break;
			}
			case VK_F1: // створити фільтр
			{
				system("cls");
				Window.createfilter();
				filtersmenu.generate(Window.getfilters());
				break;
			}
			case ESC: // повернутися назад
			{
				system("cls");
				return;
			}
			}
		}
	}
	void generate(const fs::path& path, const bool& side)
	{
		Window.enterdir(path);
		this->side = side;
	}
	void print(const bool& sidenum)
	{
		FilesMenu menu(Window.getfiles());
		menu.drawOptions(side, Window.getcurrentpath());
		menu.drawFrame(side, sidenum);
	}
	bool Sure(const std::string& message)
	{
		system("cls");
		return Sure_Table(message);
	}
	size_t mainMenu() {
		FilesMenu menu(Window.getfiles());
		menu.drawOptions(side, Window.getcurrentpath());
		WCHAR key;
		while (true)
		{
			SetColor(WHITE, BLACK);
			key = GetKey();
			if (key == -1) {
				Sleep(10);
				continue;
			}
			switch (key)
			{
			case VK_UP: // вверх по меню
			{
				menu.up();
				menu.drawOptions(side, Window.getcurrentpath());
				break;
			}
			case VK_DOWN: // вниз по меню
			{
				menu.down();
				menu.drawOptions(side, Window.getcurrentpath());
				break;
			}
			case VK_F1:// створити файл
			{
				if (Sure("Create File?"))
				{
					Window.createfile();
					system("cls");
					Window.enterdir(Window.getcurrentpath());
				}
				return CREATEFILE;
			}
			case VK_F2:// ввести шлях
			{
				Window.gotopath();
				return NOTHING;
			}
			case VK_F3: // перейменувати файл
			{
				if (Sure("Rename " + Window.getfilenames()[menu.getSelectedOption()] + '?'))
				{
					Window.renamefile(Window.getfiles()[menu.getSelectedOption()]);
					menu.drawOptions(side, Window.getcurrentpath());
				}
				return NOTHING;
			}
			case VK_F4: // меню фільтрів
			{
				system("cls");
				filtersmenu();
				Window.enterdir(Window.getcurrentpath());
				return NOTHING;
			}
			case VK_DELETE:// видалити папку
			{
				if (Sure("Delete " + Window.getfilenames()[menu.getSelectedOption()] + '?')) { Window.deletefile(Window.getfiles()[menu.getSelectedOption()]); }
				system("cls");
				return DELETEFILE;
			}
			case VK_TAB: // перейти в інше вікно
			{
				return SHIFTWINDOW;
			}
			case BACKSPACE: //повернутися на 1 вверх
			{
				Window.returntoprevdir();
				system("cls");
				return RETURNTOPREVDIR;
			}
			case ESC:  // вийти до меню
			{
				return MAINMENU;
			}
			case ENTER:// зайти в файл
			{
				SetColor(WHITE, BLACK);
				if (fs::is_directory(Window.getfiles()[menu.getSelectedOption()]))
				{
					Window.enterdir(Window.getfiles()[menu.getSelectedOption()]);
				}
				else { Window.readfile(Window.getfiles()[menu.getSelectedOption()]); }
				system("cls");
				return ENTERFILE;
			}
			}
			if (GetAsyncKeyState(VK_LMENU))
			{
				if (Sure("Move " + Window.getfilenames()[menu.getSelectedOption()] + '?'))
				{
					SetColor(WHITE, BLACK);
					system("cls");
					if (Window.movefile(Window.getfiles()[menu.getSelectedOption()]))
					{
						return MOVETOWINDOW;
					}
					else
					{
						return NOTHING;
					}
					break;
				}
				else return NOTHING;
			}
			if (GetAsyncKeyState(VK_RMENU))
			{
				if (Sure("Copy " + Window.getfilenames()[menu.getSelectedOption()] + '?'))
				{
					SetColor(WHITE, BLACK);
					system("cls");
					if (Window.copyfile(Window.getfiles()[menu.getSelectedOption()], true))
					{
						return COPYTOWINDOW;
					}
					else
					{
						return NOTHING;
					}
					break;
				}
				else return NOTHING;
			}
		}
	}
};