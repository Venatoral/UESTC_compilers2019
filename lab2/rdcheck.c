#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyleng;
extern char *yytext;
extern FILE *yyin;
extern FILE *yyout;

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

int tokenType_is_ASSIGN();
int tokenType_is_CMP();

void advance();
int program();
int external_declaration();
int decl_or_stmt();
int declarator_list();
int intstr_list();
int initializer();
int declarator();
int parameter_list();
int parameter();
int type();
int statement();
int statement_list();
int expression_statement();
int expr();
int cmp_expr();
int add_expr();
int mul_expr();
int primary_expr();
int expr_list();
int id_list();

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        yyin = fopen(argv[1], "r");
        if (yyin == NULL)
        {
            printf("Can't open file %s\n", argv[1]);
            return 1;
        }
    }
    if (argc >= 3)
    {
        yyout = fopen(argv[2], "w");
        if (yyout == NULL)
        {
            printf("Can't open file %s\n", argv[2]);
            return 1;
        }
    }
    
    if(program())
    printf("Successfully match all content.\n");
    return 0;
}

int tokenType_is_ASSIGN()
{
    int is_ASSIGN = 0;
    switch (tokenType)
    {
    case 61:  // =
    case 281: // +=
    case 283: // -=
    case 284: // *=
    case 285: // /=
    case 286: // %=
        is_ASSIGN = 1;
        break;
    default:
        break;
    }
    return is_ASSIGN;
}
int tokenType_is_CMP()
{

    int is_CMP = 0;
    switch (tokenType)
    {
    case 278: // ==
    case 279: // !=
    case 60:  // <
    case 293: // <=
    case 62:  // >
    case 297: // >=
        is_CMP = 1;
        break;
    default:
        break;
    }
    return is_CMP;
}

//scan one token
void advance()
{
    tokenType = yylex();
    if (tokenType == EOF)
        printf("Encounter EOF\n");
    else if (tokenType == UNKNOWN)
        printf("unknown:  %s\n", yytext);
    else
        printf("token: %s\n", yytext);
}

//  program
//      : external_declaration
//      | program external_declaration
//      ;
int program()
{
    int isMatch = 0;
    advance(); //first advance
label:
    if (external_declaration())
    {
        isMatch = 1;
        goto label;
    }
    return isMatch;
}

//  external_declaration
//      : type declarator decl_or_stmt
//      ;
int external_declaration()
{
    int isMatch = 0;
    if (type())
    {
        if (declarator())
        {
            if (decl_or_stmt())
                isMatch = 1;
        }
    }
    return isMatch;
}

//  decl_or_stmt
//      : '{' statement_list '}'
//      | '{' '}'
//      | ',' declarator_list ';'
//      | ';'
//      ;
int decl_or_stmt()
{
    int isMatch = 0;
    if (tokenType == '{')
    {
        advance();
        statement_list();
        if (tokenType == '}')
        {
            advance();
            isMatch = 1;
        }
    }
    else if (tokenType == ',')
    {
        advance();
        if (declarator_list())
        {
            if (tokenType == ';')
            {
                advance();
                isMatch = 1;
            }
        }
    }
    else if (tokenType == ';')
    {
        advance();
        isMatch = 1;
    }
    return isMatch;
}

//  declarator_list
//      : declarator
//      | declarator_list ',' declarator
//      ;
int declarator_list()
{
    int isMatch = 0;
label:
    if (declarator())
    {
        if (tokenType == ',')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  intstr_list
//      : initializer
//      | intstr_list ',' initializer
//      ;
int intstr_list()
{
    int isMatch = 0;
label:
    if (initializer())
    {
        if (tokenType == ',')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  initializer
//      : NUMBER
//      | STRING
//      ;
int initializer()
{
    int isMatch = 0;
    if (tokenType == NUMBER)
    {
        advance();
        isMatch = 1;
    }
    else if (tokenType == STRING)
    {
        advance();
        isMatch = 1;
    }
    return isMatch;
}

//  declarator
//      : ID
//      | ID '=' expr
//      | ID '(' parameter_list ')'
//      | ID '(' ')'
//      | ID '[' expr ']'
//      | ID '[' ']'
//      | ID '[' expr ']' '=' '{' intstr_list '}'
//      | ID '[' ']' '=' '{' intstr_list '}'
//      ;
int declarator()
{
    int isMatch = 0;
    if (tokenType == ID)
    {
        advance();
        if (tokenType == '=')
        {
            advance();
            if (expr())
                isMatch = 1;
        }
        else if (tokenType == '(')
        {
            advance();
            parameter_list();
            if (tokenType == ')')
            {
                isMatch = 1;
                advance();
            }
        }
        else if (tokenType == '[')
        {
            advance();
            expr();
            if (tokenType == ']')
            {
                advance();
                if (tokenType == '=')
                {
                    advance();
                    if (tokenType == '{')
                    {
                        advance();
                        if (intstr_list())
                        {
                            if (tokenType == '}')
                            {
                                isMatch = 1;
                                advance();
                            }
                        }
                    }
                }
                else
                    isMatch = 1;
            }
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  parameter_list
//      : parameter
//      | parameter_list ',' parameter
//      ;
int parameter_list()
{
    int isMatch = 0;
label:
    if (parameter())
    {
        if (tokenType == ',')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  parameter
//      : type ID
//      ;
int parameter()
{
    int isMatch = 0;
    if (type())
    {
        if (tokenType == ID)
        {
            isMatch = 1;
            advance();
        }
    }
    return isMatch;
}

//  type
//      : INT
//      | STR
//      | VOID
//      ;
int type()
{
    int isMatch = 0;
    if (tokenType == INT)
    {
        isMatch = 1;
        advance();
    }
    else if (tokenType == STR)
    {
        isMatch = 1;
        advance();
    }
    else if (tokenType == VOID)
    {
        isMatch = 1;
        advance();
    }
    return isMatch;
}

//  statement
//      : type declarator_list ';'
//      | '{' statement_list '}'
//      | expr_statement
//      | IF '(' expr ')' statement
//      | IF '(' expr ')' statement ELSE statement
//      | WHILE '(' expr ')' statement
//      | RETURN ';'
//      | RETURN expr ';'
//      | PRINT ';
//      | PRINT expr_list ';'
//      | SCAN id_list ';'
//      ;
int statement()
{
    int isMatch = 0;
    if (type())
    {
        if (declarator_list())
        {
            if (tokenType == ';')
                isMatch = 1;
            advance();
        }
    }
    else if (tokenType == '{')
    {
        advance();
        if (statement_list())
        {
            if (tokenType == '}')
            {
                isMatch = 1;
                advance();
            }
        }
    }
    else if (expression_statement())
    {
        isMatch = 1;
    }
    else if (tokenType == IF)
    {
        advance();
        if (tokenType == '(')
        {
            advance();
            if (expr())
            {
                if (tokenType == ')')
                {
                    advance();
                    if (statement())
                    {
                        if (tokenType == ELSE)
                        {
                            advance();
                            if (statement())
                                isMatch = 1;
                        }
                        else
                            isMatch = 1;
                    }
                }
            }
        }
    }
    else if (tokenType == WHILE)
    {
        advance();
        if (tokenType == '(')
        {
            advance();
            if (expr())
            {
                if (tokenType == ')')
                {
                    advance();
                    if (statement)
                        isMatch = 1;
                }
            }
        }
    }
    else if (tokenType == RETURN)
    {
        advance();
        expr();
        if (tokenType == ';')
        {
            isMatch = 1;
            advance();
        }
    }
    else if (tokenType == PRINT)
    {
        advance();
        expr_list();
        if (tokenType == ';')
        {
            isMatch = 1;
            advance();
        }
    }
    else if (tokenType == SCAN)
    {
        advance();
        if (id_list())
        {
            if (tokenType == ';')
            {
                isMatch = 1;
                advance();
            }
        }
    }
    return isMatch;
}

// statement_list
//     : statement
//     | statement_list statement
//     ;
int statement_list()
{
    int isMatch = 0;
label:
    if (statement())
    {
        isMatch = 1;
        goto label;
    }
    return isMatch;
}

//  expression_statement
//      : ';'
//      | expr ';'
//      ;
int expression_statement()
{
    int isMatch = 0;
    if (tokenType == ';')
    {
        isMatch = 1;
        advance();
    }
    else if (expr())
    {
        if (tokenType == ';')
        {
            isMatch = 1;
            advance();
        }
    }
    return isMatch;
}

//  expr
//      : cmp_expr
//      ;
int expr()
{
    int isMatch = 0;
    if (cmp_expr())
        isMatch = 1;
    return isMatch;
}

//  cmp_expr
//      : add_expr
//      | cmp_expr CMP add_expr
//      ;
int cmp_expr()
{
    int isMatch = 0;
label:
    if (add_expr())
    {
        if (tokenType_is_CMP())
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  add_expr
//      : mul_expr
//      | add_expr '+' mul_expr
//      | add_expr '-' mul_expr
//      ;
int add_expr()
{
    int isMatch = 0;
label:
    if (mul_expr())
    {
        if (tokenType == '+')
        {
            advance();
            goto label;
        }
        else if (tokenType == '-')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  mul_expr
//      : primary_expr
//      | mul_expr '*' primary_expr
//      | mul_expr '/' primary_expr
//      | mul_expr '%' primary_expr
//      | '-' primary_expr
//      ;
int mul_expr()
{
    int isMatch = 0;
label:
    if (primary_expr())
    {
        if (tokenType == '*')
        {
            advance();
            goto label;
        }
        else if (tokenType == '/')
        {
            advance();
            goto label;
        }
        else if (tokenType == '%')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    else if (tokenType == '-')
    {
        advance();
        if (primary_expr())
            isMatch = 1;
    }
    return isMatch;
}

//  primary_expr
//      | ID
//      | ID '(' ')'
//      : ID '(' expr_list ')'
//      | ID '[' expr ']'
//      | ID '[' expr ']' '=' expr
//      | ID '=' expr
//      | ID ASSIGN expr
//      | '(' expr ')'
//      | NUMBER
//      | STRING
//      ;
int primary_expr()
{
    int isMatch = 0;
    if (tokenType == ID)
    {
        advance();
        if (tokenType == '(')
        {
            advance();
            expr_list();
            if (tokenType == ')')
            {
                isMatch = 1;
                advance();
            }
        }
        else if (tokenType == '[')
        {
            advance();
            if (expr())
            {
                if (tokenType == ']')
                {
                    advance();
                    if (tokenType == '=')
                    {
                        advance();
                        if (expr())
                            isMatch = 1;
                    }
                    else
                        isMatch = 1;
                }
            }
        }
        else if (tokenType == '=')
        {
            advance();
            if (expr())
                isMatch = 1;
        }
        else if (tokenType_is_ASSIGN())
        {
            advance();
            if (expr())
                isMatch = 1;
        }
        else
            isMatch = 1;
    }
    else if (tokenType == '(')
    {
        advance();
        if (expr())
        {
            if (tokenType == ')')
            {
                isMatch = 1;
                advance();
            }
        }
    }
    else if (tokenType == NUMBER)
    {
        isMatch = 1;
        advance();
    }
    else if (tokenType == STRING)
    {
        isMatch = 1;
        advance();
    }
    return isMatch;
}

//  expr_list
//      : expr
//      | expr_list ',' expr
//      ;
int expr_list()
{
    int isMatch = 0;
label:
    if (expr())
    {
        if (tokenType == ',')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}

//  id_list
//      : ID
//      | id_list ',' ID
//      ;
int id_list()
{
    int isMatch = 0;
label:
    if (tokenType == ID)
    {
        advance();
        if (tokenType == ',')
        {
            advance();
            goto label;
        }
        else
            isMatch = 1;
    }
    return isMatch;
}
