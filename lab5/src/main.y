%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL 

%token KEY_IF KEY_ELSE KEY_FOR KEY_WHILE KEY_CONTINUE KEY_BREAK KEY_RETURN

%token LOP_MINUS LOP_PLUS LOP_NOT
%token LOP_MUL LOP_DIV LOP_MOD
%token LOP_LESS LOP_GREA LOP_LE LOP_GE
%token LOP_EEQ LOP_NEQ
%token LOP_LAND
%token LOP_LOR

%token LOP_ASSIGN 
%token LOP_LPAREN LOP_RPAREN LOP_LBRACE LOP_RBRACE
%token SEMICOLON

%token IDENTIFIER INTEGER CHAR BOOL STRING


%right LOP_UMINUS LOP_UPLUS LOP_NOT
%left LOP_MUL LOP_DIV LOP_MOD
%left LOP_LESS LOP_GREA LOP_LE LOP_GE
%left LOP_EEQ LOP_NEQ
%left LOP_LAND
%left LOP_LOP
%left LOP_EQ

%%

program
: statements {root = new TreeNode(0, NODE_PROG); root->addChild($1);}
;

block
: LOP_LBRACE statements LOP_RBRACE{
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_BLOCK;
    node->addChild($2);
    $$ = node;
}
;

statements
:  statement {$$=$1;}
|  statements statement {$$=$1; $$->addSibling($2);}
;

statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| declarestmt SEMICOLON {$$ = $1;}
| block {$$ = $1;}
| ifstmt {$$ = $1;}
| forstmt {$$ = $1;}
| whilestmt {$$ = $1;}
| returnstmt SEMICOLON {$$ = $1;}
| KEY_BREAK SEMICOLON {$$= new TreeNode(lineno, NODE_STMT); $$->stype = STMT_BREAK;}
| KEY_CONTINUE SEMICOLON {$$= new TreeNode(lineno, NODE_STMT); $$->stype = STMT_CONTINUE;}
| assignstmt SEMICOLON {$$ = $1;}
;

forstmt
: KEY_FOR LOP_LPAREN assignstmt SEMICOLON expr SEMICOLON assignstmt LOP_RPAREN statement{
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_FOR;
    node->addChild($3); node->addChild($5); node->addChild($7);
    node->addChild($9);
    $$ = node;
}
;

declarestmt
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

assignstmt
: LValExp LOP_ASSIGN expr{
    TreeNode* node = new TreeNode(lineno, NODE_STMT);
    node->stype = STMT_ASSIGN;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
;

returnstmt
: KEY_RETURN {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RETURN;}
| KEY_RETURN LorExp {
    TreeNode* node = new TreeNode($2->lineno, NODE_STMT);
    node->stype = STMT_RETURN;
    node->addChild($2);
    $$ = node;
}
;

whilestmt
: KEY_WHILE LOP_LPAREN LorExp LOP_RPAREN statement{
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_WHILE;
    node->addChild($3);
    node->addChild($5);
    $$ = node;
}
;

ifstmt
: KEY_IF LOP_LPAREN LorExp LOP_RPAREN statement {
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_IF;
    node->addChild($3);
    node->addChild($5);
    $$ = node;
}
| KEY_IF LOP_LPAREN LorExp LOP_RPAREN statement KEY_ELSE statement {
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_IFELSE;
    node->addChild($3);
    node->addChild($5);
    node->addChild($7);
    $$ = node;
}
;

expr
: LorExp{$$=$1;}
;

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

//left value expression for further process
LValExp
: IDENTIFIER{
    TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LVAL;
    node->addChild($1);
    $$ = node;
}
;

UnaryExp
: PrimaryExp{
    $$ = $1;
}
| LOP_PLUS UnaryExp %prec LOP_UPLUS{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_PLUS;
    node->addChild($2);
    $$ = node;
}
| LOP_MINUS UnaryExp %prec LOP_UMINUS{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_MINUS;
    node->addChild($2);
    $$ = node;
} 
| LOP_NOT UnaryExp{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_NOT;
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

AddExp
: MulExp{
    $$ = $1;
}
| AddExp LOP_PLUS MulExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_PLUS;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
| AddExp LOP_MINUS MulExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_MINUS;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
;

RelExp
: AddExp{
    $$ = $1;
}
| RelExp RelOp AddExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = $2->optype;
    node->addChild($1);
    node->addChild($3);
    $$ = node;    
}
;

EqExp
: RelExp{
    $$ = $1;
}
| EqExp EqOp RelExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = $2->optype;
    node->addChild($1);
    node->addChild($3);
    $$ = node;  
}
;

LandExp
: EqExp{
    $$ = $1;
}
| LandExp LOP_LAND EqExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LAND;
    node->addChild($1);
    node->addChild($3);
    $$ = node;  
}
;

LorExp
: LandExp{
    $$ = $1;
}
| LorExp LOP_LOR LandExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LOR;
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

RelOp: LOP_LESS {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_LESS;}
| LOP_GREA {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_GREA;}
| LOP_LE {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_LE;}
| LOP_GE {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_GE;}
;

EqOp: LOP_EEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_EEQ;}
| LOP_NEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_NEQ;}
;

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}