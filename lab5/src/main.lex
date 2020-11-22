%option nounput
%{
#include "common.h"
#include "main.tab.h"  // yacc header
int lineno=1;
%}
BLOCKCOMMENT \/\*([^\*^\/]*|[\*^\/*]*|[^\**\/]*)*\*\/
LINECOMMENT \/\/[^\n]*
EOL	(\r\n|\r|\n)
WHILTESPACE [[:blank:]]

INTEGER [0-9]+

IDENTIFIER [[:alpha:]_][[:alpha:][:digit:]_]*

CHAR \'.?\'
STRING \".+\"
%%

{BLOCKCOMMENT} {
    int len=strlen(yytext);
    for(int i=0;i<len;i++){
        if(yytext[i]=='\n'||yytext[i]=='\r') lineno++;
    }
}
{LINECOMMENT}  /*do nothing*/

"int" return T_INT;
"bool" return T_BOOL;
"char" return T_CHAR;
"string" return T_STRING;
"void" return T_VOID;

"if" return KEY_IF;
"else" return KEY_ELSE;
"while" return KEY_WHILE;
"for" return KEY_FOR;
"break" return KEY_BREAK;
"continue" return KEY_CONTINUE;
"return" return KEY_RETURN;
"scanf" return KEY_SCANF;
"printf" return KEY_PRINTF;

"+" return LOP_PLUS;
"-" return LOP_MINUS;
"!" return LOP_NOT;

"*" return LOP_MUL;
"/" return LOP_DIV;
"%" return LOP_MOD;

"<" return LOP_LESS;
">" return LOP_GREA;
"<=" return LOP_LE;
">=" return LOP_GE;

"==" return LOP_EEQ;
"!=" return LOP_NEQ;

"&&" return LOP_LAND;
"||" return LOP_LOR;

"=" return LOP_ASSIGN;

"+=" return LOP_PLUSEQ;
"-=" return LOP_MINUSEQ;
"%=" return LOP_MODEQ;
"*=" return LOP_MULEQ;
"/=" return LOP_DIVEQ;

"," return LOP_COMMA;

"(" return LOP_LPAREN;
")" return LOP_RPAREN;
"{" return LOP_LBRACE;
"}" return LOP_RBRACE;
";" return SEMICOLON;

{INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = atoi(yytext);
    yylval = node;
    return INTEGER;
}

{IDENTIFIER} {
    TreeNode* node = new TreeNode(lineno, NODE_VAR);
    node->var_name = string(yytext);
    yylval = node;
    return IDENTIFIER;
}

{CHAR} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_CHAR;
    node->int_val = yytext[1];
    yylval = node;
    return CHAR;
}

{STRING} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_STRING;
    node->str_val = yytext;
    yylval = node;
    return STRING;
}

{WHILTESPACE} /* do nothing */

{EOL} lineno++;

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%