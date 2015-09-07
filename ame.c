#include "ame.h"
#include <fstream>
#include <iostream>
#include <sstream>

double AmeElementMass(std::string element)
{
	if (element == "a")
		return AmeElementMass("4He");
	if (element == "p")
		return AmeElementMass("1H");
	std::string symbol;
	int mass;

	std::stringstream line(element);
	line >> mass >> symbol;

	std::ifstream file("mass.mas12");
	for (int i = 0; i < 39; i++)
	{
		std::string dummy;
		std::getline(file, dummy);
	}
	while (!file.eof())
	{
		int dummy, a;
		std::string e;
		if (file.peek() == '0')
			file >> dummy;
		file >> dummy >> dummy >> dummy >> a >> e;
		if (a == mass && e == symbol)
		{
			double dd, u;
			file >> dd;
			if (file.fail())
			{
				file.clear();
				file >> e;
				file >> dd;
			}
			file >> dd >> dd >> dd >> e >> dd >> dd >> mass >> u;
			u += mass * 1000000;
			return (931.493061*u/1000000.);
		}
		std::getline(file, e);
	}
	return 0;
}
