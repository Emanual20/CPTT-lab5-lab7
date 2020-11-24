#include "common.h"
#include <fstream>

extern TreeNode *root;
extern FILE *yyin;
extern int yyparse();

using namespace std;

TreeNode* TreeNode::ptr_nst = new TreeNode(-1,NODE_ASSIST);
stack<TreeNode*> TreeNode::ptr_vec = stack<TreeNode*>();

int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        FILE *fin = fopen(argv[1], "r");
        if (fin != nullptr)
        {
            yyin = fin;
        }
        else
        {
            cerr << "failed to open file: " << argv[1] << endl;
        }
    }
    yyparse();
    if(root != NULL) {
        root->genNodeId(0);
        root->genSymbolTable();
        root->printAST();
    }
    return 0;
}