#include "com.h"

#define HIGH (1)
#define LOW (0)
#define OUTPUT (1)

void digitalWrite(int PINNO, int HIGHLOW){
}

void delay(int TIME){
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
}

void Serial::print(const char* str)
{
}

void Serial::begin(int rate)
{
}

void pinMode(int PINNO, int DIRECTION)
{
}

Serial Serial;
