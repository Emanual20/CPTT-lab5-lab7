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

struct TreeNode {
public:
    // items to record in SymbolTable
    struct varItem{
        TreeNode* fDecNode; // a pointer to first declared TreeNode
    // the node may have lineno, var_name, nodeID
        Type* type;
    // the type pointer of this var don't record in fDecNode
    };


    int nodeID;
    int lineno;
    NodeType nodeType;

    TreeNode* child = nullptr;
    TreeNode* sibling = nullptr;

    void addChild(TreeNode*);
    void addSibling(TreeNode*);
    
    void printNodeInfo();
    void printChildrenId();

    void printAST(); 
    void printSpecialInfo();

    int genNodeId(int);
    void genSymbolTable();

public:
    AuthorityType authtype;
    OperatorType optype;  // if StmtType=STMT_EXP, it has property OperatorType
    Type* type;  
    StmtType stype;
    ItemType itype;

    int int_val;
    char ch_val;
    bool b_val;
    string str_val;
    string var_name;
public:
    // symbol will be off as default
    bool is_SymbolTable_on = 0;
    map<string,varItem> SymTable;
public:
    static string authType2String (AuthorityType authtype);
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);
    static string iType2String (ItemType type);

public:
    TreeNode(int lineno, NodeType type);
    void OpenSymbolTable();
    void CloseSymbolTable();
    bool IsSymbolTableOn(); 
};

#endif