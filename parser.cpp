#include "expert_system.hpp"

class InputManager input_data;
class SyntaxChecker data_parser;
class DecisionManager info_storage;
class RuleManager rule_manager;
class ParsedRuleList depend_man;


/*

WHAT YOU SHOULD DO?

PARSE RULES AND PROCESS THEM

FEW NOTES:
1. At right side you will have something like 
	=> LETTER + LETTER and maybe INVERSION(!) 
	=> LETTER_A + !LETTER_B and result will affect both letters

2. INIT facts will be same to the end of the program run

3. When you will see contradictions 
	blabla => A == FALSE
	blabla => A == TRUE
	IF YOU LETTER == TRUE, ANYWAY YOU FINAL RESULT WILL BE TRUE, don't take into account FALSE 

29.01.20 -> stopped at process rules

	// example
	// test1.convertToRPN("C|!G+Z+(A+B)+D");
	// rule_list.push_back(test1);
	// std::cout << rule_list[0].rule;

*/

int precedence(char x)
{
    if (x == '(') {
        return 0;
    } else if (x != '!') {
        return 1;
    }
    return 2;
}

std::string convertToRPN(std::string expression)// to add &
{
	//expression = "C|!G+Z+(A+B)+D";
    std::stack<char> s;
    std::string postfix;
    for (auto tok : expression) {
        if (std::isupper(tok)) {
            postfix += tok;
        } else if (tok == '(') {
            s.emplace(tok);
        } else if (tok == ')') {
            while (!s.empty()) {
                tok = s.top();
                s.pop();
                if (tok == '(') {
                    break;
                }
                postfix += tok;
            }
        } else {
            while (!s.empty() && precedence(tok) <= precedence(s.top())) {
                postfix += s.top();
                s.pop();
            }
            s.emplace(tok);
        }
    }
    while (!s.empty()) {
        postfix += s.top();
        s.pop();
    }
    expression = postfix;
    std::cout << "Converted to RPN: " << expression << std::endl;
    return expression;
}

std::set<char> GetInvolvedChars(std::string str){

	std::set<char> used_chars;
	std::set<char> inverted_ch;
//GET LIST WITH INVOLVED CHARS
	for (auto i: str){
		if (i >= LEX_START && i <= LEX_END){
			used_chars.insert(i);
		}
		else
			continue;
	}

/*
// i think we can don't handle char:(!), because it will automatically handled by final expression with true/false.
// 	for (auto i: str){
// 		if (i == VOSKL){
// 			std::cout << "FIND !VOSKL: " << i << std::endl;
// 		}
// 		else
// 			continue;
// 	}
*/

	std::cout << "Ivolved chars in the string" << std::endl;
  	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;

	return used_chars;
}

void PrintSet(std::set<char> used_chars){
	std::cout << "PRINT SET, HELLO" << std::endl;
	for (std::set<char>::iterator it = used_chars.begin(); it != used_chars.end(); ++it)
  	  std::cout << *it;
	std::cout << std::endl;
}

std::set<char> ConvertToSet(std::string str){
	
	std::set<char> convertedKeys;
	for (auto i: str){
		convertedKeys.insert(i);
	}
	return convertedKeys;
}

int process_rules(void){
	ParsedRuleList temp; //class to process each line with that you will manage solver
	std::vector<ParsedRuleList> rule_list; //here will be saved all metadata
	std::vector<std::string> facts = rule_manager.getRu(); //here full list of rules

	std::set<char> init_facts = ConvertToSet(info_storage.GetterInitFacts());
#if DEBUG_SOLVER
	// std::string init_facts = info_storage.GetterInitFacts();
	// std::cout << "Init facts that should be true: " << init_facts << std::endl; //truth to start
	int size = facts.size(); //size
	std::cout << "Size: " << facts.size() << std::endl; // size of rule_list, how many cycles will be
#endif

/*	example
	std::cout << temp.isEnd << std::endl;
	rule_list.push_back(temp);
	std::cout << rule_list[0].isEnd << std::endl;
	test.isEnd = true;

*/

	std::string delimiter = "<=>";

	for (auto i: facts){
		//Step 1.
		//Check if bidirectional -> if true -> decline and except error
#if DEBUG_SOLVER
		std::cout << i << std::endl;
#endif
		// std::cout << i.find(delimiter) << std::endl;  // position in the line
		if (i.find(delimiter) != std::string::npos){
			std::cout << "Bidirectional not supported. That's bonus part" << std::endl;
			exit(2);
		}
#if DEBUG_SOLVER
		else{
		//	std::cout << "Rule is onedirectional - passed" << std::endl;
		}
#endif
	}

	delimiter = "=>";

	for (auto i: facts){
		//Step 2. split with delimeters and process each line
		std::string left_token = i.substr(0, i.find(delimiter)); // token is "left side of the expression"
		std::string right_token = i.substr(i.find(delimiter) + 2, i.size());
#if DEBUG_SOLVER
		std::cout << "Str: " << i << std::endl;
		std::cout << "LEFT: " << left_token << std::endl;
		std::cout << "RIGHT: " << right_token << std::endl;
		std::cout << std::endl;
#endif
		std::set<char> left_inv_ch = GetInvolvedChars(left_token);
		std::set<char> right_inv_ch = GetInvolvedChars(right_token);

		temp.invL = left_inv_ch;
		temp.invR = right_inv_ch;

		PrintSet(temp.invL);
		PrintSet(temp.invR);
		// PrintSet();
		// std::cout << "TOKENS" << std::endl;
		std::cout << convertToRPN(left_token) << std::endl;
		std::cout << convertToRPN(right_token) << std::endl;
		std::cout << "===================================" << std::endl;


	///############ put to struct ###############///
	//std::string rule;
	// //splitted rules
	// std::string rside;
	// std::string lside;
	// //Involved symbols upper case letters
	// //std::set invSymb;
	// //std::vector<char> invSymb;
	// std::vector<char> invL;
	// std::vector<char> invR;
	// // Involved Operators ('+, - etc')
	// std::vector<char> invOperLeft;
	// std::vector<char> invOperRight;
	// std::string init_facts;
	}
}	

int parsing(void){
	data_parser.setterCL(input_data.getterCL());
	data_parser.dataType();
	rule_manager.setterRulesList(data_parser.getterRules());

#if DEBUG_RULES_WORK
	rule_manager.printRules();
#endif

#if DEBUG_PARSING
	std::cout << "======================================\n";
	std::cout << "Inititialize final dict\n";
	std::cout << "======================================\n";
	info_storage.print_alpha();
#endif

	info_storage.setterInitF(data_parser.getterFacts());

	info_storage.ApplyInitFacts();

#if DEBUG_PARSING
	std::cout << "======================================\n";
	std::cout << "Applied init facts to the dictionary\n";
	std::cout << "======================================\n";
	info_storage.print_alpha();
#endif
	//convertToRPN("C|!G+Z+(A+B)+D");
	process_rules();
}

int main(int ac, char **av){
	try{
		if (ac == 2) {
			input_data.get_input_file(std::string(av[1]));
			input_data.line_cleaner();
#if DEBUG_READING
		input_data.print_cleaned_lines();
#endif
		parsing();
		}
		else{
			throw std::invalid_argument( "received invalid numbers of arguements" );
		}
	}
	catch( const std::invalid_argument& e ) {
		    std::cout << "ERROR. Invalid numbers of arguement\n";
		 }
	catch (char const * line){
		std::cout << line << std::endl;
	}
	return (0);
}

 
