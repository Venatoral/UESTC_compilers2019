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
    BYTE,
    SHORT,
    INT,
    LONG,
    FLOAT,
    DOUBLE,
    CHAR,
    STRING,
    BOOL,
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
};
*/
%}

comment     \/\/.*|\/\*(.|\n)*\*\/
space       [\ \t\n\f\r]
long        [0-9]+[lL]
int         [0-9]+
float       [0-9]+\.[0-9]*[fF]|\.[0-9]+[fF]
double      [0-9]+\.[0-9]*|\.[0-9]+
char        \'([^\\\n\']|\\[\\\'\"btnfr0])\'
string      \"([^\\\n\"]|\\[\\\'\"btnfr0])*\"
bool        true|false
multiop     \=\=|\!\=|\+\+|\+\=|\-\-|\-\=|\*\=|\/\=|\%\=|\&\&|\&\=|\|\||\|\=|\^\=|\<\<|\<\=|\<\<\=|\>\>|\>\>\>|\>\=|\>\>\=|\>\>\>\=
symbol      [\(\)\[\]\{\}\<\>\=\+\-\*\/\%\&\|\^\~\,\.\:\;\!\?\@]
keywords    private|protected|public|abstract|class|extends|final|implements|interface|native|new|static|strictfp|synchronized|transient|volatile|break|case|continue|default|do|else|for|if|instanceof|return|switch|while|assert|catch|finally|throw|throws|try|import|package|boolean|byte|char|double|float|int|long|short|super|this|void|goto|const|null
id          [a-zA-Z$_][a-zA-Z$_0-9]*
unknown     .|[\\\'\"\`\#]

%%

{comment}       {                                                    return        0;}
{space}         {                                                    return        0;}
<<EOF>>         {fprintf(yyout, "<%6d, %-9s >\n",  -1,       "EOF"); return       -1;}
{long}          {fprintf(yyout, "<%6d, %-9s >\n", 264,      yytext); return      264;}
{int}           {fprintf(yyout, "<%6d, %-9s >\n", 263,      yytext); return      263;}
{float}         {fprintf(yyout, "<%6d, %-9s >\n", 265,      yytext); return      265;}
{double}        {fprintf(yyout, "<%6d, %-9s >\n", 266,      yytext); return      266;}
{char}          {fprintf(yyout, "<%6d, %-9s >\n", 267,      yytext); return      267;}
{string}        {fprintf(yyout, "<%6d, %-9s >\n", 268,      yytext); return      268;}
{bool}          {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return assign();}
{multiop}       {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return assign();}
{symbol}        {fprintf(yyout, "<%6d, %-9s >\n", *yytext,  yytext); return  *yytext;}
{keywords}      {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return assign();}
{id}            {fprintf(yyout, "<%6d, %-9s >\n", 260,      yytext); return      260;}
{unknown}       {fprintf(yyout, "<%6d, %-9s >\n",   0,      yytext); return        0;}

%%

int main(int argc, char const *argv[]) {
    int yyres = 1;
    if (argc >= 2) {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL) {
            printf("Can't open file %s\n", argv[1]);
            return 1;
        }
    }
    if (argc >= 3) {
        yyout = fopen(argv[2], "w");
        if (yyout == NULL) {
            printf("Can't open file %s\n", argv[2]);
            return 1;
        }
    }
    while (yyres != EOF) {
        yyres = yylex();
    }
    return 0;
}

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
