#include "tree.h"

const int INT_SIZE = 4;
const int ALLOC_MINIMUM_SIZE = 4;
const int STACK_RESERVE_SIZE = 8;
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
        cout<<"invalid offset param < 0.."<<endl;
        return nullptr;
    }
    TreeNode* tmp_ptr = this->child;
    for(int i=1;i<offset;i++){
        if(!(tmp_ptr->sibling)){
            cerr<<"invalid offset param "<<offset<<" in findChild.."<<endl;
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

    cout<<setw(11)<<this->nodeType2String(this->nodeType)<<" ";

    if(this->fath){
        cout<<setw(8)<<" fath:@"<<this->fath->nodeID<<" ";
    }

    this->printSpecialInfo();

    if(this->IsSymbolTableOn()) this->printSymbolTable();

    cout<<" ["<<this->local_var_size<<" "<<thisscope_var_space<<" "<<scope_offset<<"]";

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
    if(this->IsNeedSwitchScope()){
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

    // gen_identifier_types
    root_ptr->Type_Check_SecondTrip(root_ptr);

    // expr type check
    if(!root_ptr->Type_Check_ThirdTrip(root_ptr)){
        cout<<"expr accordinate error"<<endl;
        return -1;
    }

    // stmt type check
    if(!root_ptr->Type_Check_FourthTrip(root_ptr)){
        cout<<"stmt accordinate error"<<endl;
        return -1;
    }

    return 1;
}

bool TreeNode::Type_Check_FirstTrip(TreeNode* root_ptr){
    bool ret = true;
    if(this->child!=nullptr) ret = ret && this->child->Type_Check_FirstTrip(root_ptr);

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
    if(this->sibling!=nullptr) ret = ret && this->sibling->Type_Check_FirstTrip(root_ptr);
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
                        if(this->findChild(1)->type->is_can_shrinktobool()&&this->findChild(2)->type->is_can_shrinktobool()){
                            this->type = TYPE_BOOL;
                        }
                        else this->type = TYPE_ERROR;
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
                        //cout<<"undefined optype of STMT_EXP of NODE_EXPR in Type_Check_SecondTrip.."<<endl;
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
                // TODO: in v2.0 we have to finish this
                break;
            }
            case STMT_STRUCTDECL:{
                // TODO: in v2.0 we have to finish this
                break;
            }
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
        case NODE_FUNCALL: return "NODE_FUNCCALL";

        case NODE_STMT:    return "NODE_STMT";
        case NODE_PROG:    return "NODE_PROG";
        case NODE_AUTH:    return "NODE_AUTH";

        case NODE_ASSIST:  return "NODE_ROOT-1";
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

string TreeNode::new_constring_name(){
    string ret = "LC.";
    ret = ret + std::to_string(TreeNode::conststring_cnt);
    TreeNode::conststring_cnt += 1;
    return ret;
}

void TreeNode::gen_constring_decl(ostream &asmo, TreeNode* t){
    if(this->child!=nullptr) this->child->gen_constring_decl(asmo,this->child);
    this->asmo_constring(asmo,this);
    if(this->sibling!=nullptr) this->sibling->gen_constring_decl(asmo,this->sibling);
}

void TreeNode::asmo_constring(ostream &asmo, TreeNode* t){
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
                if(ptr->findChild(1)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
                if(ptr->findChild(2)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
                TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
                break;
            }
            case OP_FPLUS:
            case OP_FMINUS:
            case OP_NOT:{
                if(ptr->findChild(1)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
                TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
                break;
            }
            case OP_EQ:
            case OP_PLUSEQ:
            case OP_MINUSEQ:
            case OP_MODEQ:
            case OP_MULEQ:
            case OP_DIVEQ:{
                if(ptr->findChild(2)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
                TreeNode::max_localvar_cnt = max(TreeNode::max_localvar_cnt, TreeNode::localvar_cnt);
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
    else if(this->nodeType == NODE_ITEM){
        if(this->itype == ITEM_DECL){
            if(this->optype == OP_EQ){
                if(this->findChild(2)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
            }
        }
    }

    if(this->sibling!=nullptr) this->sibling->gen_intervar(t);
}

void TreeNode::gen_label(TreeNode* root_ptr){
    if(this!=root_ptr){
        cerr<<"non-root node shall not call gen_label func.."<<endl;
    }
    this->label.begin_label = "_start";

    TreeNode* compunit_ptr = this->child;

    // for exery stmt & expr in func generate labels
    while(compunit_ptr){
        TreeNode* aim_node = compunit_ptr;
        if(aim_node->nodeType == NODE_FUNC){
            // THIS IS A BLOCK NODE
            aim_node = aim_node->findChild(3);
            // THIS IS THE FIRST NODE IN BLOCKLIST
            aim_node = aim_node->findChild(1);
            while(aim_node){
                // cerr<<aim_node->nodeID<<endl;
                aim_node->gen_rec_stmtorexpr_label(aim_node);
                aim_node = aim_node->sibling;
            }
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

void TreeNode::gen_rec_stmtorexpr_label(TreeNode* t){
    if(t->nodeType == NODE_STMT){
        this->gen_stmt_label(t);
    }
    else if(t->nodeType == NODE_EXPR){
        this->gen_expr_label(t);
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
            ptr_cond->gen_rec_stmtorexpr_label(ptr_cond);
            ptr_stmt->gen_rec_stmtorexpr_label(ptr_stmt);

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
            ptr_cond->gen_rec_stmtorexpr_label(ptr_cond);
            ptr_stmt->gen_rec_stmtorexpr_label(ptr_stmt);

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
            ptr_firstmt->label.next_label = ptr_secstmt->label.next_label = ptr_firstmt->new_label();

            // deliver next label
            if(t->sibling!=nullptr){
                t->sibling->label.begin_label = t->label.next_label;
            }

            // recursion
            ptr_cond->gen_rec_stmtorexpr_label(ptr_cond);
            ptr_firstmt->gen_rec_stmtorexpr_label(ptr_firstmt);
            ptr_secstmt->gen_rec_stmtorexpr_label(ptr_secstmt);

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
        child1->gen_rec_stmtorexpr_label(child1);
    if(child2!=nullptr)
        child2->gen_rec_stmtorexpr_label(child2);
}

void TreeNode::gen_code(ostream &asmo,TreeNode* root_ptr){
    if(this == root_ptr) this->gen_asm_header(asmo,root_ptr);

    TreeNode* child_ptr = root_ptr->child;
    asmo<<"\t.bss"<<endl;
    // generate all the intermediate vars decl in global
    this->gen_intervardecl_code(asmo,this);

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
    this->gen_constring_decl(asmo,this);

    asmo<<endl<<endl<<"# my asm code here"<<endl;
    asmo<<"\t.text"<<endl<<"\t.globl main"<<endl;
    asmo<<"\t.type\tmain, @function"<<endl<<endl;
    asmo<<"main:"<<endl;
    // recursive generate code
    this->gen_rec_code(asmo,root_ptr);
    if(root_ptr->label.next_label!=""){
        asmo<<root_ptr->label.next_label<<":"<<endl;
    }
}

void TreeNode::gen_asm_header(ostream &asmo,TreeNode* t){
    asmo<<"# my asm code header here"<<endl;
}

void TreeNode::gen_glob_decl(ostream &asmo,TreeNode* t){
    // NOTE: the TreeNode ptr t is a NODE_PROG's child
    // tmp_ptr point to first vardeclitem
    TreeNode* tmp_ptr = t->child->sibling;
    // asmo<<tmp_ptr->nodeID<<endl;
    // asmo<<t->child->type->getTypeInfo()<<endl;
    if(*(t->child->type) == *TYPE_INT){
        while(tmp_ptr){
            asmo<<"_"<<tmp_ptr->child->var_name<<":"<<endl;
            asmo<<"\t.long\t";
            if(tmp_ptr->child->sibling){
                asmo<<tmp_ptr->child->sibling->int_val<<endl;
            }
            else asmo<<0<<endl;
            asmo<<"\t.zero\t4"<<endl;
            asmo<<"\t.align\t4"<<endl;
            tmp_ptr = tmp_ptr->sibling;
        }
    }
    asmo<<endl;
}

void TreeNode::gen_intervardecl_code(ostream &asmo, TreeNode* t){
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
    if(t->label.begin_label!="")
        asmo<<t->label.begin_label<<":"<<endl;

    if(t->nodeType == NODE_STMT){
        t->gen_stmt_code(asmo,t);
    }
    else if(t->nodeType == NODE_EXPR){
        t->gen_expr_code(asmo,t);
    }
    else if(t->nodeType == NODE_FUNC){
        t->gen_func_code(asmo,t);
    }
    else if(t->nodeType == NODE_PROG){
        t->gen_prog_code(asmo,t);
    }
    else if(t->nodeType == NODE_FUNCALL){
        t->gen_funcall_code(asmo,t);
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
    // TODO: can expand by struct or function or something
    int local_apply_space = ceil((var_num * INT_SIZE + STACK_RESERVE_SIZE) * 1.0 / 16) * 16;

    asmo<<"\tpushl\t"<<"%ebp"<<endl;
    asmo<<"\tmovl\t"<<"%esp, %ebp"<<endl;
    asmo<<"\tsubl\t$"<<local_apply_space<<", %esp"<<endl<<endl;
}

void TreeNode::recover_localdec_stack(ostream &asmo){
    // TODO: in level 2 we will change eax into ret params
    asmo<<endl;
    asmo<<"\tmovl\t$0, %eax"<<endl;
    asmo<<"\tmovl\t%ebp, %esp"<<endl;
    asmo<<"\tpopl\t%ebp"<<endl;
}

void TreeNode::gen_stmt_code(ostream &asmo,TreeNode* t){
    if(t->nodeType != NODE_STMT){
        return;
    }

    if(t->stype == STMT_VARDECL){
        TreeNode* declitem_ptr = t -> findChild(2);
        while(declitem_ptr){
            TreeNode* declitem_id_ptr = declitem_ptr -> findChild(1);
            TreeNode* declitem_expr_ptr = declitem_ptr -> findChild(2);

            if(declitem_expr_ptr){
                if(declitem_expr_ptr -> nodeType == NODE_CONST){
                    // asmo<<"\txorl\t%eax, %eax"<<endl;
                    asmo<<"\tmovl\t$"<<declitem_expr_ptr->int_val<<", %eax"<<endl;
                    asmo<<"\tmovl\t%eax, "<<declitem_id_ptr->lookup_locglosymtab(declitem_id_ptr)<<endl<<endl;
                }
                else{
                    // generate expr_ptr asm code
                    declitem_expr_ptr -> gen_rec_code(asmo,declitem_expr_ptr);

                    // mov expr_ptr's interval reg's num to eax
                    // asmo<<"\txorl\t%eax, %eax"<<endl;
                    asmo<<"\tmovl\t_lc"<<declitem_expr_ptr->intervar_num<<", %eax"<<endl;
                    asmo<<"\tmovl\t%eax, "<<declitem_id_ptr->lookup_locglosymtab(declitem_id_ptr)<<endl<<endl;
                }
            }

            declitem_ptr = declitem_ptr -> sibling;
        }   
    }
    else if(t->stype == STMT_IF){
        TreeNode* cond_ptr = t -> findChild(1);
        TreeNode* stmt_ptr = t -> findChild(2);

        // begin label will print out of this func
        // if(t->label.begin_label!="")
        //     asmo<<t->label.begin_label<<":"<<endl;

        // TODO: pack these codes to a function may be explicit
        if(cond_ptr -> nodeType == NODE_EXPR){
            cond_ptr->gen_rec_code(asmo,cond_ptr);
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<cond_ptr->int_val<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<cond_ptr->lookup_locglosymtab(cond_ptr)<<endl;
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
        // if(t->label.begin_label!="")
        //     asmo<<t->label.begin_label<<":"<<endl;

        // TODO: pack these codes to a function may be explicit
        if(cond_ptr -> nodeType == NODE_EXPR){
            cond_ptr->gen_rec_code(asmo,cond_ptr);
            asmo<<"\tmovl\t_lc"<<cond_ptr->intervar_num<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_CONST){
            asmo<<"\tmovl\t$"<<cond_ptr->int_val<<", %eax"<<endl;
        }
        else if(cond_ptr -> nodeType == NODE_VAR){
            asmo<<"\tmovl\t"<<cond_ptr->lookup_locglosymtab(cond_ptr)<<endl;
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
    // if (t->kind_kind == COMP_STMT)
    // {
    //     for (int i = 0; t->children[i]; i++)
    //     {
    //         recursive_gen_code(out, t->children[i]);
    //         for (Node *p = t->children[i]->sibling; p; p = p->sibling)
    //             recursive_gen_code(out, p);
    //     }
    // }
    // else if (t->kind_kind == WHILE_STMT)
    // {
    //     if (t->label.begin_label != "")
    //         out << t->label.begin_label << ":" << endl;
    //     recursive_gen_code(out, t->children[0]);
    //     recursive_gen_code(out, t->children[1]);
    //     out << "\tjmp " << t->label.begin_label << endl;
    // }
    // /* ... */
}

void TreeNode::gen_expr_code(ostream &asmo,TreeNode* t){
    if(this->nodeType != NODE_EXPR) return;

    switch(this->optype){
        case OP_PLUS:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_plus code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\taddl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_plus code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\taddl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\taddl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<", %eax"<<endl;
            }

            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_MINUS:{
            TreeNode* ptr_param1 = this->findChild(1);
            TreeNode* ptr_param2 = this->findChild(2);
            if(ptr_param1) ptr_param1->gen_rec_code(asmo,ptr_param1);
            if(ptr_param2) ptr_param2->gen_rec_code(asmo,ptr_param2);

            asmo<<"\txorl\t%eax, %eax"<<endl;

            if(ptr_param1 -> nodeType == NODE_EXPR){
                if(ptr_param1->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param1->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_minus code"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param1->int_val<<", %eax"<<endl;
            }
            else if(ptr_param1 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tsubl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
                }
                else cerr<<"error in gen op_minus code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tsubl\t$"<<ptr_param2->int_val<<", %eax"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tsubl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<", %eax"<<endl;
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

            asmo<<"\txorl\t%eax, %eax"<<endl;

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
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            asmo<<"\tcltd"<<endl;

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tidivl\t_lc"<<ptr_param2->intervar_num<<endl;
                }
                else cerr<<"error in gen op_div code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tidivl\t$"<<ptr_param2->int_val<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tidivl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<endl;
            }

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

            asmo<<"\txorl\t%eax, %eax"<<endl;
            asmo<<"\txorl\t%edx, %edx"<<endl;

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
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            if(ptr_param2 -> nodeType == NODE_EXPR){
                if(ptr_param2->intervar_num != -1){
                    asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %edx"<<endl;
                }
                else cerr<<"error in gen op_div code"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_CONST){
                asmo<<"\tmovl\t$"<<ptr_param2->int_val<<", %edx"<<endl;
            }
            else if(ptr_param2 -> nodeType == NODE_VAR){
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<", %edx"<<endl;
            }

            asmo<<"\timull\t%edx, %eax"<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;

            break;
        }
        case OP_FPLUS:{

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
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl;
            break;
        }
        case OP_FMINUS:{

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
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
            }

            asmo<<"\tnegl\t%eax"<<endl;
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

            asmo<<"\txorl\t%eax, %eax"<<endl;

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
                asmo<<"\tmovl\t"<<ptr_param1->lookup_locglosymtab(ptr_param1)<<", %eax"<<endl;
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
                asmo<<"\tcmpl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<", %eax"<<endl;
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

            asmo<<"\txorl\t%eax, %eax"<<endl;

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
                asmo<<"\tmovl\t"<<ptr_param2->lookup_locglosymtab(ptr_param2)<<", %eax"<<endl;
            }

            // asmo<<"\tmovl\t_lc"<<ptr_param2->intervar_num<<", %eax"<<endl;
            asmo<<"\tmovl\t%eax, "<<ptr_lvalparam->lookup_locglosymtab(ptr_lvalparam)<<endl;
            asmo<<"\tmovl\t%eax, _lc"<<this->intervar_num<<endl<<endl;
            break;
        }
    }
}

void TreeNode::gen_func_code(ostream &asmo,TreeNode* t){
    if(this->nodeType!=NODE_FUNC) return;

    // TODO: func shall push its param to stack

    TreeNode* block_ptr = t->findChild(3);
    if(block_ptr){
        block_ptr->gen_localdec_code(asmo);

        // generate codes for every stmt in block
        block_ptr->gen_rec_code(asmo,block_ptr);

        block_ptr->recover_localdec_stack(asmo);
        asmo<<"\tret"<<endl<<endl;
    }
}

void TreeNode::gen_funcall_code(ostream &asmo,TreeNode *t){
    // TODO: must finish in level 2, at this stage printf only
    if(this->nodeType != NODE_FUNCALL){
        return;
    }

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
                    asmo<<"\tmovl\t"<<itemdata_ptr->lookup_locglosymtab(itemdata_ptr)<<", %eax"<<endl;
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
                // lval expr's first child is a NODE_VAR
                TreeNode* itemdata_ptr = itemlval_ptr -> findChild(1);

                if(itemdata_ptr -> nodeType == NODE_VAR){
                    string ret_str = itemdata_ptr->lookup_locglosymtab(itemdata_ptr);
                    if(ret_str.substr(ret_str.length()-6,6) == "(%ebp)"){
                        asmo<<"\tleal\t"<<itemdata_ptr->lookup_locglosymtab(itemdata_ptr)<<", %eax"<<endl;
                    }
                    else{
                        asmo<<"\tmovl\t$"<<itemdata_ptr->lookup_locglosymtab(itemdata_ptr)<<", %eax"<<endl;
                    }
                }
                asmo<<"\tpushl\t%eax"<<endl;

                scanfitem_ptr = scanfitem_ptr -> left_sibling;
            }
        }

        asmo<<"\tpushl\t$"<<strpara_ptr->var_name<<endl;
        asmo<<"\tcall\t__isoc99_scanf"<<endl;
        asmo<<"\taddl\t$"<< (calc + 1) * INT_SIZE << ", %esp" << endl << endl;
    }
}

void TreeNode::gen_prog_code(ostream &asmo,TreeNode* t){
    TreeNode* progitem_ptr = t -> findChild(1);

    while(progitem_ptr){
        if(progitem_ptr -> nodeType == NODE_FUNC){
            progitem_ptr -> gen_func_code(asmo,progitem_ptr);
        }
        progitem_ptr = progitem_ptr -> sibling;
    }
}

string TreeNode::lookup_locglosymtab(TreeNode* t){
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
                ret = to_string(- ptr_temp->scope_offset + INT_SIZE +
                    ptr_temp->SymTable[this->var_name].local_offset) 
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
    this->calc_local_offset(rt);
    if(this->sibling!=nullptr) this->sibling->gen_offset(rt);
    if(this->child!=nullptr) this->child->gen_offset(rt);
}

void TreeNode::calc_local_offset(TreeNode* rt){
    // calc the local offset of vars in current symboltable
    // TODO: you shall change this to support other types
    if(!this->IsSymbolTableOn()) return;

    int tmp_cnt = 0;
    for(map<string,varItem>::iterator it = this->SymTable.begin();it!=SymTable.end();it++){
        it->second.local_offset = tmp_cnt * INT_SIZE;
        tmp_cnt += 1;
    }
}

void TreeNode::calc_local_var_size(){
    if(this->IsSymbolTableOn()){
        this->local_var_size += this->SymTable.size();
    }
    if(this->nodeType != NODE_PROG && this->nodeType != NODE_FUNC){
        this->fath->local_var_size = 
        max(ceil(this->fath->local_var_size * 1.0 / 4) * 4,
            ceil(this->local_var_size * 1.0 / 4) * 4);
    }
}

void TreeNode::calc_thisscope_var_space(){
    if(this->IsNeedSwitchScope() && this->IsSymbolTableOn()){
        int var_num = this->SymTable.size();
        // calc the local space need to allocate
        // TODO: can expand by struct or function or something
        this->thisscope_var_space = ceil((var_num * INT_SIZE + STACK_RESERVE_SIZE) * 1.0 / 16) * 16;
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
