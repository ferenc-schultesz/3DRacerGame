#pragma once
#include <iostream>
#include <conio.h>
#include <time.h>	

class Timer {
public:
	Timer();
	void           start();
	void           stop();
	void           reset();
	bool           isRunning();
	unsigned long  getTime();
	bool           isOver(unsigned long seconds);
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;
};