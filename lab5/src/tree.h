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

    NODE_OP,
    NODE_ITEM,
    NODE_LIST, // for some func para list
    NODE_FUNC,

    NODE_STMT,
    NODE_PROG,
};

enum OperatorType
{
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
};

enum StmtType {
    STMT_SKIP,
    STMT_DECL,
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

enum ItemType{
    ITEM_DECL,
    ITEM_SPF, // scanf or printf
    ITEM_UFUNC,
};

struct TreeNode {
public:
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

public:
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
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);
    static string iType2String (ItemType type);

public:
    TreeNode(int lineno, NodeType type);
};

#endif