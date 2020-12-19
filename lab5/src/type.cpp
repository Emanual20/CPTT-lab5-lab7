#include "type.h"

Type::Type(ValueType valueType) {
    this->type = valueType;
}

string Type::getTypeInfo() {
    switch(this->type) {
        case VALUE_BOOL:       return "bool";
        case VALUE_INT:        return "int";
        case VALUE_CHAR:       return "char";
        case VALUE_STRING:     return "string";
        case VALUE_VOID:       return "void";
        case COMPOSE_STRUCT:   return "u_struct";
        case COMPOSE_FUNCTION: return "u_function";
        case VALUE_ERROR:       return "type error";
        default:
            cerr << "shouldn't reach here, typeinfo";
            assert(0);
    }
    return "?";
}

void Type::addChild(Type* t){
    if(this->st_child==nullptr){
        this->st_child=t;
        return;
    }
    Type* nowchild = this->st_child;
    for(;nowchild->st_sibling!=nullptr;nowchild=nowchild->st_sibling){
        ;
    }
    nowchild->st_sibling = t;
}

bool Type::is_can_expandtoint(){
    return this->type == VALUE_INT || this->type == VALUE_CHAR || this->type == VALUE_BOOL;
}

bool Type::is_can_shrinktobool(){
    return this->type == VALUE_INT || this->type == VALUE_CHAR || this->type == VALUE_BOOL;
}

bool Type::is_string_compatiable(Type* t2){
    return (this->type == VALUE_CHAR && t2->type == VALUE_STRING) ||
           (this->type == VALUE_STRING && t2->type == VALUE_CHAR) ||
           (this->type == VALUE_STRING && t2->type == VALUE_STRING);
}