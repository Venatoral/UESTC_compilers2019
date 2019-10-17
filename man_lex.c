#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 100

enum CharClass
{
    UNKNOWN,
    LETTER,
    DIGIT,
    SYMBOL,
    __EOF__,
    INT = 261,
    FLOAT = 262,
    CHAR = 263,
    STRING = 264,
    BOOL = 265,
    ID = 269,
} charClass;
int lexLen;
char lexeme[MAX_LEN];
char nextChar;
char next2Char;
int nextToken;
FILE *inFile;

void getChar();
void addChar();
void removeBlank();
void removeComment();
void lexer();
int checkNumber();
int checkChar();
int checkString();
int addSymbol();
int checkSymbol();
int checkKeywords();

void main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("ERROR: input file name is needed.\n");
        return;
    }
    inFile = fopen(argv[1], "r");
    if (inFile == NULL)
    {
        printf("ERROR: can not open file.\n");
        return;
    }

    getChar();
    while (nextToken != EOF)
        lexer();
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
        charClass = __EOF__;
    else if (isalpha(nextChar) || nextChar == '_' || nextChar == '$')
        charClass = LETTER;
    else if (isdigit(nextChar))
        charClass = DIGIT;
    else if (nextChar == '\'')
        charClass = CHAR;
    else if (nextChar == '\"')
        charClass = STRING;
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
        while (charClass == DIGIT || nextChar == '.')
        {
            addChar();
            getChar();
        }
        nextToken = checkNumber();
        break;
    case CHAR:
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
    case STRING:
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
    case SYMBOL:
        if (addSymbol())
            nextToken = checkSymbol();
        else
            nextToken = 0;
        break;
    case UNKNOWN:
        addChar();
        getChar();
        nextToken = 0;
        break;
    case __EOF__:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = '\0';
        break;
    }
    printf("<%6d, %-9s >\n", nextToken, lexeme);
}

int checkNumber()
{
    int i = 0;
    int dotCount = 0;
    for (i = 0; i < lexLen; i++)
    {
        if (lexeme[i] == '.')
            dotCount++;
    }
    if (dotCount == 0)
        return INT;
    else if (dotCount == 1)
        return FLOAT;
    else
        return UNKNOWN;
}

int checkChar()
{

    if (lexeme[0] == '\'' && lexeme[lexLen - 1] == '\'')
    {
        if (lexLen == 3)
            return STRING;
        else if (lexLen == 4 && lexeme[1] == '\\')
            return STRING;
    }
    return UNKNOWN;
}

int checkString()
{
    if (lexeme[0] == '\"' && lexeme[lexLen - 1] == '\"')
        return STRING;
    return UNKNOWN;
}

int addSymbol()
{

    switch (nextChar)
    {
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
    case '.':
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
        return 0;
        break;
    }
    return 1;
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
        return nextChar;
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
    if (strcmp(lexeme, "true") == 0 || strcmp(lexeme, "false") == 0)
        return BOOL;
    while (keywords[i] != 0)
    {
        if (strcmp(lexeme, keywords[i]) == 0)
            return offset + i;
        i++;
    }
    return ID;
}
