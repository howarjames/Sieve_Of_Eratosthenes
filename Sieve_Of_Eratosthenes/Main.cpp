
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>
#include <format>
#include <cwchar>
#include <cmath>

void cli();
void sieve(int input);
void find_primitives(int input);
void diophantine(std::vector<int> input);
void big_mod(std::vector<int> input);
std::vector<std::vector<int>> factor(const int input);
int find_phi(const int input);
int find_phi(const int input, std::vector<std::vector<int>> divisors);

int main()
{
	//find_primes(1234);
	cli();
}

void cli() {

	//loop that asks for input
	//print the character sheet to screen after most commands (not help or exit for example)
	bool loop = true;
	std::string input = "";
	std::string tempstr = "";
	std::stringstream inputss;
	std::vector<std::string> inputvect = {};
	std::vector<int> intvect = {};

	while (loop) {

		std::cout <<
			"1 [n]: Perform a Sieve of Eratosthenes on n \n" <<
			"2 [n]: Find the primitive roots of n \n" <<
			"3 [n] [m] [o]: Solve a diophantine of the form nx + my = o \n" <<
			"4 [n] [m] [o]: Find n^m mod(o) \n" <<
			"5 [n]: Factor n \n" <<
			"6 [n]: Find phi(n) \n" <<
			"exit: closes the program\n\n"
			"Command: ";

		//reset stuff before loops
		inputvect = {};
		intvect = {};
		input = "";
		inputss.clear();
		//std::cout << std::to_string((20 ^ 100) % 7817);
		//ask for input, then put it into a vector
		getline(std::cin, input);
		inputss.str(input);
		while (getline(inputss, tempstr, ' ')) {
			inputvect.push_back(tempstr);
		}
		if (inputvect.size() == 0) {
			std::cout << "Error, no input\n";
		}
		else if (inputvect[0] == "exit") {
			//do nothing or change a bool or something to exit the loop and end the program
			loop = false;
			std::cout << "Exiting program\n";
		}
		else if (inputvect.size() == 1) {
			std::cout << "Error, invalid input\n";
		}
		else {
			//start evaluating numbers
			for (std::string i : inputvect) {
				try {
					intvect.push_back(stoi(i));
				}
				catch (std::invalid_argument const& ex) {
					std::cout << "Error, invalid input\n";
					intvect = {};
					break;
				}
				catch (std::out_of_range const& ex) {
					std::cout << "Error, number too large to compute\n";
					intvect = {};
					break;
				}
			}
			if (intvect.size() == 0) {
				//do nothing, don't check the others
			} else if (intvect[0] == 1 && intvect.size() == 2) {
				//perform seive
				sieve(intvect[1]);
			} else if (intvect[0] == 2 && intvect.size() == 2) {
				//find primitive roots
				find_primitives(intvect[1]);
			} else if (intvect[0] == 3 && intvect.size() == 4) {
				//solve diophantine
				diophantine(intvect);
			} else if (intvect[0] == 4 && intvect.size() == 4) {
				//find large modulo
				big_mod(intvect);
			} else if (intvect[0] == 5 && intvect.size() == 2) {
				//factor
				std::ignore = factor(intvect[1]);
			} else if (intvect[0] == 6 && intvect.size() == 2) {
				//find phi
				std::ignore = find_phi(intvect[1]);
			} else {
				std::cout << "Error, option out of range\n";
			}
		}
	}
	return;
}

//This is really fucking buggy, crashes on 2 for example.
//Could take another stab at making this not garbage
void sieve(int value) {
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

void find_primitives(const int input) {
	//first find prime divisors
	//then find varphi
	//then brute force, we can optimize later

	std::vector<std::vector<int>> divisors = factor(input);
	int phi = find_phi(input, divisors);

	std::vector<std::vector<int>> orders;
	std::vector<std::vector<int>> primitives;
	int j = 0;
	int current_n = 0;
	
	//now we brute force order until we get a primitive
	//this should just check stuff in divisors (I think?), but oh well
	for (int i = 2; i < input + 1; i++) {
		current_n = i;
		for (int j = 1; j < phi + 1; j++) {
			if (current_n % input == 1) {
				orders.push_back({ i, j, current_n });
				j = phi + 1;
			}
			else {
				current_n = (current_n * i) % input;
			}
		}

	}
	
	for (std::vector<int> i : orders) {
		std::cout << std::to_string(i[0]) << " ^ " << std::to_string(i[1])  << " is " << std::to_string(i[2]) << " mod " << std::to_string(input) << "\n";
		if (i[1] == phi) {
			primitives.push_back(i);
		}
	}
	if (primitives.size() > 1) {
		for (std::vector<int> i : primitives) {
			std::cout << std::to_string(i[0]) << " ";
		}
		std::cout << "are primitives.\n";
		//for (int i = 0; i < orders.size(); i++) {
			//std::cout << "Order of " << std::to_string(i + 2) << " is " << std::to_string(orders[i]) << " mod " << std::to_string(input) << "\n";
		//}
	}
	else {
		std::cout << "There are no primitives of " << std::to_string(input) << "\n";
	}
	std::cout << "\n\n";
	return;
}

void diophantine(std::vector<int> input) {
	return;
}

void big_mod(std::vector<int> input) {
	//"4 [n] [m] [o]: Find n^m mod(o) \n" <<
	//n = input[1]
	//m = input[2]
	//o = input [3]

	//This gives the wrong answer, probably due to overflow:
	//Not due to overflow! ^ is not square, its bitwise xor! pow is square!
	//std::cout << std::to_string((input[1]^input[2])%input[3]) << "\n";
	unsigned long int x = pow(input[1], input[2]);
	std::cout << std::to_string( x%input[3]) << "\n";
	int output = input[1];
	for (int i = 1; i < input[2]; i++) {
		output = (input[1] * output) % input[3];
		//std::cout << std::to_string(output) << "\n";
	}
	//(20 ^ 100) % 7817) = 4040;
	std::cout << std::to_string(input[1]) << 
		"^" << std::to_string(input[2]) << 
		"mod(" << std::to_string(input[3]) << ") = " << std::to_string(output) << "\n\n";

	return;
}

std::vector<std::vector<int>> factor(const int input) {
	std::vector<std::vector<int>> divisors = { {0,0} };
	int remainder = input;
	int square = sqrt(input);
	int j = 0;

	//we could speed this up quite a bit, but I don't think I will
	for (int i = 2; remainder != 1 && i < square + 1; i++)
	{
		//check if i is a divisor of input
		if (remainder % i == 0) {
			remainder = remainder / i;
			//if next line was implemented correctly this could be much faster
			//square = sqrt(remainder);
			//if i is already in divisors, increment power
			if (i == divisors[j][0]) {
				divisors[j][1]++;
			} //if i is not in divisors, add an entry
			else {
				divisors.push_back({ i,1 });
				j++;
			} //check i again
			i--;
		}
	}
	if (remainder > 1) {
		divisors.push_back({ remainder,1 });
	}
	divisors.erase(divisors.begin());
	//some output code
	std::cout << "Prime divisors of " << std::to_string(input) << " are: ";
	for (std::vector<int> i : divisors) {
		std::cout << std::to_string(i[0]) << "^" << std::to_string(i[1]) << " ";
	}
	std::cout << "\n\n";
	return divisors;
}

int find_phi(const int input) {
	return find_phi(input,factor(input));
}

int find_phi(const int input, std::vector<std::vector<int>> divisors) {
	int phi = 1;
	//now find phi(n)
	for (std::vector<int> i : divisors) {
		phi = pow(i[0], (i[1] - 1)) * ((i[0]) - 1) * phi;
	}
	std::cout << "phi(n) = " << std::to_string(phi) << "\n\n";
	return phi;
}
