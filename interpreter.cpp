/*************************************************************************
 *
 *       ##########  ##########  #####
 *       ##########  ##########  #####
 *       #####         ######    #####
 *       #####         ######    #####
 *       #####         ######    #####
 *       ##########  ##########  ###########
 *       ##########  ##########  ###########
 * 
 * 
 * Copyright (C) Braedon Behnke, <bcbehnke@hotmail.com>.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution. The terms
 * among other details are referenced in the official documentation
 * seen here : https://github.com/Behnke02/CIL/ along with
 * important files seen in this project.
 *
 * You may opt to use, copy, modify, merge, publish, distribute
 * and/or sell copies of the Software, and permit persons to whom
 * the Software is furnished to do so, under the terms of the
 * LICENSE file. As this is an Open Source effort, all implementations
 * must be of the same methodology.
 *
 *
 *
 * This software is distributed on an AS IS basis, WITHOUT
 * WARRANTY OF ANY KIND, either express or implied.
 *
 ************************************************************************/

#include "interpreter.hpp"

//Variable Vector for later implementation of variable support
std::vector<Variable*> programVariables;

//Initialize current working line to 1
unsigned int lineCount = 1;

int main(int argc, char **argv)
{
    //Check for proper argument count
    if(argc < 2 || std::strstr(argv[1], ".cil") == NULL)
    {
        //Display proper program execution format
        displayProgramExecution();
        //Exit Program: Fail
        return EXIT_FAILURE;
    }

    //Initialize program variables
        
        //Pointer to .cil file
    std::ifstream fileStream;
    
        //Vector of additional program arguments
    std::vector<std::string> kwargs;

    //Attempt to open provided file name
    fileStream.open(argv[1], std::ifstream::in);

    //Check for successful file open
    if(fileStream.is_open())
    {
        //For remaining provided arguments, push onto kwarg vector
        int argIndex = 2;
        while(argIndex < argc)
        {
            kwargs.push_back(argv[argIndex]);
            argIndex += 1;
        }
    
        //Log beginning of program execution
        {
            std::cout << "Beginning Program Execution" << std::endl;
            std::cout << "===========================" << std::endl;
        }

    //Attempt to run .cil program
        if(!processCommands(fileStream, kwargs))
        {
            //Error in Program Execution
            //TODO: Log line of error occurence
            //Log Error Message
            std::cout << "Error occured in program execution at line " << lineCount << std::endl;
            std::cout << "Ending Program" << std::endl;
        }

        for(Variable* variable : programVariables)
        {
            std::cout << variable->name << std::endl;
            std::cout << variable->value << std::endl;
            delete variable;
        }

        fileStream.close();
        //Log end of program execution
        {
            std::cout << "========================" << std::endl;
            std::cout << "End of Program Execution" << std::endl;
        }
    
        //Program execution successful
            //Exit Program: Success
        return EXIT_SUCCESS;
    }
    else
    {
        std::cerr << "Error opening file.  Please check file spelling." << std::endl;
        return EXIT_FAILURE;
    }
}

bool processCommands(std::ifstream &commandFile, std::vector<std::string> kwargs)
{
    char stringBuffer[MAX_CSTR_LEN];
    //TODO: Think of better variable name
    std::string bufferString;
    //Check for additional keyword arguments
    if(kwargs.size() > 0)
    {

    }

    //Execute program by line until End of File
    while(!commandFile.eof())
    {
        while(commandFile.peek() == ' ')
        {
            commandFile.ignore(1);
        }
        commandFile.getline(stringBuffer, MAX_CSTR_LEN, '\n');

        bufferString = stringBuffer;
        if(!tokenizeCommands(bufferString))
        {
            return false;
        }
        lineCount += 1;
    }
    return true;
}

void displayProgramExecution()
{
    std::cout << "Instructions for running CIL Program" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "    CILInterpreter <program file name> <program file arguments>" << std::endl;
    std::cout << "    Program File Name [required]: Name of .cil file to execute" << std::endl;
    std::cout << "    Program File Args [optional]: Command Line Arguments for .cil program" << std::endl;
}

bool tokenizeCommands(std::string bufferString)
{
    char *p;
    Token *scannedToken;

    std::vector<char *> stringTokens;
    char *cString = new char[bufferString.length() + 1];
    std::strcpy(cString, bufferString.c_str());

    char* identifer = std::strtok(cString, " ");

    p = std::strtok(NULL," \"");
    while(p!=0)
    {
        char *newTokenArg = new char[std::strlen(p) + 1];
        std::strcpy(newTokenArg, p);
        std::strcat(newTokenArg, "\0");
        stringTokens.push_back(newTokenArg);
        p = std::strtok(NULL," \"");
    }

    scannedToken = new Token(scanToken(identifer));
    //Process Tokens
    //First token should always be a keyword (For Now)

    //Other arguments for token
    for(char* token : stringTokens)
    {
        scannedToken->appendArgs(token);
    }
    
    delete[] cString;
    stringTokens.clear();
    if(!scannedToken->executeToken())
    {
        delete scannedToken;
        return false;
    }

    //Return operation success
    delete scannedToken;
    return true;
}

bool Token::executeToken()
{
    switch(this->type)
    {
        /*
         * Scopes are created for each switch case for case-specific
         * variable declaration and initialization
        */
        case PRINT:
        {
            /*
             * I do not know why, but appending to an empty string
             * does not print the first character of the full string.
             * Initializing the printed string with a SPACE fixes the issue.
            */
            std::string printString = " ";
             for(char* arg : this->kwargs)
            {
                printString.append(arg);
                printString += " ";
            } 
            std::cout << printString << std::endl;
            break;
        }
        case CREATE:
        {
            //Create variable pointer
            Variable *newVariable = new Variable;

            //Identify type of variable
                //1st kwarg
            newVariable->type = scanToken(this->kwargs[0]);
            //Identify variable name
                //2nd kwarg
            newVariable->name = this->kwargs[1];
            //Identify variable value, adjusting for type
                //4th... kwarg
            newVariable->value = this->kwargs[3];
            /* switch(newVariable->type)
            {
                case CHAR:

                    break;
                case STRING:

                    break;
                case BOOL:

                    break;
                default:
                    newVariable->value = this->kwargs[3];
            }
            */
            //Store pointer to global vector
            programVariables.push_back(newVariable);

            break;
        }
        default:
            std::cerr << "Error: Unknown Keyword" << std::endl;
            return false;
    }

    return true;
}

TokenType scanToken(char* commandString)
{
    //Match keyword to identifiers
    auto identifier = tokenMap.find(commandString);
    if(identifier != tokenMap.end())
    {
        return identifier->second;
    }

    //Return keyword
    return UNKNOWN_KEYWORD;
}

Token::Token(const TokenType tokentype, std::vector<char *> keywords)
{
    this->type = tokentype;
    this->kwargs = keywords;
}

Token::Token(const TokenType tokentype, char *keywords)
{
    this->type = tokentype;
    this->kwargs.push_back(keywords);
}

Token::Token(const TokenType tokentype)
{
    this->type = tokentype;
}

Token::Token()
{
    this->type = UNKNOWN_KEYWORD;
}

Token::~Token()
{
    for(char* arg : kwargs)
    {
        delete[] arg;
    }
}

TokenType Token::getType()
{
    return this->type;
}

std::vector<char *> Token::getArgs()
{
    return this->kwargs;
}

void Token::setType(TokenType newType)
{
    this->type = newType;
}

void Token::setArgs(char* newArgs)
{
    this->kwargs.push_back(newArgs);
}

void Token::appendArgs(char* newArgs)
{
    this->kwargs.push_back(newArgs);
}

void Token::appendArgs(const std::vector<char *> newArgs)
{
    for(char* arg : newArgs)
    {
        this->kwargs.push_back(arg);
    }
}
