#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern int yyleng;
extern char *yytext;
extern FILE *yyin;
extern FILE *yyout;

typedef struct _ast ast;
typedef struct _ast *past;
struct _ast
{
    char *nodeType;
    char *ivalue;
    past left;
    past right;
};

enum TokenType
{
    //EOF = -1 or 4294967295
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
};
int tokenType;

/*
//strdup in string.h
char *strdup(const char *str)
{
    if (str == NULL)
        return NULL;
    size_t len = strlen(str) + 1;
    char *new = malloc(len);
    if (new == NULL)
        return NULL;
    return (char *)memcpy(new, str, len);
}
*/

past newAstNode(char *nodeType, char *ivalue, past left, past right);
void showAst(past node, int nest);
void freeAst(past node);
int tokenType_is_ASSIGN();
int tokenType_is_CMP();

void advance();
past program();
past external_declaration();
past decl_or_stmt();
past declarator_list();
past intstr_list();
past initializer();
past declarator();
past parameter_list();
past parameter();
past type();
past statement();
past statement_list();
past expression_statement();
past expr();
past cmp_expr();
past add_expr();
past mul_expr();
past primary_expr();
past expr_list();
past id_list();

int main(int argc, char **argv)
{
    past root = NULL;
    char *name = "stdio";
    if (argc >= 2)
    {
        name = argv[1];
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

    advance(); //first advance
    root = newAstNode(name, "", program(), NULL);
    showAst(root, 0);
    freeAst(root);
    return 0;
}

past newAstNode(char *nodeType, char *ivalue, past left, past right)
{
    past node = (past)malloc(sizeof(ast));
    if (node == NULL)
    {
        printf("run out of memory.\n");
        exit(0);
    }
    node->nodeType = nodeType;
    node->ivalue = ivalue;
    node->left = left;
    node->right = right;
    return node;
}

void showAst(past node, int level)
{
    int i = 0;
    if (node == NULL)
        return;

    if (strcmp(node->nodeType, "list") != 0)
    {
        for (i = 0; i < level; i++)
            printf("  ");
        printf("%s: %s\n", node->nodeType, node->ivalue);

        showAst(node->left, level + 1);
        showAst(node->right, level + 1);
    }
    else
    {
        showAst(node->left, level);
        showAst(node->right, level);
    }
}

void freeAst(past node)
{
    if (node == NULL)
    {
        return;
    }
    freeAst(node->left);
    freeAst(node->right);
    free(node->ivalue);
    free(node);
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
    /*
    if (tokenType == EOF)
        printf("Encounter EOF\n");
    else if (tokenType == UNKNOWN)
        printf("unknown:  %s\n", yytext);
    else
        printf("token: %s\n", yytext);
    */
}

//  program
//      : external_declaration
//      | program external_declaration
//      ;
past program()
{
    past root = NULL;
    if (root = external_declaration())
    {
        root = newAstNode("list", "", root, program());
    }
    return root;
}

//  external_declaration
//      : type declarator decl_or_stmt
//      ;
past external_declaration()
{
    past root = NULL;
    if (root = type())
    {
        root = newAstNode("DeclStmt", "",
                          root, newAstNode("list", "", NULL, NULL));
        if (root->right->left = declarator())
        {
            if (root->right->right = decl_or_stmt())
            {
                ;
            }
        }
    }
    return root;
}

//  decl_or_stmt
//      : '{' statement_list '}'
//      | '{' '}'
//      | ',' declarator_list ';'
//      | ';'
//      ;
past decl_or_stmt()
{
    past root = NULL;
    if (tokenType == '{')
    {
        advance();
        if (root = statement_list())
        {
            root = newAstNode("CompoundStmt", "", root, NULL);
        }
        if (tokenType == '}')
        {
            advance();
        }
    }
    else if (tokenType == ',')
    {
        advance();
        if (root = declarator_list())
        {
            if (tokenType == ';')
            {
                advance();
            }
        }
    }
    else if (tokenType == ';')
    {
        advance();
    }
    return root;
}

//  declarator_list
//      : declarator
//      | declarator_list ',' declarator
//      ;
past declarator_list()
{
    past root = NULL;
    if (root = declarator())
    {
        root = newAstNode("list", "", root, NULL);
        if (tokenType == ',')
        {
            advance();
            root->right = declarator_list();
        }
    }
    return root;
}

//  intstr_list
//      : initializer
//      | intstr_list ',' initializer
//      ;
past intstr_list()
{
    past root = NULL;
    if (root = initializer())
    {
        root = newAstNode("list", "", root, NULL);
        if (tokenType == ',')
        {
            advance();
            root->right = intstr_list();
        }
    }
    return root;
}

//  initializer
//      : NUMBER
//      | STRING
//      ;
past initializer()
{
    past root = NULL;
    if (tokenType == NUMBER)
    {
        root = newAstNode("IntegerLiteral", strdup(yytext), NULL, NULL);
        advance();
    }
    else if (tokenType == STRING)
    {
        root = newAstNode("StringLiteral", strdup(yytext), NULL, NULL);
        advance();
    }
    return root;
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
past declarator()
{
    past root = NULL;
    if (tokenType == ID)
    {
        root = newAstNode("Varible", strdup(yytext), NULL, NULL);
        advance();
        if (tokenType == '=')
        {
            advance();
            root->right = expr();
        }
        else if (tokenType == '(')
        {
            root->nodeType = "Function";
            advance();
            root->left = parameter_list();
            if (tokenType == ')')
            {
                advance();
            }
        }
        else if (tokenType == '[')
        {
            root->nodeType = "VaribleList";
            advance();
            root->left = expr();
            if (tokenType == ']')
            {
                advance();
                if (tokenType == '=')
                {
                    advance();
                    if (tokenType == '{')
                    {
                        advance();
                        if (root->right = intstr_list())
                        {
                            root->right = newAstNode("InitListExpr", "", root->right, NULL);
                            if (tokenType == '}')
                            {
                                advance();
                            }
                        }
                    }
                }
            }
        }
    }
    return root;
}

//  parameter_list
//      : parameter
//      | parameter_list ',' parameter
//      ;
past parameter_list()
{
    past root = NULL;
    if (root = parameter())
    {
        root = newAstNode("list", NULL, root, NULL);
        if (tokenType == ',')
        {
            advance();
            root->right = parameter_list();
        }
    }
    return root;
}

//  parameter
//      : type ID
//      ;
past parameter()
{
    past root = NULL;
    if (root = type())
    {
        if (tokenType == ID)
        {
            root = newAstNode("parameter", "",
                              root, newAstNode("Varible", strdup(yytext), NULL, NULL));
            advance();
        }
    }
    return root;
}

//  type
//      : INT
//      | STR
//      | VOID
//      ;
past type()
{
    past root = NULL;
    if (tokenType == INT || tokenType == STR || tokenType == VOID)
    {
        root = newAstNode("type", strdup(yytext), NULL, NULL);
        advance();
    }
    return root;
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
//      | PRINT ';'
//      | PRINT expr_list ';'
//      | SCAN id_list ';'
//      ;
past statement()
{
    past root = NULL;
    if (root = type())
    {
        root = newAstNode("DeclStmt", "", root, NULL);
        if (root->right = declarator_list())
        {
            if (tokenType == ';')
            {
                advance();
            }
        }
    }
    else if (tokenType == '{')
    {
        advance();
        if (root = statement_list())
        {
            root = newAstNode("CompoundStmt", "", root, NULL);
            if (tokenType == '}')
            {
                advance();
            }
        }
    }
    else if (root = expression_statement())
    {
        ;
    }
    else if (tokenType == IF)
    {
        root = newAstNode("IfStmt", "",
                          NULL, newAstNode("list", "", NULL, NULL));
        advance();
        if (tokenType == '(')
        {
            advance();
            if (root->left = expr())
            {
                if (tokenType == ')')
                {
                    advance();
                    if (root->right->left = statement())
                    {
                        if (tokenType == ELSE)
                        {
                            advance();
                            if (root->right->right = statement())
                                ;
                        }
                    }
                }
            }
        }
    }
    else if (tokenType == WHILE)
    {
        root = newAstNode("WhileStmt", "", NULL, NULL);
        advance();
        if (tokenType == '(')
        {
            advance();
            if (root->left = expr())
            {
                if (tokenType == ')')
                {
                    advance();
                    if (root->right = statement())
                        ;
                }
            }
        }
    }
    else if (tokenType == RETURN)
    {
        root = newAstNode("ReturnStmt", "", NULL, NULL);
        advance();
        root->left = expr();
        if (tokenType == ';')
        {
            advance();
        }
    }
    else if (tokenType == PRINT)
    {
        root = newAstNode("PrintStmt", "", NULL, NULL);
        advance();
        root->left = expr_list();
        if (tokenType == ';')
        {
            advance();
        }
    }
    else if (tokenType == SCAN)
    {
        root = newAstNode("ScanStmt", "", NULL, NULL);
        advance();
        if (root->left = id_list())
        {
            if (tokenType == ';')
            {
                advance();
            }
        }
    }
    return root;
}

// statement_list
//     : statement
//     | statement_list statement
//     ;
past statement_list()
{
    past root = NULL;
    if (root = statement())
    {
        root = newAstNode("list", NULL, root, statement_list());
    }
    return root;
}

//  expression_statement
//      : ';'
//      | expr ';'
//      ;
past expression_statement()
{
    past root = NULL;
    if (tokenType == ';')
    {
        advance();
    }
    else if (root = expr())
    {
        if (tokenType == ';')
        {
            advance();
        }
    }
    return root;
}

//  expr
//      : cmp_expr
//      ;
past expr()
{
    return cmp_expr();
}

//  cmp_expr
//      : add_expr
//      | cmp_expr CMP add_expr
//      ;
past cmp_expr()
{
    past root = NULL;
    root = add_expr();
    if (root)
    {
        while (tokenType_is_CMP())
        {
            root = newAstNode("Operator", strdup(yytext), root, NULL);
            advance();
            root->right = add_expr();
        }
    }
    return root;
}

//  add_expr
//      : mul_expr
//      | add_expr '+' mul_expr
//      | add_expr '-' mul_expr
//      ;
past add_expr()
{
    past root = NULL;
    root = mul_expr();
    if (root)
    {
        while (tokenType == '+' || tokenType == '-')
        {
            root = newAstNode("Operator", strdup(yytext), root, NULL);
            advance();
            root->right = mul_expr();
        }
    }
    return root;
}

//  mul_expr
//      : primary_expr
//      | mul_expr '*' primary_expr
//      | mul_expr '/' primary_expr
//      | mul_expr '%' primary_expr
//      | '-' primary_expr
//      ;
past mul_expr()
{
    past root = NULL;
    root = primary_expr();
    if (root)
    {
        while (tokenType == '*' || tokenType == '/' || tokenType == '%')
        {
            root = newAstNode("Operator", strdup(yytext), root, NULL);
            advance();
            root->right = primary_expr();
        }
    }
    else if (tokenType == '-')
    {
        root = newAstNode("Operator", strdup(yytext), NULL, NULL);
        advance();
        root->right = primary_expr();
    }
    return root;
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
past primary_expr()
{
    past root = NULL;
    if (tokenType == ID)
    {
        root = newAstNode("Varible", strdup(yytext), NULL, NULL);
        advance();
        if (tokenType == '(')
        {
            root->nodeType = "Function";
            advance();
            root->left = expr_list();
            if (tokenType == ')')
            {
                advance();
            }
        }
        else if (tokenType == '[')
        {
            advance();
            if (root->left = expr())
            {
                root->nodeType = "VaribleList";
                if (tokenType == ']')
                {
                    advance();
                    if (tokenType == '=')
                    {
                        root = newAstNode("Operator", strdup(yytext), root, NULL);
                        advance();
                        root->right = expr();
                    }
                }
            }
        }
        else if (tokenType == '=' || tokenType_is_ASSIGN())
        {
            root = newAstNode("Operator", strdup(yytext), root, NULL);
            advance();
            root->right = expr();
        }
    }
    else if (tokenType == '(')
    {
        advance();
        if (root = expr())
        {
            if (tokenType == ')')
            {
                advance();
            }
        }
    }
    else if (tokenType == NUMBER)
    {
        root = newAstNode("IntegerLiteral", strdup(yytext), NULL, NULL);
        advance();
    }
    else if (tokenType == STRING)
    {
        root = newAstNode("StringLiteral", strdup(yytext), NULL, NULL);
        advance();
    }
    return root;
}

//  expr_list
//      : expr
//      | expr_list ',' expr
//      ;
past expr_list()
{
    past root = NULL;
    if (root = expr())
    {
        root = newAstNode("list", NULL, root, NULL);
        if (tokenType == ',')
        {
            advance();
            root->right = expr_list();
        }
    }
    return root;
}

//  id_list
//      : ID
//      | id_list ',' ID
//      ;
past id_list()
{
    past root = NULL;
    if (tokenType == ID)
    {
        root = newAstNode("list", NULL,
                          newAstNode("Varible", strdup(yytext), NULL, NULL), NULL);
        advance();
        if (tokenType == ',')
        {
            advance();
            root->right = id_list();
        }
    }
    return root;
}
