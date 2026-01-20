// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <format>
#include <cwchar>

void seive();
void find_primes(int value);

int main()
{
	//find_primes(1234);
	seive();
}

void seive() {

	//loop that asks for input
	//print the character sheet to screen after most commands (not help or exit for example)
	std::string input = "";
	std::string tempstr = "";
	std::stringstream inputss;
	std::vector<std::string> inputvect = {};
	std::cout << "\n";

	while (input != "exit") {

		//reset stuff before loops
		inputvect = {};
		input = "";
		inputss.clear();

		//ask for input, then put it into a vector
		std::cout << "\n\nCommand: ";
		getline(std::cin, input);
		inputss.str(input);
		while (getline(inputss, tempstr, ' ')) {
			inputvect.push_back(tempstr);
		}
		if (inputvect.size() == 0) {
			inputvect = { " " };
		}
		if (inputvect[0] == "exit") {
			//do nothing or change a bool or something to exit the loop and end the program
			input = "exit";
			std::cout << "Exiting program\n";
		}
		else if (inputvect[0] == "a number") {
			//do some math;
		}
		else if (inputvect[0] == "test") {
			std::cout << "Test command.\n";
		}
		else try {
			find_primes(stoi(inputvect[0]));
		}
		catch (std::invalid_argument const& ex) {
			std::cout << "Invalid input\n";
		}
		catch (std::out_of_range const& ex) {
			std::cout << "Number too large to compute\n";
		}
	}
	return;
}

void find_primes(int value) {
	int max = sqrt(value);
	std::vector<std::vector<int>> output;
	std::vector<int> temp;
	std::vector<int> primes;
	int row_width = 0;
	int i = 0;
	int j = 0;
	int k = 0;

	//initialize first row of numbers to be checked for prime
	for (i = 0; i < max - 1; i++) {
		//we start counting primes at 2
		primes.push_back(i + 2);
		temp.push_back(primes[i]);
	}
	output.push_back(temp);
	//j is prime we are checking against
	//i is the number we are checking the divisibility of
	//k is the element of temp we are writing to
	for (j = 0; j < primes.size(); j++) {
		//check for next prime
		for (i = j + 1; i < primes.size(); i++) {
			if ((primes[i] % primes[j]) == 0) {
				primes.erase(primes.begin() + i);
				i--;
			}
		}
		//go through the seive and find factors of the prime we found
		i = 0;
		for (k = 0; k < temp.size(); k++) {
			if (i < primes.size() && primes[i] == temp[k]) {
				i++;
			}
			else {
				temp[k] = 0;
			}
		}
		output.push_back(temp);
	}
	row_width = log10(primes.back());
	//ouputing data
	std::cout << "Finding factors of: " << std::to_string(value) << "\n" << "    ";
	//top row, the primes
	for (i = 0; i < primes.size(); i++) {
		std::cout << std::format("{0:<4}", primes[i]);
	}
	std::cout << "\n";
	if (value > 998001) {
		std::cout << "Entry to large to show sieve";
	}
	else {
		//subsequent rows
		for (i = 0; i < max - 1; i++) {
			for (j = 0; j < primes.size(); j++) {
				if (output[j][i] == 0) {
					std::cout << std::format("{0:<4}", " ");
				}
				else {
					//,row_width
					std::cout << std::format("{0:<4}", output[j][i]);
				}
			}
			std::cout << "\n";
		}
	}


	return;
}
