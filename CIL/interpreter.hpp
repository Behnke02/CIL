/*************************************************************************
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

#ifndef INTERPRETER
#define INTERPRETER

#include <iostream> //For I/O operations
#include <string>   //For std::string
#include <cstring>  //For C-strings and tokenization
#include <vector>   //For dynamic array storage (Tokens, Keyword Arguments, Variables, etc.)
#include <map>      //For keyword -> token mapping
#include <cstdarg>  //For dynamic parameter counts

#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'
#define SPACE ' '

enum commandReturn
{
    EOF_ERROR = 0,
    PARSE_SUCCESS
};

enum TokenType
{
    //Primitive Functions
    PRINT, CREATE,

    //Variable Types
    INT, FLOAT, CHAR, STRING, BOOL,

    //Scanning Errors
    UNKNOWN_KEYWORD = -1,
    SCAN_ERROR = -2
};

std::map<std::string, TokenType> tokenMap = 
{
    {"PRINT", PRINT},
    {"CREATE", CREATE},
    {"INT", INT},
    {"FLOAT", FLOAT},
    {"CHAR", CHAR},
    {"STRING", STRING},
    {"BOOL", BOOL}
};

typedef struct VariableType
{
    std::string name;
    std::string value;
} Variable;

class Token
{
    public:

    /*
     * @brief Default Constructor for Token Class
    */
    Token();

    /*
     * @brief Initialization Constructor for Token Class
     * @param tokenType TokenType enum representing token's command
     * @param kwargs C-String vector representing command argument(s)
    */
    Token(const TokenType tokentype, std::vector<char *> kwargs);

    /*
     * @brief Initialization Constructor for Token Class
     * @param tokenType TokenType enum representing token's command
     * @param kwargs C-String representing command argument
    */
    Token(const TokenType tokentype, char *keywords);

    /*
     * @brief Initialization Constructor for Token Class
     * @param tokenType TokenType enum representing token's command
     * 
     * Tokens initialized with this method must have keyword arguments
     * set/appended by other class set/append methods
    */
    Token(const TokenType tokentype);

    /*
     * @brief Destructor for Token Class
     * 
     * Deallocates memory for C-String keyword arguments
    */
    ~Token();

    /*
     * @brief Safely fetches token command
     * @return TokenType enum for Token command execution
    */
    TokenType getType();

    /*
     * @brief Safely fetches token keyword arguments
     * @return C-String vector for Token command execution
    */
    std::vector<char *> getArgs();

    /*
     * @brief Safely sets token command type
     * @param newType TokenType enum representing new token command
    */
    void setType(const TokenType newType);

    /*
     * @brief Safely sets token arguments
     * @param newArgs C-String representing new token arguments
    */
    void setArgs(char* newArgs);

    /*
     * @brief Appends keyword arguments to already existing token
     * @param newArgs C-String representing new token argument to be appended
    */
    void appendArgs(char* newArgs);

    /*
     * @brief Appends keyword arguments to already existing token
     * @param newArgs C-String vector representing new token arguments to be appended
    */
    void appendArgs(const std::vector<char *> newArgs);

    private:
    TokenType type;
    std::vector<char *> kwargs;
};

/*
 * @brief Displays arguments needed for program execution
 *
*/
void displayProgramExecution();

/*
 * @brief Master function for .cil program execution
 * @param commandFile Pointer to .cil file
 * @param kwargs Optional arguments for .cil file
 * @return State of program success/failure
*/
bool processCommands(FILE* const commandFile, std::vector<std::string> kwargs);

/*
 * @brief Reads in one line from provided .cil file
 * @param commandFile Pointer to .cil file
 * @param capturedString Reference to string buffer to be filled
 * @return Enum representing success/failure of string capture
*/
int captureCommandString(FILE* const commandFile, std::string &capturedString);

/*
 * @brief Converts captured string into token
 * @param bufferString String containing CIL command to be executed
 * @return Boolean representing success/failure of tokenization
*/
bool tokenizeCommands(std::string bufferString);

/*
 * @brief Identifies primary token command
 * @param commandString C-string containing primary token keyword
 * @return TokenType enum for Token class initialization
*/
TokenType scanToken(char* commandString);

/*
 * @brief Executes token from tokenized string
 * @param token Token object to be executed
 * @return Boolean representing success/failure of token execution
*/
bool executeToken(Token* const token);


#endif