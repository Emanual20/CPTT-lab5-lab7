#include "tree.h"

const int INT_SIZE = 4;
const int ALLOC_MINIMUM_SIZE = 4;
const int STACK_RESERVE_SIZE = 8;
const int SCOPE_MINIMUM_SIZE = 16;
const int PRINTF_STR_SIZE = 1;
const int CHAR_SIZE = 1;

void TreeNode::addChild(TreeNode* child) {
    if(this->child==nullptr) {
        this->child=child;
        TreeNode* tmp_ptr = this->child; 
        while(tmp_ptr){
            if(!tmp_ptr->fath){
                tmp_ptr->fath = this;
            }
            tmp_ptr = tmp_ptr->sibling;
        }
        return;
    }
    
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
    
    if(!(it->sibling->fath)){
        it->sibling->fath = it->fath;
    }
}

TreeNode* TreeNode::findChild(int offset){
    if(offset<=0){
        //cerr<<"invalid offset param < 0.."<<endl;
        return nullptr;
    }
    TreeNode* tmp_ptr = this->child;
    for(int i=1;i<offset;i++){
        if(!(tmp_ptr->sibling)){
            //cerr<<"invalid offset param "<<offset<<" in findChild.."<<endl;
            return nullptr;
        }
        tmp_ptr = tmp_ptr->sibling;
    }
    return tmp_ptr;
}

TreeNode* TreeNode::findrightmostchild(){
    TreeNode* ptr_temp = this->findChild(1);
    if(!ptr_temp){
        //cerr<<"this nodeID "<<this->nodeID<<" has no child"<<endl;
        return nullptr;
    }

    while(ptr_temp->sibling){
        ptr_temp = ptr_temp->sibling;
    }
    return ptr_temp;
}

int TreeNode::calc_child_num(){
    int ret = 0;
    TreeNode* tmp = this->child;
    while(tmp){
        ret += 1;
        tmp = tmp -> sibling;
    }
    return ret;
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

    cout<<setw(11)<<this->nodeType2String(this->nodeType)<<" ";

    if(this->fath){
        cout<<setw(8)<<" fath:@"<<this->fath->nodeID<<" ";
    }

    this->printSpecialInfo();

    if(this->IsSymbolTableOn()) this->printSymbolTable();

    cout<<endl<<"[";
    if(this->label.begin_label!="")
        cout<<"BL:"<<this->label.begin_label<<" ";
    if(this->label.next_label!="")
        cout<<"NL:"<<this->label.next_label<<" ";
    if(this->label.true_label!="")
        cout<<"TL:"<<this->label.true_label<<" ";
    if(this->label.false_label!="")
        cout<<"FL:"<<this->label.false_label<<" ";
    cout<<"]";

    cout<<"  isdec = "<<this->is_dec;
    cout<<"  isparam = "<<this->is_param;

    cout<<" [LocVAR_SZ: "<<this->local_var_size<<" "
        <<" ThisScope_Space: "<<this->thisscope_var_space<<" "
        <<" Scope_OFFSET: "<<this->scope_offset<<"]";

    //cout<<" INTERVAL: "<<this->intervar_num;

    cout<<endl<<endl;
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

    if(this->nodeType == NODE_FUNC && this->is_dec){
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

    // switch the scope and open this node's symboltable
    if(this->IsNeedSwitchScope()){
        TreeNode::ptr_nst = this;
        TreeNode::ptr_vec.push(this);
        isSwitch=true;
        this->OpenSymbolTable();
    }

    if(this->nodeType == NODE_VAR && this->is_dec){
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
    else if(this->nodeType == NODE_VAR && this->is_param){
        // param shall be in the function scope
        // this->fath(node_item)->fath(node_list)->sibling(node_block)
        TreeNode* ptr_funcblock = this->fath->fath->sibling;
        bool is_exist = ptr_funcblock->SymTable.find(this->var_name) 
                        != ptr_funcblock->SymTable.end();
        varItem items;
        // cout<<this->var_name<<" "<<is_exist<<" ";
        if(!is_exist){
            items.fDecNode = this;
            items.dec_cnt = 1;
            items.is_param = true;
        }
        else{
            items = ptr_funcblock->SymTable[this->var_name];
            items.dec_cnt++;
        }
        //items.type has not been recorded;
        // cout<<items.dec_cnt<<endl;
        ptr_funcblock->SymTable[this->var_name]=items;
    }
    else if(this->nodeType == NODE_ARRAY && this->is_dec){
        bool is_exist = TreeNode::ptr_nst->SymTable.find(this->child->var_name) 
                        != TreeNode::ptr_nst->SymTable.end();
        varItem items;

        if(!is_exist){
            // has to pull type from item_node
            this->type = this->fath->type;
            items.fDecNode = this;
            items.dec_cnt = 1;
        }
        else{
            items = TreeNode::ptr_nst->SymTable[this->child->var_name];
            items.dec_cnt++;
        }
        TreeNode::ptr_nst->SymTable[this->child->var_name]=items;
    }
    else if(this->nodeType == NODE_ARRAY && this->is_param){
        TreeNode* ptr_funcblock = this->fath->fath->sibling;
        bool is_exist = ptr_funcblock->SymTable.find(this->child->var_name) 
                        != ptr_funcblock->SymTable.end();
        varItem items;

        if(!is_exist){
            // has to pull type from item_node
            this->type = this->fath->type;
            items.fDecNode = this;
            items.dec_cnt = 1;
            items.is_param = true;
        }
        else{
            items = ptr_funcblock->SymTable[this->child->var_name];
            items.dec_cnt++;
        }
        ptr_funcblock->SymTable[this->child->var_name]=items;
    }

    if(this->child!=nullptr) this->child->genSymbolTable();

    // to traversal the parse tree further..

    if(this->sibling!=nullptr) this->sibling->genSymbolTable();

    // check isSwitch to recover the scope if nec
    if(isSwitch){
        TreeNode::ptr_vec.pop();
        TreeNode::ptr_nst = TreeNode::ptr_vec.top();
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

bool TreeNode::IsNeedSwitchScope(){
    return this->nodeType == NODE_STMT 
        &&(this->stype == STMT_STRUCTDECL || this->stype == STMT_BLOCK);
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
        // cerr<<this->SymTable.find(var_name)->second.fDecNode->lineno<<" "<<uselino<<endl;
        return this->SymTable.find(var_name)->second.fDecNode->lineno <= uselino;
    }
    else return false;
}

void TreeNode::printSpecialInfo() {
    switch(this->nodeType){
        case NODE_CONST:{          
            string typeinfo = this->type->getTypeInfo();
            if(typeinfo=="int") cout<<" "<<this->int_val<<" ";
            else if(typeinfo=="char") cout<<" "<<this->str_val<<" ";
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
            cout<<" intervar_num: ";
            cout<<this->intervar_num;
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
        case NODE_ARRAY:{
            cout<<"children:[";
            this->printChildrenId();
            cout<<"] ";
            break;
        }
        default:
            break;
    }
}

int TreeNode::Type_Check(TreeNode *root_ptr){
    if(this!=root_ptr) return -1;

    if(!root_ptr->Type_Check_FirstTrip(root_ptr)){
        cout<<"type error"<<endl;
        return -1;
    }
    // cerr<<"type check first trip finish"<<endl;

    if(!root_ptr->Type_Check_FirstPointFiveTrip(root_ptr)){
        cout<<"something error in 1.5 trip"<<endl;
        return -1;
    }
    // cerr<<"type check 1.5 trip finish"<<endl;

    // gen_identifier_types
    root_ptr->Type_Check_SecondTrip(root_ptr);
    // cerr<<"type check second trip finish"<<endl;

    // expr type check
    if(!root_ptr->Type_Check_ThirdTrip(root_ptr)){
        cout<<"expr accordinate error"<<endl;
        return -1;
    }
    // cerr<<"type check third trip finish"<<endl;

    // stmt type check
    if(!root_ptr->Type_Check_FourthTrip(root_ptr)){
        cout<<"stmt accordinate error"<<endl;
        return -1;
    }
    // cerr<<"type check fourth trip finish"<<endl;

    return 1;
}

bool TreeNode::Type_Check_FirstTrip(TreeNode* root_ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_FirstTrip(root_ptr);

    if(this->nodeType==NODE_VAR && !this->is_ignore){
        if(this->is_dec && !(this->is_param)){
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
        else if(this->is_param){
            ;
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
    else if(this->nodeType == NODE_ARRAY){
        // cerr<<this->nodeID<<" "<<this->is_param<<" "<<this->is_dec<<endl;
        if(this->is_dec && !(this->is_param)){
            if(this->Is_Dupdefined(this->child->var_name,root_ptr)){
                cerr<<"the array "<<this->child->var_name
                    <<" in line:"<<this->lineno
                    <<" is dup_defined.."<<endl;
                return false;
            }

            // // add type record to first declare node
            // TreeNode* tmp_ptr = this;
            // while (tmp_ptr){
            //     if((tmp_ptr->nodeType == NODE_STMT &&
            //     (tmp_ptr->stype == STMT_CONSTDECL || tmp_ptr->stype == STMT_VARDECL))){
            //         this->type = tmp_ptr->type;
            //         break;
            //     }
            //     tmp_ptr = tmp_ptr -> fath;
            // }
        }
        else if(this->is_param){
            ;
        }
        else{
            if(!this->Is_Defined(this->child->var_name,root_ptr)){
                cerr<<"the array "<<this->child->var_name
                    <<" in line:"<<this->lineno
                    <<" is undefined.."<<endl;
                return false;
            }
        }
    }
    // recursive type check
    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check_FirstTrip(root_ptr);
    return ret;
}

bool TreeNode::Type_Check_FirstPointFiveTrip(TreeNode* root_ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_FirstPointFiveTrip(root_ptr);

    // cerr<<this->nodeID<<endl;
    if(this->nodeType==NODE_FUNCALL && 
        (this->var_name != "scanf" && this->var_name != "printf")){
        // if funcall name is "scanf" or "printf", ignore them
        // cerr<<this->nodeID<<endl;
        TreeNode* ptr_fdecnode;
        if(!this->is_dec){
            TreeNode* now_ptr = this;
            while(now_ptr){
                if(now_ptr->IsSymbolTableOn() && 
                    now_ptr->Is_InSymbolTable(this->lineno,this->child->var_name)){
                    ptr_fdecnode = now_ptr->SymTable[this->child->var_name].fDecNode;
                    break;
                }
                if(now_ptr==root_ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
        if(this->findChild(2)->calc_child_num() !=
            ptr_fdecnode->findChild(2)->calc_child_num()){
            // cerr<<this->findChild(2)->calc_child_num()<<" "<<ptr_fdecnode->findChild(2)->calc_child_num()<<endl;
            this->type = TYPE_ERROR;
            ret = false;
            cerr<<"[error] funccall params number accordinate error in lineno "<<this->lineno<<endl;
        }
    }
    else if(this->nodeType==NODE_EXPR && 
        (this->optype == OP_EQ || this->optype == OP_PLUSEQ || this->optype == OP_MINUSEQ
            || this->optype == OP_MODEQ || this->optype == OP_MULEQ || this->optype == OP_DIVEQ)){
        // cerr<<this->nodeID<<endl;
        TreeNode* ptr_lvalidusenode = this->findChild(1)->findChild(1);
        // cerr<<this->nodeID<<" "<<ptr_lvalidusenode->nodeID<<endl;

        TreeNode* ptr_fdecnode;
        string usevar_name = "";
        if(ptr_lvalidusenode->nodeType == NODE_ARRAY) 
            usevar_name = ptr_lvalidusenode -> findChild(1) -> var_name;
        else
            usevar_name = ptr_lvalidusenode -> var_name;

        if(!ptr_lvalidusenode->is_dec){
            TreeNode* now_ptr = ptr_lvalidusenode;
            while(now_ptr){
                // cerr<<now_ptr->nodeID<<" ";
                if(now_ptr->IsSymbolTableOn() && 
                    now_ptr->Is_InSymbolTable(ptr_lvalidusenode->lineno,usevar_name)){
                    // cerr<<"note"<<now_ptr->nodeID<<endl;
                    ptr_fdecnode = now_ptr->SymTable[usevar_name].fDecNode;
                    break;
                }
                if(now_ptr==root_ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
        // cerr<<ptr_fdecnode->nodeID<<endl;
        if(ptr_fdecnode->is_const==true){
            this->type = TYPE_ERROR;
            ret = false;
            cerr<<"[error] assign value for a const var in lineno "<<this->lineno<<endl;
        }
    }

    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check_FirstPointFiveTrip(root_ptr);
    return ret;
}

bool TreeNode::Type_Check_SecondTrip(TreeNode* ptr){
    if(this->child!=nullptr) this->child->Type_Check_SecondTrip(ptr);

    // to generate all types from symboltable
    if(this->nodeType==NODE_VAR && !this->is_ignore){
        if(!this->is_dec){
            TreeNode* now_ptr = this;
            while(now_ptr){
                if(now_ptr->IsSymbolTableOn() && 
                    now_ptr->Is_InSymbolTable(this->lineno,this->var_name)){
                    this->type = now_ptr->SymTable[this->var_name].fDecNode->type;
                    break;
                }
                if(now_ptr==ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
    }
    else if(this->nodeType==NODE_ARRAY){
        // nodes which use vars pull type from symboltable
        // NOTE: fdecnode's type was recorded in genSymbolTable, now just pull it down in NODE_ARRAY
        if(!this->is_dec){
            TreeNode* now_ptr = this;
            while(now_ptr){
                if(now_ptr->IsSymbolTableOn() && 
                    now_ptr->Is_InSymbolTable(this->lineno,this->child->var_name)){
                    this->type = now_ptr->SymTable[this->child->var_name].fDecNode->type;
                    break;
                }
                if(now_ptr==ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
    }
    else if(this->nodeType==NODE_FUNCALL){
        if(!this->is_dec){
            TreeNode* now_ptr = this;
            while(now_ptr){
                if(now_ptr->IsSymbolTableOn() && 
                    now_ptr->Is_InSymbolTable(this->lineno,this->child->var_name)){
                    this->type = now_ptr->SymTable[this->child->var_name].fDecNode->type;
                    break;
                }
                if(now_ptr==ptr) break;
                now_ptr = now_ptr -> fath;
            }
        }
    }

    // NOTE: maybe cause some problem cuz the income of array
    if(this->nodeType == NODE_EXPR && this->stype == STMT_EXP){
        if(this->optype == OP_LVAL){
            this->type = this->child->type;
        }
    }

    if(this->sibling!=nullptr) this->sibling->Type_Check_SecondTrip(ptr);
    return true;
}

bool TreeNode::Type_Check_ThirdTrip(TreeNode* ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_ThirdTrip(ptr);

    // cerr<<this->nodeID<<endl;
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
                    case OP_QUOTE:{
                        // TODO: type check for pointer
                        this->type = TYPE_INT;
                        break;
                    }
                    case OP_FVALUE:{
                        this->type = TYPE_INT;
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
                        }
                        break;
                    }
                    case OP_LESS:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_GREA:{
                        // cerr<<"hello"<<endl;
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        // cerr<<"goodbye"<<endl;
                        break;
                    }
                    case OP_LE:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_GE:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_EEQ:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_NEQ:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_LAND:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_LOR:{
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_COMMA:{
                        // TODO: OP_COMMA has bugs unfinished, in C++ grammar, if all the sub-comma expr are false,
                        // it will be false;
                        if(this->findChild(1)->type == TYPE_ERROR || this->findChild(2)->type == TYPE_ERROR){
                            this->type = TYPE_ERROR;
                        }
                        else this->type = TYPE_BOOL;
                        break;
                    }
                    case OP_EQ:{
                        // cout<<this->findChild(1)->type->getTypeInfo()<<" "<<this->findChild(2)->type->getTypeInfo()
                        //     <<(*(this->findChild(1)->type) == *(this->findChild(2)->type)) << endl;
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_PLUSEQ:{
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) 
                             ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_MINUSEQ:{
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_MODEQ:{
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_MULEQ:{
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    case OP_DIVEQ:{
                        if((*(this->findChild(1)->type) == *(this->findChild(2)->type))
                            || (this->findChild(1)->type == TYPE_INT && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_CHAR && this->findChild(2)->type->is_can_expandtoint())
                            || (this->findChild(1)->type == TYPE_BOOL && this->findChild(2)->type->is_can_expandtoint()) ){
                            this->type = TYPE_VOID;
                        }
                        else this->type = TYPE_ERROR;
                        break;
                    }
                    default:{
                        ;
                        // cout<<"undefined optype of STMT_EXP of NODE_EXPR in Type_Check_SecondTrip.."<<endl;
                    }
                }
            }
            else{
                ;
                //cout<<"undefined StmtType of NODE_EXPR in Type_Check_ThirdTrip.."<<endl;
            }
            break;
        }
        default:{
            ;
            //cout<<"undefined nodeType in Type_Check_ThirdTrip.."<<endl;
        }
    }

    if(this->type==TYPE_ERROR){
        ret = false;
        cerr<<"expression type accordinate error in lineno "<<this->lineno<<endl;
    }

    if(this->sibling!=nullptr) ret = this->sibling->Type_Check_ThirdTrip(ptr) && ret;
    return ret;
}

bool TreeNode::Type_Check_FourthTrip(TreeNode* ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_FourthTrip(ptr);

    // if(this->nodeType==NODE_EXPR&&this->stype==STMT_EXP)
    if(this->nodeType==NODE_STMT){
        switch(this->stype){
            case STMT_IF:{
                if(this->findChild(1)->type->is_can_shrinktobool()){
                    this->type = this->findChild(2)->type;
                }
                else this->type = TYPE_ERROR;

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong if_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_IFELSE:{
                if(this->findChild(1)->type->is_can_shrinktobool() && 
                    (this->findChild(2)->type!=TYPE_ERROR && this->findChild(3)->type != TYPE_ERROR)){
                    this->type = TYPE_VOID;
                }
                else this->type = TYPE_ERROR;

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong ifelse_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_WHILE:{
                if(this->findChild(1)->type->is_can_shrinktobool()){
                    this->type = this->findChild(2)->type;
                }
                else this->type = TYPE_ERROR;

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong while_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_FOR:{
                if(this->findChild(2)->type == TYPE_VOID &&
                    this->findChild(3)->type->is_can_shrinktobool()&&
                    this->findChild(4)->type == TYPE_VOID){
                    this->type = this->findChild(1)->type;
                }
                else this->type = TYPE_ERROR;

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong for_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_SKIP:{
                this->type = TYPE_VOID;
                break;
            }
            case STMT_BREAK:{
                TreeNode* tmp_ptr = this;
                this->type = TYPE_ERROR;
                while(tmp_ptr){
                    if(tmp_ptr->stype == STMT_FOR || tmp_ptr->stype == STMT_WHILE){
                        this->type = TYPE_VOID;
                        break;
                    }
                    tmp_ptr = tmp_ptr -> fath;
                }

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong break_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_CONTINUE:{
                TreeNode* tmp_ptr = this;
                this->type = TYPE_ERROR;
                while(tmp_ptr){
                    if(tmp_ptr->stype == STMT_FOR || tmp_ptr->stype == STMT_WHILE){
                        this->type = TYPE_VOID;
                        break;
                    }
                    tmp_ptr = tmp_ptr -> fath;
                }

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong continue_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_RETURN:{
                // TODO: didn't finish yet, we need to check the return type & params' types
                // now just check if it is a void function
                TreeNode* tmp_ptr = this;
                bool is_have_return_value = (this->child != nullptr);

                this->type = TYPE_ERROR;
                while(tmp_ptr){
                    if(tmp_ptr->nodeType == NODE_FUNC){
                        if(tmp_ptr->type->retType == VALUE_VOID && is_have_return_value){
                            this->type = TYPE_ERROR;
                            cerr<<"[error] void func can't have return value.."<<endl;
                            break;
                        }
                        this->type = TYPE_VOID;
                        break;
                    }
                    tmp_ptr = tmp_ptr -> fath;
                }

                if(this->type == TYPE_ERROR){
                    cerr<<"[error] wrong return_stmt grammar in lineno "<<this->lineno<<endl;
                }
                break;
            }
            case STMT_BLOCK:{
                TreeNode* tmp_ptr = this->child;
                this->type = TYPE_VOID;
                while(tmp_ptr){
                    if(tmp_ptr->type == TYPE_ERROR){
                        cerr<<"[error] wrong grammar in lineno "<<tmp_ptr->lineno<<endl; 
                        this->type = TYPE_ERROR;
                        break;
                    }
                    tmp_ptr = tmp_ptr -> sibling;
                }
                break;
            }
            case STMT_VARDECL:{
                // cuz we have finish undef & dupdef check in last trip
                this->type = TYPE_VOID;
                break;
            }
            case STMT_CONSTDECL:{
                // cuz we didn't support with no assignment in yacc, so don't need check in constdecl
                this->type = TYPE_VOID;
                break;
            }
            // case STMT_STRUCTDECL:{
            //     // TODO: in v2.0 we have to finish this
            //     break;
            // }
            default:{
                cout<<"we didn't support this type of statement yet.."<<endl;
            }
        }
    }
    else if(this->nodeType==NODE_FUNC){
        this->checktype = this->findChild(3)->type;
    }
    else if(this->nodeType==NODE_PROG){
        TreeNode* tmp_ptr = this->child;
        this->type = TYPE_VOID;
        while(tmp_ptr){
            if(tmp_ptr->nodeType == NODE_STMT){
                if(tmp_ptr->stype == STMT_VARDECL || tmp_ptr->stype == STMT_CONSTDECL){
                    if(tmp_ptr->type == TYPE_ERROR){
                        cerr<<"[error] mistake remaining in this decl in lineno "<<tmp_ptr->lineno<<" .."<<endl;
                        this->type = TYPE_ERROR;
                        break;
                    }
                }
            }
            else if(tmp_ptr->nodeType == NODE_FUNC){
                if(tmp_ptr->checktype == TYPE_ERROR){
                    cerr<<"[error] mistake remaining in function "<<tmp_ptr->var_name<<" .."<<endl;
                    this->type = TYPE_ERROR;
                    break;
                }
            }
            else{
                cout<<"u shall not reach here in Type_Check_FourthTrip.."<<endl;
            }
            tmp_ptr = tmp_ptr -> sibling;
        }

        if(this->type == TYPE_ERROR){
            ret = false;
            cerr<<"[error] mistake remaining in this program.."<<endl;
        }

        //cout<<this->type->getTypeInfo()<<endl;
    }

    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check_FourthTrip(ptr);
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
        case NODE_CONST:   return "NODE_CONST";
        case NODE_VAR:     return "NODE_VAR";
        case NODE_EXPR:    return "NODE_EXPR";
        case NODE_TYPE:    return "NODE_TYPE";

        case NODE_OP:      return "NODE_OP";
        case NODE_ITEM:    return "NODE_ITEM";
        case NODE_LIST:    return "NODE_LIST";
        case NODE_FUNC:    return "NODE_FUNC";
        case NODE_FUNCALL: return "NODE_FUNCALL";

        case NODE_STMT:    return "NODE_STMT";
        case NODE_PROG:    return "NODE_PROG";
        case NODE_AUTH:    return "NODE_AUTH";
        case NODE_ARRAY:   return "NODE_ARRAY";

        case NODE_ASSIST:  return "NODE_ROOT-1";
    }
    return "unknown type";
}

string TreeNode::iType2String (ItemType type){
    switch(type){
        case ITEM_DECL:   return "ITEM_DECL";
        case ITEM_SPF:    return "ITEM_SPF";
        case ITEM_UFUNC:  return "ITEM_UFUNC";
        case ITEM_ARRAY:  return "ITEM_ARRAT";
    }
    return "unknown type";
}

string TreeNode::opType2String (OperatorType type){
    switch(type){
        case OP_POINT:   return ". ";
        case OP_EQ:      return "= ";
        case OP_PLUS:    return "+ ";
        case OP_MINUS:   return "- ";
        case OP_FPLUS:   return "+f";
        case OP_FMINUS:  return "-f";
        case OP_QUOTE:   return "& ";
        case OP_FVALUE:  return "*f";
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

string TreeNode::new_constring_name(){
    string ret = "LC.";
    ret = ret + std::to_string(TreeNode::conststring_cnt);
    TreeNode::conststring_cnt += 1;
    return ret;
}

void TreeNode::gen_constring_decl(ostream &asmo){
    if(this->child!=nullptr) this->child->gen_constring_decl(asmo);
    this->asmo_constring(asmo);
    if(this->sibling!=nullptr) this->sibling->gen_constring_decl(asmo);
}

void TreeNode::asmo_constring(ostream &asmo){
    if(this->nodeType == NODE_CONST && *(this->type) == *TYPE_STRING){
        this->var_name = this->new_constring_name();
        asmo<<this->var_name<<":"<<endl;
        asmo<<"\t.string\t"<<this->str_val<<endl;
    }
}

void TreeNode::gen_intervar(TreeNode* t){
    // bottom -> top
    if(this->child!=nullptr) this->child->gen_intervar(t);

    TreeNode* ptr = this;
    this->calc_thisscope_var_space();
    this->calc_local_var_size();

    if(ptr->nodeType == NODE_EXPR){
        switch(ptr->optype){
            case OP_PLUS:
            case OP_MINUS:
            case OP_MUL:
            case OP_DIV:
            case OP_MOD:
            case OP_LESS:
            case OP_GREA:
            case OP_LE:
            case OP_GE:
            case OP_EEQ:
            case OP_NEQ:
            case OP_LAND:
            case OP_LOR:{
                if(ptr->findChild(1)->nodeType == NODE_EXPR 
                    || ptr->findChild(1)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
                if(ptr->findChild(2)->nodeType == NODE_EXPR
                    || ptr->findChild(2)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
                TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
                break;
            }
            case OP_FPLUS:
            case OP_FMINUS:
            case OP_NOT:
            case OP_QUOTE:
            case OP_FVALUE:{
                if(ptr->findChild(1)->nodeType == NODE_EXPR 
                    || ptr->findChild(1)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
                TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
                break;
            }
            case OP_PLUSEQ:
            case OP_MINUSEQ:
            case OP_MODEQ:
            case OP_MULEQ:
            case OP_DIVEQ:{
                if(ptr->findChild(2)->nodeType == NODE_EXPR
                    || ptr->findChild(2)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
                break;
            }
            case OP_EQ:{
                // NOTE: maybe OP_EQ's return num can be used as a expr to reassign
                if(ptr->findChild(2)->nodeType == NODE_EXPR 
                    || ptr->findChild(2)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
                // ptr->intervar_num = TreeNode::localvar_cnt;
                // TreeNode::localvar_cnt += 1;
                // TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
                break;
            }

            case OP_LVAL:{
                break;
            }
            default:{
                cout<<endl<<"we don't support this optype: "<<ptr->opType2String(ptr->optype)<<endl;
            }
        }
    }
    else if(ptr->nodeType == NODE_ITEM){
        if(ptr->itype == ITEM_DECL){
            if(ptr->optype == OP_EQ){
                if(ptr->findChild(2)->nodeType == NODE_EXPR
                    || ptr->findChild(2)->nodeType == NODE_ARRAY)
                    TreeNode::localvar_cnt -= 1;
            }
        }
    }
    else if(ptr->nodeType == NODE_ARRAY){
        // generate interval register for array
        TreeNode* ptr_item = ptr->findChild(2);
        while(ptr_item){
            if(ptr->findChild(1)->nodeType == NODE_EXPR
                || ptr->findChild(1)->nodeType == NODE_ARRAY)
                TreeNode::localvar_cnt -= 1;
            ptr_item = ptr_item -> sibling;
        }
        ptr->intervar_num = TreeNode::localvar_cnt;
        TreeNode::localvar_cnt += 1;
        TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
    }
    else if(ptr->nodeType == NODE_FUNCALL && 
        (ptr->var_name != "scanf" && ptr->var_name != "printf")){

        TreeNode* ptr_item = ptr->findChild(2)->findChild(1);
        // cuz scanf or printf's first param is not a expr
        while(ptr_item){
            if(ptr_item->nodeType == NODE_ITEM 
                && ptr_item->itype == ITEM_UFUNC){
                if(ptr->findChild(1)->nodeType == NODE_EXPR
                    || ptr->findChild(1)->nodeType == NODE_ARRAY){
                    TreeNode::localvar_cnt -= 1;
                }
            }
            ptr_item = ptr_item -> sibling;
        }
        if(ptr->var_name != "scanf" && ptr->var_name != "printf"){
            ptr->intervar_num = TreeNode::localvar_cnt;
            TreeNode::localvar_cnt += 1;
            TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
        }   
    }

    if(this->sibling!=nullptr) this->sibling->gen_intervar(t);
}

bool TreeNode::is_have_intervar(){
    return this->nodeType == NODE_ARRAY 
        || this->nodeType == NODE_FUNCALL;
}

void TreeNode::gen_label(TreeNode* root_ptr){
    if(this!=root_ptr){
        cerr<<"non-root node shall not call gen_label func.."<<endl;
    }
    // this->label.begin_label = "main";

    TreeNode* compunit_ptr = this->child;

    // for exery stmt & expr in func generate labels
    while(compunit_ptr){
        TreeNode* aim_node = compunit_ptr;
        if(aim_node->nodeType == NODE_FUNC){
            // THIS IS A BLOCK NODE
            aim_node = aim_node->findChild(3);
            aim_node->gen_rec_stmtorexpr_label();
        }
        compunit_ptr = compunit_ptr -> sibling;
    }
}

string TreeNode::new_label(){
    string ret = "";
    ret = ret + "label" + std::to_string(TreeNode::label_cnt);
    TreeNode::label_cnt += 1;
    return ret;
}

void TreeNode::gen_rec_stmtorexpr_label(){
    if(this->nodeType == NODE_STMT){
        this->gen_stmt_label(this);
    }
    else if(this->nodeType == NODE_EXPR){
        this->gen_expr_label(this);
    }
}

void TreeNode::gen_stmt_label(TreeNode* t){
    // cout<<1<<endl;
    if(this->nodeType!=NODE_STMT) return;

    switch(t->stype){
        case STMT_WHILE:{

            TreeNode* ptr_cond = t->findChild(1);
            TreeNode* ptr_stmt = t->findChild(2);

            // maybe begin label was made by left_sibling already
            if(t->label.begin_label == "")
                t->label.begin_label = this->new_label();

            // label cond's true label & false label
            if(t->label.next_label == ""){
                t->label.next_label = t->new_label();
            }
            ptr_cond->label.false_label = t->label.next_label;
            ptr_cond->label.true_label = ptr_stmt->label.begin_label = ptr_cond->new_label();

            // label stmt's next_label as t.begin label
            ptr_stmt->label.next_label = t->label.begin_label;

            // deliver next label(maybe new in this node) to sibling's begin_label
            if(t->sibling!=nullptr){
                t->sibling->label.begin_label = t->label.next_label;
            }

            // recursion
            ptr_cond->gen_rec_stmtorexpr_label();
            ptr_stmt->gen_rec_stmtorexpr_label();

            break;
        }
        case STMT_IF:{

            TreeNode* ptr_cond = t->findChild(1);
            TreeNode* ptr_stmt = t->findChild(2);

            // begin label will print out of this func
            if(t->label.begin_label == "")
                t->label.begin_label = t->new_label();

            // label cond's true label & false label
            ptr_cond->label.true_label = ptr_stmt->label.begin_label = ptr_cond->new_label();
            if(t->label.next_label == "")
                t->label.next_label = t->new_label();
            ptr_cond->label.false_label = t->label.next_label;

            // stmt's next_label
            ptr_stmt->label.next_label = t->label.next_label;

            // deliver next label(maybe new in this node)
            if(t->sibling!=nullptr){
                t->sibling->label.begin_label = t->label.next_label;
            }

            // recursion
            ptr_cond->gen_rec_stmtorexpr_label();
            ptr_stmt->gen_rec_stmtorexpr_label();

            break;
        }
        case STMT_IFELSE:{
            TreeNode* ptr_cond = t->findChild(1);
            TreeNode* ptr_firstmt = t->findChild(2);
            TreeNode* ptr_secstmt = t->findChild(3);

            // begin label will print out of this func
            if(t->label.begin_label == "")
                t->label.begin_label = t->new_label();

            // label cond's true label & false label
            ptr_cond->label.true_label = ptr_firstmt->label.begin_label = ptr_cond->new_label();
            ptr_cond->label.false_label = ptr_secstmt->label.begin_label = ptr_cond->new_label();

            // generate next label
            if(t->label.next_label == "")
                t->label.next_label = t->new_label();
            ptr_firstmt->label.next_label = ptr_secstmt->label.next_label = t->label.next_label;  

            // deliver next label
            if(t->sibling!=nullptr){
                t->sibling->label.begin_label = t->label.next_label;
            }

            // recursion
            ptr_cond->gen_rec_stmtorexpr_label();
            ptr_firstmt->gen_rec_stmtorexpr_label();
            ptr_secstmt->gen_rec_stmtorexpr_label();

            break;
        }
        case STMT_FOR:{
            TreeNode* ptr_expr1 = t->findChild(2);
            TreeNode* ptr_expr2 = t->findChild(3);
            TreeNode* ptr_expr3 = t->findChild(4);
            TreeNode* ptr_stmt = t->findChild(1);

            if(t->label.begin_label == "")
                t->label.begin_label = t->new_label();
            ptr_expr1->label.begin_label = t->label.begin_label;

            // label s2.begin_label
            ptr_expr2->label.begin_label = ptr_expr1->label.next_label = new_label();

            // label cond(expr2)'s true label & false label
            ptr_expr2->label.true_label = ptr_stmt->label.begin_label = ptr_expr2->new_label();
            if(t->label.next_label == "")
                t->label.next_label = t->new_label();
            ptr_expr2->label.false_label = t->label.next_label;

            // NOTE: after run stmt_code, must run expr3 codes
            ptr_expr3->label.next_label = ptr_expr2->label.begin_label;

            // NOTE: very important
            ptr_expr3->label.begin_label = ptr_stmt->label.next_label = new_label();

            // deliver next label(maybe new in this node)
            if(t->sibling!=nullptr){
                t->sibling->label.begin_label = t->label.next_label;
            }

            ptr_expr1->gen_rec_stmtorexpr_label();
            ptr_expr2->gen_rec_stmtorexpr_label();
            ptr_expr3->gen_rec_stmtorexpr_label();
            ptr_stmt->gen_rec_stmtorexpr_label();

            break;
        }
        case STMT_BLOCK:{
            if(t->label.begin_label != "" && t->findChild(1)!=nullptr){
                t->findChild(1)->label.begin_label = t->label.begin_label;
            }

            TreeNode* ptr_rightmostchild = t->findrightmostchild();
            if(t->label.next_label=="")
                t->label.next_label = new_label();
            if(ptr_rightmostchild!=nullptr){
                ptr_rightmostchild->label.next_label = t->label.next_label;
            }

            // if(t->sibling!=nullptr){
            //     t->sibling->label.begin_label = t->label.next_label;
            // }

            TreeNode* aim_node = t->findChild(1);
            while(aim_node){
                aim_node->gen_rec_stmtorexpr_label();
                aim_node = aim_node->sibling;
            }
            break;
        }
        case STMT_BREAK:{
            // find the pointer to the loop node
            TreeNode* loop_ptr = this;
            while(loop_ptr){
                if(loop_ptr->stype == STMT_FOR || loop_ptr->stype == STMT_WHILE){
                    break;
                }
                loop_ptr = loop_ptr -> fath;
            }

            this->label.next_label = loop_ptr->label.next_label;

            break;
        }
        case STMT_CONTINUE:{
            // find the pointer to the loop node
            TreeNode* loop_ptr = this;
            while(loop_ptr){
                if(loop_ptr->stype == STMT_FOR || loop_ptr->stype == STMT_WHILE){
                    break;
                }
                loop_ptr = loop_ptr -> fath;
            }

            if(loop_ptr->stype == STMT_FOR){
                // forstmt's first child is stmt
                this->label.next_label = loop_ptr->findChild(1)->label.next_label;
            }
            else if(loop_ptr->stype == STMT_WHILE){
                // while's second child is stmt
                this->label.next_label = loop_ptr->findChild(2)->label.next_label;
            }

            break;
        }
        default:{
            ;
        }
    }
}

void TreeNode::gen_expr_label(TreeNode* t){
    if(this->nodeType!=NODE_EXPR || !(*(this->type) == *TYPE_BOOL)){
        return;
    }
    
    TreeNode* child1 = t->findChild(1);
    TreeNode* child2 = t->findChild(2);

    switch(t->optype){
        case OP_LAND:{
            // cerr<<1<<endl;
            child1->label.true_label = child1->new_label();
            child2->label.true_label = t->label.true_label;
            child1->label.false_label = t->label.false_label;
            child2->label.false_label = t->label.false_label;
            break;
        }
        case OP_LOR:{
            child1->label.false_label = child1->new_label();
            child2->label.false_label = t->label.false_label;
            child1->label.true_label = t->label.true_label;
            child2->label.true_label = t->label.true_label;
            break;
        }
        default:{
            ;
        }
    }

    if(child1!=nullptr)
        child1->gen_rec_stmtorexpr_label();
    if(child2!=nullptr)
        child2->gen_rec_stmtorexpr_label();
}

void TreeNode::gen_code(ostream &asmo,TreeNode* root_ptr){
    if(this == root_ptr) this->gen_asm_header(asmo);

    TreeNode* child_ptr = root_ptr->child;
    asmo<<"\t.bss"<<endl;
    // generate all the intermediate vars decl in global
    this->gen_intervardecl_code(asmo);

    // generate global variable declare
    asmo<<endl<<"# define global vars here"<<endl;
    asmo<<"\t.data"<<endl;
    while(child_ptr){
        if(child_ptr->stype == STMT_VARDECL
            || child_ptr->stype == STMT_CONSTDECL){
            child_ptr->gen_glob_decl(asmo,child_ptr);
        }
        child_ptr = child_ptr -> sibling;
    }
    
    // recursive generate constring
    asmo<<"# my const string here"<<endl;
    asmo<<"\t.section\t.rodata"<<endl;
    this->gen_constring_decl(asmo);

    asmo<<endl<<endl<<"# my asm code here"<<endl;
    // asmo<<"\t.text"<<endl<<"\t.globl main"<<endl;
    // asmo<<"\t.type\tmain, @function"<<endl<<endl;
    // recursive generate code
    this->gen_rec_code(asmo,root_ptr);
    if(root_ptr->label.next_label!=""){
        asmo<<root_ptr->label.next_label<<":"<<endl;
    }
}

void TreeNode::gen_asm_header(ostream &asmo){
    asmo<<"# my asm code header here"<<endl;
}

void TreeNode::gen_glob_decl(ostream &asmo,TreeNode* t){
    // NOTE: the TreeNode ptr t is a NODE_PROG's child
    // tmp_ptr point to first vardeclitem
    TreeNode* tmp_ptr = t->child->sibling;
    // asmo<<tmp_ptr->nodeID<<endl;
    // asmo<<t->child->type->getTypeInfo()<<endl;
    if(*(t->child->type) == *TYPE_INT || *(t->child->type) == *TYPE_CHAR){
        // generate global decl from first item
        while(tmp_ptr){
            // to judge if it's a single var
            if(tmp_ptr->child->nodeType == NODE_VAR){
                asmo<<"_"<<tmp_ptr->child->var_name<<":"<<endl;
                asmo<<"\t.long\t";
                if(tmp_ptr->child->sibling){
                    asmo<<tmp_ptr->child->sibling->int_val<<endl;
                }
                else asmo<<0<<endl;
                asmo<<"\t.zero\t4"<<endl;
                asmo<<"\t.align\t4"<<endl;
            }
            // to judge if it's an array node
            else if(tmp_ptr->child->nodeType == NODE_ARRAY){
                TreeNode* array_ptr = tmp_ptr->child;
                TreeNode* temp_ptr = array_ptr->findChild(2);
                
                int tot = 1;
                while(temp_ptr){
                    tot = tot * (temp_ptr -> findChild(1) -> int_val);
                    temp_ptr = temp_ptr -> sibling;
                }

                asmo<<"_"<<array_ptr->findChild(1)->var_name<<":"<<endl;
                asmo<<"\t.long\t0"<<endl;
                asmo<<"\t.zero\t"<<tot*INT_SIZE<<endl;
                asmo<<"\t.align\t4"<<endl;
            }
            // traverse
            tmp_ptr = tmp_ptr->sibling;
        }
    }
    asmo<<endl;
}

void TreeNode::gen_intervardecl_code(ostream &asmo){
    for(int i=0;i<=TreeNode::max_localvar_cnt;i++){
        asmo<<"_lc"<<i<<":"<<endl;
        asmo<<"\t.long\t0"<<endl;
        asmo<<"\t.zero\t4"<<endl;
        asmo<<"\t.align\t4"<<endl;
        asmo<<endl;
    }
}

void TreeNode::gen_rec_code(ostream &asmo,TreeNode* t){
    // generate every node begin_label, if exist
    // if(t->label.begin_label!="")
    //     asmo<<t->label.begin_label<<":"<<endl;

    if(t->nodeType == NODE_STMT){
        t->gen_stmt_code(asmo,t);
    }
    else if(t->nodeType == NODE_EXPR){
        t->gen_expr_code(asmo);
    }
    else if(t->nodeType == NODE_FUNC){
        t->gen_func_code(asmo);
    }
    else if(t->nodeType == NODE_PROG){
        t->gen_prog_code(asmo,t);
    }
    else if(t->nodeType == NODE_FUNCALL){
        t->gen_funcall_code(asmo);
    }
    else if(t->nodeType == NODE_ARRAY){
        t->gen_array_code(asmo);
    }
}

void TreeNode::gen_localdec_code(ostream &asmo){
    // need to search from symboltable, since the symboltable must be on
    if(!this->IsSymbolTableOn()){
        return;
    }
    // // only can be called by a func_node, cuz while a scope switched, don't mean will change ebp again
    // if(this->nodeType!=NODE_FUNC){
    //     return;
    // }

    int var_num = this->local_var_size;
    // calc the local space need to allocate
    // TODO: can expand by struct or something
    int local_apply_space = ceil((var_num * INT_SIZE + STACK_RESERVE_SIZE) * 1.0 / SCOPE_MINIMUM_SIZE) * SCOPE_MINIMUM_SIZE;

    asmo<<"\tpushl\t"<<"%ebp"<<endl;
    asmo<<"\tmovl\t"<<"%esp, %ebp"<<endl;
    asmo<<"\tsubl\t$"<<local_apply_space<<", %esp"<<endl<<endl;
}

void TreeNode::gen_stmt_code(ostream &asmo,TreeNode* t){
    if(t->nodeType != NODE_STMT){
        return;
    }

    if(t->stype == STMT_VARDECL){
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;

        TreeNode* declitem_ptr = t -> findChild(2);
        while(declitem_ptr){
            TreeNode* declitem_id_ptr = declitem_ptr -> findChild(1);
            TreeNode* declitem_expr_ptr = declitem_ptr -> findChild(2);

            if(declitem_expr_ptr){
                if(declitem_expr_ptr -> nodeType == NODE_CONST){
                    asmo<<"\tmovl\t$"<<declitem_expr_ptr->int_val<<", %eax"<<endl;
                    asmo<<"\tmovl\t%eax, "<<declitem_id_ptr->lookup_locglosymtab()<<endl<<endl;
                }
                else{
                    // generate expr_ptr asm code
                    declitem_expr_ptr -> gen_rec_code(asmo,declitem_expr_ptr);

                    // mov expr_ptr's interval reg's num to eax
                    // asmo<<"\txorl\t%eax, %eax"<<endl;
                    asmo<<"\tmovl\t_lc"<<declitem_expr_ptr->intervar_num<<", %eax"<<endl;
                    asmo<<"\tmovl\t%eax, "<<declitem_id_ptr->lookup_locglosymtab()<<endl<<endl;
                }
            }

            declitem_ptr = declitem_ptr -> sibling;
        }   
    }
    else if(t->stype == STMT_IF){
        TreeNode* cond_ptr = t -> findChild(1);
        TreeNode* stmt_ptr = t -> findChild(2);

        // begin label
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;

        if(cond_ptr -> nodeType == NODE_EXPR){
            cond_ptr->gen_rec_code(asmo,cond_ptr);
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<cond_ptr->int_val<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<cond_ptr->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(cond_ptr -> is_have_intervar()){
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }

        asmo<<"\tcmpl\t$0, %eax"<<endl;
        asmo<<"\tje\t"<<cond_ptr->label.false_label<<endl<<endl;
        stmt_ptr->gen_rec_code(asmo,stmt_ptr);
    }
    else if(t->stype == STMT_IFELSE){
        TreeNode* cond_ptr = t -> findChild(1);
        TreeNode* stmt1_ptr = t -> findChild(2);
        TreeNode* stmt2_ptr = t -> findChild(3);

        // begin label will print out of this func
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;

        if(cond_ptr -> nodeType == NODE_EXPR){
            cond_ptr->gen_rec_code(asmo,cond_ptr);
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<cond_ptr->int_val<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<cond_ptr->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(cond_ptr -> is_have_intervar()){
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }

        asmo<<"\tcmpl\t$0, %eax"<<endl;
        asmo<<"\tje\t"<<cond_ptr->label.false_label<<endl<<endl;

        asmo<<cond_ptr->label.true_label<<":"<<endl;
        stmt1_ptr->gen_rec_code(asmo,stmt1_ptr);
        asmo<<"\tjmp\t"<<t->label.next_label<<endl<<endl;

        asmo<<cond_ptr->label.false_label<<":"<<endl;
        stmt2_ptr->gen_rec_code(asmo,stmt2_ptr);
    }
    else if(t->stype == STMT_BLOCK){
        TreeNode* blockitem_ptr = t -> findChild(1);
        while(blockitem_ptr){
            blockitem_ptr->gen_rec_code(asmo,blockitem_ptr);
            blockitem_ptr = blockitem_ptr -> sibling;
        }
    }
    else if(t->stype == STMT_WHILE){
        TreeNode* cond_ptr = this->findChild(1);
        TreeNode* stmt_ptr = this->findChild(2);

        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;
        
        if(cond_ptr -> nodeType == NODE_EXPR){
            cond_ptr->gen_rec_code(asmo,cond_ptr);
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<cond_ptr->int_val<<", %eax"<<endl;

        }
        else if(cond_ptr -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<cond_ptr->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(cond_ptr -> is_have_intervar()){
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }

        asmo<<"\tcmpl\t$0, %eax"<<endl;
        asmo<<"\tje\t"<<cond_ptr->label.false_label<<endl<<endl;
        stmt_ptr->gen_rec_code(asmo,stmt_ptr);
        asmo<<"\tjmp\t"<<t->label.begin_label<<endl;
    }
    else if(t->stype == STMT_FOR){
        TreeNode* ptr_expr1 = t->findChild(2);
        TreeNode* ptr_expr2 = t->findChild(3);
        TreeNode* ptr_expr3 = t->findChild(4);
        TreeNode* ptr_stmt = t->findChild(1);

        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;

        ptr_expr1->gen_rec_code(asmo,ptr_expr1);
        asmo<<ptr_expr2->label.begin_label<<":"<<endl;

        if(ptr_expr2 -> nodeType == NODE_EXPR){
            ptr_expr2->gen_rec_code(asmo,ptr_expr2);
            asmo<<"\tmovl\t_lc"<<ptr_expr2->intervar_num<<", %eax"<<endl;
        }
        else if(ptr_expr2 -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<ptr_expr2->int_val<<", %eax"<<endl;
        }
        else if(ptr_expr2 -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<ptr_expr2->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(ptr_expr2 -> is_have_intervar()){
            asmo<<"\tmovl\t_lc"<<ptr_expr2->intervar_num<<", %eax"<<endl;
        }

        asmo<<"\tcmpl\t$0, %eax"<<endl;
        asmo<<"\tje\t"<<ptr_expr2->label.false_label<<endl;
        //asmo<<ptr_expr2->label.true_label<<":"<<endl;
        ptr_stmt->gen_rec_code(asmo,ptr_stmt);
        ptr_expr3->gen_rec_code(asmo,ptr_expr3);
        asmo<<"\tjmp\t"<<ptr_expr2->label.begin_label<<endl;
        asmo<<endl;
    }
    else if(t->stype == STMT_BREAK || t->stype == STMT_CONTINUE){
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;

        asmo<<"\tjmp\t"<<t->label.next_label<<endl;
    }
    else if(t->stype == STMT_RETURN){
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;
        // LEVEL3 func expand to return expr
        // maybe return void
        asmo<<"# return.."<<endl;
        if(t->child){
            TreeNode* ptr_expr = t->child;
            if(ptr_expr -> nodeType == NODE_EXPR){
                ptr_expr->gen_rec_code(asmo,ptr_expr);
                asmo<<"\tmovl\t_lc"<<ptr_expr->intervar_num<<", %eax"<<endl;
            }
            else if(ptr_expr -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_expr->int_val<<", %eax"<<endl;
            }
            else if(ptr_expr -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_expr->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_expr -> is_have_intervar()){
                ptr_expr->gen_rec_code(asmo,ptr_expr);
                asmo<<"\tmovl\t_lc"<<ptr_expr->intervar_num<<", %eax"<<endl;
            }
        }
        else{
            asmo<<"\txorl\t%eax, %eax"<<endl;
        }
        asmo<<"\tmovl\t%ebp, %esp"<<endl;
        asmo<<"\tpopl\t%ebp"<<endl;
        asmo<<"\tret"<<endl<<endl;
    }
    else{
        // begin label will print out of this func
        if(t->label.begin_label!="")
            asmo<<t->label.begin_label<<":"<<endl;
    }
}

void TreeNode::gen_expr_code(ostream &asmo){
    if(this->nodeType != NODE_EXPR) return;

    if(this->label.begin_label!="")
        asmo<<this->label.begin_label<<":"<<endl;

    switch(this->optype){
        case OP_PLUS:
        case OP_MINUS:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            if(this->optype == OP_MINUS) asmo<<"\tsubl";
            else if(this->optype == OP_PLUS) asmo<<"\taddl";

            if(ptr_param2 -> nodeType == NODE_EXPR){
                asmo<<"\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_MOD:
        case OP_DIV:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tcltd"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %ecx"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %ecx"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %ecx"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %ecx"<<endl;
            }

            asmo<<"\tidivl\t%ecx"<<endl;

            if(this->optype == OP_DIV)
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            else if(this->optype == OP_MOD)
                asmo<<"\tmovl\t%edx, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_MUL:{

            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;
            // asmo<<"\txorl\t%edx, %edx"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            if(ptr_param2 -> nodeType == NODE_EXPR){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %edx"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %edx"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %edx"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %edx"<<endl;
            }

            asmo<<"\timull\t%edx, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_FPLUS:
        case OP_FMINUS:
        case OP_NOT:{
            TreeNode* ptr_param1 = this->findChild(1);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);

            if(ptr_param1 -> nodeType == NODE_EXPR){
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_div code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }


            if(this->optype == OP_FPLUS){
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            }
            else if(this->optype == OP_FMINUS){
                asmo<<"\tnegl\t%eax"<<endl;
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            }
            else if(this->optype == OP_NOT){
                asmo<<"\tcmpl\t$0, %eax"<<endl;
                asmo<<"\tsete\t%al"<<endl;
                asmo<<"\tmovzbl\t%al, %eax"<<endl;
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            }
            break;
        }
        case OP_QUOTE:{
            // TODO: to finish type check
            TreeNode* ptr_param1 = this->findChild(1);
            if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tleal\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_ARRAY){
                ptr_param1->gen_array_calcaddress_code(asmo);
                asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;   
            }
            else{
                cerr<<"[WARNING] OP_QUOTE param MUST be left-val.."<<endl;
            }          

            break;
        }
        case OP_FVALUE:{
            // TODO: to finish type check 
            TreeNode* ptr_param1 = this->findChild(1);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);

            if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else{
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tmovl\t(%eax), %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;  

            break;
        }
        case OP_LESS:
        case OP_LE:
        case OP_GREA:
        case OP_GE:
        case OP_NEQ:
        case OP_EEQ:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tcmpl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tcmpl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tcmpl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tcmpl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            if(this->optype == OP_EEQ)
                asmo<<"\tsete\t%al"<<endl;
            else if(this->optype == OP_NEQ)
                asmo<<"\tsetne\t%al"<<endl;
            else if(this->optype == OP_LESS)
                asmo<<"\tsetl\t%al"<<endl;
            else if(this->optype == OP_LE)
                asmo<<"\tsetle\t%al"<<endl;
            else if(this->optype == OP_GREA)
                asmo<<"\tsetg\t%al"<<endl;
            else if(this->optype == OP_GE)
                asmo<<"\tsetge\t%al"<<endl;

            asmo<<"\tmovzbl\t%al, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_EQ:{
            // lval expr node ptr
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_lvalparam = ptr_param1->findChild(1);
            // lval_expr '=' lor_expr, gen lor_expr code recursively
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            // asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<"\tmovl\t%eax, "<<ptr_lvalparam->lookup_locglosymtab()<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }
            // asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl<<endl;
            break;
        }
        case OP_PLUSEQ:
        case OP_MINUSEQ:{
            // lval expr node ptr
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_lvalparam = ptr_param1->findChild(1);
            // lval_expr '=' lor_expr, gen lor_expr code recursively
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_plus/minuseq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            // asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            if(this->optype == OP_PLUSEQ)
                asmo<<"\taddl\t%eax, ";
            else if(this->optype == OP_MINUSEQ)
                asmo<<"\tsubl\t%eax, ";

            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<ptr_lvalparam->lookup_locglosymtab()<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }

            asmo<<endl;
            break;
        }
        case OP_DIVEQ:
        case OP_MODEQ:{
            // lval expr node ptr
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_lvalparam = ptr_param1->findChild(1);
            // lval_expr '=' lor_expr, gen lor_expr code recursively
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_div/modeq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            // asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            asmo<<"\tidivl\t";
            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<ptr_lvalparam->lookup_locglosymtab()<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }

            if(this->optype == OP_DIVEQ)
                asmo<<"\tmovl\t%eax, ";
            else if(this->optype == OP_MODEQ)
                asmo<<"\tmovl\t%edx, ";

            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<ptr_lvalparam->lookup_locglosymtab()<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }

            asmo<<endl;
            break;
        }
        case OP_MULEQ:{
            // lval expr node ptr
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_lvalparam = ptr_param1->findChild(1);
            // lval_expr '=' lor_expr, gen lor_expr code recursively
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            // asmo<<"\txorl\t%eax, %eax"<<endl;
            // asmo<<"\txorl\t%edx, %edx"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_muleq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_lvalparam->lookup_locglosymtab()<<", %edx"<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }

            asmo<<"\timull\t%edx, %eax"<<endl;

            if(ptr_lvalparam->nodeType == NODE_VAR){
                asmo<<"\tmovl\t%eax, "<<ptr_lvalparam->lookup_locglosymtab()<<endl;
            }
            else if(ptr_lvalparam->nodeType == NODE_ARRAY){
                // NOTE: i don't know if pop & push eax works, cuz gen_code will corrupt eax!
                ptr_lvalparam->gen_array_store_code(asmo);
            }

            asmo<<endl;
            break;
        }
        case OP_LAND:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);

            if(ptr_param1 -> nodeType == NODE_EXPR){
                ptr_param1->gen_rec_code(asmo,ptr_param1);
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tcmpl\t$0, %eax"<<endl;
            asmo<<"\tje\t"<<this->label.false_label<<"_1"<<endl;
        
            asmo<<ptr_param1->label.true_label<<":"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                ptr_param2->gen_rec_code(asmo,ptr_param2);
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tcmpl\t$0, %eax"<<endl;
            asmo<<"\tje\t"<<this->label.false_label<<"_1"<<endl;

            asmo<<"\tmovl\t$1, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            asmo<<"\tjmp\t"<<this->label.true_label<<endl;

            asmo<<this->label.false_label<<"_1:"<<endl;
            asmo<<"\tmovl\t$0, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            asmo<<"\tjmp\t"<<this->label.false_label<<endl;

            break;
        }
        case OP_LOR:{
            // cerr<<"hello"<<endl;
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);

            if(ptr_param1 -> nodeType == NODE_EXPR){
                ptr_param1->gen_rec_code(asmo,ptr_param1);
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param1 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }

            asmo<<"\tcmpl\t$0, %eax"<<endl;
            asmo<<"\tjne\t"<<this->label.true_label<<"_1"<<endl;
        
            asmo<<ptr_param1->label.false_label<<":"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                ptr_param2->gen_rec_code(asmo,ptr_param2);
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_eeq code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab()<<", %eax"<<endl;
            }
            else if(ptr_param2 -> is_have_intervar()){
                asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            }


            asmo<<"\tcmpl\t$0, %eax"<<endl;
            asmo<<"\tjne\t"<<this->label.true_label<<"_1"<<endl;

            asmo<<"\tmovl\t$0, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            asmo<<"\tjmp\t"<<this->label.false_label<<endl;

            asmo<<this->label.true_label<<"_1:"<<endl;
            asmo<<"\tmovl\t$1, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            asmo<<"\tjmp\t"<<this->label.true_label<<endl;

            break;
        }
        case OP_POINT:
        case OP_LVAL:
        case OP_COMMA:{
            break;
        }
        default:{
            cerr<<"invalid optype in gen_expr_code.."<<endl;
            break;
        }
    }
}

void TreeNode::gen_func_code(ostream &asmo){
    if(this->nodeType!=NODE_FUNC) return;

    asmo<<"\t.text"<<endl<<"\t.globl "<<this->findChild(1)->var_name<<endl;
    asmo<<"\t.type\t"<<this->findChild(1)->var_name<<", @function"<<endl<<endl;
    asmo<<this->findChild(1)->var_name<<":"<<endl;

    if(this->label.begin_label!="")
        asmo<<this->label.begin_label<<":"<<endl;

    TreeNode* block_ptr = this->findChild(3);
    if(block_ptr){
        block_ptr->gen_localdec_code(asmo);

        // generate codes for every stmt in block
        block_ptr->gen_rec_code(asmo,block_ptr);

        asmo<<endl;
    }
}

void TreeNode::gen_funcall_code(ostream &asmo){
    if(this->nodeType != NODE_FUNCALL){
        return;
    }

    if(this->label.begin_label!="")
        asmo<<this->label.begin_label<<":"<<endl;

    if(this->var_name == "printf"){
        TreeNode* strpara_ptr = this->findChild(1);
        TreeNode* printlist_ptr = this->findChild(2);
        int calc = 0;

        if(printlist_ptr){

            TreeNode* printitem_ptr = printlist_ptr -> findChild(1);

            while(printitem_ptr -> sibling){
                printitem_ptr = printitem_ptr -> sibling;
            }

            // push params from the rightmost
            while(printitem_ptr){
                calc += 1;

                TreeNode* itemdata_ptr = printitem_ptr -> findChild(1);

                if(itemdata_ptr -> nodeType == NODE_EXPR){
                    // NOTE: may cause some problem
                    itemdata_ptr -> gen_rec_code(asmo, itemdata_ptr);
                    if(itemdata_ptr->intervar_num != -1){
                        asmo<<"\tmovl\t_lc"<<itemdata_ptr->intervar_num<<", %eax"<<endl;
                    }
                    else cerr<<"error in gen printf code"<<endl;
                }
                else if(itemdata_ptr -> nodeType == NODE_CONST){
                    asmo<<"\tmovl\t$"<<itemdata_ptr->int_val<<", %eax"<<endl;
                }
                else if(itemdata_ptr -> nodeType == NODE_VAR){
                    asmo<<"\tmovl\t"<<itemdata_ptr->lookup_locglosymtab()<<", %eax"<<endl;
                }
                else if(itemdata_ptr -> is_have_intervar()){
                    itemdata_ptr -> gen_rec_code(asmo,itemdata_ptr);
                    asmo<<"\tmovl\t_lc"<<itemdata_ptr->intervar_num<<", %eax"<<endl;
                }
                asmo<<"\tpushl\t%eax"<<endl;

                printitem_ptr = printitem_ptr -> left_sibling;
            }
        }

        asmo<<"\tpushl\t$"<<strpara_ptr->var_name<<endl;
        asmo<<"\tcall\tprintf"<<endl;
        asmo<<"\taddl\t$"<< (calc + 1) * INT_SIZE << ", %esp" << endl << endl;
    }
    else if(this->var_name == "scanf"){
        TreeNode* strpara_ptr = this->findChild(1);
        TreeNode* scanflist_ptr = this->findChild(2);
        int calc = 0;

        if(scanflist_ptr){

            TreeNode* scanfitem_ptr = scanflist_ptr -> findChild(1);

            while(scanfitem_ptr -> sibling){
                scanfitem_ptr = scanfitem_ptr -> sibling;
            }

            // push params from the rightmost
            while(scanfitem_ptr){
                calc += 1;

                // item's first child is a lval expr
                TreeNode* itemlval_ptr = scanfitem_ptr -> findChild(1);
                // lval expr's first child is a NODE_VAR/NODE_ARRAY
                TreeNode* itemdata_ptr = itemlval_ptr -> findChild(1);

                if(itemdata_ptr -> nodeType == NODE_VAR){
                    string ret_str = itemdata_ptr->lookup_locglosymtab();
                    if(ret_str.length()>=6 && ret_str.substr(ret_str.length()-6,6) == "(%ebp)"){
                        asmo<<"\tleal\t"<<ret_str<<", %eax"<<endl;
                    }
                    else{
                        asmo<<"\tmovl\t$"<<ret_str<<", %eax"<<endl;
                    }
                }
                else if(itemdata_ptr -> nodeType == NODE_ARRAY){
                    //youwenti
                    // string ret_str = itemdata_ptr->gen_array_offset_code();
                    // if(ret_str.length()>=6 && ret_str.substr(ret_str.length()-6,6) == "(%ebp)"){
                    //     asmo<<"\tleal\t"<<ret_str<<", %eax"<<endl;
                    // }
                    // else{
                    //     asmo<<"\tmovl\t$"<<ret_str<<", %eax"<<endl;
                    // }
                    cerr<<"didn't support yet"<<endl;
                }

                asmo<<"\tpushl\t%eax"<<endl;

                scanfitem_ptr = scanfitem_ptr -> left_sibling;
            }
        }

        asmo<<"\tpushl\t$"<<strpara_ptr->var_name<<endl;
        asmo<<"\tcall\t__isoc99_scanf"<<endl;
        asmo<<"\taddl\t$"<< (calc + 1) * INT_SIZE << ", %esp" << endl << endl;
    }
    else{
        TreeNode* funcalllist_ptr = this->findChild(2);

        int calc = 0;

        if(funcalllist_ptr){
            TreeNode* funcallitem_ptr = funcalllist_ptr -> findrightmostchild();

            // push params from the rightmost
            while(funcallitem_ptr){
                calc += 1;

                TreeNode* itemdata_ptr = funcallitem_ptr -> findChild(1);

                if(itemdata_ptr -> nodeType == NODE_EXPR){
                    // NOTE: may cause some problem
                    itemdata_ptr -> gen_rec_code(asmo, itemdata_ptr);
                    if(itemdata_ptr->intervar_num != -1){
                        asmo<<"\tmovl\t_lc"<<itemdata_ptr->intervar_num<<", %eax"<<endl;
                    }
                    else cerr<<"error in gen printf code"<<endl;
                }
                else if(itemdata_ptr -> nodeType == NODE_CONST){
                    asmo<<"\tmovl\t$"<<itemdata_ptr->int_val<<", %eax"<<endl;
                }
                else if(itemdata_ptr -> nodeType == NODE_VAR){
                    asmo<<"\tmovl\t"<<itemdata_ptr->lookup_locglosymtab()<<", %eax"<<endl;
                }
                else if(itemdata_ptr -> is_have_intervar()){
                    itemdata_ptr -> gen_rec_code(asmo,itemdata_ptr);
                    asmo<<"\tmovl\t_lc"<<itemdata_ptr->intervar_num<<", %eax"<<endl;
                }
                asmo<<"\tpushl\t%eax"<<endl;

                funcallitem_ptr = funcallitem_ptr -> left_sibling;
            }
        }

        asmo<<"\tcall\t"<<this->var_name<<endl;
        asmo<<"\taddl\t$"<< calc * INT_SIZE << ", %esp" << endl;
        asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
        asmo<<endl;
    }
}

void TreeNode::gen_prog_code(ostream &asmo,TreeNode* t){
    TreeNode* progitem_ptr = t -> findChild(1);

    while(progitem_ptr){
        if(progitem_ptr -> nodeType == NODE_FUNC){
            progitem_ptr -> gen_func_code(asmo);
        }
        progitem_ptr = progitem_ptr -> sibling;
    }
}

void TreeNode::gen_array_code(ostream &asmo){
    if(this->nodeType!=NODE_ARRAY) return;
    TreeNode* ptr_useid = this->findChild(1);// ptr to this id
    TreeNode* temp = ptr_useid;
    bool Is_Global = 0,Is_Param = 0;
    while(temp){
        if(temp->IsSymbolTableOn() 
            && temp->Is_InSymbolTable(ptr_useid->lineno,ptr_useid->var_name)){
            if(temp->nodeType == NODE_PROG){
                Is_Global = 1;
            }
            break;
        }
        temp = temp -> fath;
    }
    TreeNode* ptr_usenode = this;
    TreeNode* ptr_fdecnode = temp->SymTable[ptr_useid->var_name].fDecNode;

    if(ptr_fdecnode->is_param)
        Is_Param = true;

    // clear register eax for interval, ecx for offset counter
    asmo<<"\txorl\t%eax, %eax"<<endl;
    asmo<<"\txorl\t%ecx, %ecx"<<endl;

    TreeNode* iterator_useitem = ptr_usenode->findChild(2);
    TreeNode* iterator_fdecitem = ptr_fdecnode->findChild(2);
    int r = 0;
    while(iterator_useitem){
        asmo<<"# calc array offset"<<endl;
        if(r!=0){
            asmo<<"\tmovl\t%ecx, %eax"<<endl;
            asmo<<"\tmovl\t$"<<iterator_fdecitem->findChild(1)->int_val<<", %edx"<<endl;
            asmo<<"\timull\t%edx, %eax"<<endl;
            asmo<<"\tmovl\t%eax, %ecx"<<endl;
        }
        
        // generate code for expr(if exist)
        TreeNode* ptr_param1 = iterator_useitem->findChild(1);
        if(ptr_param1 -> nodeType == NODE_EXPR){
            if(ptr_param1->intervar_num != -1){
                ptr_param1->gen_rec_code(asmo,ptr_param1);
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else cerr<<"error in gen array code"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(ptr_param1 -> is_have_intervar()){
            ptr_param1->gen_rec_code(asmo,ptr_param1);
            asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
        }
        asmo<<"\taddl\t%eax, %ecx"<<endl;

        iterator_useitem = iterator_useitem -> sibling;
        iterator_fdecitem = iterator_fdecitem -> sibling;
        r += 1;
    }
    
    // if var is in global or local, two conditions
    if(Is_Global){
        asmo<<"\tmovl\t_"<<ptr_useid->var_name<<"(,%ecx,4), %eax"<<endl;
        asmo<<"\tmovl\t%eax, "<<"_lc"<<this->intervar_num<<endl;
    }
    else if(Is_Param){
        asmo<<"\tmovl\t"<<ptr_useid->lookup_locglosymtab()<<", %eax"<<endl;
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\taddl\t%ecx"<<", %eax"<<endl;
        asmo<<"\tmovl\t(%eax), %eax"<<endl;
        asmo<<"\tmovl\t%eax, "<<"_lc"<<this->intervar_num<<endl;
    }
    else{
        // mult offset * 4 to calc real offset (within INT_SIZE)
        // calc local offset in asm
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\taddl\t$4, %ecx"<<endl;
        asmo<<"\taddl\t$"<<to_string(temp->SymTable[ptr_useid->var_name].local_offset)<<", %ecx"<<endl;
        asmo<<"\tsubl\t$"<<to_string(temp->scope_offset)<<", %ecx"<<endl;

        // just to translate movl %ecx(%ebp), _lc xx
        asmo<<"\tmovl\t%ebp, %eax"<<endl;
        asmo<<"\taddl\t%ecx, %eax"<<endl;
        // asmo<<"\tmovl\t(%eax), _lc"<<this->intervar_num<<endl;
        asmo<<"\tmovl\t(%eax), %eax"<<endl;
        asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
    }

    asmo<<endl;
}

void TreeNode::gen_array_store_code(ostream &asmo){
    // before call this function, you must store the number in %eax
    if(this->nodeType!=NODE_ARRAY) return;
    asmo<<"\tpushl\t%eax"<<endl;

    TreeNode* ptr_useid = this->findChild(1);// ptr to this id
    TreeNode* temp = ptr_useid;
    bool Is_Global = 0,Is_Param = 0;
    while(temp){
        // cerr<<temp->nodeID<<endl;
        if(temp->IsSymbolTableOn() 
            && temp->Is_InSymbolTable(ptr_useid->lineno,ptr_useid->var_name)){
            if(temp->nodeType == NODE_PROG){
                Is_Global = 1;
            }
            break;
        }
        temp = temp -> fath;
    }
    TreeNode* ptr_usenode = this;
    TreeNode* ptr_fdecnode = temp->SymTable[ptr_useid->var_name].fDecNode;

    if(ptr_fdecnode->is_param)
        Is_Param = true;

    // clear register eax for interval, ecx for offset counter
    asmo<<"\txorl\t%eax, %eax"<<endl;
    asmo<<"\txorl\t%ecx, %ecx"<<endl;

    TreeNode* iterator_useitem = ptr_usenode->findChild(2);
    TreeNode* iterator_fdecitem = ptr_fdecnode->findChild(2);
    int r = 0;
    while(iterator_useitem){
        asmo<<"# calc array offset"<<endl;
        if(r!=0){
            asmo<<"\tmovl\t%ecx, %eax"<<endl;
            asmo<<"\tmovl\t$"<<iterator_fdecitem->findChild(1)->int_val<<", %edx"<<endl;
            asmo<<"\timull\t%edx, %eax"<<endl;
            asmo<<"\tmovl\t%eax, %ecx"<<endl;
        }
        
        // generate code for expr(if exist)
        TreeNode* ptr_param1 = iterator_useitem->findChild(1);
        if(ptr_param1 -> nodeType == NODE_EXPR){
            if(ptr_param1->intervar_num != -1){
                ptr_param1->gen_rec_code(asmo,ptr_param1);
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else cerr<<"error in gen array code"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(ptr_param1 -> is_have_intervar()){
            ptr_param1->gen_rec_code(asmo,ptr_param1);
            asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
        }
        asmo<<"\taddl\t%eax, %ecx"<<endl;

        iterator_useitem = iterator_useitem -> sibling;
        iterator_fdecitem = iterator_fdecitem -> sibling;
        r += 1;
    }
    
    // popl from the stack, which is originated in %eax, which is the number it wanna store 
    asmo<<"\tpopl\t%edx"<<endl;
    // if var is in global or local, two conditions
    if(Is_Global){
        asmo<<"\tmovl\t%edx, _"<<ptr_useid->var_name<<"(,%ecx,4)"<<endl;
    }
    else if(Is_Param){
        asmo<<"\tmovl\t"<<ptr_useid->lookup_locglosymtab()<<", %eax"<<endl;
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\taddl\t%ecx, %eax"<<endl;
        asmo<<"\tmovl\t%edx, (%eax)"<<endl;
    }
    else{
        // mult offset * 4 to calc real offset (within INT_SIZE)
        // calc local offset in asm
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\taddl\t$4, %ecx"<<endl;
        asmo<<"\taddl\t$"<<to_string(temp->SymTable[ptr_useid->var_name].local_offset)<<", %ecx"<<endl;
        asmo<<"\tsubl\t$"<<to_string(temp->scope_offset)<<", %ecx"<<endl;

        // just to translate movl %ecx(%ebp), _lc xx
        asmo<<"\tmovl\t%ebp, %eax"<<endl;
        asmo<<"\taddl\t%ecx, %eax"<<endl;
        asmo<<"\tmovl\t%edx, (%eax)"<<endl;
    }

    asmo<<endl;
}

void TreeNode::gen_array_calcaddress_code(ostream &asmo){
    if(this->nodeType!=NODE_ARRAY) return;
    TreeNode* ptr_useid = this->findChild(1);// ptr to this id
    TreeNode* temp = ptr_useid;
    bool Is_Global = 0;
    while(temp){
        if(temp->IsSymbolTableOn() 
            && temp->Is_InSymbolTable(ptr_useid->lineno,ptr_useid->var_name)){
            if(temp->nodeType == NODE_PROG){
                Is_Global = 1;
            }
            break;
        }
        temp = temp -> fath;
    }
    TreeNode* ptr_usenode = this;
    TreeNode* ptr_fdecnode = temp->SymTable[ptr_useid->var_name].fDecNode;

    // clear register eax for interval, ecx for offset counter
    asmo<<"\txorl\t%eax, %eax"<<endl;
    asmo<<"\txorl\t%ecx, %ecx"<<endl;

    TreeNode* iterator_useitem = ptr_usenode->findChild(2);
    TreeNode* iterator_fdecitem = ptr_fdecnode->findChild(2);
    while(iterator_useitem){
        asmo<<"# calc array offset"<<endl;
        asmo<<"\tmovl\t%ecx, %eax"<<endl;
        asmo<<"\tmovl\t$"<<iterator_fdecitem->findChild(1)->int_val<<", %edx"<<endl;
        asmo<<"\timull\t%edx, %eax"<<endl;
        asmo<<"\tmovl\t%eax, %ecx"<<endl;
        
        // generate code for expr(if exist)
        TreeNode* ptr_param1 = iterator_useitem->findChild(1);
        if(ptr_param1 -> nodeType == NODE_EXPR){
            if(ptr_param1->intervar_num != -1){
                ptr_param1->gen_rec_code(asmo,ptr_param1);
                asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
            }
            else cerr<<"error in gen array code"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
        }
        else if(ptr_param1 -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab()<<", %eax"<<endl;
        }
        else if(ptr_param1 -> is_have_intervar()){
            ptr_param1->gen_rec_code(asmo,ptr_param1);
            asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
        }
        asmo<<"\taddl\t%eax, %ecx"<<endl;

        iterator_useitem = iterator_useitem -> sibling;
        iterator_fdecitem = iterator_fdecitem -> sibling;
    }

    // if var is in global or local, two conditions
    if(Is_Global){
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\tmovl\t$_"<<ptr_useid->var_name<<", %eax"<<endl;
        asmo<<"\taddl\t%ecx, %eax"<<endl;
    }
    else{
        // mult offset * 4 to calc real offset (within INT_SIZE)
        // calc local offset in asm
        asmo<<"\tshll\t$2, %ecx"<<endl;
        asmo<<"\taddl\t$4, %ecx"<<endl;
        asmo<<"\taddl\t$"<<to_string(temp->SymTable[ptr_useid->var_name].local_offset)<<", %ecx"<<endl;
        asmo<<"\tsubl\t$"<<to_string(temp->scope_offset)<<", %ecx"<<endl;

        // just to translate movl %ecx(%ebp), _lc xx
        asmo<<"\tmovl\t%ebp, %eax"<<endl;
        asmo<<"\taddl\t%ecx, %eax"<<endl;
    }

    asmo<<endl;
}

string TreeNode::lookup_locglosymtab(){
    TreeNode* ptr_temp = this;
    string ret = "";
    // TODO : how to find the var if it was not declare either in this scope or global
    while(ptr_temp){
        if(ptr_temp->IsSymbolTableOn()
            && ptr_temp->Is_InSymbolTable(this->lineno,this->var_name)){
            if(ptr_temp->nodeType == NODE_PROG){
                ret = "_" + this->var_name;
            }
            else{
                // cerr<<this->nodeID<<" "<<this->scope_offset<<endl;
                ret = to_string( - ptr_temp->scope_offset 
                                 + INT_SIZE 
                                 + ptr_temp->SymTable[this->var_name].local_offset) 
                    + "(%ebp)";
            }
            break;
        }
        ptr_temp = ptr_temp -> fath;
    }

    return ret;
}

void TreeNode::gen_offset(TreeNode* rt){
    this->dist_scope_offset();
    this->calc_local_offset();
    if(this->sibling!=nullptr) this->sibling->gen_offset(rt);
    if(this->child!=nullptr) this->child->gen_offset(rt);
}

void TreeNode::gen_params_offset(){
    this->calc_params_offset();
    if(this->child!=nullptr) this->child->gen_params_offset();
    if(this->sibling!=nullptr) this->sibling->gen_params_offset();
}

void TreeNode::calc_params_offset(){
    if(this->nodeType != NODE_FUNC){
        return;
    }
    // cerr<<this->nodeID<<endl;

    TreeNode* ptr_paramslist = this->findChild(2);
    TreeNode* ptr_funcblock = this->findChild(3);

    TreeNode* ptr_item = ptr_paramslist->findChild(1);
    int cnt = 0;
    while(ptr_item){
        TreeNode* ptr_itemdata;
        if(ptr_item->findChild(1)->nodeType == NODE_VAR)
            ptr_itemdata = ptr_item->findChild(1);
        else if(ptr_item->findChild(1)->nodeType == NODE_ARRAY)
            ptr_itemdata = ptr_item->findChild(1)->findChild(1);

        // cerr<<"blk:"<<ptr_funcblock->nodeID<<endl;

        varItem sym_item =  ptr_funcblock->SymTable[ptr_itemdata->var_name];
        // cerr<<"before:"<<sym_item.local_offset<<endl;
        sym_item.local_offset = ptr_funcblock->scope_offset
                              + STACK_RESERVE_SIZE
                              + cnt * INT_SIZE
                              - INT_SIZE;
        // cerr<<"after calc:"<<sym_item.local_offset<<endl;
        ptr_funcblock->SymTable[ptr_itemdata->var_name] = sym_item;
        // cerr<<"after written:"<<ptr_funcblock->SymTable[ptr_itemdata->var_name].local_offset<<endl;

        cnt++;
        ptr_item = ptr_item -> sibling;
    }
}

void TreeNode::calc_local_offset(){
    // calc the local offset of vars in current symboltable
    // TODO: you shall change this to support other types
    if(!this->IsSymbolTableOn()) return;

    int tmp_cnt = 0;
    for(map<string,varItem>::iterator it = this->SymTable.begin();it!=SymTable.end();it++){
        // it->second.local_offset = tmp_cnt * INT_SIZE;
        // tmp_cnt += 1;
        // add local offset to now item in symboltable
        it->second.local_offset = tmp_cnt * INT_SIZE;

        // calc the next local offset now item by judge last item's fDecNode's NODETYPE
        TreeNode* fdec_ptr = it->second.fDecNode;
        if(fdec_ptr->nodeType == NODE_VAR){
            tmp_cnt += 1;
        }
        else if(fdec_ptr->nodeType == NODE_ARRAY){
            int this_size = 1;
            TreeNode* fdecitem_ptr = fdec_ptr -> findChild(2);
            while(fdecitem_ptr){
                this_size *= fdecitem_ptr -> findChild(1) -> int_val;
                fdecitem_ptr = fdecitem_ptr -> sibling;
            }
            tmp_cnt += this_size;
        }
    }
}

void TreeNode::calc_local_var_size(){
    if(this->IsSymbolTableOn()){
        // this->local_var_size += this->SymTable.size();
        // LEVEL2: change to calc array size
        int tot_var_size = 0;
        for(map<string,varItem>::iterator it = this->SymTable.begin(); it!=this->SymTable.end(); it++){
            if(it->second.is_param) continue;

            TreeNode* fdec_ptr = it->second.fDecNode;
            if(fdec_ptr->nodeType == NODE_VAR){
                tot_var_size += 1;
            }
            else if(fdec_ptr->nodeType == NODE_ARRAY){
                int this_size = 1;
                TreeNode* fdecitem_ptr = fdec_ptr -> findChild(2);
                while(fdecitem_ptr){
                    this_size *= fdecitem_ptr -> findChild(1) -> int_val;
                    fdecitem_ptr = fdecitem_ptr -> sibling;
                }
                tot_var_size += this_size;
            }
        }
        this->local_var_size += tot_var_size;
    }
    if(this->nodeType != NODE_PROG && this->nodeType != NODE_FUNC){
        this->fath->local_var_size = 
        max(ceil(this->fath->local_var_size * 1.0 / INT_SIZE) * INT_SIZE,
            ceil(this->local_var_size * 1.0 / INT_SIZE) * INT_SIZE);
    }
}

void TreeNode::calc_thisscope_var_space(){
    if(this->IsNeedSwitchScope() && this->IsSymbolTableOn()){
        // int var_num = this->SymTable.size();
        // // calc the local space need to allocate
        // // TODO: can expand by struct or something
        // this->thisscope_var_space = ceil((var_num * INT_SIZE + STACK_RESERVE_SIZE) * 1.0 / 16) * 16;
        // LEVEL2: change to calc array size
        // LEVEL3: ignore params while calculating
        int tot_space = 0;
        for(map<string,varItem>::iterator it = this->SymTable.begin(); it!=this->SymTable.end(); it++){
            if(it->second.is_param) continue;

            TreeNode* fdec_ptr = it->second.fDecNode;
            if(fdec_ptr->nodeType == NODE_VAR){
                tot_space += INT_SIZE;
            }
            else if(fdec_ptr->nodeType == NODE_ARRAY){
                int this_space = 1;
                TreeNode* fdecitem_ptr = fdec_ptr -> findChild(2);
                while(fdecitem_ptr){
                    this_space *= fdecitem_ptr -> findChild(1) -> int_val;
                    fdecitem_ptr = fdecitem_ptr -> sibling;
                }
                tot_space += this_space * INT_SIZE;
            }
        }
        this->thisscope_var_space = ceil((tot_space + STACK_RESERVE_SIZE) * 1.0 / 16) * 16;
    }
}

void TreeNode::dist_scope_offset(){

    if(this->IsNeedSwitchScope()){
        this->scope_offset += this->thisscope_var_space;
    }

    TreeNode* child_ptr = this->findChild(1);
    while(child_ptr){
        child_ptr->scope_offset += this->scope_offset;
        child_ptr = child_ptr -> sibling;
    }
}
