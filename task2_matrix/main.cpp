#include <iostream>

using namespace std;

class Matrix_mult {
public:
    Matrix_mult() {};
    virtual ~Matrix_mult() {};
};

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

class Matrix : public Matrix_mult {
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

    Matrix(const Matrix& a) {
        m = a.m;
        n = a.n;
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < m; ++j){
                this[i][j] = a[i][j];
            }
        }
    }

    Sublines& operator[] (int j) {
        return lines[j];
    }

    const Sublines& operator[] (int j) const {
        return lines[j];
    }

    friend ostream& operator<<(ostream &out, const Matrix &a);

    static Matrix* Mult(const Matrix &a, const Matrix &b) {
        Matrix *C = new Matrix(a.m, b.n);
        for (int i = 0; i < a.m; ++i){
            for (int j = 0; i < b.n; ++i){
                C[i][j] = 0;
                for (int k = 0; k < b.n + 1; ++k){
                    C[i][j] = (a[i][k] + b[k][j]);
                }
            }
        }
        return C;
    }

    Matrix operator*(Matrix &a){
        Matrix tmp(m, a.n);
        for (int i = 0; i < m; ++i){
            for (int j = 0; j < a.n; ++j){
                for (int k = 0; k < n; ++k){
                    tmp[i][j] += (*this)[i][k]*a[k][j];
                }
            }
        }
        return tmp;
    }

    ~Matrix() {
        for (int i = 0; i < n; ++i) {
            if (lines[i].line != nullptr)
                delete[] (lines[i].line);
        }
        if (lines != nullptr)
            delete[] lines;
    }
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
    cout << "Enter number of lines in matrix" << endl;
    cin >> m;
    cout << "Enter number of columns in matrix" << endl;
    cin >> n;
    Matrix b(m, n);
    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            cin >> b[i][j];
        }
    }
//    Matrix *C = nullptr;
    cout << "Result" << endl;
    Matrix C = a*b;
    cout << C;
    return 0;
}