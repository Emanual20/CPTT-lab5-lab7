#include "common.h"
#include <fstream>

extern TreeNode *root;
extern FILE *yyin;
extern int yyparse();

using namespace std;

TreeNode* TreeNode::ptr_nst = new TreeNode(-1,NODE_ASSIST);
stack<TreeNode*> TreeNode::ptr_vec = stack<TreeNode*>();
int TreeNode::localvar_cnt = 0;
int TreeNode::label_cnt = 0;
ofstream asmfo("asm_output/myasm.asm");

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

        if(root->Type_Check(root)==-1){
            return 0;
        }

        // generate the intervar mark used in generating codes
        root->gen_intervar(root);
        root->gen_offset(root);
        root->printAST();

        root->gen_label(root);
        root->gen_code(asmfo,root);
    }
    return 0;
}