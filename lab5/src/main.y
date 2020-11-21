%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL 

%token LOP_MINUS LOP_PLUS LOP_NOT
%token LOP_MUL LOP_DIV LOP_MOD

%token LOP_ASSIGN 

%token SEMICOLON

%token IDENTIFIER INTEGER CHAR BOOL STRING


%right LOP_MINUS LOP_PLUS LOP_NOT
%left LOP_MUL LOP_DIV LOP_MOD
%left LOP_EQ

%%

program
: statements {root = new TreeNode(0, NODE_PROG); root->addChild($1);};

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
;

statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declaration SEMICOLON {$$ = $1;}
;

declaration
: T IDENTIFIER LOP_ASSIGN expr{  // declare and init
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    node->addChild($4);
    $$ = node;   
} 
| T IDENTIFIER {
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_DECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;   
}
;

expr
: MulExp{$$=$1;};

PrimaryExp
: IDENTIFIER {
    $$ = $1;
}
| INTEGER {
    $$ = $1;
}
| CHAR {
    $$ = $1;
}
| STRING {
    $$ = $1;
}
;

UnaryExp
: PrimaryExp{
    $$ = $1;
}
| UnaryOp UnaryExp{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = $1 -> optype;
    node->addChild($2);
    $$ = node;
}
;

MulExp
: UnaryExp{
    $$ = $1;
}
| MulExp MulOp UnaryExp{
    TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = $2 -> optype;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;} 
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
| T_STRING {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_STRING;}
;

UnaryOp: LOP_PLUS {$$ = new TreeNode(lineno, NODE_OP); $$->optype=OP_PLUS;}
| LOP_MINUS {$$ = new TreeNode(lineno, NODE_OP); $$->optype=OP_MINUS;}
| LOP_NOT {$$ = new TreeNode(lineno, NODE_OP); $$->optype=OP_NOT;}
;

MulOp: LOP_MUL {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_MUL;}
| LOP_DIV {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_DIV;}
| LOP_MOD {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_MOD;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}