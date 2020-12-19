#include "tree.h"
void TreeNode::addChild(TreeNode* child) {
    if(this->child==nullptr) {this->child=child; this->child->fath = this; return;}
    
    //find right position to insert a child
    TreeNode * it=this->child;
    for(;it->sibling!=nullptr;it=it->sibling){;}
    it->sibling=child;
    it->sibling->left_sibling = it;
    it->sibling->fath = this;
}

void TreeNode::addSibling(TreeNode* sibling){
    if(this->sibling==nullptr) {this->sibling=sibling; this->sibling->left_sibling = this; return;}

    //find right position to insert a sibling
    TreeNode * it=this->sibling;
    for(;it->sibling!=nullptr;it=it->sibling){;}
    it->sibling=sibling;
    it->sibling->left_sibling = it;
}

TreeNode* TreeNode::findChild(int offset){
    if(offset<=0){
        cout<<"invalid offset param.."<<endl;
        return nullptr;
    }
    TreeNode* tmp_ptr = this->child;
    for(int i=1;i<offset;i++){
        if(!(tmp_ptr->sibling)){
            cout<<"invalid offset param "<<offset<<".."<<endl;
            return nullptr;
        }
        tmp_ptr = tmp_ptr->sibling;
    }
    return tmp_ptr;
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
        bool is_exist = TreeNode::ptr_nst->SymTable.find(this->var_name) != TreeNode::ptr_nst->SymTable.end();
        varItem items;
        // cout<<this->var_name<<" "<<is_exist<<" ";
        if(!is_exist){
            items.fDecNode = this;
            items.dec_cnt = 1;
        }
        else{
            items = TreeNode::ptr_nst->SymTable[this->var_name];
            items.dec_cnt++;
        }
        //items.type has not been recorded;
        // cout<<items.dec_cnt<<endl;
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

bool TreeNode::Is_InSymbolTable(int uselino,string var_name){
    if(!this->IsSymbolTableOn()){
        cout<<"this nodeID "<<this->nodeID<<" shall not call Is_InSymbolTable() func"<<endl;
        return false;
    }
    /*
        to avoid a condition that
        int main(){
            int a;
            b=10;
            int b;
        }
        the first decl lineno of b is smaller than the first use lineno
    */
    if(this->SymTable.find(var_name) != this->SymTable.end()){
        return this->SymTable.find(var_name)->second.fDecNode->lineno <= uselino;
    }
    else return false;
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

bool TreeNode::Type_Check(TreeNode* root_ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check(root_ptr);

    if(this->nodeType==NODE_VAR){
        if(this->is_dec){
            if(this->Is_Dupdefined(this->var_name,root_ptr)){
                cerr<<"the variable "<<this->var_name
                    <<" in line:"<<this->lineno
                    <<" is dup_defined.."<<endl;
                return false;
            }

            // add type record to first declare node
            TreeNode* tmp_ptr = this;
            while (tmp_ptr){
                if((tmp_ptr->nodeType == NODE_STMT &&
                (tmp_ptr->stype == STMT_CONSTDECL || tmp_ptr->stype == STMT_VARDECL))){
                    this->type = tmp_ptr->type;
                    break;
                }
                tmp_ptr = tmp_ptr -> fath;
            }
        }
        else{
            if(!this->Is_Defined(this->var_name,root_ptr)){
                cerr<<"the variable "<<this->var_name
                    <<" in line:"<<this->lineno
                    <<" is undefined.."<<endl;
                return false;
            }

        }
    }
    // recursive type check
    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check(root_ptr);
    return ret;
}

bool TreeNode::Type_Check_SecondTrip(TreeNode* ptr){
    if(this->child!=nullptr) this->child->Type_Check_SecondTrip(ptr);

    // to generate all types from symboltable
    if(this->nodeType==NODE_VAR){
        if(!this->is_dec){
            TreeNode* now_ptr = this;
            while(now_ptr){
                if(now_ptr->IsSymbolTableOn()&&now_ptr->Is_InSymbolTable(this->lineno,this->var_name)){
                    this->type = now_ptr->SymTable[this->var_name].fDecNode->type;
                    break;
                }
                if(now_ptr==ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
    }

    if(this->sibling!=nullptr) this->sibling->Type_Check_SecondTrip(ptr);
    return true;
}

bool TreeNode::Type_Check_ThirdTrip(TreeNode* ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_ThirdTrip(ptr);

    switch(this->nodeType){
        case NODE_EXPR:{
            if(this->stype==STMT_EXP){
                switch(this->optype){
                    case OP_FPLUS:{
                        if(this->findChild(1)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else if(this->findChild(1)->type==TYPE_ERROR){
                            this->type = TYPE_ERROR;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_FMINUS:{
                        if(this->findChild(1)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else if(this->findChild(1)->type==TYPE_ERROR){
                            this->type = TYPE_ERROR;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_NOT:{
                        if(this->findChild(1)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else if(this->findChild(1)->type==TYPE_ERROR){
                            this->type = TYPE_ERROR;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_MUL:{
                        if(this->findChild(1)->type->is_can_expandtoint()&&this->findChild(2)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else{
                            this->type = TYPE_ERROR;
                        }
                        break;
                    }
                    case OP_DIV:{
                        if(this->findChild(1)->type->is_can_expandtoint()&&this->findChild(2)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else{
                            this->type = TYPE_ERROR;
                        }
                        break;
                    }
                    case OP_MOD:{
                        if(this->findChild(1)->type->is_can_expandtoint()&&this->findChild(2)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else{
                            this->type = TYPE_ERROR;
                        }
                        break;
                    }
                    case OP_PLUS:{
                        if(this->findChild(1)->type->is_can_expandtoint()&&this->findChild(2)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else if(this->findChild(1)->type->is_string_compatiable(this->findChild(2)->type)){
                            this->type = TYPE_STRING;
                        }
                        else{
                            this->type = TYPE_ERROR;
                        }
                        break;
                    }
                    case OP_MINUS:{
                        if(this->findChild(1)->type->is_can_expandtoint()&&this->findChild(2)->type->is_can_expandtoint()){
                            this->type = TYPE_INT;
                        }
                        else{
                            this->type = TYPE_ERROR;
                            cerr<<"type error.."<<endl;
                        }
                        break;
                    }
                    default:{
                        cout<<"undefined optype of STMT_EXP of NODE_EXPR in Type_Check_SecondTrip.."<<endl;
                    }
                }
            }
            else{
                cout<<"undefined StmtType of NODE_EXPR in Type_Check_ThirdTrip.."<<endl;
            }
            break;
        }
        default:{
            cout<<"undefined nodeType in Type_Check_ThirdTrip.."<<endl;
        }
    }

    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check_ThirdTrip(ptr);
    return ret;
}

bool TreeNode::Is_Defined(string var_name, TreeNode* root_ptr){
    TreeNode* now_ptr = this;
    while(now_ptr){
        if(now_ptr->IsSymbolTableOn()&&now_ptr->Is_InSymbolTable(this->lineno,var_name)){
            return true;
        }
        if(now_ptr==root_ptr) break;
        now_ptr = now_ptr -> fath;
    }
    return false;
}

bool TreeNode::Is_Dupdefined(string var_name,TreeNode* root_ptr){
    TreeNode* now_ptr = this;
    while(now_ptr){
        if(now_ptr->IsSymbolTableOn()&&now_ptr->Is_InSymbolTable(this->lineno,var_name)){
            // cout<<var_name<<" "<<now_ptr->SymTable[var_name].dec_cnt<<endl;
            if(now_ptr->SymTable[var_name].dec_cnt>1){
                return true;
            }
        }
        if(now_ptr==root_ptr) break;
        now_ptr = now_ptr -> fath;
    }
    return false;
}

bool TreeNode::Is_TypeAccordance(TreeNode* ptr){

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
        case OP_FPLUS:   return "+ ";
        case OP_FMINUS:  return "- ";
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