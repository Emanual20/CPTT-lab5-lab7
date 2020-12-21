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

        cout<<this->type->getTypeInfo()<<endl;
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
        case NODE_AUTH:  return "NODE_AUTH";
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

void TreeNode::gen_intervar(TreeNode* t){
    if(this->child!=nullptr) this->child->gen_intervar(t);

    TreeNode* ptr = this;
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
                break;
            }
            case OP_FPLUS:
            case OP_FMINUS:
            case OP_NOT:{
                if(ptr->findChild(1)->nodeType == NODE_EXPR)
                    TreeNode::localvar_cnt -= 1;
                ptr->intervar_num = TreeNode::localvar_cnt;
                TreeNode::localvar_cnt += 1;
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
                break;
            }
            default:{
                cout<<endl<<"we don't support this optype: "<<ptr->opType2String(ptr->optype)<<endl;
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
    this->gen_rec_stmtorexpr_label(this);
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

}
void TreeNode::gen_expr_label(TreeNode* t){

}

void TreeNode::gen_code(ostream &asmo,TreeNode* root_ptr){
    if(this == root_ptr) this->gen_asm_header(asmo,root_ptr);

    TreeNode* child_ptr = root_ptr->child;
    // generate global variable declare
    asmo<<endl<<"# define global vars here"<<endl;
    asmo<<"\t.bss"<<endl;
    while(child_ptr){
        if(child_ptr->stype == STMT_VARDECL
            || child_ptr->stype == STMT_CONSTDECL){
            child_ptr->gen_glob_decl(asmo,child_ptr);
        }
        child_ptr = child_ptr -> sibling;
    }
    asmo<<endl<<endl<<"# my asm code here"<<endl;
    asmo<<"\t.text"<<endl<<"\t.globl _start"<<endl;
    // recursive generate code
    this->gen_rec_stmtorexpr_code(asmo,root_ptr);
    if(root_ptr->label.next_label!=""){
        asmo<<root_ptr->label.next_label<<":"<<endl;
    }
    asmo<<"\tret"<<endl;
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
}

void TreeNode::gen_rec_stmtorexpr_code(ostream &asmo,TreeNode* t){
    if(t->nodeType == NODE_STMT){
        this->gen_stmt_code(asmo,t);
    }
    else if(t->nodeType == NODE_EXPR){
        this->gen_expr_code(asmo,t);
    }
}

void TreeNode::gen_stmt_code(ostream &asmo,TreeNode* t){
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
    // else if (t->kind_kind == PRINT_STMT)
    // {
    //     /* ... */
    // }
    // /* ... */
}

void TreeNode::gen_expr_code(ostream &asmo,TreeNode* t){
    // Node *e1 = t->children[0];
    // Node *e2 = t->children[1];
    // switch (t->attr.op)
    // {
    // case ASSIGN:
    //     break;

    // case PLUS:
    //     out << "\tmovl $";
    //     if (e1->kind_kind == ID_EXPR)
    //         out << "_" << symtbl.getname(e1->attr.symtbl_seq);
    //     else if (e1->kind_kind == CONST_EXPR)
    //         out << e1->attr.vali;
    //     else out << "t" << e1->temp_var;
    //     out << ", %eax" <<endl;
    //     out << "\taddl $";
    //     if (e2->kind_kind == ID_EXPR)
    //         out << "_" << symtbl.getname(e2->attr.symtbl_seq);
    //     else if (e2->kind_kind == CONST_EXPR)
    //         out << e2->attr.vali;
    //     else out << "t" << e2->temp_var;
    //     out << ", %eax" << endl;
    //     out << "\tmovl %eax, $t" << t->temp_var << endl;
    //     break;
    // case AND:
    //     out << "\t# your own code of AND operation here" << endl;
    //     out << "\tjl @1" << endl;
    //     out << "\t# your asm code of AND operation end" << endl;
    // case LT:
    //     break;
    // /* ... */
    // }
}