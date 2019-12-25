#ifndef EXPERT_SYSTEM_HPP
#define EXPERT_SYSTEM_HPP

//#include <utility> 
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <experimental/numeric>
#include <map>
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <iterator>
//#define DEBUG 1

enum DATA_SWITCH{
	RULES_START = 65,
	RULES_END = 90,
	FACTS = 61,
	QUERRIES = 63
};

enum ALLOWED_CHARS{
	LEX_START = 65,
	LEX_END = 90,
	LSKOBKA = 40,
	PSKOBKA = 41,
	PLUS = 43,
	LUKAZ = 60,
	RAVNO = 61,
	PUKAZ = 62,
	STEPEN = 94,
	VOSKL = 33,
	PIPE = 124
};

bool is_char_operator(char c) {
	if (c == '+' || c == '|' || c == '^')
		return true;
	else
		return false;
}

class InputManager {
public:
	InputManager(){
		this->input_list = {};
		this->cleaned_lines = {};
	}
	void line_cleaner() {
		std::size_t found;

		for (auto i : this->input_list) {
			/* Delete comments */
			found = i.find('#');
			if (found != -1) {
				i.resize(found);
			}
			/* Delete spaces */
			found = 0;
			do{
				found = i.find(' ', found);
				if (found != -1) i.erase(found, 1);
			} while (found != -1);
			/* Write to cleaned_rules */
			if (i.size() > 0)
				this->cleaned_lines.push_back(i);
		}
	}

	void get_input_file(std::string filename){
		std::string line;
		std::ifstream input(filename);
		while(std::getline(input, line)) {
			//std::cout << line << std::endl;
			this->input_list.push_back(line);
		}
	}

	void print_cleaned_lines(){
	for(auto i : this->cleaned_lines){
		std::cout << i << std::endl;
	}

	}
//	std::vector<std::string> cleaned_lines;

	std::vector<std::string> getterCL(){
		return this->cleaned_lines;
	}
private:
	std::vector<std::string> input_list;

	std::vector<std::string> cleaned_lines;

};

class SyntaxChecker {

public:
	SyntaxChecker(){
		this->cleaned_lines = {};
		this->isQuestions = false;
		this->isFacts = false;
		this->isRules = false;
}
	int print_data(void){
		for (auto i: this->cleaned_lines){
			std::cout << i << std::endl;
		}
	}

	void setterCL(std::vector<std::string> in){
		this->cleaned_lines = in;
	}


	std::vector<std::string> getterCL(){
		return this->cleaned_lines;
	}


	bool isAlpha(char c){
#ifdef DEBUG
		std::cout << std::endl<<"Parsing char: " << c << std::endl;
#endif
		//IsUpperCaseLetter?
		if (c >= RULES_START && c <= RULES_END)
			return true;
		return false;

}
/*	void getRules(std::string str) {
		int i = 0;
		while (str[i]){*/
			/* Rule one - after each letter should goes operator */
			/*if (str[i] >= 65 && str[i] <= 90 && is_char_operator(str[i + 1])) {
				std::cout<< str[i + 1] << std::endl;
			} else if (str[i] == '(' && (str[i + 1] >= 65 || str[i + 1] <= 90)) {
						i++;
		}
	}
*/



int SyntaxRuleChecker(std::string line){
	if (line == "\n" || line == "")
		return false;
	char lastSymb = '\0';
	int skobki = 0;

#ifdef DEBUG
	std::cout << "Entered SyntaxRuleChecker Loop" << std::endl;
#endif
	for (auto i: line){
#ifdef DEBUG
		/* 
		DEBUG PRINTS
		*/
		std::cout << "Calling isAlpha";
		if (isAlpha(i))
			std::cout << "Result: True" << std::endl;
		else
			std::cout << "Result: False" << std::endl;
		std::cout << "character " << i << std::endl;
		std::cout << "lastSymb " << lastSymb << std::endl;

		std::cout << "\n";
#endif


		if (i == LSKOBKA){
#ifdef DEBUG
			std::cout << "Found LSKOBKA" << std::endl;
#endif
			if (isAlpha(lastSymb))
				return false;
			skobki++;
		}
		else if (i == PSKOBKA){
#ifdef DEBUG
			std::cout << "Found PSKOBKA" << std::endl;
#endif
			if (skobki <= 0)
				return false;
			if (!(isAlpha(lastSymb)) && !(lastSymb == ')')) 
				return false;
			skobki--;
		}
		else if (i == VOSKL){
#ifdef DEBUG
			std::cout << "Found VOSKL" << std::endl;
#endif
			if (isAlpha(lastSymb))
				return false;
		}
		else if (i == PLUS){
#ifdef DEBUG
			std::cout << "Found PLUS" << std::endl;
#endif
			if ((isAlpha(lastSymb) == false) && (lastSymb != ')'))
				return false;
		}
		else if (i == PIPE){
#ifdef DEBUG
			std::cout << "Found PIPE" << std::endl;
#endif
			if ( !(isAlpha(lastSymb)) )
				return false;
		}
		else if (i == STEPEN){
#ifdef DEBUG
			std::cout << "Found PIPE" << std::endl;			
#endif
			if ( !(isAlpha(lastSymb)) )
				return false;
		}
		//TWO LETTERS IN A ROW WITHOUT SPECIAL SYMB: A + BBBBBB
		else if (isAlpha(i)){
#ifdef DEBUG
			std::cout << "Found LETTER" << std::endl;
#endif
			if (isAlpha(lastSymb))
				return false;
		}
		else if (i == RAVNO){
#ifdef DEBUG
			std::cout << "Found RAVNO" << std::endl;
#endif
			if ( !(isAlpha(lastSymb)) && lastSymb != '<' && lastSymb != ')')
				return false;
		}
		else if (i == PUKAZ){
#ifdef DEBUG
			std::cout << "Found PRAVII UKAZATEL (>)" << std::endl;
#endif
			if (lastSymb != '=')
				return false;
		}
		else if (i == LUKAZ){
#ifdef DEBUG
			std::cout << "Found LEVII UKAZATEL (<)" << std::endl;
#endif
			if (isAlpha(lastSymb) == false && lastSymb != ')')
				return false;
		}
		else{
#ifdef DEBUG
			std::cout << "Found UNDEFINED VARIANT" << std::endl;			
#endif		
			return false;
		}

		lastSymb = i;
	}
	if (skobki > 0 || lastSymb == '!'){
		return false;
	}
	return true;
}

	void validRules(std::string line) {
		if (isFacts == true)
			throw("Error. Rules should be defined before facts");
		if (isQuestions == true)
			throw("Error. Rules should be defined before querries");
		isRules = true;
#ifdef DEBUG
		std::cout << "==========================" << std::endl;
		std::cout << "Rule: " << line << std::endl;
		std::cout << "==========================" << std::endl;
#endif
		if (SyntaxRuleChecker(line) == false){
			throw("Error. Bad syntax in the rule statements\n");
		}
		this->Rules.push_back(line);
		//ToDo Validate Facts!
		//ToDo Starting handling rule statements

}

	bool SyntaxFactsAndQuerriesChecker(std::string line){
		for (auto i: line){
			if (isAlpha(i))
				continue;
			else
				return false;
		}
		return true;
	}
	
	void validFacts(std::string line){
		if (isRules == false)
			throw("Error. Rules should be defined");
		if (isFacts == true)
			throw("Error. Already defined facts");
		this->isFacts = true;

		if (SyntaxFactsAndQuerriesChecker(line.erase(0,1)) == false)
			throw ("Error. Crap in the fact statement");

		this->Facts.push_back(line);

#ifdef DEBUG
		std::cout << "Facts: " << line << std::endl;
#endif


	}

	void validQuerries(std::string line){
		if (isQuestions == true)
			throw("Error. Already defined querries");
		if (!isFacts)
			throw("Error. Querries should be defined after facts");

		this->isQuestions = true;

		if (SyntaxFactsAndQuerriesChecker(line.erase(0,1)) == false)
			throw ("Error. Crap in the querries statement");

		this->Querries.push_back(line);

#ifdef DEBUG
		std::cout << "Querries: " << line << std::endl;
#endif
	}

	void print_Rules(){
		for (auto i: this->Rules)
			std::cout << i << std::endl;
	}

	void print_Facts(){
		for (auto i: this->Facts)
			std::cout << i << std::endl;
	}

	void print_Querries(){
		for (auto i: this->Querries)
			std::cout << i << std::endl;		
	}
	
	void dataType(){
		/*
		Function for validaton each line syntax and etc stuff according to specific rules;
		-Rules
		-Facts
		-Querries
		*/
		for (auto line: this->cleaned_lines)  { // chech each line
			int ascii_code = line[0];
			DATA_SWITCH ascii = DATA_SWITCH(ascii_code);

			// std::cout << ascii <<std::endl;
			if (FACTS == ascii_code){
				validFacts(line);
			}
			else if (QUERRIES == ascii_code){
				validQuerries(line);
			}
			else if (ascii_code >= RULES_START && ascii_code <= RULES_END){
				validRules(line);
			}
			else
				throw("Error. Bad line\n");
		}
		//print_Rules();
		//print_Facts();
		//print_Querries();

}

	std::vector<std::string> getterQuerry(){
		return (this->Querries);
	}


	std::vector<std::string> getterRules(){
		return (this->Rules);
	}


	std::vector<std::string> getterFacts(){
		return (this->Facts);
	}

private:
	std::vector<std::string> cleaned_lines;

	int rules_qty;
	std::vector<std::string> Rules;

	int facts_qty;
	std::vector<std::string> Facts;

	int querries_qty;
	std::vector<std::string> Querries;

	bool isFacts;
	bool isQuestions;
	bool isRules;
};

// std::vector<char> alphabet(26);
// std::iota(alphabet.begin(), alphabet.end(), 'A');

class DecisionManager
{
public:

	DecisionManager(){
/*
initialization of the alphabet
*/
	this->alphabet_status.insert(std::make_pair('A', false));
	this->alphabet_status.insert(std::make_pair('B', false));
	this->alphabet_status.insert(std::make_pair('C', false)); 
	this->alphabet_status.insert(std::make_pair('D', false)); 
	this->alphabet_status.insert(std::make_pair('E', false)); 
	this->alphabet_status.insert(std::make_pair('F', false)); 
	this->alphabet_status.insert(std::make_pair('G', false)); 
	this->alphabet_status.insert(std::make_pair('H', false)); 
	this->alphabet_status.insert(std::make_pair('I', false)); 
	this->alphabet_status.insert(std::make_pair('J', false)); 
	this->alphabet_status.insert(std::make_pair('K', false)); 
	this->alphabet_status.insert(std::make_pair('L', false)); 
	this->alphabet_status.insert(std::make_pair('M', false)); 
	this->alphabet_status.insert(std::make_pair('N', false)); 
	this->alphabet_status.insert(std::make_pair('O', false)); 
	this->alphabet_status.insert(std::make_pair('P', false)); 
	this->alphabet_status.insert(std::make_pair('Q', false)); 
	this->alphabet_status.insert(std::make_pair('R', false)); 
	this->alphabet_status.insert(std::make_pair('S', false)); 
	this->alphabet_status.insert(std::make_pair('T', false)); 
	this->alphabet_status.insert(std::make_pair('U', false)); 
	this->alphabet_status.insert(std::make_pair('V', false)); 
	this->alphabet_status.insert(std::make_pair('W', false)); 
	this->alphabet_status.insert(std::make_pair('X', false)); 
	this->alphabet_status.insert(std::make_pair('Y', false)); 
	this->alphabet_status.insert(std::make_pair('Z', false)); 
};
	void print_alpha(){
		std::map<char, bool>::iterator itr; 
		for (itr = this->alphabet_status.begin(); itr != this->alphabet_status.end(); ++itr){
			std::cout << itr->first << "\t" << itr->second << std::endl;
		}
	}

private:
	// this is a dictionary where the keys are Letters and the values True/False
	std::map<char, bool> alphabet_status;

};

class RuleManager{
public:
	void setterRulesList(std::vector<std::string> in){
		this->Rules = in;
	}

	void printRules(){
	std::cout << "RuleManager object" << std::endl;
		for (auto i: this->Rules)
			std::cout << i << std::endl;
	}

private:
	std::vector<std::string> Rules;
};
#endif
