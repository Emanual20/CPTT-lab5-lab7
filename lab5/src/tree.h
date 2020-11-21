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
};

enum StmtType {
    STMT_SKIP,
    STMT_DECL,
    STMT_EXP,// if StmtType=STMT_EXP, it has property OperatorType
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

    int int_val;
    char ch_val;
    bool b_val;
    string str_val;
    string var_name;
public:
    static string nodeType2String (NodeType type);
    static string opType2String (OperatorType type);
    static string sType2String (StmtType type);

public:
    TreeNode(int lineno, NodeType type);
};

#endif