#include "RandomGenerator.h"
#include <ctime>

RandomGenerator::RandomGenerator()
{
	srand(time(NULL));
}

RandomGenerator::~RandomGenerator()
{
}

int RandomGenerator::GetValue(int lowValue, int highValue)
{
	int randomNumber = rand() % (highValue - lowValue) + lowValue;
	int sign = rand() % 1;
	if (!sign) sign = -1;
	return randomNumber * sign;
}
