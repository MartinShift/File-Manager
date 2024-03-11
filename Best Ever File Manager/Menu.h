#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include "console.h"
#include<Windows.h>
#include <string>
enum Settings
{
	SetWidth, SetHeight, Exit
};
enum MainMenuOptions { START, CONTROLS, EXIT };
enum FilesOptions { NOTHING, CREATEFILE, DELETEFILE, ENTERFILE, COPYTOWINDOW, MOVETOWINDOW, RETURNTOPREVDIR, MAINMENU, SHIFTWINDOW };
namespace fs = std::filesystem;
using namespace std;
class FilesMenu {
private:
	int activeOption = 0;
	std::vector<fs::path> files;
	size_t getMaxItemSize() const {
		size_t max = 0;
		for (const auto& item : files)
		{
			if (item.string().size() > max) {
				max = item.string().size();
			}
		}
		return max;
	}
	size_t getFrameWidth() const {
		return getMaxItemSize() + 10;
	}
	size_t getFrameHeight() const {
		return files.size() + 4;
	}
public:
	FilesMenu(const vector<fs::path>& files) {
		this->files = files;
	}
	void drawcurrentframe(const bool&window,const bool&current)
	{
		size_t width = 55;
		size_t height = 3;
		for (short y = 0; y < height; y++)
		{
			for (short x = 0; x < width; x++)
			{
				if (x == 0 or x == width - 1 or y == 0 or y == height - 1) {
					SetCursorPosition(x + window * 55, y);
					SetColor(ConsoleColor::WHITE, current == window ? GREEN : GRAY);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}
	void drawFrame(const bool& window, const bool& current) {
		size_t width = 55;
		size_t height = getFrameHeight();
		drawcurrentframe(window,current);
		for (short y = 0; y < height; y++)
		{
			for (short x = 0; x < width; x++)
			{
				if (x == 0 or x == width - 1 or y == 0 or y == height - 1) {
					SetCursorPosition(x + window * 55, y+2);
					SetColor(ConsoleColor::WHITE, current == window ? GREEN : GRAY);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}
	void drawOptions(const bool& window,const fs::path & currentpath) {
		if (!files.empty())
		{
			if (files[0] == "C:\\" and files[1] == "D:\\")
			{
				drawStart(window);
				return;
			}
		}	
		SetCursorPosition(2+window*55, 1);
		std::cout << currentpath;
		short startX = 3;
		short startY = 4;
		size_t max = getMaxItemSize();
		int i = 0;
		std::string sub;
		for (const auto& s : files)
		{
			sub = s.filename().string() + ' ' + pathtype(s);
			if (sub.size() > 55) { sub = sub.substr(0, 50); }
			SetCursorPosition(startX + window * 55, startY + (short)i);
			activeOption == i ? SetColor(RED, WHITE) : SetColor(WHITE, BLACK);
			std::cout << sub;
			i++;
		}
	}
	void down() {
		activeOption++;
		if (activeOption >= files.size()) {
			activeOption = 0;
		}
	}
	void up() {
		activeOption--;
		if (activeOption < 0) {
			activeOption = (int)files.size() - 1;
		}
	}
	size_t getSelectedOption() const {
		return activeOption;
	}
	void drawStart(const bool& window) {
		const std::string startpaths[2] = { "C:\\","D:\\" };
		short startX = 3;
		short startY = 5;
		for (size_t i{}; i < 2; i++)
		{
			SetCursorPosition(startX + window * 55, startY + (short)i);
			activeOption == i ? SetColor(RED, WHITE) : SetColor(WHITE, BLACK);
			std::cout << startpaths[i] << '\n';
		}
	}
	std::string pathtype(const fs::path& p)
	{
		if (fs::is_regular_file(p)) return "FILE";
		if (fs::is_directory(p)) return "DIR";
		if (fs::is_symlink(p)) return "SYM";
		if (fs::is_block_file(p)) return "BLOCK";
		if (fs::is_character_file(p)) return "CHAR";
		if (fs::is_fifo(p)) return "IPC";
		if (fs::is_socket(p)) return "SOCKET";	
		return "OTHER";
	}
};
class Menu {
private:
	size_t activeOption = 0;
	vector<string> options;
	size_t getMaxItemSize() const {
		size_t max = 0;
		for (string item : options)
		{
			if (item.size() > max) {
				max = item.size();
			}
		}
		return max;
	}
	size_t getFrameWidth() const {
		return getMaxItemSize() + 6;
	}
	size_t getFrameHeight() const {
		return options.size() + 4;
	}
public:
	Menu(const vector<string>& options) {
		this->options = options;
	}
	void generate(const std::vector<std::string> options)
	{
		this->options = options;
	}
	void drawFrame() {
		size_t width = getFrameWidth();
		size_t height = getFrameHeight();
		for (short y = 0; y < height; y++)
		{
			for (short x = 0; x < width; x++)
			{
				if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
					SetCursorPosition(x, y);
					SetColor(ConsoleColor::WHITE, (ConsoleColor)rand() % 16);
					cout << ' ';
				}
			}
		}
		SetColor(WHITE, BLACK);
	}
	void drawOptions() {
		short startX = 3;
		short startY = 2;
		size_t max = getMaxItemSize();
		for (size_t i = 0; i < options.size(); i++)
		{
			SetCursorPosition(startX, startY + (short)i);
			if (activeOption == i) {
				SetColor(RED, WHITE);
			}
			else {
				SetColor(WHITE, BLACK);
			}
			auto item = options[i];
			item.append(string(max - item.size(), ' '));
			cout << item;
		}
	}
	void down() {
		activeOption++;
		if (activeOption >= options.size()) {
			activeOption = 0;
		}
	}
	void up() {
		activeOption--;
		if (activeOption < 0) {
			activeOption = options.size() - 1;
		}
	}
	size_t getSelectedOption() const {
		return activeOption;
	}
};
void drawnameframe(const size_t width, const size_t height, const size_t startX, const size_t startY)
{
	for (size_t y = startY; y < height; y++)
	{
		for (size_t x = startX; x < width; x++)
		{
			if (x == startX or x == width - 1 or y == startY or y == height - 1) {
				SetCursorPosition((short)x, (short)y);
				SetColor(ConsoleColor::WHITE, GRAY);
				std::cout << ' ';
			}
		}
	}
}
bool suretable(const std::string& message, const std::string& leftmessage, const std::string& rightmessage, const COORD& start, const COORD& end) {
	WCHAR key;
	bool sure = false;
	drawnameframe(end.X, end.Y, start.X, start.Y);
	SetColor(WHITE, BLACK);
	SetCursorPosition((55 - (short)message.size() / 2), start.Y + 1);
	std::cout << message;
	SetCursorPosition((end.X + start.X) / 2 - (short)leftmessage.size(), end.Y - 2);
	std::cout << leftmessage;
	SetCursorPosition((end.X + start.X) / 2 + (short)rightmessage.size(), end.Y - 2);
	std::cout << rightmessage;
	while (true)
	{
		key = GetKey();
		if (key == -1) {
			Sleep(10);
			continue;
		}
		switch (key)
		{
		case VK_LEFT:
			SetCursorPosition((end.X + start.X) / 2 + (short)rightmessage.size(), end.Y - 2);
			SetColor(WHITE, BLACK);
			std::cout << rightmessage;
			SetCursorPosition((end.X + start.X) / 2 - (short)leftmessage.size(), end.Y - 2);
			SetColor(BLACK, WHITE);
			std::cout << leftmessage;
			sure = false;
			break;
		case VK_RIGHT:
			SetCursorPosition((end.X + start.X) / 2 - (short)leftmessage.size(), end.Y - 2);
			SetColor(WHITE, BLACK);
			std::cout << leftmessage;
			SetCursorPosition((end.X + start.X) / 2 + (short)rightmessage.size(), end.Y - 2);
			SetColor(BLACK, WHITE);
			std::cout << rightmessage;
			sure = true;
			break;
		case ENTER:
			SetColor(WHITE, BLACK);
			system("cls");
			return sure;
		}
	}
}
bool Sure_Table(const std::string& message) {
	WCHAR key;
	bool sure = false;
	const size_t width = 76;
	const size_t height = 12;
	const short startX = 35;
	const short startY = 7;
	drawnameframe(width, height, startX, startY);
	SetColor(WHITE, BLACK);
	SetCursorPosition((55 - (short)message.size() / 2), startY + 1);
	std::cout << message;
	SetCursorPosition((width + startX) / 2 - 5, height - 2);
	SetColor(RED, BLACK);
	std::cout << "No";
	SetColor(GREEN, BLACK);
	SetCursorPosition((width + startX) / 2 + 3, height - 2);
	std::cout << "Yes";
	while (true)
	{
		key = GetKey();
		if (key == -1) {
			Sleep(10);
			continue;
		}
		switch (key)
		{
		case VK_LEFT:
			SetCursorPosition((width + startX) / 2 + 3, height - 2);
			SetColor(GREEN, BLACK);
			std::cout << "Yes";
			SetCursorPosition((width + startX) / 2 - 5, height - 2);
			SetColor(BLACK, RED);
			std::cout << "No";
			sure = false;
			break;
		case VK_RIGHT:
			SetCursorPosition((width + startX) / 2 - 5, height - 2);
			SetColor(RED, BLACK);
			std::cout << "No";
			SetCursorPosition((width + startX) / 2 + 3, height - 2);
			SetColor(BLACK, GREEN);
			std::cout << "Yes";
			sure = true;
			break;
		case ENTER:
			SetColor(WHITE, BLACK);
			system("cls");
			return sure;
		}
	}
}
bool createtable() {
	const std::string message = "Choose format";
	const std::string leftmessage = "File";
	const std::string rightmessage = "Dir";
	bool sure = false;
	const size_t width = 76;
	const size_t height = 12;
	const short startX = 35;
	const short startY = 7;
	drawnameframe(width, height, startX, startY);
	return suretable(message, leftmessage, rightmessage, { startX,startY }, { width,height });
}
bool copytable(const bool type)
{
	std::string message = "Choose type of ";
	message += type ? "copying" : "moving";
	const std::string leftmessage = "Enter path";
	const std::string rightmessage = "To another window";
	WCHAR key;
	bool sure = false;
	const size_t width = 76;
	const size_t height = 12;
	const short startX = 35;
	const short startY = 7;
	drawnameframe(width, height, startX, startY);
	SetColor(WHITE, BLACK);
	SetCursorPosition((55 - (short)message.size() / 2), startY + 1);
	std::cout << message;
	SetCursorPosition((width + startX) / 2 - 14, height - 2);
	std::cout << leftmessage;
	SetCursorPosition((width + startX) / 2, height - 2);
	std::cout << rightmessage;
	while (true)
	{
		key = GetKey();
		if (key == -1) {
			Sleep(10);
			continue;
		}
		switch (key)
		{
		case VK_LEFT:
			SetCursorPosition((width + startX) / 2, height - 2);
			SetColor(WHITE, BLACK);
			std::cout << rightmessage;
			SetCursorPosition((width + startX) / 2 - 14, height - 2);
			SetColor(BLACK, WHITE);
			std::cout << leftmessage;
			sure = false;
			break;
		case VK_RIGHT:
			SetCursorPosition((width + startX) / 2 - 14, height - 2);
			SetColor(WHITE, BLACK);
			std::cout << leftmessage;
			SetCursorPosition((width + startX) / 2, height - 2);
			SetColor(BLACK, WHITE);
			std::cout << rightmessage;
			sure = true;
			break;
		case ENTER:
			SetColor(WHITE, BLACK);
			system("cls");
			return sure;
		}
	}
}
void warnmessage(const std::string message)
{
	std::string mes = message + " Press Enter:";
	drawnameframe(75, 12, 35, 8);
	SetCursorPosition(55 - (short)message.size() / 2, 10);
	SetColor(RED, BLACK);
	std::cout << message;
	WCHAR key = -1;
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
