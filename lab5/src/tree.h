#ifndef TREE_H
#define TREE_H

#include "pch.h"
#include "type.h"

enum NodeType
{
    NODE_CONST, 
    NODE_VAR,
    NODE_EXPR,
    NODE_TYPE,

    NODE_AUTH, // node auth do not need to be add to parse tree, because it has been record in authtype;
    NODE_OP, // node_op do not need to be add to parse tree, because it has been record in optype;
    NODE_ITEM,
    NODE_LIST, // for some func para list
    NODE_FUNC,

    NODE_STMT,
    NODE_PROG,
    NODE_ASSIST, // assist to occupy a position
};

enum StmtType {
    STMT_SKIP,
    STMT_VARDECL,
    STMT_CONSTDECL,
    STMT_STRUCTDECL,
    STMT_EXP,// if StmtType=STMT_EXP, it has property OperatorType
    STMT_IF,
    STMT_IFELSE,
    STMT_BLOCK,
    STMT_WHILE,
    STMT_RETURN,
    STMT_BREAK,
    STMT_CONTINUE,
    STMT_FOR,
};

enum AuthorityType{
    AUTH_PUBLIC,
    AUTH_PRIVATE,
    AUTH_PROTECTED,
};

enum OperatorType{
    OP_POINT, // .
    OP_EQ,  // =
    OP_PLUS, // +
    OP_MINUS, // -
    OP_FPLUS, // +
    OP_FMINUS, // -
    OP_NOT, // !
    OP_MUL, // *
    OP_DIV, // / 
    OP_MOD, // %
    OP_LESS, // <
    OP_GREA, // >
    OP_LE, // <=
    OP_GE, // >=
    OP_EEQ, // ==
    OP_NEQ, // !=
    OP_LAND, // &&
    OP_LOR, // ||
    OP_LVAL, // left value
    OP_PLUSEQ,  // +=
    OP_MINUSEQ, // -=
    OP_MODEQ, // %=
    OP_MULEQ, // *=
    OP_DIVEQ, // /=
    OP_COMMA, // ,
};

enum ItemType{
    ITEM_DECL,
    ITEM_SPF, // scanf or printf
    ITEM_UFUNC,
};

struct Label{
    string true_label;
    string false_label;
    string begin_label;
    string next_label;
};

struct TreeNode {
public:
    // items to record in SymbolTable
    struct varItem{
        TreeNode* fDecNode; // a pointer to first declared TreeNode
    // the node may have lineno, var_name, nodeID
        Type* type;
    // the type pointer of this var don't record in fDecNode
        int dec_cnt = 0;
    // local offset
        int local_offset = -1;
    };

    int nodeID;
    int lineno;
    NodeType nodeType;

    TreeNode* child = nullptr;
    TreeNode* fath = nullptr;
    TreeNode* sibling = nullptr;      // pointer point to right sibling
    TreeNode* left_sibling = nullptr; // pointer point to left sibling

    void addChild(TreeNode*);
    void addSibling(TreeNode*);
    TreeNode* findChild(int offset);
    
    void printNodeInfo();
    void printChildrenId();

    void printAST(); 
    void printSpecialInfo();

    int genNodeId(int);

public:
    AuthorityType authtype;
    OperatorType optype;  // if StmtType=STMT_EXP, it has property OperatorType
    Type* type;
    Type* checktype; // to reserve type of function  
    StmtType stype;
    ItemType itype;

    int int_val;
    char ch_val;
    bool b_val;
    bool is_dec=false; // to record if a NODE_VAR appears in a declstmt 
    string str_val;
    string var_name;   
public:
    static string authType2String (AuthorityType authtype);
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);
    static string iType2String (ItemType type);

public:
    // functions related to type check
    bool Type_Check(TreeNode*); // to check is_defined & is_dupdefined
    bool Type_Check_SecondTrip(TreeNode*); // to generate all types from symboltable
    bool Type_Check_ThirdTrip(TreeNode*); // to analyze expr type
    bool Type_Check_FourthTrip(TreeNode*); // to analyze all other statements and other tokens
    bool Is_Defined(string,TreeNode*);  // the TreeNode* param shall be ptr to root 
    bool Is_Dupdefined(string,TreeNode*); // the TreeNode* param shall be ptr to root

public:
    // params related to symbol table
    bool is_SymbolTable_on = 0;
    map<string,varItem> SymTable;
    // functions related to symbol table
    static TreeNode* ptr_nst;
    static stack<TreeNode*> ptr_vec; 
    TreeNode(int lineno, NodeType type);
    void OpenSymbolTable();
    void CloseSymbolTable();
    bool IsSymbolTableOn();
    bool IsNeedSwitchScope();
    bool Is_InSymbolTable(int,string);
    void genSymbolTable();
    void printSymbolTable();

public:
    // gen_label
    static int localvar_cnt;
    static int label_cnt;

    int intervar_num = -1;
    void gen_intervar(TreeNode* t);

    struct Label label;
    void gen_label(TreeNode* root_ptr);
    string new_label();
    void gen_rec_stmtorexpr_label(TreeNode* t);
    void gen_stmt_label(TreeNode* t);
    void gen_expr_label(TreeNode* t);

public:
    // gen_code
    void gen_code(ostream &out,TreeNode* t);
    void gen_asm_header(ostream &out,TreeNode* t);
    void gen_glob_decl(ostream &out,TreeNode* t);
    void gen_rec_code(ostream &out,TreeNode* t);
    void gen_stmt_code(ostream &out,TreeNode* t);
    void gen_expr_code(ostream &out,TreeNode* t);
    void gen_func_code(ostream &out,TreeNode* t);
    void gen_prog_code(ostream &out,TreeNode* t);
    void gen_localdec_code(ostream &out);
    void recover_localdec_stack(ostream &out);
    string lookup_locglosymtab(ostream &out,TreeNode* t);
};

#endif