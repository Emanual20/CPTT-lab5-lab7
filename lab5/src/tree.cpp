#include "tree.h"
void TreeNode::addChild(TreeNode* child) {
    if(this->child==nullptr) {this->child=child; return;}
    
    //find right position to insert a child
    TreeNode * it=this->child;
    for(;it->sibling!=nullptr;it=it->sibling){;}
    it->sibling=child;
}

void TreeNode::addSibling(TreeNode* sibling){
    if(this->sibling==nullptr) {this->sibling=sibling; return;}

    //find right position to insert a sibling
    TreeNode * it=this->sibling;
    for(;it->sibling!=nullptr;it=it->sibling){;}
    it->sibling=sibling;
}

TreeNode::TreeNode(int lineno, NodeType type) {
    this->lineno=lineno;
    this->nodeType=type;
}

int TreeNode::genNodeId(int initno) {
    this->nodeID=initno;
    if(this->child!=nullptr) {
        initno++;
        initno=this->child->genNodeId(initno);
    }
    if(this->sibling!=nullptr){
        initno++;
        initno=this->sibling->genNodeId(initno);
    }
    return initno;
}

void TreeNode::printNodeInfo() {
    cout<<"Lno@"<<this->lineno;
    cout<<setw(8)<<"@"<<this->nodeID<<" ";

    cout<<this->nodeType2String(this->nodeType)<<" ";

    this->printSpecialInfo();
    cout<<endl;
}

void TreeNode::printChildrenId() {
    for(TreeNode* fson_ptr=this->child; fson_ptr!=nullptr; fson_ptr=fson_ptr->sibling){
        cout<<"@"<<fson_ptr->nodeID<<" ";
    }
}

void TreeNode::printAST() {
    this->printNodeInfo();
    if(this->child!=nullptr) this->child->printAST();
    if(this->sibling!=nullptr) this->sibling->printAST();
}


// You can output more info...
void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:{
            
            string typeinfo = this->type->getTypeInfo();
            if(typeinfo=="int") cout<<" "<<this->int_val<<" ";
            else if(typeinfo=="char") cout<<" "<<this->ch_val<<" ";
            else if(typeinfo=="bool") cout<<" "<<this->b_val<<" ";
            else if(typeinfo=="string") cout<<" "<<this->str_val<<" ";

            break;
        }
        case NODE_VAR:{

            cout<<" "<<this->var_name<<" ";
            break;
        }
        case NODE_EXPR:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"]";
            cout<<" optype: ";
            switch(this->optype){
                case OP_EQ:{
                    cout<<"= ";
                    break;
                }
                case OP_PLUS:{
                    cout<<"+ ";
                    break;
                }
                case OP_MINUS:{
                    cout<<"- ";
                    break;
                }
                case OP_NOT:{
                    cout<<"! ";
                    break;
                }
                case OP_MUL:{
                    cout<<"* ";
                    break;
                }
                case OP_DIV:{
                    cout<<"/ ";
                    break;
                }
                case OP_MOD:{
                    cout<<"% ";
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case NODE_STMT:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"]";
            break;
        }
        case NODE_PROG:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"]";
            break;
        }
        case NODE_TYPE:{
            cout<<" "<<this->type->getTypeInfo()<<" ";
            break;
        }
        default:
            break;
    }
}

string TreeNode::sType2String(StmtType type) {
    switch(type){
        case STMT_SKIP: return "STMT_SKIP";
        case STMT_DECL: return "STMT_DECL";
        case STMT_EXP:  return "STMT_EXP";
    }
    return "unknown type";
}


string TreeNode::nodeType2String (NodeType type){
    switch(type){
        case NODE_CONST: return "NODE_CONST";
        case NODE_VAR:   return "NODE_VAR";
        case NODE_EXPR:  return "NODE_EXPR";
        case NODE_TYPE:  return "NODE_TYPE";

        case NODE_OP:    return "NODE_OP";

        case NODE_STMT:  return "NODE_STMT";
        case NODE_PROG:  return "NODE_PROG";
    }
    return "unknown type";
}
