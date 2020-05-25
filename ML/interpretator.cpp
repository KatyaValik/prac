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

template <class T, int max_size > class Stack{
    T s[max_size];
    int top;
public:
    Stack(){top = 0;}
    void reset() { top = 0; }
    void push(T i);
    T pop();
    bool is_empty(){ return top == 0; }
    bool is_full() { return top == max_size; }
};
template <class T, int max_size >
void Stack <T, max_size >::push(T i) {
    if (!is_full()) {
        s[top] = i;
        ++top;
    } else
        throw "Stack_is_full";
}
template <class T, int max_size >
T Stack <T, max_size >::pop(){
    if (!is_empty()) {
        --top;
        return s[top];
    } else
        throw "Stack_is_empty";
}

enum type_of_lex
{
    LEX_NULL,       // 0
    LEX_PROGRAM,    //1
    LEX_INT,        //2
    LEX_STRING,     //3
    LEX_IF,         //4
    LEX_ELSE,       //5
    LEX_WHILE,      //6
    LEX_READ,       //7
    LEX_WRITE,      //8
    LEX_NOT,        //9
    LEX_AND,        //10
    LEX_OR,         //11
    LEX_CONTINUE,   //12
    LEX_BOOLEAN,    //13
    LEX_TRUE,       //14
    LEX_FALSE,      //15
    LEX_PLUS,           //16
    LEX_MINUS,          //17
    LEX_MULT,           //18
    LEX_SLASH,          //19
    LEX_EQ,             //20
    LEX_LESS,           //21
    LEX_GREATER,        //22
    LEX_L_F_BRACKET,    //23
    LEX_R_F_BRACKET,    //24
    LEX_L_R_BRACKET,    //25
    LEX_R_R_BRACKET,    //26
    LEX_COMMA,          //27
    LEX_SEMICOLON,      //28
    LEX_QUOTE,          //29
    LEX_COMMENT,        //30
    LEX_LEQ,            //31
    LEX_GEQ,            //32
    LEX_IFEQ,           //33
    LEX_NEQ,            //34
    LEX_FIN,            //35
    LEX_INTCONST,       //36       
    LEX_STRCONST,       //37
    LEX_BOOLCONST,      //38
    LEX_ID,             //39
    LEX_LABEL,          //40
    POLIZ_ADDRESS,      //41
    POLIZ_GO,           //42
    POLIZ_FGO,          //43
    POLIZ_LABEL         //44
};


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
    enum state { H, IDENT, NUMB, ALE, DELIM, COMMENT, STRING, BOOL };
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
        CS = H;
        clear();
        gc();
    }
};

tabl_ident TID(100);

vector <string> str;


char * Scanner::TW[] =
{
    "", // 0 позиция 0 не используется
    "program",  // 1
    "int",      //2
    "string",   //3
    "if",       //4
    "else",     //5
    "while",    //6
    "read",     //7
    "write",    //8
    "not",      //9
    "and",      //10
    "or",       //11
    "continue", //12
    "boolean",  //13
    "true",     //14
    "false",    //15
    NULL
};
char * Scanner:: TD[] =
{
    "",     // 0 позиция 0 не используется
    "+",    //1
    "-",    //2
    "*",    //3
    "/",    //4
    "=",    //5
    "<",    //6
    ">",    //7
    "{",    //8
    "}",    //9
    "(",    //10
    ")",    //11
    ",",    //12
    ";",    //13
    "\"",   //14
    "#",    //15
    "<=",   //16
    ">=",   //17
    "==",   //18
    "!=",   //19
NULL
};


type_of_lex Scanner::words[] = {
    LEX_NULL,       //0
    LEX_PROGRAM,    //1
    LEX_INT,        //2
    LEX_STRING,     //3
    LEX_IF,         //4
    LEX_ELSE,       //5
    LEX_WHILE,      //6
    LEX_READ,       //7
    LEX_WRITE,      //8
    LEX_NOT,        //9
    LEX_AND,        //10
    LEX_OR,         //11
    LEX_CONTINUE,   //12
    LEX_BOOLEAN,    //13
    LEX_TRUE,       //14
    LEX_FALSE,      //15
    LEX_NULL,       
};

type_of_lex Scanner::dlms[] = {
    LEX_NULL,           //0
    LEX_PLUS,           //1
    LEX_MINUS,          //2
    LEX_MULT,           //3
    LEX_SLASH,          //4
    LEX_EQ,             //5
    LEX_LESS,           //6
    LEX_GREATER,        //7
    LEX_L_F_BRACKET,    //8
    LEX_R_F_BRACKET,    //9
    LEX_L_R_BRACKET,    //10
    LEX_R_R_BRACKET,    //11
    LEX_COMMA,          //12
    LEX_SEMICOLON,      //13
    LEX_QUOTE,          //14
    LEX_COMMENT,        //15
    LEX_LEQ,            //16
    LEX_GEQ,            //17
    LEX_IFEQ,           //18
    LEX_NEQ,            //19
    LEX_NULL
};

lexem Scanner::get_lexem (){
    char minus[2];
    minus[0] = '-';
    minus[1] = 0;
    int d, j;
    string s, s1;
    CS = H;
    do { 
        switch (CS) {
        case H:
            if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' )
                gc ();
            else if (isalpha(c)) {
                clear();
                add();
                gc();
                CS = IDENT;
            }
            else if (isdigit(c)) {
                d = c - '0';
                gc ();
                CS = NUMB;
            }
            else if ( c== '=' || c== '<' || c== '>'|| c== '!') {
                clear ();
                add ();
                gc ();
                CS = ALE;
            } else if ( c == '"' ) {
                gc();
                s = c;
                gc();
                CS = STRING;
            } else if ( c == '-') {
                gc();
                j = look(minus, TD);
                return lexem(LEX_MINUS, j);
            }
            else if ( c == EOF )
                return lexem(LEX_FIN);
            else if (c == '#') {
                while (c != '\n') {
                    if (c == EOF) {
                        return lexem(LEX_FIN);
                    }
                    gc();
                }
                CS = H;
                break;
            }
            else
                CS = DELIM;
            break;
        case IDENT:
            if ( isalpha(c) || isdigit(c) ) {
                add();
                gc();
            }
            else if ( j = look (buf, TW) )
                return lexem (words[j], j);
            else {
                j = TID.put(buf);
                return lexem(LEX_ID, j);
            }
            break;
        case NUMB:
            if (isdigit(c)) {
                d = d * 10 + (c - '0');
                gc();
            } else  
                return lexem(LEX_INTCONST, d);
            break;
        case ALE:
            if ( c == '=' ) {
                add ();
                gc ();
                j = look ( buf, TD );
                return lexem( dlms[j], j );
            } else {
                j = look (buf, TD);
                return lexem( dlms[j], j );
            }
            break;
        case STRING:
            if (c != '"') {
						s += c;
						gc();
			} else if (c == '"') {
                gc();
				str.push_back(s);
                return lexem(LEX_STRCONST, str.size()-1);
			} else if (c == EOF) {
                throw c;
            }
            break;
        case DELIM:
            clear ();
            add ();
            if ( j = look(buf, TD) ) {
                gc ();
                return lexem( dlms[j], j );
            } else
                throw c;
            break;
            } // end switch
        }
    while ( true );
}

class Poliz {
    lexem *p;
    int size;
    int free;
public:
Poliz ( int max_size ) {
    p = new lexem [size = max_size];
    free = 0;
};
~Poliz() { delete []p; };
void put_lex(lexem l) { p[free]=l; ++free; };
void put_lex(lexem l, int place) { p[place]=l; };
void blank() { ++free; };
int get_free() { return free; };
lexem& operator[] ( int index ) {
    if (index > size)
        throw "POLIZ:out of array";
    else
        if ( index > free )
            throw "POLIZ:indefinite element of array";
        else
            return p[index];
}
void print() {
    for ( int i = 0; i < free; ++i )
    cout << p[i];
}
};

class Parser
{
    lexem curr_lex; 
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    Stack < type_of_lex, 100 > st_lex;
    void P(); // процедуры РС-метода
    void D();
    void D1();
    void D2();
    void D3();
    void B();
    void S();
    void E();
    void E1();
    void T();
    void F();
    void dec ( type_of_lex type) {
        if ( TID[c_val].get_declare() )
            throw "twice";
        else {
            TID[c_val].put_declare();
            TID[c_val].put_type(type);
        }
    }   
    void check_id() {
        if ( TID[c_val].get_declare() )
            st_lex.push(TID[c_val].get_type());
        else
            throw "not declared";
    }

    void check_op () {
        type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOLEAN, s = LEX_STRING;
        t2 = st_lex.pop();
        op = st_lex.pop();
        t1 = st_lex.pop();
        if ( t1 == LEX_BOOLEAN ) {
            if ( op == LEX_AND || op == LEX_OR ) {
                if ( t2 == LEX_FALSE || t2 == LEX_TRUE ) {
                    st_lex.push(LEX_BOOLEAN);
                } else
                    throw "wrong types are in operation"; 
            } else                               
                throw "wrong types are in operation"; 
        }
        else if ( t1 == LEX_INT ) {
            if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH ) {
                if ( t2 == LEX_INT ) {
                    st_lex.push(LEX_INT);
                } else 
                    throw "wrong types are in operation"; 
            } else if ( op == LEX_GREATER || op == LEX_LESS || op == LEX_LEQ || op == LEX_GEQ ||
                    op == LEX_IFEQ || op == LEX_NEQ ) {
                st_lex.push(LEX_BOOLEAN);
            }else 
                throw "wrong types are in operation"; 
        }
        else if ( t1 == LEX_STRING ) {
            if ( op == LEX_PLUS ) {
                if ( t2 == LEX_STRING ) {
                    st_lex.push(LEX_STRING);
                } else 
                    throw "wrong types are in operation"; 
            } else if (op == LEX_GREATER || op == LEX_LESS || op == LEX_IFEQ || op == LEX_NEQ){
                    st_lex.push(LEX_BOOLEAN);
                
            } else 
                throw "wrong types are in operation"; 
        }
        else if ( t1 == LEX_MINUS ) {
            t1 = st_lex.pop();
            if ( t1 == LEX_INT ) {
                if ( op == LEX_PLUS || op == LEX_MINUS || op == LEX_MULT || op == LEX_SLASH ) {
                    if ( t2 == LEX_INT ) {
                        st_lex.push(LEX_INT);
                    } else 
                        throw "wrong types are in operation"; 
                } else if ( op == LEX_GREATER || op == LEX_LESS || op == LEX_LEQ || op == LEX_GEQ ||
                        op == LEX_IFEQ || op == LEX_NEQ ) {
                    st_lex.push(LEX_BOOLEAN);
                }
            } else 
                throw "wrong type with unary minus";
        } else
            throw "error in operation";
        
    }

    void check_un_minus() {
        if ( st_lex.pop() != LEX_INT ) 
            throw "wrong type with unary minus";
        else {
            st_lex.push (LEX_INT);
            st_lex.push (LEX_MINUS);
        }
    }

    void check_not () {
        if (st_lex.pop() != LEX_BOOLEAN)
            throw "wrong type is in not";
        else {
            st_lex.push (LEX_BOOLEAN);
        }
    }
    void eq_type () {
        if ( st_lex.pop() != st_lex.pop() ) throw "wrong types are in =";
    }
 
    void eq_bool () {
        if ( st_lex.pop() != LEX_BOOLEAN )
            throw "expression is not boolean";
    }
    void check_id_in_read () {
        if ( TID[c_val].get_type()!= LEX_BOOLEAN ) {
            if ( !TID [c_val].get_declare() )
                throw "not declared";
        }
    }
    void gl () {
        curr_lex = scan.get_lexem();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }
public:
    Poliz prog; // внутреннее представление программы
    Parser () : scan (), prog (1000) {}
    void analyze(); // анализатор с действиями
};
        
void Parser::analyze () {
    gl ();
    P ();
    prog.print();
    cout << endl << "Yes!!!" << endl;
}

void Parser::P () {
    if ( c_type == LEX_PROGRAM )
        gl ();
    else
        throw curr_lex;
    if ( c_type == LEX_L_F_BRACKET )
        gl();
    else 
        throw curr_lex;
    D ();
    B ();
    S();
    while ( c_type == LEX_SEMICOLON ) {
        gl();
        S();
    }
    if ( c_type == LEX_R_F_BRACKET )
        gl();
    else 
        throw curr_lex;
    if ( c_type != LEX_FIN )
        throw curr_lex;
}

void Parser::D () {
    if ( c_type == LEX_INT ) {
        gl(); 
        D1();
    } else if ( c_type == LEX_STRING ) {
        gl();
        D2();
    }else if ( c_type == LEX_BOOLEAN ) {
        gl();
        D3();
    } else
        throw curr_lex;
}

void Parser::D1 () {
    if ( c_type != LEX_ID )
        throw curr_lex;
    else {
        dec( LEX_INT );
        gl ();
        if ( c_type == LEX_COMMA ) {
            gl();
            D1();
        }
        else if ( c_type == LEX_SEMICOLON ) {
            gl();
        } else if ( c_type == LEX_EQ ) {
            gl();
            if ( c_type == LEX_INTCONST ) {
                gl();
            } else if ( c_type == LEX_MINUS ) {
                gl();
                if ( c_type == LEX_INTCONST ) {
                    c_val = -c_val;
                    gl();
                } else 
                    throw curr_lex;
            } else 
                throw curr_lex;
            if ( c_type == LEX_COMMA ) {
                gl();
                D1();
            } else if ( c_type == LEX_SEMICOLON ) {
                gl();
            } else 
                throw curr_lex;
        } else 
            throw curr_lex;
    }
}

void Parser::D2 () {
    if ( c_type != LEX_ID )
        throw curr_lex;
    else {
        dec( LEX_STRING );
        gl ();
        if ( c_type == LEX_COMMA ) {
            gl();
            D2();
        }
        else if ( c_type == LEX_SEMICOLON ) {
            gl();
        } else if ( c_type == LEX_EQ ) {
            gl();
            if ( c_type == LEX_STRCONST ) {
                gl();
            }
            else 
                throw curr_lex;
            if ( c_type == LEX_COMMA ) {
                gl();
                D2();
            }else if ( c_type == LEX_SEMICOLON ) {
                gl();
            } else 
                throw curr_lex;
        } else 
            throw curr_lex;
    }
}
void Parser::D3 () {
    if ( c_type != LEX_ID )
        throw curr_lex;
    else {
        dec( LEX_BOOLEAN );
        gl ();
        if ( c_type == LEX_COMMA ) {
            gl();
            D3();
        }
        else if ( c_type == LEX_SEMICOLON ) {
            gl();
        } else if ( c_type == LEX_EQ ) {
            gl();
            if ( c_type == LEX_TRUE ) {
                gl();
            } else if ( c_type == LEX_FALSE ) {
                gl();
            } else 
                throw curr_lex;
            if ( c_type == LEX_COMMA ) {
                gl();
                D3();
            } else if ( c_type == LEX_SEMICOLON ) {
                gl();
            } else 
                throw curr_lex;
        } else 
            throw curr_lex;
    }
}

void Parser::B () {
    if ( c_type == LEX_INT ) {
        gl();
        D1();
        B();
    } else if ( c_type == LEX_STRING ) {
        gl();
        D2();
        B();
    } else if ( c_type == LEX_BOOLEAN ) {
        gl();
        D3();
        B();
    }
}

void Parser::S () {
    int pl0, pl1, pl2, pl3;
    if ( c_type == LEX_IF ) {
        gl();
        if ( c_type == LEX_L_R_BRACKET ) 
            gl();
        else 
            throw curr_lex;
        E();
        if ( c_type == LEX_R_R_BRACKET ) 
            gl();
        else 
            throw curr_lex;
        eq_bool();
        pl2 = prog.get_free ();
        prog.blank();
        prog.put_lex (lexem (POLIZ_FGO) );
        if ( c_type == LEX_L_F_BRACKET ) {
            gl();
            if ( c_type == LEX_FIN ) 
                throw curr_lex;
            else {
                S();
                while ( c_type == LEX_SEMICOLON ) {
                    gl();
                    if ( c_type != LEX_R_F_BRACKET )
                        S();
                    else {
                        gl();
                        S();
                    }
                }
            }
        } else     
            S();
        pl3 = prog.get_free();
        prog.blank();
        prog.put_lex (lexem(POLIZ_GO));
        prog.put_lex (lexem(POLIZ_LABEL, prog.get_free()),pl2);
        if (c_type == LEX_ELSE) {
            gl();
            if ( c_type == LEX_L_F_BRACKET ) {
                gl();
                if ( c_type == LEX_FIN ) 
                    throw curr_lex;
                else {
                    S();
                    while ( c_type == LEX_SEMICOLON ) {
                        gl();
                        if ( c_type != LEX_R_F_BRACKET )
                            S();
                        else {
                            gl();
                            S();
                        }
                    }
                }
            } else     
                S();
            prog.put_lex(lexem(POLIZ_LABEL,prog.get_free()),pl3);
        }
    } else if ( c_type == LEX_WHILE ) {
        pl0 = prog.get_free();
        gl();
        if ( c_type == LEX_L_R_BRACKET ) 
            gl();
        else 
            throw curr_lex;
        E();
        if ( c_type == LEX_R_R_BRACKET ) 
            gl();
        else 
            throw curr_lex;
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex (lexem(POLIZ_FGO));
        if ( c_type == LEX_L_F_BRACKET ) {
            gl();
            if ( c_type == LEX_FIN ) 
                throw curr_lex;
            else {
                S();
                while ( c_type == LEX_SEMICOLON ) {
                    gl();
                    if ( c_type != LEX_R_F_BRACKET )
                        S();
                    else {
                        gl();
                        S();
                    }
                }
            }
        } else     
            S();
        prog.put_lex (lexem (POLIZ_LABEL, pl0));
        prog.put_lex (lexem( POLIZ_GO));
        prog.put_lex (lexem(POLIZ_LABEL, prog.get_free()),pl1);
    } else if ( c_type == LEX_READ ) {
        gl();
            if ( c_type == LEX_L_R_BRACKET ) {
            gl();
            if ( c_type == LEX_ID ) {
                check_id_in_read();
                prog.put_lex (lexem ( POLIZ_ADDRESS, c_val) );
                gl();
            }else
                throw curr_lex;
            if ( c_type == LEX_R_R_BRACKET ) {
                gl();
                prog.put_lex (lexem (LEX_READ));
            } else
                throw curr_lex;
        } else
            throw curr_lex;
    } else if ( c_type == LEX_WRITE ) {
        gl();
        if ( c_type == LEX_L_R_BRACKET ) {
            gl();
            E();
            if ( c_type == LEX_R_R_BRACKET ) {
                gl();
                prog.put_lex (lexem(LEX_WRITE));
            } else
                throw curr_lex;
        }
        else
            throw curr_lex;
    } //end write
    else if ( c_type == LEX_ID ) {
        check_id ();
        prog.put_lex (lexem(POLIZ_ADDRESS,c_val));
        gl();
        if ( c_type == LEX_EQ ) {
            gl();
            E();
            eq_type();
            prog.put_lex (lexem (LEX_EQ) );
        } else
            throw curr_lex;
        } //assign-end 
    else if ( c_type == LEX_CONTINUE ) { //polis
        gl();
    }
    else if ( c_type == LEX_MINUS ) {
        gl();
        if ( c_type == LEX_ID ) {
            check_id();
            check_un_minus();
            gl();
        } else 
            throw curr_lex;
    }
    else
        B();
}

void Parser::E () {
    E1();
    if ( c_type == LEX_EQ || c_type == LEX_LESS || c_type == LEX_GREATER ||
            c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ || c_type == LEX_IFEQ ) {
        st_lex.push (c_type);
        gl();
        E1();
        check_op();
    }
}

void Parser::E1 () {
    T();
    while ( c_type==LEX_PLUS || c_type==LEX_MINUS || c_type==LEX_OR ) {
        st_lex.push (c_type);
        gl();
        T();
        check_op();
    }
}

void Parser::T () {
    F();
    while ( c_type==LEX_MULT || c_type==LEX_SLASH || c_type==LEX_AND ) {
        st_lex.push (c_type);
        gl();
        F();
        check_op();
    }
}
        
void Parser::F () {
    if ( c_type == LEX_ID ) {
        check_id();
        prog.put_lex (lexem (LEX_ID, c_val));
        gl();
    }
    else if ( c_type == LEX_INTCONST ) {
        st_lex.push ( LEX_INT );
        prog.put_lex ( curr_lex );
        gl();
        }
    else if ( c_type == LEX_TRUE ) {
        st_lex.push ( LEX_BOOLEAN );
        prog.put_lex (lexem (LEX_TRUE, 1) );
        gl();
    }
    else if ( c_type == LEX_FALSE ) {
        st_lex.push ( LEX_BOOLEAN );
        prog.put_lex (lexem (LEX_FALSE, 0) );
        gl();
    }
    else if ( c_type == LEX_NOT ) {
        gl();
        F();
        check_not();
    }
    else if ( c_type == LEX_MINUS ) {
        gl();
        if ( c_type == LEX_ID ) {
            check_id();
            check_un_minus();
            gl();
        }
    } else if ( c_type == LEX_L_R_BRACKET ) {
        gl();
        E();
        if ( c_type == LEX_R_R_BRACKET )
            gl();
        else
            throw curr_lex;
    }else
        throw curr_lex;
}


int main() {
/*    Scanner s;
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
    }*/
    Parser p;
    p.analyze();
    return 0;
}
