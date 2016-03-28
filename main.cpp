#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <fstream>
#include <algorithm>
using namespace std;



//converts boolean value into string
string to_string(bool value);
//converts vector boolean into bit values
void convert(size_t numBits, size_t value, vector<bool> &bits);
//evaluates and retreives each token, then stores in string vector
vector<string> tokenizeLine(string line);
//converts expression into RPN
vector<string> infixToPostfix(vector<string> expression);
//lists all expressions (used for LIST command)
void displayExpressions(vector< vector<string> > expressionList);
//evaluates an RPN value (used for TABLE command)
void evaluateRPN(vector<string> expression);
//converts RPN by plugging in bit values into operands
vector<string> convertRPN(vector<string> expression, vector<bool> mybits, string myoperands);
//new RPN string after conversion
bool evaluateNewRPN(vector<string> expression);
//converts string to upper case
void toUpper(string &str);


//write to File (used in STORE command)
void writeFile(string fileName, vector< vector<string> > expressions);
//read from File (used in LOAD command)
void readFile(string fileName, vector< vector<string> > &expressions);
//determines if string ends with specific string (used for checking extension)
bool hasExtension(string &str, string ext);
//determines if two expressions are equal (used in IN command)
bool isEqual(vector <string> exp1, vector <string> exp2);
//converts string to upper case
void toUpper(string &str);

//inline bool exist(const str& name);



//propositional logic
bool conjunction(bool x, bool y); //&
bool disjunction(bool x, bool y); //|
bool implication(bool x, bool y); //->
bool bi_implication(bool x, bool y); //=>
bool negation(bool x); //!
bool equivalence(bool x, bool y); //==
bool entailment(bool x, bool y); //
bool exclusiveOr(bool x, bool y); //^
bool nand(bool x, bool y); //@
bool nor(bool x, bool y);//%



int main()
{
    //user input, command entered
    string input, command, theExpression;
    vector< vector<string> > myExpressions;
    cout<<"Enter command: ";
    getline(cin, input);
    stringstream ss(input);

    ss>>command;//user entered command
    ss>>theExpression;//"argument" following the command
    toUpper(command);
    toUpper(theExpression);
    while (command != "EXIT" && command != "QUIT")
    {
        if(command == "NEW")
        {
            //create new expressionr
            vector<string> newExpression;
            newExpression.push_back(theExpression);
            //attach rpn into the string vector
            vector<string> rpn = infixToPostfix(tokenizeLine(theExpression));
            for(int i = 0;i<rpn.size();i++)
                newExpression.push_back(rpn[i]);
            //store in expressions 2d string vector
            myExpressions.push_back(newExpression);
            cout<<"EXPRESSION "<<myExpressions.size()<<" entered"<<endl;

        }
        else if (command == "DELETE")
        {
                //deletes expression from expressions vector
                int index = std::stoi(theExpression);
                myExpressions.erase(myExpressions.begin()+index - 1);
                cout<<"EXPRESSION "<<index<<" deleted"<<endl;
        }
        else if (command == "TABLE")
        {
            //outputs table given expression number
            int index = stoi(theExpression) - 1;
            evaluateRPN(myExpressions[index]);
        }
        else if (command == "IS")
        {
            //prints out whether or not two expressions are equal
            int firstVal = stoi(theExpression.substr(0,1)) - 1; // is [A]=B
            int secondVal = stoi(theExpression.substr(2,1)) - 1;// is A=[B]


            cout<<myExpressions[firstVal][0]<<" is ";
            //if not equal, then output as such after comparing
            if(!isEqual(myExpressions[firstVal], myExpressions[secondVal]))
                cout<<"not ";
            cout<<"equivalent to "<<myExpressions[secondVal][0]<<endl;

        }
        else if (command == "LIST")
        {
            //prints all expressions to console
            displayExpressions(myExpressions);
        }
        else if (command == "STORE")
        {
            //given filename, check if extensions is.truth, if not append to it
            if(!hasExtension(theExpression, ".truth"))
                theExpression += ".truth";
            //write to file
//            char response;
//            while(exist(theExpression))
//            {

//                cout<<"File already exists. Overwrite? (y/n): ";
//                cin>>response;
//                if(response == 'y')
//                {
//                    writeFile(theExpression, myExpression);
//                }
//                else
//                {
//                    cout<<"Enter new file name: ";
//                    getline(cin, theExpression);
//                }
//            } else
//            {
            writeFile(theExpression, myExpressions);
//            }
        }
        else if (command == "LOAD")
        {
            if(!hasExtension(theExpression, ".truth"))
                theExpression += ".truth";

            readFile(theExpression, myExpressions);

        }
        else if (command == "EDIT")
        {
            //clears corresponding expression number
            int index = stoi(theExpression) - 1;
            myExpressions[index].clear();
            string edittedExpression;
            cout<<"Enter new expression "<<index + 1<<": ";
            getline(cin, edittedExpression);
            //replace with new expression
            toUpper(edittedExpression);
            myExpressions[index].push_back(edittedExpression);
            vector<string> rpn = infixToPostfix(tokenizeLine(edittedExpression));
            for(int i = 0;i<rpn.size();i++)
                myExpressions[index].push_back(rpn[i]);

        }
        else if (command == "HELP")
        {
            //lists all functions and precedence relation
            cout<<"Precedence Relations (descending): ~&|@%^"<<endl;
            cout<<"Enter any of the following commands: "<<endl;
            cout<<"NEW x to enter an expression x"<<endl;
            cout<<"DELETE x to delete expression number x"<<endl;
            cout<<"TABLE x to print trut table for expression x"<<endl;
            cout<<"LIST to see all expressions"<<endl;
            cout<<"STORE x to store all expressions to a file x"<<endl;
            cout<<"LOAD x to load all expressions from a file x"<<endl;
            cout<<"EXIT/QUIT to exit the program"<<endl;
            cout<<"WEXIT/WQUIT to write the file x and exit the program"<<endl;
        }
        else if (command == "WEXIT" || command == "WQUIT")
        {
            //saves to filename then exits
            if(!hasExtension(theExpression, ".truth"))
                theExpression += ".truth";
            writeFile(theExpression, myExpressions);
            return 0;
        }
        else
        {
            cout<<"ERROR: UNRECOGNIZED COMMAND."<<endl;
        }
        string previousCommand = command;
        cout<<"Enter command: ";
        getline(cin, input);
        stringstream ss(input);
        ss>>command;
        ss>>theExpression;
        toUpper(command);
        toUpper(theExpression);


    }
    //user must have entered quit, so program terminates
    return 0;

}

//returns bit values of a vector bool
//@param numBits
//@param value to be converted
//@param vector bool to store conversion
void convert(size_t numBits, size_t value, vector<bool> &bits)
{
    size_t i = numBits - 1;
    for(size_t j = 0;j<numBits;++j)
        bits[j] = 0;
    //cout<<"hey";
    for(size_t j = 0;j<numBits;++j)
    {
        bits[i--] = value%2;
        value /=2;
    }



}



//tokenize line and store into vector
//@param args line, line to be parsed
//@returns string vector containing tokens;
vector<string> tokenizeLine(string line)
{
    //tokenize by parsing each character
    vector<string> tokens;
    for(int i = 0;i<line.size();i++)
    {
        //if it begins with <, it must be <=>, so we take next 3 values
        if(line[i] == '<')
        {
            tokens.push_back(line.substr(i,3));
            i += 2;
        }
       //if it begins with =, then it must be =>, so we take next two values
        else if (line[i] == '=')
        {
            tokens.push_back(line.substr(i,2));
            i += 1;
        }
        //otherwise, it is single character token
        else
            tokens.push_back(line.substr(i,1));

    }
    return tokens;
}


//converts expression into reverse polish using shunting yard algorithm
//@param expression, contains the expression
//@returns vector string now containg the string as well as RPN form
vector<string> infixToPostfix(vector<string> expression)
{
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string precedence = "~&|@%^=><=>";
    vector<string> outputQueue;
    vector<string> operatorStack;
    //while there are tokens to be read
    for(unsigned int i = 0;i<expression.size();i++)
    {
        //USES SHUNTING ALGORITHM AS GIVEN IN WIKIPEDI
        if ((letters.find(expression[i])+1))
        {
            outputQueue.push_back(expression[i]);
        }
        else if (expression[i] == "(")
        {
            operatorStack.push_back(expression[i]);
        }
        else if (expression[i] == ")")
        {

            while(!operatorStack.empty() && operatorStack.back() != "(")
            {
                outputQueue.push_back(operatorStack.back());
                operatorStack.pop_back();
            }
            operatorStack.pop_back();
        }
        else if (precedence.find(expression[i]) + 1)
        {

            while(!operatorStack.empty() && precedence.find(expression[i]) >= precedence.find(operatorStack.back()))
            {
                    outputQueue.push_back(operatorStack.back());
                    operatorStack.pop_back();


            }
            operatorStack.push_back(expression[i]);
        }


    }
    while(!operatorStack.empty())
    {
        outputQueue.push_back(operatorStack.back());
        operatorStack.pop_back();
    }
    return outputQueue;
}

//evaluates RPN of an expression
//@param expression to be evaluated
void evaluateRPN(vector<string> expression)
{
    //determines number of operands and where
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string operands = "";
    vector<string> newRPN;
    for(int i = 1;i<expression.size();i++)
        cout<<expression[i];
    for(int i = 1;i<expression.size();i++)
        if(letters.find(expression[i])+ 1)
            operands+=expression[i];
    int numOperands = operands.length();
    vector<bool> bits(numOperands);
    //format table
    cout<<endl;
    cout<<operands<<"\t"<<expression[0]<<endl;
    cout<<setfill('-')<<setw(21)<<"-"<<setfill(' ')<<endl;
    //for number of operands n, there are 2^n number of bit combinations
    for(size_t i = 0;i<pow(2,numOperands);++i)
    {
        convert(numOperands, i, bits);
        for(int i = 0;i<numOperands;i++)
        {
            cout<<bits[i];

        }
        cout<<": \t";
        //convert the appropriate combinations
        newRPN = convertRPN(expression, bits,operands);
        //evaluate to get boolean result
        cout<<evaluateNewRPN(newRPN)<<endl;
        cout<<setfill('-')<<setw(21)<<"-"<<setfill(' ')<<endl;
    }
    cout<<endl;
}

//evalutes new RPN value after it has been converted
//@param newRPN string vector containg RPN expression after conversion
//@returns boolean value after evaluation
bool evaluateNewRPN(vector<string> newRPN)
{
    //BASED OFF REVERSE POLISH NOTATION ALGORITHM ON WIKI
    string precedence = "~&|@%^";
    vector<int> stack;
    int index = 0;
    while (index != newRPN.size())
    {
        //if it is an operand(0/1), then add it
        if(newRPN[index] == "1" || newRPN[index] == "0")
            stack.push_back(stoi(newRPN[index]));
        else
        {
            //if not, then evaluate/pop/push accordingly
            if (newRPN[index] == "~")
            {
                bool val1 = stack.back();
                stack.pop_back();
                stack.push_back(negation(val1));
            }
            else if (newRPN[index] == "&")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();
                stack.pop_back();
                stack.push_back(conjunction(val1, val2));
            }
            else if (newRPN[index] == "|")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();

                stack.pop_back();
                stack.push_back(disjunction(val1, val2));
            }
            else if (newRPN[index] == "@")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();
                stack.pop_back();
                stack.push_back(nand(val1, val2));
            }
            else if (newRPN[index] == "%")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();
                stack.pop_back();
                stack.push_back(nor(val1, val2));
            }
            else if (newRPN[index] == "^")
            {
                int val1 = stack.back();
                stack.pop_back();
                int val2 = stack.back();
                stack.pop_back();
                stack.push_back(exclusiveOr(val1, val2));
            }
            else if (newRPN[index] == "=>")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();
                stack.pop_back();
                stack.push_back(implication(val1, val2));
            }
            else if (newRPN[index] == "<=>")
            {
                bool val1 = stack.back();
                stack.pop_back();
                bool val2 = stack.back();
                stack.pop_back();
                stack.push_back(bi_implication(val1, val2));
            }

        }
        index++;

    }
    return stack.back();
}


//converts the RPN expression by replacing operands with respective bit values
//@param expression, expression to be plugged into
//@param mybits, vector bool containing bits
//@param myoperands, operands in expression
//@returns new RPN form as string vector
vector<string> convertRPN(vector<string> expression, vector<bool> mybits, string myoperands)
{
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    vector<string> newRPN;
    for(int i = 1;i<expression.size();i++)
    {
        //cout<<"now examining "<<expression[i]<<endl;
        //cout<<"here"<<endl;
        if(myoperands.find(expression[i]) + 1)
        {
            //cout<<"here"<<endl;
            newRPN.push_back(to_string(mybits[myoperands.find(expression[i])]));
        }
        else
        {
            //cout<<"added operator "<<expression[i]<<endl;
            newRPN.push_back(expression[i]);
        }
    }
    return newRPN;
}



//list all expressions given two dimensional vector
//@param expressionList, string vector vector containing expressions
void displayExpressions(vector< vector<string> > expressionList)
{
    for(unsigned int i = 0;i<expressionList.size();i++)
    {
        cout<<i+1<<". "<<expressionList[i][0]<<endl;
    }
}

//writes to file of TRUTH extension
//@param fileName
//@param string vector to read from and print onto file
void writeFile(string fileName, vector< vector<string> > expressions)
{
    //open file and write to it by numbering
    ofstream myFile;
    myFile.open(fileName.c_str());
    for(unsigned int i = 0;i<expressions.size();i++)
        myFile<<i + 1<<". "<<expressions[i][0]<<endl;

}

//reads a file and loads into myexpression
//@param fileName
//@param expressions, the string vector to store values into
void readFile(string fileName, vector< vector<string> > &expressions )
{
    //open file and read from it until lines are gone
    string line;
    ifstream myFile (fileName.c_str());
    while (getline(myFile,line))
    {
        //since they are numbered, extract "second" word of the string (ie 1. A&B)->A&B
        string expression;
        int index = 0;
        for(index; index < line.length();index++)
            if(line[index] == ' ')
                expression = line.substr(index+1);
        vector<string> newExpression;
        newExpression.push_back(expression);
        vector<string> rpn = infixToPostfix(tokenizeLine(expression));
        for(int i = 0;i<rpn.size();i++)
            newExpression.push_back(rpn[i]);
        expressions.push_back(newExpression);


    }
}

//@param str, string to be evalluated
//@param ext to be compared with
//@returns true/false
bool hasExtension(string &str, string ext)
{
    //use string compare and check if size is the same
    return str.size() >= ext.size() && str.compare(str.size() - ext.size(), ext.size(), ext) == 0;
}

//checks if two expressions are equal
//@param exp1 first expression
//@param exp2 second expression
//@returns true/false
bool isEqual(vector<string> exp1, vector<string> exp2)
{
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string operands1 = "";
    string operands2 = "";
    vector<string> newRPN1;
    vector<string> newRPN2;

    for(int i = 1;i<exp1.size();i++)
        if(letters.find(exp1[i])+ 1)
            operands1+=exp1[i];
    //determine number of operands
    int numOperands1 = operands1.length();
    vector<bool> bits(numOperands1);
    for(int i = 1;i<exp2.size();i++)
        if(letters.find(exp2[i])+ 1)
            operands2+=exp2[i];
    //determine number of operands
    int numOperands2 = operands2.length();
    if(numOperands1 != numOperands2)
    {
        cout<<"Differing operand sizes"<<endl;
        return false;
    } else

    //check each boolean result
    {
        vector<bool> bits1(numOperands1);
        vector<bool> bits2(numOperands2);
        for(size_t i = 0;i<pow(2,numOperands1);++i)
        {
            convert(numOperands1, i, bits1);
            convert(numOperands2, i, bits2);
            vector<string> newRPN1 = convertRPN(exp1, bits1,operands1);
            vector<string> newRPN2 = convertRPN(exp2, bits2, operands2);
            if(evaluateNewRPN(newRPN1) != evaluateNewRPN(newRPN2))
                //return false if one differs
                return false;
        }
        //if have not returned false, we can assume all were the same, thus true
        return true;
    }
}




bool conjunction(bool x, bool y)
{
    return x & y;
}

bool disjunction(bool x, bool y)
{
    return x | y;
}

bool implication(bool x, bool y)
{
    return !x & !y | y;
}

bool bi_implication(bool x, bool y)
{
    return !x & !y | x & y;
}

bool negation(bool x)
{
    return !x;
}

bool equivalence(bool x, bool y)
{
    return x == y;
}

bool entailment(bool x, bool y)
{
   return implication(x,y);
}

bool exclusiveOr(bool x, bool y)
{
    return x ^ y;
}


bool nand(bool x, bool y)
{
    return !(x&y);
}

bool nor(bool x, bool y)
{
    return !(x|y);
}

//converts boolean to string
string to_string(bool value)
{
    ostringstream os;
    os << value;
    //use str() func
    return os.str();
}

void toUpper(string &str)
{
    transform(str.begin(),str.end(),str.begin(), toupper);
}

//inline bool exist(const str& name)
//{
//    ifstream file(name);
//    return file;
//}

/*sample code
 *
Enter command: new a=>b
EXPRESSION 1 entered
Enter command: table 1
AB=>
AB      A=>B
---------------------
00:     1
---------------------
01:     0
---------------------
10:     1
---------------------
11:     1
---------------------

Enter command: new a|~b
EXPRESSION 2 entered
Enter command: table 2
AB~|
AB      A|~B
---------------------
00:     1
---------------------
01:     0
---------------------
10:     1
---------------------
11:     1
---------------------

Enter command: is 1=2
A=>B is equivalent to A|~B
Enter command: list
1. A=>B
2. A|~B
Enter command: edit 1
Enter new expression 1: a|b
Enter command: is 1=2
A|B is not equivalent to A|~B
Enter command: quit
Press <RETURN> to close this window...
*/
