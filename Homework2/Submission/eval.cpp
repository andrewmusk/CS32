//
//  main.cpp
//  Evaluate
//
//  Created by Andrew Musk on 2017/02/07.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include <iostream>
#include <stack>
#include <string>
#include <cassert>
using namespace std;

//method declarations
int getType(char a);
int getPrecedence(char a);
bool checkString(string infix);
bool process(string postfixEpr, const bool values[]);

//main evaluation method
int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    stack<char> postStack;
    string postfixEpr;
    //check if the string is valid
    if(!checkString(infix))
    {
        //return 1 if not valid
        return 1;
    }
    //convert the infix to postfix
    for(int i = 0; i< infix.length(); i++)
    {
        //getting the type of character
        int type = getType(infix[i]);
        switch (type)
        {
            //if not valid character
            case 0:
                return 1;
            //if it is a digit
            case 1:
                postfixEpr += infix[i];
                break;
            //if it is an open brace
            case 2:
                postStack.push(infix[i]);
                break;
            //if it is an operator
            case 3:
                //if an the stack is empty, push the operator to the stack. If there are operators of greater precedence on the stack, push the operator. Stop if you encounter a lower precedence operator or open brace
                while(!postStack.empty() && postStack.top()!='(' && (getPrecedence(infix[i])<=getPrecedence(postStack.top())))
                {
                    postfixEpr += postStack.top();
                    postStack.pop();
                }
                postStack.push(infix[i]);
                break;
            case 4:
                //if you encounter a closed brace, run through the stack and pop off each operator until you reach the close brace
                while(postStack.top()!='(')
                {
                    postfixEpr += postStack.top();
                    postStack.pop();
                }
                postStack.pop();
                break;
            case 5:
                break;
        }
    }
    //add the rest of the operators to the postfix expression
    while(!postStack.empty())
    {
        postfixEpr += postStack.top();
        postStack.pop();
    }
    //calculate the result of the postfix expression
    result = process(postfixEpr,values);
    postfix = postfixEpr;
    return 0;
}

bool process(string postfixEpr, const bool values[])
{
    stack<bool> finalStack;
    //run through each character in the postfix expression
    for(int i = 0 ; i < postfixEpr.length() ; i++)
    {
        //get the current character
        char current  = postfixEpr[i];
        //if it is a digit
        if(isdigit(current))
        {
            //get the value in integer version
            int correct = postfixEpr[i] - '0';
            //push the value in the array onto the stack
            bool value;
            value = values[correct];
            finalStack.push(value);
        }
        else
        {
            bool tempans;
            //get the top value
            bool operand1 = finalStack.top();
            finalStack.pop();
            bool operand2;
            
            switch (current)
            {
                case '!':
                    //if the operator is ! then only use top value
                    tempans = !operand1;
                    break;
                case '&':
                    //pop off the second value and check && comparison
                    operand2 = finalStack.top();
                    finalStack.pop();
                    tempans = operand1 && operand2;
                    break;
                case '|':
                    //pop off the second and check the || comparison
                    operand2 = finalStack.top();
                    finalStack.pop();
                    tempans = operand1 || operand2;
                    break;
            }
            //push the result from the relevant operation to stack
            finalStack.push(tempans);
        }
    }
    //return the final result on top of the stack
    return finalStack.top();
}

//get the type of the character
int getType(char a)
{
    if(isdigit(a))
        return 1;
    if(a=='(')
        return 2;
    if(a=='&'||a=='|'||a=='!')
        return 3;
    if(a==')')
        return 4;
    if(a==' ')
        return 5;
    
    return 0;
}
//giving the precedence of operators an int value
int getPrecedence(char a)
{
    switch (a) {
        case '|':
            return 1;
            break;
        case '&':
            return 2;
            break;
        case '!':
            return 3;
            break;
    }
    return 0;
}
//check the validity of each string
bool checkString(string infix)
{
    //remove any spaces from the infix string
    for(int i=0; i<infix.length();i++)
    {
        while(infix[i]==' ')
        {
            infix.erase(i, 1);
        }
    }
    //count the open and close braces
    int countOpen = 0;
    int countClose = 0;
    int countDig = 0;
    int countOper = 0;
    
    for(int i=0; i<infix.length(); i++)
    {
        char current = infix[i];
        //check starting position and make sure it is a valid character: (!digit
        if(i==0 && (current != '(' && !isdigit(current) && current != '!'))
        {
            return false;
        }
        
        //checking when the current is open braces
        if(current=='(')
        {
            //increment no. of open braces
            countOpen++;
            //only these characters can precede an open brace: !&|
            if(i!=0 && (infix[i-1] != '!' && infix[i-1] != '|' && infix[i-1] != '&' && infix[i-1] != '('))
            {
                return false;
            }
        }
        //checking when it is a close brace
        else if(current==')')
        {
            //only a digit can precede a close brace
            if(isdigit(infix[i-1]) || infix[i-1] == ')')
                countClose++;
            else
            {
                return false;
            }
        }
        //checking when it is an operator
        else if(current=='&'||current=='|')
        {
            countOper++;
            //only a digit or a closing brace can precede an operator
            if(!isdigit(infix[i-1]) && infix[i-1]!=')')
            {
                return false;
            }
        }
        //checking when it is !
        else if(current == '!')
        {
            if(i!=0 && (infix[i-1] != '!' && infix[i-1] != '|' && infix[i-1] != '&'))
            {
                return false;
            }
        }
        else if (isdigit(current))
        {
            countDig++;
        }
    }
    //the number of open and closing braces has to be equal
    if(countClose!=countOpen)
        return false;
    //the number of digits is always one greater than operators
    if(countDig-1!=countOper)
        return false;
    return true;
}



int main()
{
    
    bool ba[10] = {
        //  0      1      2      3      4      5      6      7      8      9
        true,  true,  true,  false, false, false, true,  false, true,  false
    };
    string pf;
    bool answer;
    
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



