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

            cout<<" variable_name: "<<this->var_name<<" ";
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
                case OP_LESS:{
                    cout<<"< ";
                    break;
                }
                case OP_GREA:{
                    cout<<"> ";
                    break;
                }
                case OP_LE:{
                    cout<<"<= ";
                    break;
                }
                case OP_GE:{
                    cout<<">= ";
                    break;
                }
                case OP_EEQ:{
                    cout<<"== ";
                    break;
                }
                case OP_NEQ:{
                    cout<<"!= ";
                    break;
                }
                case OP_LAND:{
                    cout<<"&& ";
                    break;
                }
                case OP_LOR:{
                    cout<<"|| ";
                    break;
                }
                case OP_LVAL:{
                    cout<<"lval expr ";
                    break;
                }
                case OP_PLUSEQ:{
                    cout<<"+= ";
                    break;
                }
                case OP_MINUSEQ:{
                    cout<<"-= ";
                    break;
                }
                case OP_MODEQ:{
                    cout<<"%= ";
                    break;
                }
                case OP_MULEQ:{
                    cout<<"*= ";
                    break;
                }
                case OP_DIVEQ:{
                    cout<<"/= ";
                    break; 
                }
                default:
                    break;
            }
            break;
        }
        case NODE_TYPE:{
            cout<<" "<<this->type->getTypeInfo()<<" ";
            break;
        }
        case NODE_STMT:{
            if(this->stype!=STMT_SKIP){
                cout<<"children:[";
                this->printChildrenId();
                cout<<"]";
            }
            cout<<this->sType2String(this->stype);
            break;
        }
        case NODE_PROG:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"]";
            break;
        }
        case NODE_ITEM:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"] ";
            cout<<this->iType2String(this->itype);
            break;
        }
        case NODE_LIST:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"] ";
            break;
        }
        case NODE_FUNC:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"] ";
            cout<<" func_name: "<<this->var_name;
            break;
        }
        default:
            break;
    }
}

string TreeNode::sType2String(StmtType type) {
    switch(type){
        case STMT_SKIP:     return " STMT_SKIP ";
        case STMT_DECL:     return " STMT_DECL ";
        case STMT_EXP:      return " STMT_EXP ";
        case STMT_IF:       return " STMT: if ";
        case STMT_IFELSE:   return " STMT: ifelse ";
        case STMT_BLOCK:    return " STMT: block {} ";
        case STMT_WHILE:    return " STMT: while ";
        case STMT_RETURN:   return " STMT: return ";
        case STMT_BREAK:    return " STMT: break ";
        case STMT_CONTINUE: return " STMT: continue ";
        case STMT_FOR:      return " STMT: for ";
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
        case NODE_ITEM:  return "NODE_ITEM";
        case NODE_LIST:  return "NODE_LIST";
        case NODE_FUNC:  return "NODE_FUNC";

        case NODE_STMT:  return "NODE_STMT";
        case NODE_PROG:  return "NODE_PROG";
    }
    return "unknown type";
}

string TreeNode::iType2String (ItemType type){
    switch(type){
        case ITEM_DECL:   return "ITEM_DECL";
        case ITEM_SPF:    return "ITEM_SPF";
    }
    return "unknown type";
}