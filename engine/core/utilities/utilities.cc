#include "pch.h"
#include "utilities.h"

namespace engine
{
	int RoundUpToMultiple(int num_to_round, int multiple)
	{
		if (multiple == 0)
			return num_to_round;

		int remainder = num_to_round % multiple;
		if (remainder == 0)
			return num_to_round;

		return num_to_round + multiple - remainder;
	}
}