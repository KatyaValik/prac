#include <iostream>

using namespace std;

class Sublines {
public:
    int *line;
    Sublines() {}
    Sublines(int n) {
        line = new int[n];
        for (int i = 0; i < n; ++i) {
            line[i] = 0;
        }
    }
    int& operator[] (int ind) {
        return line[ind];
    }
    const int& operator[] (int ind) const {
        return line[ind];
    }
};

class Matrix {
    int m;
    int n;
    Sublines *lines;
public:
    Matrix(int m, int n){
        this->m = m;
        this->n = n;
        lines = new Sublines[m];
        cout << "Enter elements of matrix" << endl;
        for (int i = 0; i < m; ++i){
            lines[i] = Sublines(n);
        }
    }

    Sublines& operator[] (int j) {
        return lines[j];
    }

    const Sublines& operator[] (int j) const {
        return lines[j];
    }

    friend ostream& operator<<(ostream &out, const Matrix &a);

//    virtual Matrix_mult& Mult(const Matrix &a, const Matrix &b) const = 0;

/*    ~Matrix() {
        for (int i = 0; i < n; ++i) {
            if (lines[i].line != nullptr)
                delete[] (lines[i].line);
        }
        if (lines != nullptr)
            delete[] lines;
    }*/
};

ostream& operator<<(ostream &out, const Matrix &a){
    for (int i = 0; i < a.m; ++i){
        for (int j = 0; j < a.n; ++j){
            out << a[i][j] << " ";                
        }
        out << endl;
    }
    return out;
}

/*class Matrix_mult: public Matrix {
public:
    int m;
    int n;
    Sublines *lines;
public:
    Matrix_mult(int m, int n){
        this->m = m;
        this->n = n;
        lines = new Sublines[m];
        cout << "Enter elements of matrix" << endl;
        for (int i = 0; i < m; ++i){
            lines[i] = Sublines(n);
        }
    }
    virtual Matrix_mult& Mult(const Matrix_mult &a, const Matrix_mult &b) const {
        Matrix_mult& C(a.n, b.m);
        return C;
    }
    ~Matrix_mult() {
    for (int i = 0; i < n; ++i) {
        if (lines[i].line != nullptr)
            delete[] (lines[i].line);
        }
        if (lines != nullptr)
            delete[] lines;
    }

};*/



int main() {
    int m, n;
    cout << "Enter number of lines in matrix" << endl;
    cin >> m;
    cout << "Enter number of columns in matrix" << endl;
    cin >> n;
    Matrix a(m, n);
    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            cin >> a[i][j];
        }
    }
    cout << a;
    a[0][0] = 5;
    cout <<a;
    return 0;
}