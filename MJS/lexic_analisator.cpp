#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <exception>
#include <cstring>

using namespace std;

enum type_of_lex
{
    LEX_NULL,       // 0
    LEX_VAR,        //1
    LEX_TRUE,       //2
    LEX_FALSE,      //3
    LEX_UNDEFINED,  //4
    LEX_TYPEOF,     //5
    LEX_FUNCTION,   //6
    LEX_IF,         //7
    LEX_ELSE,       //8
    LEX_WHILE,      //9
    LEX_FOR,        //10
    LEX_DO,         //11
    LEX_BREAK,      //12
    LEX_CONTINUE,   //13
    LEX_RETURN,     //14
    LEX_SEMICOLON,  //15
    LEX_QUOTE,      //16
    LEX_SINGLE_QUOTE,//17
    LEX_PLUS,       //18
    LEX_MINUS,      //19
    LEX_MULT,       //20
    LEX_SLASH,      //21
    LEX_LESS,       //22
    LEX_GREATER,    //23
    LEX_NOT,        //24
    LEX_PERCENT,    //25
    LEX_POINT,      //26
    LEX_COMMA,      //27
    LEX_L_SQ_BRACKET, //28
    LEX_R_SQ_BRACKET, //29
    LEX_L_F_BRACKET, //30
    LEX_R_F_BRACKET, //31
    LEX_L_R_BRACKET, //32
    LEX_R_R_BRACKET, //33
    LEX_EQ,         //34
    LEX_IFEQ,       //35
    LEX_NEQ,        //36
    LEX_LEQ,        //37
    LEX_GEQ,        //38
    LEX_INC,        //39
    LEX_DEC,        //40
    LEX_AND,        //41
    LEX_OR,         //42
    LEX_SHEILD_QUATE, //43
    LEX_SHEILD_SINGLE_QUATE, //44
    LEX_DOUBLE_SHEILD, //45
    LEX_PLUS_EQ,    //46
    LEX_MINUS_EQ,   //47
    LEX_MULT_EQ,    //48
    LEX_SLASH_EQ,   //49
    LEX_PERCENT_EQ, //50
    LEX_ID,         //51
    LEX_NUMBER,     //52
    LEX_STRING,     //53
    LEX_DOUBLE,     //54
    LEX_FLOAT,      //55
    LEX_FIN         //56
};

vector<string> STR;
vector<double> d_numbers;
vector<float> f_numbers;

class lexem {
    type_of_lex t_lex;
    int v_lex;
public:
    lexem(type_of_lex t = LEX_NULL, int v = 0) {
        t_lex = t;
        v_lex = v;
    }
    type_of_lex get_type() { return t_lex; }
    int get_value () { return v_lex; }
    friend ostream& operator << (ostream &s, lexem l) {
        s << '(' << l.t_lex << ',' << l.v_lex << ");";
        return s;
    }
};

class Ident {
    char* name;
    bool declare;
    type_of_lex type;
    bool assign;
    int value;
public:
    Ident() {
        declare = false;
        assign = false;
    }
    char* get_name() {
        return name;
    }
    void put_name(const char* n) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    bool get_declare() { return declare; }
    void put_declare () { declare = true; }
    type_of_lex get_type() { return type; }
    void put_type (type_of_lex t) { type = t; }
    bool get_assign() { return assign; }
    void put_assign() { assign = true; }
    int get_value() { return value; }
    void put_value(int v) { value = v; }
};

class tabl_ident{
    Ident *p;
    int size;
    int top;
public:
    tabl_ident(int max_size) {
        p = new Ident[size = max_size];
        top = 1;
    }
    ~tabl_ident(){
        delete []p;
    }
    Ident& operator[](int k){
        return p[k];
    }  
    int put(const char *buf){
        for (int j = 1; j < top; ++j)
            if (!strcmp(buf, p[j].get_name()))
                return j;
        p[top].put_name(buf);
        ++top;
        return top-1;
    }
};

class Scanner{
    enum state { BEGIN, IDENTIFICATOR, NUMBER, OPERATOR, STRING, ARRAY, OBJECT, DELIM, DOUBLE, FLOAT };
    static char * TW[];
    static type_of_lex words[];
    static char * TD[];
    static type_of_lex dlms[];
    state CS;
    char c;
    char c_begin;
    char buf[80];
    int buf_top;
    void clear() {
        buf_top = 0;
        for (int j = 0; j < 80; ++j)
            buf[j] = '\0';
    }
    void add() {
        buf[buf_top++] = c;   
    }
    int look(const char *buf, char **list) {
        int i = 0;
        while (list[i]) {
            if (!strcmp(buf, list[i]))
            return i;
            ++i;
        }
        return 0;
    }
    void gc() {
        //cout << "gc " << c << endl;
        c = cin.get();
        //cout << "gc " << c << endl;
    }
    public:
    lexem get_lexem();
    Scanner(){
        c_begin = ' ';
        CS = BEGIN;
        clear();
    //    gc();
    }
};

tabl_ident TID(100);


char * Scanner::TW[] =
{
"",              // 0 позиция 0 не используется
"var",          // 1
"true",         // 2
"false",        // 3
"undefined",    // 4
"null",         // 5
"typeof",       // 6
"funclion",     // 7
"if",           // 8
"else",         // 9
"while",        // 10
"for",          // 11
"do",           // 12
"break",        // 13
"continue",     // 14
"return",       // 15
NULL
};
char * Scanner:: TD[] =
{
 "", // 0 позиция 0 не используется
";",        // 1
"\"",       // 2
"'",        // 3
"+",        // 4
"-",       // 5
"*",        // 6
"/",        // 7
"<",        // 8
">",        // 9
"!",        // 10
"%",        // 11
".",        // 12
",",        // 13
"[",        // 14
"]",        //15
 "{",       // 16
"}",        // 17
"(",        // 18
")",        //19
"=",        //20
"==",        //21
"!=",       //22
"<=",       //23
">=",       //24
"++",       //25
"--",       //26
"&&",       //27
"||",       //28
"\\\"",     //29
"\'",       //30
"\\",       //31
"+=",       //32
"-=",       //33
"*=",       //34
"/=",       //35
"%=",       //36
NULL
};


type_of_lex Scanner::words[] = {
    LEX_NULL,
    LEX_VAR,
    LEX_TRUE,
    LEX_FALSE,
    LEX_UNDEFINED,
    LEX_NULL,
    LEX_TYPEOF,
    LEX_FUNCTION,
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_FOR,
    LEX_DO,
    LEX_BREAK,
    LEX_CONTINUE,
    LEX_RETURN,
    LEX_NULL
};
type_of_lex Scanner::dlms[] = {
    LEX_NULL,           //0
    LEX_SEMICOLON,      //1
    LEX_QUOTE,          //2
    LEX_SINGLE_QUOTE,   //3
    LEX_PLUS,           //4
    LEX_MINUS,          //5
    LEX_MULT,           //6
    LEX_SLASH,          //7
    LEX_LESS,           //8
    LEX_GREATER,        //9
    LEX_NOT,            //10
    LEX_PERCENT,        //11
    LEX_POINT,          //12
    LEX_COMMA,          //13
    LEX_L_SQ_BRACKET,   //14
    LEX_R_SQ_BRACKET,   //15
    LEX_L_F_BRACKET,    //16
    LEX_R_F_BRACKET,    //17
    LEX_L_R_BRACKET,    //18
    LEX_R_R_BRACKET,    //19
    LEX_EQ,             //20
    LEX_IFEQ,           //21
    LEX_NEQ,            //22
    LEX_LEQ,            //23
    LEX_GEQ,            //24
    LEX_INC,            //25
    LEX_DEC,            //26
    LEX_AND,            //27
    LEX_OR,             //28
    LEX_SHEILD_QUATE,   //29
    LEX_SHEILD_SINGLE_QUATE, //30
    LEX_DOUBLE_SHEILD,  //31
    LEX_PLUS_EQ,        //32
    LEX_MINUS_EQ,       //33
    LEX_MULT_EQ,        //34
    LEX_SLASH_EQ,       //35
    LEX_PERCENT_EQ,     //36
    LEX_NULL            //37
};

lexem Scanner::get_lexem() {
    string str = "";
    int d, j, neg;
    double x,n;
    float y;
    //cout << c << endl;
    CS = BEGIN;
    if (c_begin == ' ')
        gc();
    else {
        c = c_begin;
        c_begin = ' ';
    }
    try{
        do {
            switch ( CS ){
                case BEGIN:
                    if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' )
                        gc ();
                    else if (isalpha(c)) {
                        clear();
                        add();
                        gc();
                        CS = IDENTIFICATOR;
                    }
                    else if (isdigit(c)){
                        d = c - '0';
                        gc();
                        CS = NUMBER;
                    }
                    else if (c == '"' || c == '\'') {
                        gc();
                        CS = STRING;
                    }
                    else if ( c== '=' || c== '!' || c== '<' ||  c== '>' || c== '+' || 
                                c== '-' || c== '&' || c== '|' || c== '\\' || c == '*' || c == '/' || c == '%'){
                        clear();
                        add();
                        gc();
                        CS = OPERATOR;
                    }
                    else if (c == EOF) {
                        return lexem(LEX_FIN);
                    }
                    else {
                        CS = DELIM;
                    }
                    break;
                case IDENTIFICATOR:
                    if (isalpha(c) || isdigit(c) || c =='_'){
                        add();
                        gc();
                    }
                    else if (j = look (buf, TW)){
                        c_begin = c;
                        return lexem(words[j], j);
                    } else {
                        c_begin = c;
                        j = TID.put(buf);
                        return lexem(LEX_ID, j);
                    }
                    break;
                case NUMBER:
                    if (isdigit(c)){
                        d = d * 10 + (c - '0');
                        gc();
                    } 
                    else if(c == '.') {
                        x = d;
                        n = 1;
                        gc();
                        CS = DOUBLE;
                    }
                    else {
                        c_begin = c;
                        return lexem(LEX_NUMBER, d);
                    }
                    break;
                case DOUBLE:
                    if (isdigit(c)){
                        x += (c - '0')*pow(10,-n);
                        n++;
                        gc();
                    } else if (c == 'e' || c == 'E') {
                        y = x;
                        gc();
                        if (c == '-') {
                            neg = 1;
                            gc();
                        } else {
                            neg = 0;
                        }
                        d = c - '0';
                        gc();
                        CS = FLOAT;
                    } else {
                        c_begin = c;
                        cout << x << endl;
                        d_numbers.push_back(x);
                        return lexem(LEX_DOUBLE, d_numbers.size());
                    }
                    break;
                case FLOAT:
                    if (isdigit(c)){
                        d = 10*d + (c - '0');
                        gc();
                    } else {
                        c_begin = c;
                        if (neg) {
                            y = x*pow(10, -d);
                        } else {
                            y = x*pow(10, d);
                        }
                        f_numbers.push_back(y);
                        cout << y << endl;
                        return lexem(LEX_FLOAT, f_numbers.size());
                    }
                    break;
                case STRING:
                    if ((c == '"') || (c == '\'')) {
                        STR.push_back(str);
                        return lexem(LEX_STRING, STR.size());
                    } else {
                        if (c != '\\') {
                            str+=c;
                            gc();
                        } else {
                            gc();
                        }
                        if (c == EOF) {
                            //cout << str << endl;
                            throw("error in string");
                        }
                    }
                    break;            
                case OPERATOR:
                    if (c == '=' || c == '+' || c == '-' || c == '&' || c == '|') {
                        add();
                        gc();
                        c_begin = c;
                        j = look (buf, TD);
                        return lexem(dlms[j], j);
                    } else {
                        c_begin = c;
                        j = look(buf, TD);
                        return lexem(dlms[j], j);
                    }
                    break;
                case DELIM:
                    //cout << "DELIM " << c << endl;
                    clear();
                    add();
                    if (j = look(buf, TD)){
                        gc ();
                        c_begin = c;
                        return lexem(dlms[j], j);
                    } else
                        throw c;
                    break;
                } // end switch
            }
        while ( true );        
    }
    catch (char const* s) {
        cout << s << endl;
    }
    catch (char c) {
        cout << "incorrect sympol: " << c << endl;
    }
}

int main() {
    Scanner s;
    lexem x;
    vector<lexem> v;
    x = s.get_lexem();
    v.push_back(x);
    while (x.get_type() != LEX_FIN) {
        //cout << x.get_type() << endl;
        x = s.get_lexem();
        v.push_back(x);
    }
    for (int i = 0; i < v.size(); ++i){
        cout << v[i] << endl;
    }
    return 0;
}