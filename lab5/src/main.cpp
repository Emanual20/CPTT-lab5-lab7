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
        root->gen_intervar(root);
        root->printAST();

        if(!root->Type_Check(root)){
            cout<<"type error"<<endl;
            return 0;
        }

        // gen_identifier_types
        root->Type_Check_SecondTrip(root);

        // expr type check
        if(!root->Type_Check_ThirdTrip(root)){
            cout<<"expr accordinate error"<<endl;
            return 0;
        }

        // stmt type check
        if(!root->Type_Check_FourthTrip(root)){
            cout<<"stmt accordinate error"<<endl;
            return 0;
        }

        root->gen_label(root);
        root->gen_code(asmfo,root);
    }
    return 0;
}