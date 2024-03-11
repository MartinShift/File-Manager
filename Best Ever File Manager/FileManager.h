#pragma once
#include "Menu.h"
#include <filesystem>
#include <Windows.h>
#include "Files.h"
class FileMenu
{
private:
	window sides[2];
	bool currentside;
public:
	FileMenu(const fs::path& path)
	{
		sides[0].generate(path, false);
		sides[1].generate("C:\\", true);
		currentside = false;
	}
	FileMenu()
	{
		sides[0].generate("C:\\", false);
		sides[1].generate("D:\\", true);
		currentside = false;
	}
	void runwindow() {

		bool over = false;
		fs::path currentpath = sides[currentside].getmanager().getcurrentpath();
		fs::path secpath = sides[!currentside].getmanager().getcurrentpath();
		const auto copyOptions = fs::copy_options::overwrite_existing | fs::copy_options::recursive;
		while (true)
		{
			switch (MainMenu())
			{
			case START:
				while (true)
				{
					SetColor(WHITE, BLACK);
					sides[0].print(currentside);
					sides[1].print(currentside);
					switch (sides[currentside].mainMenu())
					{
					case MAINMENU:
						over = true;
						SetColor(WHITE, BLACK);
						system("cls");
						break;
					case SHIFTWINDOW:
						SetColor(WHITE, BLACK);
						system("cls");
						currentside = !currentside;
						break;
					case COPYTOWINDOW:
						currentpath = sides[currentside].getmanager().getcurrentpath();
						secpath = sides[!currentside].getmanager().getcurrentpath();
						sides[currentside].getmanager().makecopy(secpath.string(), currentpath);
						sides[currentside].getmanager().returntoprevdir();
						sides[!currentside].getmanager().renew();
						sides[currentside].getmanager().renew();
						break;
					case MOVETOWINDOW:
						currentpath = sides[currentside].getmanager().getcurrentpath();
						secpath = sides[!currentside].getmanager().getcurrentpath();
						sides[currentside].getmanager().makecopy(secpath.string(), currentpath);
						fs::remove_all(currentpath);
						sides[currentside].getmanager().returntoprevdir();
						sides[!currentside].getmanager().renew();
						sides[currentside].getmanager().renew();
						break;
					}
					if (over) { over = false; break; }

				}
				break;
			case CONTROLS:
				Controls();
				break;
			case EXIT:
				system("cls");
				return;
			}
		}
	}
	size_t MainMenu()
	{
		Menu start({ "Start","Controls","Exit" });
		start.drawOptions();
		WCHAR key;
		while (true)
		{
			start.drawFrame();
			key = GetKey();
			if (key == -1) {
				Sleep(100);
				continue;
			}
			switch (key)
			{
			case VK_UP:
				start.up();
				start.drawOptions();
				break;
			case VK_DOWN:
				start.down();
				start.drawOptions();
				break;
			case ENTER:
				system("cls");
				return start.getSelectedOption();
			default:
				break;
			}
			Sleep(100);
		}
	}
	void Controls()
	{
		WCHAR key;
		std::cout << " Our controls\n\n"
			<< " Go Through Menu: UP/DOWN arrows\n\n"
			<< " Create File/Dir/Filter: F1\n\n"
			<< " Input Dir to enter: F2\n\n"
			<< " Rename File/Dir: F3\n\n"
			<< " Delete Selected File/Filter: DELETE\n\n"
			<< " Change Window: TAB\n\n"
			<< " Move File: LMENU\n\n"
			<< " Copy File: RMENU\n\n"
			<< " Return to parent path: BACKSPACE\n\n"
			<< " Return to main menu: ESC\n\n"
			<< " Enter Selected File/Read Text file: ENTER\n";
		while (true)
		{
			key = GetKey();
			if (key == -1) {
				Sleep(100);
				continue;
			}
			switch (key)
			{
			case ESC:
				system("cls");
				return;
			}
		}
	}
		
};