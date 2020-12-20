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

HEX_INTEGER 0(x|X)[0-9a-fA-F]+
DEC_INTEGER 0|[1-9][0-9]*
OCT_INTEGER 0[0-7]+


IDENTIFIER [[:alpha:]_][[:alpha:][:digit:]_]*

CHAR \'.?\'
STRING \".+\"
%%

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
"const" return KEY_CONST;
"struct" return KEY_STRUCT;
"private" return KEY_PRIVATE;
"public" return KEY_PUBLIC;

"." return LOP_POINT;

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
"[" return LOP_LBRKET;
"]" return LOP_RBRKET;
"{" return LOP_LBRACE;
"}" return LOP_RBRACE;
";" return SEMICOLON;

{HEX_INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;

    int decval=0;
    // transform hexa int to dec int to store
    int len = strlen(yytext), flag=0;
    for(int i=2;i<len;i++){
        // ignore 0 ahead
        if(yytext[i]!='0') flag=1;
        if(flag==0&&yytext[i]=='0') continue;

        decval*=16;

        if(yytext[i]=='A'||yytext[i]=='a') decval+=10;
        else if(yytext[i]=='B'||yytext[i]=='b') decval+=11;
        else if(yytext[i]=='C'||yytext[i]=='c') decval+=12;
        else if(yytext[i]=='D'||yytext[i]=='d') decval+=13;
        else if(yytext[i]=='E'||yytext[i]=='e') decval+=14;
        else if(yytext[i]=='F'||yytext[i]=='f') decval+=15;
        else decval+=(yytext[i]-'0');
    }
    node->int_val = decval;

    yylval = node;
    return HEX_INTEGER;
}

{DEC_INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;
    node->int_val = atoi(yytext);
    yylval = node;
    return DEC_INTEGER;
}

{OCT_INTEGER} {
    TreeNode* node = new TreeNode(lineno, NODE_CONST);
    node->type = TYPE_INT;

    int decval=0;
    // transform oct int to dec int to store
    int len = strlen(yytext), flag=0;
    for(int i=1;i<len;i++){
        // ignore 0 ahead
        if(yytext[i]!='0') flag=1;
        if(flag==0&&yytext[i]=='0') continue;

        decval*=8;
        decval+=(yytext[i]-'0');
    }
    node->int_val = decval;

    yylval = node;
    return OCT_INTEGER;
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
    node->str_val = yytext;
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

{BLOCKCOMMENT} {
    int len=strlen(yytext);
    for(int i=0;i<len;i++){
        if(yytext[i]=='\n'||yytext[i]=='\r') lineno++;
    }
}
{LINECOMMENT}  /*do nothing*/

{WHILTESPACE} /* do nothing */

{EOL} lineno++;

. {
    cerr << "[line "<< lineno <<" ] unknown character:" << yytext << endl;
}
%%