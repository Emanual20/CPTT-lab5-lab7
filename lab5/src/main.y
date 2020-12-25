%{
    #include "common.h"
    #define YYSTYPE TreeNode *  
    TreeNode* root;
    extern int lineno;
    int yylex();
    int yyerror( char const * );
%}
%token T_CHAR T_INT T_STRING T_BOOL T_VOID

%token KEY_IF KEY_ELSE KEY_FOR KEY_WHILE KEY_CONTINUE KEY_BREAK KEY_RETURN KEY_SCANF KEY_PRINTF KEY_CONST KEY_STRUCT KEY_PUBLIC KEY_PRIVATE

%token LOP_POINT
%token LOP_MINUS LOP_PLUS LOP_NOT LOP_QUOTE
%token LOP_MUL LOP_DIV LOP_MOD
%token LOP_LESS LOP_GREA LOP_LE LOP_GE
%token LOP_EEQ LOP_NEQ
%token LOP_LAND
%token LOP_LOR

%token LOP_ASSIGN LOP_PLUSEQ LOP_MINUSEQ LOP_MODEEQ LOP_MULEQ LOP_DIVEQ 
%token LOP_COMMA LOP_LPAREN LOP_RPAREN LOP_LBRACE LOP_RBRACE
%token SEMICOLON

%token IDENTIFIER HEX_INTEGER DEC_INTEGER OCT_INTEGER CHAR BOOL STRING

%right LOP_LBRKET
%left LOP_POINT LOP_RBRKET
%right LOP_UMINUS LOP_UPLUS LOP_NOT LOP_QUOTE
%left LOP_MUL LOP_DIV LOP_MOD
%left LOP_LESS LOP_GREA LOP_LE LOP_GE
%left LOP_EEQ LOP_NEQ
%left LOP_LAND
%left LOP_LOR
%left LOP_EQ 
%left LOP_PLUSEQ LOP_MINUSEQ LOP_MODEQ LOP_MULEQ LOP_DIVEQ
%left LOP_COMMA

%%

Program: CompUnits{root = new TreeNode(0,NODE_PROG); root->addChild($1);};

CompUnits: CompUnit {$$=$1;}
| CompUnits CompUnit {
    $$=$1; $$->addSibling($2);
}
;

CompUnit
: decl SEMICOLON{$$ = $1;}
| funcdef {$$ = $1;}
;

// for further const decl later
decl
: vardeclstmt{
    $$ = $1;
}
| constdeclstmt{
    $$ = $1;
}
| structdeclstmt{
    $$ = $1;
}
;

constdeclstmt
: KEY_CONST T constdefitem{
    TreeNode* node = new TreeNode($2->lineno, NODE_STMT);
    node->stype = STMT_CONSTDECL;
    node->addChild($2);
    node->addChild($3);
    $$ = node;

    // add type to first declare node
    $$->type = $3->type = $2->type;
}
| constdeclstmt LOP_COMMA constdefitem{
    $1->addChild($3);
    $$ = $1;

    // add type to first declare node
    $3->type = $$->type;
}
;

constdefitem
: IDENTIFIER LOP_ASSIGN constinitval{
    TreeNode* node = new TreeNode($1->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    $1->is_dec = true; 
    node->addChild($1);
    node->addChild($3);
    $$ = node;
} 
;

// for further expand to array
constinitval
: LorExp{
    $$ = $1;
}
;

vardeclstmt
: T declareitem{  // declare and init
    TreeNode* node = new TreeNode($1->lineno, NODE_STMT);
    node->stype = STMT_VARDECL;
    node->addChild($1);
    node->addChild($2);
    $$ = node;

    // add type to first declare node
    $$->type = $2->type = $1->type;
} 
| vardeclstmt LOP_COMMA declareitem {
    $1->addChild($3);
    $$ = $1;

    // add type to first declare node
    $3->type = $$->type;
}
;

declareitem
: IDENTIFIER LOP_ASSIGN expr{
    TreeNode* node = new TreeNode($1->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    node->optype = OP_EQ;
    $1->is_dec=true;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
| IDENTIFIER{
    TreeNode* node = new TreeNode($1->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    $1->is_dec=true;
    node->addChild($1);
    $$ = node;
}
| Arraydeclval{
    TreeNode* node = new TreeNode($1->lineno, NODE_ITEM);
    node->itype = ITEM_DECL;
    $1->is_dec=true;
    node->addChild($1);
    $$ = node;
}
;

assignstmt
: LValExp LOP_ASSIGN LorExp{
    TreeNode* node = new TreeNode(lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_EQ;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
| LValExp AssignEqOp LorExp{
    TreeNode* node = new TreeNode(lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = $2->optype;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}
;

funcdef
: T IDENTIFIER LOP_LPAREN funcparams LOP_RPAREN block {
    TreeNode* node = new TreeNode($1->lineno,NODE_FUNC);
    node -> type = new Type(COMPOSE_FUNCTION); 
    node -> type -> retType = $1 -> type -> type; // retType
    node -> var_name = $2 -> var_name; // funcname
    $2 -> is_dec = true;
    node -> addChild($2); // this line can be ignored further
    node -> addChild($4); // params
    node -> addChild($6);
    $$ = node;
}
| TV IDENTIFIER LOP_LPAREN funcparams LOP_RPAREN block {
    TreeNode* node = new TreeNode($1->lineno,NODE_FUNC);
    node -> type = new Type(COMPOSE_FUNCTION); 
    node -> type -> retType = TYPE_VOID -> type; // retType
    node -> var_name = $2 -> var_name; // funcname
    $2 -> is_dec = true;
    node -> addChild($2); // this line can be ignored further
    node -> addChild($4); // params
    node -> addChild($6);
    $$ = node;
}
;

funcparams
: funcparam {
    $$ = new TreeNode(lineno, NODE_LIST);
    $$ -> addChild($1);
}
| funcparams LOP_COMMA funcparam {
    $$ = $1;
    $$ -> addChild($3);
}
| {$$ = new TreeNode(lineno, NODE_LIST);}
;

//maybe expand of array
funcparam
: T IDENTIFIER {
    TreeNode* node = new TreeNode(lineno, NODE_ITEM);
    node->itype=ITEM_UFUNC;
    node->type=$1->type;
    node->addChild($2);
    $$=node;
}
;

block
: LOP_LBRACE blocklist LOP_RBRACE{
    $$ = $2;
}
;

blocklist
: statement{
    $$ = new TreeNode($1->lineno, NODE_STMT);
    $$->stype = STMT_BLOCK;
    $$->addChild($1);
}
| blocklist statement{
    $$ = $1;
    $$->addChild($2);
}
;


statement
: SEMICOLON  {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_SKIP;}
| block {$$ = $1;}
| ifstmt {$$ = $1;}
| forstmt {$$ = $1;}
| whilestmt {$$ = $1;}
| returnstmt SEMICOLON {$$ = $1;}
| KEY_BREAK SEMICOLON {$$= new TreeNode(lineno, NODE_STMT); $$->stype = STMT_BREAK;}
| KEY_CONTINUE SEMICOLON {$$= new TreeNode(lineno, NODE_STMT); $$->stype = STMT_CONTINUE;}
| assignstmt SEMICOLON {$$ = $1;}
| decl SEMICOLON {$$ = $1;}
| funccall {$$=$1;}
;

funccall
: scanfstmt SEMICOLON {$$=$1;}
| printfstmt SEMICOLON {$$=$1;}
;

scanfstmt
: KEY_SCANF LOP_LPAREN STRING LOP_COMMA sflist LOP_RPAREN{
    TreeNode* node = new TreeNode(lineno, NODE_FUNCALL);
    node->type = TYPE_VOID;
    node->addChild($3);
    node->addChild($5);
    node->var_name="scanf";
    $$ = node;
}
;

printfstmt
: KEY_PRINTF LOP_LPAREN STRING LOP_COMMA pflist LOP_RPAREN{
    TreeNode* node = new TreeNode(lineno, NODE_FUNCALL);
    node->type = TYPE_VOID;
    node->addChild($3);
    node->addChild($5);
    node->var_name="printf";
    $$ = node;
}
| KEY_PRINTF LOP_LPAREN STRING LOP_RPAREN{
    TreeNode* node = new TreeNode(lineno, NODE_FUNCALL);
    node->type = TYPE_VOID;
    node->addChild($3);
    node->var_name="printf";
    $$ = node;
}
;

sflist
: LOP_QUOTE LValExp {
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild($2);
    
    TreeNode* node = new TreeNode(lineno, NODE_LIST);
    node->addChild(expnode);
    $$ = node;
}
| sflist LOP_COMMA LOP_QUOTE LValExp{
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild($4);
    
    $$ = $1;
    $$->addChild(expnode);
}
;

pflist
: expr {
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild($1);
    
    TreeNode* node = new TreeNode(lineno, NODE_LIST);
    node->addChild(expnode);
    $$ = node;
}
| pflist LOP_COMMA expr{
    TreeNode* expnode = new TreeNode(lineno, NODE_ITEM);
    expnode->itype = ITEM_SPF;
    expnode->addChild($3);
    
    $$ = $1;
    $$->addChild(expnode);
}
;

// note: you shall add block node before vardeclstmt, since scope of vars in vardeclstmt shall in the block.
forstmt
: KEY_FOR LOP_LPAREN vardeclstmt SEMICOLON CommaExp SEMICOLON assignstmt LOP_RPAREN statement{
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_FOR;

    // check if $9 is a block, if not, create a block and put $9 into this block.
    if($9->nodeType==NODE_STMT&&$9->stype==STMT_BLOCK){
        node->addChild($9);
    }
    else{
        TreeNode* blocknode = new TreeNode($3->lineno, NODE_STMT);
        blocknode -> stype = STMT_BLOCK;
        blocknode -> addChild($9);
        node -> addChild(blocknode);
    }

    node->addChild($3); node->addChild($5); node->addChild($7);

    $$ = node;
}
| KEY_FOR LOP_LPAREN assignstmt SEMICOLON CommaExp SEMICOLON assignstmt LOP_RPAREN statement{
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_FOR;

    // check if $9 is a block, if not, create a block and put $9 into this block.
    if($9->nodeType==NODE_STMT&&$9->stype==STMT_BLOCK){
        node->addChild($9);
    }
    else{
        TreeNode* blocknode = new TreeNode($3->lineno, NODE_STMT);
        blocknode -> stype = STMT_BLOCK;
        blocknode -> addChild($9);
        node -> addChild(blocknode);
    }

    node->addChild($3); node->addChild($5); node->addChild($7);
    $$ = node;
}
;

returnstmt
: KEY_RETURN {$$ = new TreeNode(lineno, NODE_STMT); $$->stype = STMT_RETURN;}
| KEY_RETURN CommaExp {
    TreeNode* node = new TreeNode($2->lineno, NODE_STMT);
    node->stype = STMT_RETURN;
    node->addChild($2);
    $$ = node;
}
;

whilestmt
: KEY_WHILE LOP_LPAREN CommaExp LOP_RPAREN statement{
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_WHILE;
    node->addChild($3);
    node->addChild($5);
    $$ = node;
}
;

ifstmt
: KEY_IF LOP_LPAREN CommaExp LOP_RPAREN statement {
    TreeNode* node = new TreeNode($3->lineno, NODE_STMT);
    node->stype = STMT_IF;
    node->addChild($3);
    node->addChild($5);
    $$ = node;
}
| KEY_IF LOP_LPAREN CommaExp LOP_RPAREN statement KEY_ELSE statement {
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

Arraydeclval
: IDENTIFIER LOP_LBRKET INTEGER LOP_RBRKET{
    $$ = new TreeNode($1->lineno,NODE_ARRAY);
    $$->addChild($1);
    TreeNode* node = new TreeNode($3->lineno,NODE_ITEM);
    node->addChild($3);
    $$->addChild(node);
    $$->array_dim = 1;
}
| Arraydeclval LOP_LBRKET INTEGER LOP_RBRKET{
    $$ = $1;
    TreeNode* node = new TreeNode($3->lineno,NODE_ITEM);
    node->addChild($3);
    $$->addChild(node);
    $$->array_dim = $$->array_dim + 1;
}
;

Arrayuseval
: IDENTIFIER LOP_LBRKET AddExp LOP_RBRKET{
    $$ = new TreeNode($1->lineno,NODE_ARRAY);
    $$->addChild($1);
    TreeNode* node = new TreeNode($3->lineno,NODE_ITEM);
    node->addChild($3);
    $$->addChild(node);
    $$->array_dim = 1;
}
| Arrayuseval LOP_LBRKET AddExp LOP_RBRKET{
    $$ = $1;
    TreeNode* node = new TreeNode($3->lineno,NODE_ITEM);
    node->addChild($3);
    $$->addChild(node);
    $$->array_dim = $$->array_dim + 1;
}
;

// note: here PrimaryExp has different nodetype
PrimaryExp
: IDENTIFIER {
    $$ = $1;
}
| IDENTIFIER LOP_POINT IDENTIFIER{
    $$ = new TreeNode($1->lineno,NODE_EXPR);
    $$ -> stype = STMT_EXP;
    $$ -> optype = OP_POINT;
    $$ -> addChild($1); $$ -> addChild($3);
    // TODO: miss struct's type expr
}
| Arrayuseval{
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
| LOP_LPAREN LorExp LOP_RPAREN {
    $$ = $2;
}
;

//left value expression for further process
LValExp
: IDENTIFIER {
    TreeNode* node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_LVAL;
    node->addChild($1);
    $$ = node;
}
| IDENTIFIER LOP_POINT IDENTIFIER {
    // miss structure's type determine
    $$ = new TreeNode($1->lineno,NODE_EXPR);
    $$ -> stype = STMT_EXP;
    $$ -> optype = OP_POINT;
    $$ -> addChild($1); $$ -> addChild($3);
}
| LValExp LOP_LBRKET AddExp LOP_RBRKET{
    // TODO: there maybe bugs of array
    TreeNode* node = new TreeNode($1->lineno,NODE_ITEM);
    node->itype = ITEM_ARRAY;
    node->addChild($3);
    $$ = $1;
    $$->addChild(node);
}
;

UnaryExp
: PrimaryExp{
    $$ = $1;
}
| LOP_PLUS UnaryExp %prec LOP_UPLUS{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_FPLUS;
    node->addChild($2);
    $$ = node;
}
| LOP_MINUS UnaryExp %prec LOP_UMINUS{
    TreeNode* node = new TreeNode($2->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_FMINUS;
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

CommaExp
: LorExp{
    $$ = $1;
}
| CommaExp LOP_COMMA LorExp{
    TreeNode *node = new TreeNode($1->lineno, NODE_EXPR);
    node->stype = STMT_EXP;
    node->optype = OP_COMMA;
    node->addChild($1);
    node->addChild($3);
    $$ = node;
}

INTEGER: HEX_INTEGER {$$=$1;}
| DEC_INTEGER {$$=$1;}
| OCT_INTEGER {$$=$1;}
;

// WARNING: maybe lost SEMICOLON 
structdeclstmt
: KEY_STRUCT IDENTIFIER LOP_LBRACE structdeclist LOP_RBRACE structinitlist{
    $$ = new TreeNode($2->lineno, NODE_STMT);
    $$ -> stype = STMT_STRUCTDECL;

    // record the struct params' types
    TreeNode* tnode = new TreeNode($2->lineno, NODE_TYPE);
    tnode->type = new Type(COMPOSE_STRUCT);

    // find all it's sons to add into a NODE_TYPE
    TreeNode* ptr_item = $4->child;
    for(;ptr_item!=nullptr;ptr_item=ptr_item->sibling){
        if(ptr_item->nodeType==NODE_ITEM){
            TreeNode* ptr_stmt = ptr_item -> child;
            if(ptr_stmt->nodeType==NODE_STMT){
                tnode->type->addChild(ptr_stmt->type);
            }
            else continue;
        }
    }
    $$->addChild(tnode);

    $$ -> addChild($2); $$ -> var_name = $2 -> var_name; // has to record struct name
    $$ -> addChild($4);
    $$ -> addChild($6);

    // to record the type in the first init node
    $4->type = $$->type;
}
;

structdeclist
: structdecitem{
    TreeNode* node = new TreeNode(lineno, NODE_LIST);
    node->addChild($1);
    $$ = node;
}
| structdeclist structdecitem {
    $$ = $1;
    $$ -> addChild($2);
}
;

// to record the privacy: public/private maybe further expand
structdecitem
: decl SEMICOLON{
    $$ = new TreeNode($1->lineno,NODE_ITEM);
    $$ -> addChild($1); $$->authtype = AUTH_PUBLIC;
}
| funcdef SEMICOLON{
    $$ = new TreeNode($1->lineno,NODE_ITEM);
    $$ -> addChild($1); $$->authtype = AUTH_PUBLIC;
}
| AutT decl SEMICOLON{
    $$ = new TreeNode($2->lineno,NODE_ITEM);
    $$ -> addChild($2); $$->authtype = $1->authtype;
}
| AutT funcdef SEMICOLON{
    $$ = new TreeNode($2->lineno,NODE_ITEM);
    $$ -> addChild($2); $$->authtype = $1->authtype;
}
;

structinitlist
: structinititem {
    $$ = new TreeNode($1->lineno,NODE_LIST);
    $$->addChild($1);
}
| structinitlist LOP_COMMA structinititem{
    $$ = $1;
    $$->addChild($3);
}
;

// need to expand for further array
structinititem
: IDENTIFIER{
    $$ = new TreeNode($1->lineno,NODE_ITEM);
    $1->is_dec = true;
    $$-> addChild($1);
}
| {$$=new TreeNode(lineno,NODE_ITEM);}
;

T: T_INT {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_INT;} 
| T_CHAR {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_CHAR;}
| T_BOOL {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_BOOL;}
| T_STRING {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_STRING;}
;

TV: T_VOID {$$ = new TreeNode(lineno, NODE_TYPE); $$->type = TYPE_VOID;}

AutT : KEY_PUBLIC {$$ = new TreeNode(lineno, NODE_AUTH); $$->authtype = AUTH_PUBLIC;}
| KEY_PRIVATE {$$ = new TreeNode(lineno, NODE_AUTH); $$->authtype = AUTH_PRIVATE;}
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

AssignEqOp: LOP_PLUSEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_PLUSEQ;}
| LOP_MINUSEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_MINUSEQ;}
| LOP_MODEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_MODEQ;}
| LOP_MULEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_MULEQ;}
| LOP_DIVEQ {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_DIVEQ;}
;

QuoteOp: LOP_QUOTE {$$ = new TreeNode(lineno,NODE_OP); $$->optype=OP_QUOTE;}

%%

int yyerror(char const* message)
{
  cout << message << " at line " << lineno << endl;
  return -1;
}