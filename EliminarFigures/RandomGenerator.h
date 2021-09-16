#pragma once
#include <random>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	int GetValue(int lowValue, int HighValue);
	void Randomize();

private:

};

