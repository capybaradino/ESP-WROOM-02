#include "com.h"

#define HIGH (3001)
#define LOW (3000)
#define OUTPUT (1)
#define INPUT (0)

#define DELAY_ACCL_RATE 0.5

//#define USEPINNUM 7
//int PinUse[USEPINNUM] = {4, 5, 12, 13, 14, 15, 16};
////int PinStatus[USEPINNUM] = {-1, -1, -1, -1, -1, -1, -1};
#define USEPINNUM 5
int PinUse[USEPINNUM] = {13, 16, 5, 4, 14};
int PinStatus[USEPINNUM] = {-1, -1, -1, -1, -1};

static std::string ledStatus(int HIGHLOW)
{
	std::string str = "";
	switch(HIGHLOW)
	{
		case HIGH:
		str += "\033[1;31mHIGH\033[0m"; break;
		//str += "HIGH"; break;
		case LOW:
		str += "\033[1;34mLOW\033[0m"; break;
		//str += "LOW"; break;
		default:
		if(HIGHLOW >= 0 && HIGHLOW <= 1023){
			str += "\033[1;30m" + std::to_string(HIGHLOW) + "\033[0m"; break;
		}else{
			str += "\033[1;30mUNKNOWN\033[0m"; break;
		}
		//str += "UNKNOWN"; break;
	}
	return str;
}

#define MODE_DEGITAL 1
#define MODE_ANALOG 2

void digianaWrite(int PINNO, int HIGHLOW, int mode){
	std::string str = ledStatus(HIGHLOW);
	if(mode == MODE_DEGITAL){
		std::cout << "[DBG] digitalWrite(" << PINNO << ", " << str << ") called\n";
	}else if(mode == MODE_ANALOG){
		std::cout << "[DBG] analogWrite(" << PINNO << ", " << str << ") called\n";
	}else{
		std::cout << "[ERROR] Error illegal mode => " << mode << "\n";
		exit(1);
	}
	int index = -1;
	for(int i=0; i<USEPINNUM; i++)
	{
		if(PinUse[i] == PINNO)
		{
			index = i;
			break;
		}
	}
	if(index == -1)
	{
		std::cout << "[ERROR] illegal pin number => " << PINNO << "\n";
		exit(1);
	}
	PinStatus[index] = HIGHLOW;

	//Print PIN Status
	std::cout << "[DBG] ===================== Ping Status =====================\n";
	std::cout << "[DBG] ";
	for(int i=0; i<USEPINNUM; i++)
	{
		std::cout << "      " << PinUse[i];
	}
	std::cout << "\n[DBG] ";
	for(int i=0; i<USEPINNUM; i++)
	{
		str = ledStatus(PinStatus[i]);
		std::string indent = "";
		for(int j=0; j<8-(str.length()-11); j++)indent += " ";
		std::cout << indent << str;
	}
	std::cout << "\n[DBG] ===================== =========== =====================\n";
	//Print PIN Status
}

void digitalWrite(int PINNO, int HIGHLOW){
	digianaWrite(PINNO, HIGHLOW, MODE_DEGITAL);
}

void analogWrite(int PINNO, int RATE){
	digianaWrite(PINNO, RATE, MODE_ANALOG);
}

void delay(int TIME){
	std::cout << "[DBG] delay(" << TIME << ") called\n";
	usleep(TIME * 1000 * DELAY_ACCL_RATE);
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
