#include "pch.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <exception>
#include <iomanip>
#include <fcntl.h>
#include <io.h>
#include <ctime>

#define line = std::cout << std::endl;
const short int MAX_NUMBER = 130;

double function(double x, double y)
{
	return cos(x)*cos(y)*exp(-1*pow(x,2) - pow(y,2));
	//return exp(-1 * pow(x, 2))*exp(-1 * pow(y, 2)) / (1 + pow(x, 2) + pow(y, 2));
}

struct individual
{
	double x = 0;
	double y = 0;
	double fit = 0;
};

double find_average(std::vector<individual> &population)
{
	double sum = 0;
	for (size_t i = 0; i < population.size(); i++)
	{
		sum += population[i].fit;
	}
	return sum / 4;
}

double find_max(std::vector<individual> &population)
{
	double max = 0;
	for (size_t i = 0; i < population.size(); i++)
	{
		if (max < population[i].fit)
			max = population[i].fit;
	}
	return max;
}

void output(int number, std::vector<individual> &population)
{
	for (size_t i = 0; i < population.size(); i++)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(L"\x2502");
		if ((i == 0))
		{
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << std::setw(7) << std::right << number << "      ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
			_setmode(_fileno(stdout), _O_TEXT);
		}
		else
		{
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << "             ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
			_setmode(_fileno(stdout), _O_TEXT);
		}
		std::cout << std::fixed << std::setprecision(4) << std::setw(10) << std::right << population[i].x << "   ";
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(L"\x2502");
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << std::setw(10) << std::right << population[i].y << "   ";
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(L"\x2502");
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << std::setw(10) << std::right << population[i].fit << "   ";
		_setmode(_fileno(stdout), _O_U16TEXT);
		wprintf(L"\x2502");
		if (i == 0)
		{
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << std::setw(10) << std::right << find_max(population) << "   ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << std::setw(10) << std::right << find_average(population) << "   ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
		}
		else
		{
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << "             ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
			_setmode(_fileno(stdout), _O_TEXT);
			_setmode(_fileno(stdout), _O_TEXT);
			std::cout << "             ";
			_setmode(_fileno(stdout), _O_U16TEXT);
			wprintf(L"\x2502");
		}
		_setmode(_fileno(stdout), _O_TEXT);
		std::cout << std::endl;
	}
	_setmode(_fileno(stdout), _O_U16TEXT);
	if (number == 10) wprintf(L"\x2514");
	else wprintf(L"\x251C");
	for (int i = 0; i < 83; i++)
	{
		if (number == 10)
			if ((i == 13) || (i == 27) || (i == 41) || (i == 55) || (i == 69)) wprintf(L"\x2534");
			else wprintf(L"\x2500");
		else
			if ((i == 13) || (i == 27) || (i == 41) || (i == 55) || (i == 69)) wprintf(L"\x253C");
			else wprintf(L"\x2500");
	}
	if (number == 10) wprintf(L"\x2518\n");
	else wprintf(L"\x2524\n");
}

void output_begin()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x250C");
	for (int i = 0; i < 83; i++)
	{
		if ((i == 13) || (i == 27) || (i == 41) || (i == 55) || (i == 69)) wprintf(L"\x252C");
		else wprintf(L"\x2500");
	}
	wprintf(L"\x2510\n\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << "  Iteration  ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << "      X      ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << "      Y      ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << "     FIT     ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << "   max FIT   ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502");
	_setmode(_fileno(stdout), _O_TEXT);
	std::cout << " average FIT ";
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\x2502\n");
	wprintf(L"\x251C");
	for (int i = 0; i < 83; i++)
	{
		if ((i == 13) || (i == 27) || (i == 41) || (i == 55) || (i == 69)) wprintf(L"\x253C");
		else wprintf(L"\x2500");
	}
	wprintf(L"\x2524\n");
}

std::vector<double> normalize_vector(std::vector<individual> vec)
{
	std::vector<double> normalized_vec;
	double sum = 0;
	for (size_t i = 0; i < vec.size(); i++)
		sum += abs(vec[i].fit);
	for (size_t i = 0; i < vec.size(); i++)
		normalized_vec.push_back(abs(vec[i].fit) / sum);
	return normalized_vec;
}

std::vector<individual> selection(std::vector<individual> population)
{
	std::vector<double> normalized_fit_vec;
	std::vector<individual> selected_population;
	normalized_fit_vec = normalize_vector(population);
	for (int i = 0; i < 2; i++)
	{
		double random = (double)rand() / RAND_MAX;
		double prob = 1 - normalized_fit_vec[normalized_fit_vec.size() - 1];
		for (int j = normalized_fit_vec.size() - 1; j >= 0; --j)
			if (random >= prob)
			{
				selected_population.push_back(population[j]);
				if (selected_population.size() == 2) return selected_population;
				//population.erase(population.begin() + j);
				//normalized_fit_vec = normalize_vector(population);
				prob = 1 - normalized_fit_vec[normalized_fit_vec.size() - 1];
				random = (double)rand() / RAND_MAX;
				j = normalized_fit_vec.size();
			}
			else
				prob -= normalized_fit_vec[j - 1];
	}
	if (selected_population.size() != 2)
		throw std::exception("ERROR[line 107]");
}

std::vector<individual> cross(std::vector<individual> population)
{
	individual a;
	a.x = population[0].x;
	a.y = population[1].y;
	population.push_back(a);
	a.y = population[0].y;
	a.x = population[1].x;
	population.push_back(a);
	return population;
}

std::vector<individual> mutation(std::vector<individual> population)
{
	double delta = 0;
	double random = 0;
	for (size_t i = 0; i < population.size(); i++)
	{
		random = (double)rand() / RAND_MAX * (0.1 - (-0.1)) + (-0.1);
		if (random <= 0.25)
		{
			delta = (double)rand() / RAND_MAX * (0.1 - (-0.1)) + (-0.1);
			population[i].x += delta;
		}
		random = (double)rand() / RAND_MAX * (0.1 - (-0.1)) + (-0.1);
		if (random <= 0.25)
		{
			delta = (double)rand() / RAND_MAX * (0.1 - (-0.1)) + (-0.1);
			population[i].y += delta;
		}
		population[i].fit = function(population[i].x, population[i].y);
	}
	return population;
}

void evolution(std::vector<individual> &population)
{
	for (size_t i = 0; i < MAX_NUMBER; i++)
	{
		output(i, population);
		try
		{
			population = selection(population);
			population = cross(population);
			population = mutation(population);
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what();
		}
	}
}

int main()
{
	//srand(time(NULL));
	srand(1);
	std::vector<individual> population;
	for (size_t i = 0; i < 4; i++)
	{
		individual *a = new individual();
		a->x = (double)rand() / RAND_MAX * (2.2 - (-2.2)) + (-2.2);
		a->y = (double)rand() / RAND_MAX * (2.2 - (-2.2)) + (-2.2);
		a->fit = function(a->x, a->y);
		population.push_back(*a);
	}
	output_begin();
	evolution(population);
}

