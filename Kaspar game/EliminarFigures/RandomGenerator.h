#pragma once
#include <random>

class RandomGenerator
{
public:
	RandomGenerator();
	~RandomGenerator();

	int GetValue(int lowValue, int HighValue) const;
	void Randomize() const;

private:

};

