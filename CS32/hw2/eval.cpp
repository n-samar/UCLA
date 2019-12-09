#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

// returns true if there is an error
bool checkError(char c1, char c2)
{
	if(c1 == '&' || c1 == '|')
	{
		if(c2>='0' && c2 <= '9') return false;	// ..&1..
		if(c2 == '!') return false;				// ..&!..
		if(c2 == '(') return false;				// ..&(..
		return true;
	}
	if(c1 >= '0' && c1 <= '9')
	{
		if(c2 == '&' || c2 == '|') 
			return false; 						// ..8&..
		if(c2 == ')')
			return false;						// ..8)..
		return true;
	}
	if(c1 == '!')
	{
		if(c2 >= '0' && c2 <= '9') return false;
		if(c2 == '(') return false;
		if(c2 == '!') return false;
		return true;
	}
	if(c1 == '(')
	{
		if(c2 >= '0' && c2 <= '9') return false;
		if(c2 == '(') return false;
		if(c2 == '!') return false;
		return true;
	}
	if(c1 == ')')
	{
		if(c2 == '|' || c2 == '&') return false;
		if(c2 == ')') return false;
		return true;
	}
	return true;
}

int getPrecedence(char ch1)
{
	switch(ch1)
	{
		case '!': return 3;
		case '&': return 2;
		case '|': return 1;
		default: return 4;
	}
}

bool isOperator(char ch)
{
	return (ch == '&' || ch == '|');
}

bool isOperand(char ch)
{
	return (ch >= '0' && ch <= '9');
}

// TODO: Check for bad input
int evaluate(string infix, const bool values[], string& postfix2, bool& result)
{
	string postfix = "";
	string nonBlank;
	stack<char> opStack;
	char ch;
	if(infix.size() == 0)
		return 1;

	// delete all blanks
	for(int i = 0 ; i < infix.size(); i++)
		if(infix[i] != ' ')
		{
			nonBlank += infix[i];
		}
	int bracketCount = 0;

	if(nonBlank.size()>0)
	{
		if(nonBlank[0] == '(') bracketCount++;
		if(nonBlank[0] == ')') return 1;
	}
	for(int i = 1 ; i < nonBlank.size(); i++)
	{
		if(nonBlank[i] == '(') bracketCount++;
		if(nonBlank[i] == ')') bracketCount--;
		if(bracketCount < 0)
			return 1;
		if(checkError(nonBlank[i-1], nonBlank[i]))
			return 1;
	}
	if(bracketCount > 0) return 1;
	// last element must be correctly formated
	if(nonBlank.size() > 0)
	{
	char last = nonBlank[nonBlank.size()-1];
	if(last != ')' 
		&& !(last >= '0' && last <= '9'))
		return 1;
	}

	// first element must be correctly formated
	if(nonBlank.size() > 0)
	{
	char first = nonBlank[0];
	if(first != '!' && first != '(' 
		&& !(first >= '0' && first <= '9'))
		return 1;
	}

	for(int i = 0 ; i < infix.size(); i++)
	{
		ch = infix[i];
		if(isOperand(ch))
		{
			postfix += ch;
			if(!opStack.empty() && opStack.top() == '!')
			{
				postfix += '!';
				opStack.pop();
			}
		}
		else
		switch(ch)
		{
			case ' ': break;
			case '(':
			case '!': opStack.push(ch); break;
			case ')': 
					char top;
					while((top = opStack.top()) != '(')
					{
						postfix += top;
						opStack.pop();
					}
					opStack.pop();
					if(!opStack.empty() && opStack.top() == '!')
					{
						postfix += '!';
						opStack.pop();
					}
					break;
			case '&':
			case '|':
				while(!opStack.empty() && opStack.top() != '(' 
						&& getPrecedence(ch) <= getPrecedence(opStack.top()))
				{
					postfix += opStack.top();
					opStack.pop();
				}
				opStack.push(ch);
				break;
		}
	}
	while(!opStack.empty())
	{
		postfix += opStack.top();
		opStack.pop();
	}
	// END OF INFIX TO POSTFIX
	stack<bool> oprndStack;
	for(int i = 0 ; i < postfix.size(); i++)
	{
		char ch = postfix[i];
		if(isOperand(ch))
			oprndStack.push(values[ch - '0']);
		else if (ch == '!')
		{
			bool temp = oprndStack.top();
			oprndStack.pop();
			oprndStack.push(!temp);
		}
		else
		{
			bool operand2 = oprndStack.top();
			oprndStack.pop();
			bool operand1 = oprndStack.top();
			oprndStack.pop();
			bool result;
			switch(ch)
			{
				case '&': result = (operand1 && operand2); break;
				case '|': result = (operand1 || operand2); break;
				default: result = false;
			}
			oprndStack.push(result);
		}
	}
	result = oprndStack.top();
	postfix2 = postfix;
	return 0;
}

int main()
{
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    
    //test for inavalid infix strings
    assert(evaluate("", ba, pf, answer) == 1);//empty string
    assert(evaluate("!()", ba, pf, answer) == 1);//no operands
    assert(evaluate("2!()", ba, pf, answer) == 1);//nothin in parentheses, no operator
    assert(evaluate("!2()", ba, pf, answer) == 1);//nothing in parentheses, no operator
    assert(evaluate("!()2", ba, pf, answer) == 1);//no operators, nothing in parentheses
    assert(evaluate("2!!!(3)5", ba, pf, answer) == 1);//no operators
    assert(evaluate("2      4", ba, pf, answer) == 1);//lots of space but no operator
    assert(evaluate("6 | 3!", ba, pf, answer) == 1);//ends with !
    
    assert(evaluate("7    ||   (9)     ", ba, pf, answer) == 1);//double operator
    assert(evaluate("8&&!2", ba, pf, answer) == 1);//double operator
    assert(evaluate("0 |& 4", ba, pf, answer) == 1);//double operator
    assert(evaluate("5&|  4", ba, pf, answer) == 1);//double operator
   
    assert(evaluate("| 8", ba, pf, answer) == 1);//starts with operator
	
    assert(evaluate("9 & 2|", ba, pf, answer) == 1);//ends with operator
    
    assert(evaluate("&0 & !(9)", ba, pf, answer) == 1);//starts with operator
    
    assert(evaluate("5 | 7 &", ba, pf, answer) == 1);//ends with operator
    assert(evaluate("faulty  6& 3", ba, pf, answer) == 1);//invalid characters
    
    
    //my own written
    assert(evaluate("5 & !7", ba, pf, answer) == 0 && !answer && pf == "57!&");//tests not with no parentheses
    assert(evaluate("!(5 & 7)", ba, pf, answer) == 0 && answer && pf == "57&!");//tests parentheses
    assert(evaluate("!4", ba, pf, answer) == 0 && answer && pf == "4!");//tests not
    assert(evaluate("!(4)", ba, pf, answer) == 0 && answer && pf == "4!");//tests not with parentheses
    assert(evaluate("4 | 8 & 0", ba, pf, answer) == 0 && answer && pf == "480&|");//tests order of precedence
    assert(evaluate("2 | 3 | 3", ba, pf, answer) == 0 && answer && pf == "23|3|");//tests order of evaluation
    assert(evaluate("2 & 6 & 9", ba, pf, answer) == 0 && !answer && pf == "26&9&");//tests order of evaluation
    assert(evaluate("2 & 6 & !9", ba, pf, answer) == 0 && answer && pf == "26&9!&");//tests order of evaluation and not sign
    assert(evaluate("!3 & 8 | 4", ba, pf, answer) == 0 && answer && pf == "3!8&4|");
    assert(evaluate("(6|7) & !(9|3&7) | !8 & 6", ba, pf, answer) == 0 && answer && pf == "67|937&|!&8!6&|");//super long, tests comvert to postfix
    
    
    
    //given test code
    assert(evaluate("2", ba, pf, answer) == 0 && answer);
    assert(evaluate("(3)", ba, pf, answer) == 0 && !answer);
    assert(evaluate("2&(3)", ba, pf, answer) == 0 && !answer);
    assert(evaluate("0 & !9", ba, pf, answer) == 0 && answer);
    assert(evaluate("!(7|8)", ba, pf, answer) == 0 && !answer);
    assert(evaluate("!7|8  ", ba, pf, answer) == 0 && answer);
    assert(evaluate("6|4&5 ", ba, pf, answer) == 0 && answer);
    assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);
    
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
    assert(evaluate("8|", ba, pf, answer) == 1);
    assert(evaluate("4 5", ba, pf, answer) == 1);
    assert(evaluate("01", ba, pf, answer) == 1);
    assert(evaluate("()", ba, pf, answer) == 1);
    assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    assert(evaluate("2(&8)", ba, pf, answer) == 1);
    assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    assert(evaluate("", ba, pf, answer) == 1);
    assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
           &&  pf == "43!03&&|"  &&  !answer);
    assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
    ba[2] = false;
    ba[9] = true;
    assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
    assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);

    
    cout << "Passed all tests" << endl;
}