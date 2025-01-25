#include "interpreter.hpp"

int main(int argc, char **argv)
{
    FILE *fileptr = fopen("DummyFile.cil", "r");
    std::string command;
    int intChar;

    {
        std::cout << "Beginning Program Execution" << std::endl;
        std::cout << "===========================" << std::endl << std::endl;
    }

    intChar = fgetc(fileptr);
    while(intChar != EOF)
    {
        command += (char)(intChar);
        intChar = fgetc(fileptr);
    }

    std::vector<char *> stringTokens;
    char *cString = new char[command.length() + 1];
    std::strcpy(cString, command.c_str());

    char * p = strtok(cString," \"");
    while (p!=0)
    {
        stringTokens.push_back(p);
        p = strtok(NULL,"\"");
    }

    Token *newToken = new Token(PRINT, stringTokens[1]);

    switch (newToken->getType())
    {
    case PRINT:
        std::cout << newToken->getArgs().value() << std::endl;
        break;
    }

    delete newToken;

    //Log end of program execution
    {
        std::cout << std::endl << "========================" << std::endl;
        std::cout << "End of Program Execution" << std::endl;
    }
    
    //Program execution successful
        //Exit Program: Success
    return EXIT_SUCCESS;
}

TokenType Token::getType()
{
    return this->type;
}

std::optional<std::string> Token::getArgs()
{
    return this->kwargs;
}

Token::Token(TokenType tokentype, std::optional<std::string> keywords = std::nullopt)
{
    this->type = tokentype;
    this->kwargs = keywords;
}