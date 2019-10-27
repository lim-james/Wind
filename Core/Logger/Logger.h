#ifndef LOGGER_H
#define LOGGER_H

#include "../MACROS.h"

#include <Windows.h>
#include <sstream>
#include <iostream>

#define BLACK			0
#define DARK_BLUE		1
#define DARK_GREEN		2
#define DARK_CYAN		4
#define DARK_RED		5
#define DARK_PINK		6
#define DARK_YELLOW		7
#define DARK_BEIGE		8
#define GREY			9
#define BLUE			10
#define GREEN			11
#define CYAN			12
#define RED				13
#define PURPLE			14
#define YELLOW			15
#define WHITE			16

namespace Console {

	class Logger {

		static HANDLE hConsole;

		static std::ostringstream logs;

	public:

		Logger(const char* file, const char* function, const long& line);
		Logger(const char* tag, const WORD& color, const char* file, const char* function, const long& line);

		template<typename Data>
		friend Logger operator<<(Logger logger, const Data& data);

		static void Dump();

	private:

		template<typename T>
		void Tag(const T& title, const WORD& color);

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
	void Logger::Tag(const T& title, const WORD& color) {
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
#define Warn Logger("WARNING", YELLOW, __FILENAME__, __func__, __LINE__)
#define Error Logger("ERROR", RED, __FILENAME__, __func__, __LINE__)
#define Custom(title, color) Logger(title, color, __FILENAME__, __func__, __LINE__)

#endif