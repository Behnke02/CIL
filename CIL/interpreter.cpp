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

int main(int argc, char **argv)
{
    //Check for proper argument count
    if(argc < 1)
    {
        //Display proper program execution format
        displayProgramExecution();
        //Exit Program: Fail
        return EXIT_FAILURE;
    }

    //Initialize program variables
        
        //Pointer to .cil file
    FILE *fileptr;
        //Vector of additional program arguments
    std::vector<std::string> kwargs;

    //Attempt to open provided file name
        //TODO: Check for correct file extension ".cil"


        //Check for successful file open
    fileptr = fopen(argv[1], "r");

    //For remaining provided arguments, push onto string vector
    if(fileptr != NULL)
    {
        int argIndex = 2;
        while(argIndex < argc)
        {
            kwargs.push_back(argv[argIndex]);
            argIndex += 1;
        }
    }

    //Log beginning of program execution
    {
        std::cout << "Beginning Program Execution" << std::endl;
        std::cout << "===========================" << std::endl;
    }

    //Attempt to run .cil program
    if(!processCommands(fileptr, kwargs))
    {
        //Error in Program Execution
        //TODO: Log line of error occurence
        //Log Error Message
        std::cout << "Error occured in program execution" << std::endl;
        std::cout << "Ending Program" << std::endl;

        for(Variable* variable : programVariables)
        {
            delete variable;
        }
        //Exit Program: Fail
        return EXIT_FAILURE;
    }
    for(Variable* variable : programVariables)
    {
        delete variable;
    }
    fclose(fileptr);

    //Log end of program execution
    {
        std::cout << "========================" << std::endl;
        std::cout << "End of Program Execution" << std::endl;
    }
    
    //Program execution successful
        //Exit Program: Success
    return EXIT_SUCCESS;
}

bool processCommands(FILE* const commandFile, std::vector<std::string> kwargs)
{
    //Check for additional keyword arguments
    if(kwargs.size() > 0)
    {

    }

    //Initialize variables
    std::string bufferStream;

    //Execute program by line until End of File
    
    while(captureCommandString(commandFile, bufferStream) > 0)
    {
        //Capture file line into string
        //Convert string into tokens
        if(!tokenizeCommands(bufferStream))
        {
            bufferStream.clear();
            return false;
        }
        //Move to next line
        bufferStream.clear();
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

int captureCommandString(FILE* const commandFile, std::string &capturedString)
{
    int intChar = EOF;

    capturedString.clear();

    intChar = fgetc(commandFile);

    //Clear leading spaces and empty lines
    while(intChar != EOF && (char)(intChar) == SPACE && (char)(intChar) == NEWLINE_CHAR)
    {
        intChar = fgetc(commandFile);
    }

    if(intChar == EOF)
    {
        return EOF_ERROR;
    }

    //Feed characters into buffer string until command delimiter is found
    while(intChar != EOF && (char)(intChar) != NEWLINE_CHAR)
    {
        capturedString += (char)(intChar);

        intChar = fgetc(commandFile);
    }

    capturedString += NULL_CHAR;

    return PARSE_SUCCESS;
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

            //Identify variable value
                //4th... kwarg
            newVariable->value = "7";

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
