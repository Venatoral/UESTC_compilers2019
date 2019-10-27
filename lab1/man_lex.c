#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

enum CharClass
{
    //EOF = -1,
    UNKNOWN = 0,
    LETTER,
    DIGIT,
    SYMBOL,
    ID = 260,
    cBYTE,
    cSHORT,
    cINT,
    cLONG,
    cFLOAT,
    cDOUBLE,
    cCHAR,
    cSTRING,
    cBOOL,
    //"false"270, "true"271,
    //single operator = ascii
    //"=="278, "!="279, "++"280, "+="281, "--"282, "-="283, "*="284, "/="285, "%="286,
    //"&&"287, "&="288, "||"289, "|="290, "^="291,
    //"<<"292, "<="293, "<<="294, ">>"295, ">>>"296, ">="297, ">>="298, ">>>="299,
    //"private"300, "protected"301, "public"302,
    //"abstract"303, "class"304, "extends"305, "final"306, "implements"307, "interface"308, "native"309, "new"310, "static"311, "strictfp"312, "synchronized"313, "transient"314, "volatile"315,
    //"break"316, "case"317, "continue"318, "default"319, "do"320, "else"321, "for"322, "if"323, "instanceof"324, "return"325, "switch"326, "while"327,
    //"assert"328, "catch"329, "finally"330, "throw"331, "throws"332, "try"333,
    //"import"334, "package"335,
    //"boolean"336, "byte"337, "char"338, "double"339, "float"340, "int"341, "long"342, "short"343,
    //"super"344, "this"345, "void"346,
    //"goto"347, "const"348, "null"349,
};
int charClass;
int lexLen;
char lexeme[MAX_LEN];
char nextChar;
char next2Char;
int nextToken;
FILE *inFile;
FILE *outFile;

void getChar();
void addChar();
void removeBlank();
void removeComment();
void lexer();
int checkNumber();
int checkChar();
int checkString();
void addSymbol();
int checkSymbol();
int checkKeywords();

int main(int argc, char const *argv[])
{
    inFile = stdin;
    outFile = stdout;
    if (argc >= 2)
    {
        inFile = fopen(argv[1], "r");
        if (inFile == NULL)
        {
            printf("ERROR: can not open file %s\n", argv[1]);
            return 1;
        }
    }
    if (argc >= 3)
    {
        outFile = fopen(argv[2], "w");
        if (outFile == NULL)
        {
            printf("ERROR: can not open file.\n", argv[2]);
            return 1;
        }
    }

    getChar();
    while (nextToken != EOF)
        lexer();
    return 0;
}

void getChar()
{
    static int firstRun = 1;
    if (firstRun)
    {
        nextChar = getc(inFile);
        next2Char = getc(inFile);
        firstRun = 0;
    }
    else
    {
        nextChar = next2Char;
        next2Char = getc(inFile);
    }

    if (nextChar == EOF)
        charClass = EOF;
    else if (isalpha(nextChar) || nextChar == '_' || nextChar == '$')
        charClass = LETTER;
    else if (isdigit(nextChar))
        charClass = DIGIT;
    else if (nextChar == '\'')
        charClass = cCHAR;
    else if (nextChar == '\"')
        charClass = cSTRING;
    else if (ispunct(nextChar))
        charClass = SYMBOL;
    else
        charClass = UNKNOWN;
}

void addChar()
{
    if (lexLen <= MAX_LEN - 2)
    {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("ERROR: lexeme is too long.\n");
}

void removeBlank()
{
    while (isspace(nextChar))
        getChar();
}

void removeComment()
{
    removeBlank();
    if (nextChar == '/')
    {
        if (next2Char == '/')
        {
            while (1)
            {
                getChar();
                if (nextChar == '\n')
                    break;
                if (next2Char == EOF)
                    break;
            }
            getChar();
        }
        else if (next2Char == '*')
        {
            getChar();
            while (1)
            {
                getChar();
                if (nextChar == '*' && next2Char == '/')
                    break;
                if (next2Char == EOF)
                    break;
            }
            getChar();
            getChar();
        }
    }
}

//recognize the integers, strings, identifiers, keywords, operators, comments.
void lexer()
{
    lexLen = 0;
    removeComment();
    removeBlank();
    switch (charClass)
    {
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
        {
            addChar();
            getChar();
        }
        nextToken = checkKeywords();
        break;
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT || nextChar == '.' || nextChar == 'l' || nextChar == 'L' || nextChar == 'f' || nextChar == 'F')
        {
            addChar();
            getChar();
        }
        nextToken = checkNumber();
        break;
    case cCHAR:
        while (1)
        {
            if (nextChar != '\\' && next2Char == '\'')
            {
                addChar();
                getChar();
                addChar();
                getChar();
                break;
            }
            else if (nextChar == '\n')
            {
                nextChar = '\0';
                addChar();
                getChar();
                break;
            }
            else if (nextChar == EOF)
            {
                break;
            }
            else
            {
                addChar();
                getChar();
            }
        }
        nextToken = checkChar();
        break;
    case cSTRING:
        while (1)
        {
            if (nextChar != '\\' && next2Char == '\"')
            {
                addChar();
                getChar();
                addChar();
                getChar();
                break;
            }
            else if (nextChar == '\n')
            {
                nextChar = '\0';
                addChar();
                getChar();
                break;
            }
            else if (nextChar == EOF)
            {
                break;
            }
            else
            {
                addChar();
                getChar();
            }
        }
        nextToken = checkString();
        break;
    case SYMBOL: //operators
        addSymbol();
        nextToken = checkSymbol();
        break;
    case UNKNOWN:
        addChar();
        getChar();
        nextToken = 0;
        break;
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
    }
    fprintf(outFile, "<%6d, %-9s >\n", nextToken, lexeme);
}

int checkNumber()
{
    int i = 0;
    int dotCount = 0;
    int type = 0;
    for (i = 0; i < lexLen; i++)
    {
        if (lexeme[i] == '.')
            dotCount++;
        if (i != lexLen - 1 && (lexeme[i] == 'l' || lexeme[i] == 'L' || lexeme[i] == 'f' || lexeme[i] == 'F'))
            return UNKNOWN;
    }
    if (dotCount == 0)
        type = cINT;
    if (dotCount == 1)
        type = cDOUBLE;
    if (lexeme[lexLen - 1] == 'l' || lexeme[lexLen - 1] == 'L')
        type = cLONG;
    if (lexeme[lexLen - 1] == 'f' || lexeme[lexLen - 1] == 'F')
        type = cFLOAT;
    return type;
}

int checkChar()
{

    if (lexeme[0] == '\'' && lexeme[lexLen - 1] == '\'')
    {
        if (lexLen == 3)
            return cSTRING;
        else if (lexLen == 4 && lexeme[1] == '\\')
            return cSTRING;
    }
    return UNKNOWN;
}

int checkString()
{
    if (lexeme[0] == '\"' && lexeme[lexLen - 1] == '\"')
        return cSTRING;
    return UNKNOWN;
}

void addSymbol()
{

    switch (nextChar)
    {
        //can be number, but we only regard it as dot
    case '.':
        addChar();
        getChar();
        break;
        //can not be binary operator
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '?':
    case ':':
    case ',':
    case ';':
    case '@':
        addChar();
        getChar();
        break;
        //can be binary operator with =
    case '=':
    case '!':
    case '*':
    case '/':
    case '%':
    case '~':
    case '^':
        if (next2Char == '=')
        {
            addChar();
            getChar();
        }
        addChar();
        getChar();
        break;
    //can be binary operator with self and =
    case '+':
    case '-':
    case '&':
    case '|':
        if (next2Char == '=' || next2Char == nextChar)
        {
            addChar();
            getChar();
        }
        addChar();
        getChar();
        break;
    //special
    case '<':
        addChar();
        getChar();
        if (nextChar == '<')
        {
            addChar();
            getChar();
        }
        if (nextChar == '=')
        {
            addChar();
            getChar();
        }
        break;
    case '>':
        addChar();
        getChar();
        if (nextChar == '>')
        {
            addChar();
            getChar();
            if (nextChar == '>')
            {
                addChar();
                getChar();
            }
        }
        if (nextChar == '=')
        {
            addChar();
            getChar();
        }
        break;
        //unknown (illegal)
    case '`':
    case '#':
    case '\\':
    default:
        addChar();
        getChar();
        break;
    }
}

int checkSymbol()
{
    int offset = 300 - 22;
    char *pmultiop[] = {
        "==",
        "!=",
        "++",
        "+=",
        "--",
        "-=",
        "*=",
        "/=",
        "%=",
        "&&",
        "&=",
        "||",
        "|=",
        "^=",
        "<<",
        "<=",
        "<<=",
        ">>",
        ">>>",
        ">=",
        ">>=",
        ">>>=",
        NULL};
    int i = 0;
    if (lexLen == 1)
    {
        if (lexeme[0] == '`' || lexeme[0] == '#' || lexeme[0] == '\\')
            return UNKNOWN;
        else
            return lexeme[0];
    }
    while (pmultiop[i] != 0)
    {
        if (strcmp(lexeme, pmultiop[i]) == 0)
            return offset + i;
        i++;
    }
    return UNKNOWN;
}

int checkKeywords()
{
    int offset = 300;
    char *keywords[] = {
        "private",
        "protected",
        "public",
        "abstract",
        "class",
        "extends",
        "final",
        "implements",
        "interface",
        "native",
        "new",
        "static",
        "strictfp",
        "synchronized",
        "transient",
        "volatile",
        "break",
        "case",
        "continue",
        "default",
        "do",
        "else",
        "for",
        "if",
        "instanceof",
        "return",
        "switch",
        "while",
        "assert",
        "catch",
        "finally",
        "throw",
        "throws",
        "try",
        "import",
        "package",
        "boolean",
        "byte",
        "char",
        "double",
        "float",
        "int",
        "long",
        "short",
        "super",
        "this",
        "void",
        "goto",
        "const",
        "null",
        NULL};
    int i = 0;

    if (strcmp(lexeme, "false") == 0)
        return 270;
    if (strcmp(lexeme, "true") == 0)
        return 271;
    while (keywords[i] != 0)
    {
        if (strcmp(lexeme, keywords[i]) == 0)
            return offset + i;
        else
            i++;
    }
    return ID;
}
