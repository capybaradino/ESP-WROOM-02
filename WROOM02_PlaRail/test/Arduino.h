#include "com.h"

#define HIGH (1)
#define LOW (0)
#define OUTPUT (1)
#define INPUT (0)

void digitalWrite(int PINNO, int HIGHLOW){
	std::string str = "";
	switch(HIGHLOW)
	{
		case HIGH:
		str += "HIGH"; break;
		case LOW:
		str += "LOW"; break;
		default:
		str += "UNKNOWN"; break;
	}
	std::cout << "[DBG] digitalWrite(" << PINNO << ", " << str << ") called\n";
}

void delay(int TIME){
	std::cout << "[DBG] delay(" << TIME << ") called\n";
}

class Serial
{
	public:
	void println(const char*);
	void println();
	void print(const char*);
	void begin(int);
	void pinMode(int, int);
	private:
};

void Serial::println(const char* str)
{
	std::cout << str << "\n";
}

void Serial::println()
{
	std::cout << "\n";
}

void Serial::print(const char* str)
{
	std::cout << str;
}

void Serial::begin(int rate)
{
	std::cout << "[DBG] Serial::begin(" << rate << ") called\n";
}

void pinMode(int PINNO, int DIRECTION)
{
	std::string str = "";
	switch(DIRECTION)
	{
		case OUTPUT:
		str += "OUTPUT"; break;
		case INPUT:
		str += "INPUT"; break;
		default:
		str += "UNKNOWN"; break;
	}
	std::cout << "[DBG] pinMode(" << PINNO << ", " + str + ") called\n";
}

Serial Serial;
