#ifndef TYPESYSTEM_H
#define TYPESYSTEM_H
#include "./pch.h"
using namespace std;

enum ValueType
{
    VALUE_BOOL,
    VALUE_INT,
    VALUE_CHAR,
    VALUE_STRING,
    VALUE_VOID,
    COMPOSE_STRUCT,
    COMPOSE_UNION,
    COMPOSE_FUNCTION,
    VALUE_ERROR,
};

class Type
{
public:
    ValueType type;
    Type(ValueType valueType);
public:  
    Type* paramType;
    ValueType retType; // for function
    
    void addChild(Type* t); // has been finish in type.cpp
    void addParam(Type* t);
    void addRet(Type* t);
public:
    Type* st_child = nullptr;
    Type* st_sibling = nullptr; 
public:
    string getTypeInfo();
public:
    bool operator ==(const Type* t){
        if(this->type==VALUE_BOOL||this->type==VALUE_INT
            ||this->type==VALUE_CHAR||this->type==VALUE_STRING
            ||this->type==VALUE_VOID){
            return this->type == t->type;
        }
        else{
            cout<<"we haven't finish compose struct comparision.."<<endl;
        }
    }
    bool is_can_expandtoint();
    bool is_can_shrinktobool();
    bool is_string_compatiable(Type*);
};

static Type* TYPE_INT = new Type(VALUE_INT);
static Type* TYPE_CHAR = new Type(VALUE_CHAR);
static Type* TYPE_BOOL = new Type(VALUE_BOOL);
static Type* TYPE_STRING = new Type(VALUE_STRING);
static Type* TYPE_VOID = new Type(VALUE_VOID);
static Type* TYPE_ERROR = new Type(VALUE_ERROR);
// NOTE: any type of struct user made shall be new in yacc

int getSize(Type* type);

#endif