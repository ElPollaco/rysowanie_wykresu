#include <iostream>
#include "conditioners.h"
#include "dataVerifiers.h"
#include "converters.h"

std::vector<std::string> inheritRPN(char s[], size_t l, bool letterOne) {
	std::vector<char> operators; // dynamiczna tablica operator�w
	std::string token; // �eton do zapisu ci�gu znak�w o tym samym typie
	std::vector<std::string> argumentStack;
	bool isLetterOnce = letterOne;
	argumentStack = elementAnalysis(token, operators, argumentStack, s, l, isLetterOnce); // dynamiczna tablica zwrotu zapisu w ONP
	
	return argumentStack;
}

std::vector<int> handleSpecialFunction(const std::string& token, std::vector<char>& operators, std::vector<std::string>& returnStack, char array[], size_t& i, size_t l, bool letterOne) {
	std::vector<std::string> arguments; // tablica argument�w funkcji
	int argCount = 0; // ilo�� zliczonych argument�w
	std::vector<int> argumentsLengthArray;
	int argumentLength = 0;
	int bracketLayer = 0;

	// Zlicza wszystkie napotkane argumenty funkcji specjalnej (maskymalnie mo�e by� ich 2)
	for (size_t j = i + 2; j < l; j++) {
		if (array[j] == ',' && bracketLayer == 0) {
			argCount++;
			argumentsLengthArray.push_back(argumentLength);
			argumentLength = 0;
		}
		else if (array[j] == '(' || array[j] == '[') {
			bracketLayer++;
		}
		else if ((array[j] == ')' || array[j] == ']') && bracketLayer > 0) {
			bracketLayer--;
		}
		else if ((array[j] == ')' || array[j] == ']') && bracketLayer == 0) {
			argCount++;
			argumentsLengthArray.push_back(argumentLength);
			argumentLength = 0;
			break;
		}
		argumentLength++;
	}

	bracketLayer = 0;
	size_t n = i + 2;
	int movingInt = 0;
	// Przetwarza ka�dy argument jaki napotka na drodze
	while (argCount > 0) {
		std::string arg;

		while (n + movingInt < l) {
			if (array[n] == ',' && bracketLayer == 0) {
				n++;
				break;
			}
			else {
				if (array[n] == '(' || array[n] == '[') {
					bracketLayer++;
				}
				else if ((array[n] == ')' || array[n] == ']') && bracketLayer > 0) {
					bracketLayer--;
				}
				else if ((array[n] == ')' || array[n] == ']') && bracketLayer == 0) {
					break;
				}
				arg += array[n];
				n++;
			}
		}
		if (!arg.empty()) {
			if (arg.length() > 1 && (hasX(arg) == true || hasOperator(arg) == true)) { // dla argument�w z�o�onych, np. x/2, 3x^2, ln(x), itp.
				// tworzenie z argumentu kolejnej funkcji, aby zapobiec b��dom zakresu tablicy vectorowej + �adniej to wygl�da, bardziej funkcyjnie
				std::string argumentEquation = "y="; 
				arg = arg.insert(0, argumentEquation);

				char* argArray = convertStringArray(arg);
				size_t sizeArgArray = arg.length();
				std::vector<std::string> convertedArg = inheritRPN(argArray, sizeArgArray, letterOne); // Konwersja na ONP argumentu

				if (argCount == 1) {
					size_t movingInt = moveArrayByDistance(argumentsLengthArray);
					n += movingInt;
				}

				// Dodanie skonwertowanego argumentu do stosu zwrotnego ONP
				for (const auto& a : convertedArg) {
					arguments.push_back(a);
				}

			}
			else {
				arguments.push_back(arg);
			}
			argCount--;
		}
	}

	// Zwraca wszystkie argumenty do tablicy zwrotnej ONP
	for (const auto& arg : arguments) {
		returnStack.push_back(arg);
	}
	returnStack.push_back(token);
	returnStack.push_back(")");

	return argumentsLengthArray;
}
