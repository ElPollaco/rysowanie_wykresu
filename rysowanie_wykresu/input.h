#pragma once
#include <string>
#include <vector>
#include "conditioners.h"

class relation { // dla przysz�ych relacji np. r�wnania
public:
	relation();
	virtual void rel() = 0;
	virtual ~relation();
};

class function : virtual public relation {
	std::string formula;
	double x;
public:
	function(std::string formula);

	std::string getFormula() const {
		return formula;
	}
	double getXVariable() const {
		return x;
	}

	void rel() override;
	void shortenFunction(const function& f);
	std::vector<std::string> convertIntoRPN(char s[], size_t l);
	double evaluateRPN(const std::vector<std::string>& expression, double x);

	virtual ~function();
};