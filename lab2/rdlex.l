%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int assign();
/*
enum TokenType
{
    //EOF = -1,
    UNKNOWN = 0,
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
    //single operator ascii
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
    //"goto"347, "const"348, "null",349
    NUMBER = 263,
    STRING = 268,
    IF = 323,
    ELSE = 321,
    WHILE = 327,
    RETURN = 325,
    INT = 341,
    STR = 350,
    VOID = 346,
    //ASSIGN -> tokenType_is_ASSIGN
    //CMP -> tokenType_is_CMP
    SCAN = 400,
    PRINT = 401,
} tokenType;
*/
%}

comment     \/\/.*|\/\*(.|\n)*\*\/
space       [\ \t\n\f\r]
clong       [0-9]+[lL]
cint        [0-9]+
cfloat      [0-9]+\.[0-9]*[fF]|\.[0-9]+[fF]
cdouble     [0-9]+\.[0-9]*|\.[0-9]+
cchar       \'([^\\\n\']|\\[\\\'\"btnfr0])\'
cstring     \"([^\\\n\"]|\\[\\\'\"btnfr0])*\"
cbool       true|false
multiop     \=\=|\!\=|\+\+|\+\=|\-\-|\-\=|\*\=|\/\=|\%\=|\&\&|\&\=|\|\||\|\=|\^\=|\<\<|\<\=|\<\<\=|\>\>|\>\>\>|\>\=|\>\>\=|\>\>\>\=
symbol      [\(\)\[\]\{\}\<\>\=\+\-\*\/\%\&\|\^\~\,\.\:\;\!\?\@]
keywords    private|protected|public|abstract|class|extends|final|implements|interface|native|new|static|strictfp|synchronized|transient|volatile|break|case|continue|default|do|else|for|if|instanceof|return|switch|while|assert|catch|finally|throw|throws|try|import|package|boolean|byte|char|double|float|int|long|short|super|this|void|goto|const|null
extrawords  str|scan|print
id          [a-zA-Z$_][a-zA-Z$_0-9]*
unknown     .|[\\\'\"\`\#]

%%

{comment}       {                }
{space}         {                }
<<EOF>>         {return       -1;}
{clong}         {return      264;}
{cint}          {return      263;}
{cfloat}        {return      265;}
{cdouble}       {return      266;}
{cchar}         {return      267;}
{cstring}       {return      268;}
{cbool}         {return assign();}
{multiop}       {return assign();}
{symbol}        {return  *yytext;}
{keywords}      {return assign();}
{extrawords}    {return assign();}
{id}            {return      260;}
{unknown}       {return        0;}

%%

int assign() {
    int offset = 300-22;
    char *pwords[] = {
        "==", "!=", "++", "+=", "--", "-=", "*=", "/=", "%=", "&&", "&=", "||", "|=", "^=", "<<", "<=", "<<=", ">>", ">>>", ">=", ">>=", ">>>=", 
        "private", "protected", "public",
        "abstract", "class", "extends", "final", "implements", "interface", "native", "new", "static", "strictfp", "synchronized", "transient", "volatile", 
        "break", "case", "continue", "default", "do", "else", "for", "if", "instanceof", "return", "switch", "while", 
        "assert", "catch", "finally", "throw", "throws", "try", 
        "import", "package", 
        "boolean", "byte", "char", "double", "float", "int", "long", "short", 
        "super", "this", "void", 
        "goto", "const", "null", 
        NULL
    };
    int i = 0;

    if (strcmp(yytext, "str") == 0)
        return 350;
    if (strcmp(yytext, "scan") == 0)
        return 400;
    if (strcmp(yytext, "print") == 0)
        return 401;

    if (strcmp(yytext, "false") == 0)
        return 270;
    if (strcmp(yytext, "true") == 0)
        return 271;
    while (pwords[i]) {
        if (strcmp(yytext, pwords[i]) == 0)
            return offset + i;
        else
            i++;
    }
    return 0;
}
