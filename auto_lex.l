%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int assign();

%}

space       [\ \t\n\f\r]
comment     \/[^\n]*\n|\/\*[^(\*\/)]*\*\/
int         [0-9]+
float       [0-9]+\.[0-9]*|\.[0-9]+
char        \'([^\\\n\']|\\[\\\'\"btnfr0])\'
string      \"([^\\\n\"]|\\[\\\'\"btnfr0])*\"
bool        true|false
multiop     \=\=|\!\=|\+\+|\+\=|\-\-|\-\=|\*\=|\/\=|\%\=|\&\&|\&\=|\|\||\|\=|\^\=|\<\<|\<\=|\<\<\=|\>\>|\>\>\>|\>\=|\>\>\=|\>\>\>\=
symbol      [\\\'\"\(\)\[\]\{\}\<\>\=\+\-\*\/\%\&\|\^\~\,\.\:\;\!\?\`\@\#]
keywords    private|protected|public|abstract|class|extends|final|implements|interface|native|new|static|strictfp|synchronized|transient|volatile|break|case|continue|default|do|else|for|if|instanceof|return|switch|while|assert|catch|finally|throw|throws|try|import|package|boolean|byte|char|double|float|int|long|short|super|this|void|goto|const|null
id          [a-zA-Z$_][a-zA-Z$_0-9]*

%%

{comment}       {                                                    return -2;}
{space}         {                                                    return -1;}
<<EOF>>         {fprintf(yyout, "<%6d, %-9s >\n",  -1,       "EOF"); return  0;}
{int}           {fprintf(yyout, "<%6d, %-9s >\n", 261,      yytext); return  1;}
{float}         {fprintf(yyout, "<%6d, %-9s >\n", 262,      yytext); return  2;}
{char}          {fprintf(yyout, "<%6d, %-9s >\n", 263,      yytext); return  3;}
{string}        {fprintf(yyout, "<%6d, %-9s >\n", 264,      yytext); return  4;}
{bool}          {fprintf(yyout, "<%6d, %-9s >\n", 265,      yytext); return  5;}
{multiop}       {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return  6;}
{symbol}        {fprintf(yyout, "<%6d, %-9s >\n", *yytext,  yytext); return  7;}
{keywords}      {fprintf(yyout, "<%6d, %-9s >\n", assign(), yytext); return  8;}
{id}            {fprintf(yyout, "<%6d, %-9s >\n", 269,      yytext); return  9;}

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
    static int offset = 300-22;
    static char *pwords[] = {
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
        if ( !strcmp(yytext, pwords[i]) ) {
            return offset + i;
        }
        i++;
    }
    return 0;
}
