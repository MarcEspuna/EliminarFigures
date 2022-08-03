#include "RandomGenerator.h"
#include <ctime>

RandomGenerator::RandomGenerator()
{
	srand(time(NULL));
}

RandomGenerator::~RandomGenerator()
{
}

int RandomGenerator::GetValue(int lowValue, int highValue) const
{
	int randomNumber = rand() % (highValue - lowValue) + lowValue;
	int sign = rand() % 2;
	if (!sign) sign = -1;
	return randomNumber * sign;
}

void RandomGenerator::Randomize() const
{
	srand(time(NULL));
}
