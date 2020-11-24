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

    if(this->IsSymbolTableOn()==true){
        this->printSymbolTable();
    }

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

void TreeNode::genSymbolTable(){
    // if(this->nodeID!=0){
    //     cerr<<"non-root node can't not call genSymbolTable() func!"<<endl;
    //     return;
    // }

    // use root's symboltable to init the scope 
    if(this->nodeID==0){
        TreeNode::ptr_nst = this;
        TreeNode::ptr_vec.push(this);
        this->OpenSymbolTable();
    }
    bool isSwitch = false;

    // switch the scope and open this node's symboltable
    if(this->nodeType==NODE_STMT&&(this->stype==STMT_STRUCTDECL||this->stype==STMT_BLOCK)){
        TreeNode::ptr_nst = this;
        TreeNode::ptr_vec.push(this);
        isSwitch=true;
        this->OpenSymbolTable();
    }

    if(this->nodeType==NODE_VAR&&this->is_dec){
        varItem items;
        items.fDecNode = this;
        //items.type has not been recorded;
        TreeNode::ptr_nst->SymTable[this->var_name]=items;
    }

    if(this->child!=nullptr) this->child->genSymbolTable();
    if(this->sibling!=nullptr) this->sibling->genSymbolTable();
    // to traversal the parse tree further..

    // check isSwitch to recover the scope if nec
    if(isSwitch){
        TreeNode::ptr_nst = TreeNode::ptr_vec.top();
        TreeNode::ptr_vec.pop();
    }
}

void TreeNode::printSymbolTable(){
    if(!this->IsSymbolTableOn()){
        cerr<<"this node shall not call printSymbolTable() func"<<endl;
    }
    cout<<" vars:[";
    for(map<string,varItem>::iterator it = this->SymTable.begin();it!=SymTable.end();it++){
        cout<<it->first<<" ";
    }
    cout<<"] ";
}

void TreeNode::OpenSymbolTable(){
    this->is_SymbolTable_on = true;
}

void TreeNode::CloseSymbolTable(){
    this->SymTable.clear();
    this->is_SymbolTable_on = false;
}

bool TreeNode::IsSymbolTableOn(){
    return this->is_SymbolTable_on;
}


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
            cout<<"children:["; this->printChildrenId(); cout<<"]";
            cout<<" optype: ";
            cout<<this->opType2String(this->optype);
            break;
        }
        case NODE_TYPE:{
            cout<<" "<<this->type->getTypeInfo()<<" ";
            break;
        }
        case NODE_STMT:{
            if(this->stype!=STMT_SKIP){
                cout<<"children:["; this->printChildrenId(); cout<<"]";
            }
            cout<<this->sType2String(this->stype);
            break;
        }
        case NODE_PROG:{
            cout<<"children:["; this->printChildrenId(); cout<<"]";
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
        case STMT_SKIP:       return " STMT_SKIP ";
        case STMT_VARDECL:    return " STMT_DECL_VAR ";
        case STMT_CONSTDECL:  return " STMT_DECL_CONST ";
        case STMT_STRUCTDECL: return " STMT_DECL_STRUCT ";
        case STMT_EXP:        return " STMT_EXP ";
        case STMT_IF:         return " STMT: if ";
        case STMT_IFELSE:     return " STMT: ifelse ";
        case STMT_BLOCK:      return " STMT: block {} ";
        case STMT_WHILE:      return " STMT: while ";
        case STMT_RETURN:     return " STMT: return ";
        case STMT_BREAK:      return " STMT: break ";
        case STMT_CONTINUE:   return " STMT: continue ";
        case STMT_FOR:        return " STMT: for ";
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
        case ITEM_UFUNC:  return "ITEM_UFUNC";
    }
    return "unknown type";
}

string TreeNode::opType2String (OperatorType type){
    switch(type){
        case OP_POINT:   return ". ";
        case OP_EQ:      return "= ";
        case OP_PLUS:    return "+ ";
        case OP_MINUS:   return "- ";
        case OP_NOT:     return "! ";
        case OP_MUL:     return "* ";
        case OP_DIV:     return "/ ";
        case OP_MOD:     return "% ";
        case OP_LESS:    return "< ";
        case OP_GREA:    return "> ";
        case OP_LE:      return "<= ";               
        case OP_GE:      return ">= ";                  
        case OP_EEQ:     return "== ";                 
        case OP_NEQ:     return "!= ";                    
        case OP_LAND:    return "&& ";
        case OP_LOR:     return "|| ";
        case OP_LVAL:    return "lval expr ";
        case OP_PLUSEQ:  return "+= ";
        case OP_MINUSEQ: return "-= ";
        case OP_MODEQ:   return "%= ";
        case OP_MULEQ:   return "*= ";
        case OP_DIVEQ:   return "/= ";
        case OP_COMMA:   return ", ";
        default:         return "unknown operator..!";
    }
}

string TreeNode::authType2String (AuthorityType type){
    switch(type){
        case AUTH_PUBLIC:       return "public ";
        case AUTH_PRIVATE:      return "private ";
        case AUTH_PROTECTED:    return "protected ";
        default:                return "unknown authority..!";
    }
}