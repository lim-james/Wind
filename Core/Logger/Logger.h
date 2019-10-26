#ifndef LOGGER_H
#define LOGGER_H

#include "../MACROS.h"

#include <Windows.h>
#include <sstream>
#include <iostream>

namespace Console {
	enum Color {
		BLACK,
		DARK_BLUE,
		DARK_GREEN,
		DARK_CYAN,
		DARK_RED,
		DARK_PINK,
		DARK_YELLOW,
		DARK_BEIGE,
		GREY,
		BLUE,
		GREEN,
		CYAN,
		RED,
		PURPLE,
		YELLOW,
		WHITE,
	};

	class Logger {

		static HANDLE hConsole;

		static std::ostringstream logs;

	public:

		Logger(const char* file, const char* function, const long& line);
		Logger(const char* tag, const Color& color, const char* file, const char* function, const long& line);

		template<typename Data>
		friend Logger operator<<(Logger logger, const Data& data);

		static void Dump();

	private:

		template<typename T>
		void Tag(const T& title, const Color& color);

	};

	template<typename Data>
	Logger operator<<(Logger logger, const Data& data) {
#ifndef _DEBUG
		return logger;
#endif
		std::cout << data;
		Logger::logs << data;
		return logger;
	}

	template<typename T>
	void Logger::Tag(const T& title, const Color& color) {
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << '[';
		Logger::logs << '[';
		SetConsoleTextAttribute(hConsole, color);
		std::cout << title;
		Logger::logs << title;
		SetConsoleTextAttribute(hConsole, WHITE);
		std::cout << ']';
		Logger::logs << ']';
	}
}

#define Log Logger(__FILENAME__, __func__, __LINE__)
#define Warn Logger("WARNING", Console::YELLOW, __FILENAME__, __func__, __LINE__)
#define Error Logger("ERROR", Console::RED, __FILENAME__, __func__, __LINE__)
#define Custom(title, color) Logger(title, color, __FILENAME__, __func__, __LINE__)

#endif