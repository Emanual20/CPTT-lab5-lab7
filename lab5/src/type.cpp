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