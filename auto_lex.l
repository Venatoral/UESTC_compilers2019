%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int assign();

%}

space       [\ \t\n\f\r]
comment     \/\/.*|\/\*(.|\n)*\*\/
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

{comment}       {                                                    return -1;}
{space}         {                                                    return -1;}
<<EOF>>         {fprintf(yyout, "<%6d, %-9s >\n",  -1,       "EOF"); return  0;}
{long}          {fprintf(yyout, "<%6d, %-9s >\n", 264,      yytext); return  1;}
{int}           {fprintf(yyout, "<%6d, %-9s >\n", 263,      yytext); return  1;}
{float}         {fprintf(yyout, "<%6d, %-9s >\n", 265,      yytext); return  1;}
{double}        {fprintf(yyout, "<%6d, %-9s >\n", 266,      yytext); return  1;}
{char}          {fprintf(yyout, "<%6d, %-9s >\n", 267,      yytext); return  1;}
{string}        {fprintf(yyout, "<%6d, %-9s >\n", 268,      yytext); return  1;}
{bool}          {fprintf(yyout, "<%6d, %-9s >\n", 269,      yytext); return  1;}
{multiop}       {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return  1;}
{symbol}        {fprintf(yyout, "<%6d, %-9s >\n", *yytext,  yytext); return  1;}
{keywords}      {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return  1;}
{id}            {fprintf(yyout, "<%6d, %-9s >\n", 260,      yytext); return  1;}
{unknown}       {fprintf(yyout, "<%6d, %-9s >\n",   0,      yytext); return -1;}

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
    while (yyres) {
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
    while ( pwords[i] ) {
        if ( strcmp(yytext, pwords[i]) )
            i++;
        else
            return offset + i;
    }
    return 0;
}
