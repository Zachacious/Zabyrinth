#ifndef BKLAYER_H_
#define BKLAYER_H_

#include <random>
#include <functional>

class RandomIntGenerator
{
	std::random_device rd;
	std::mt19937* engine;

public:

	RandomIntGenerator()
	{
		auto const seed = rd();
		engine = new mt19937(seed);
	}
	~RandomIntGenerator()
	{
		if(engine)
			delete engine;
		engine = nullptr;
	}

	int ri(int min, int max)
	{
		std::uniform_int_distribution<int> d(min,max);
		return engine ? d(*engine):0;
	}

};

#endif
