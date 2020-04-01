#include <iostream>

using namespace std;

class Sublines {
public:
    int *line;
    Sublines() {
        line = nullptr;
    }
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

class A{
public:
    static int cnt;
    virtual void matrix_counter() = 0;
    virtual ~A() {};
};

int A::cnt = 0;

class Matrix : public A{
    int m;
    int n;
    Sublines *lines;
public:
    Matrix(int m, int n){
        A::cnt++;
        this->m = m;
        this->n = n;
        lines = new Sublines[m];
         for (int i = 0; i < m; ++i){
            lines[i] = Sublines(n);
        }
    }

    Matrix(const Matrix& a) {
        A::cnt++;
        m = a.m;
        n = a.n;
        lines = new Sublines[m];
        for (int i = 0; i < m; ++i){
            lines[i] = Sublines(n);
        }
        for (int i = 0; i < n; ++i){
            for (int j = 0; j < m; ++j){
                (*this)[i][j] = a[i][j];
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

    Matrix& operator= (const Matrix &a){
        for (int i = 0; i < n; ++i) {
            if (lines[i].line != nullptr)
                delete[] (lines[i].line);
        }
        if (lines != nullptr)
            delete[] lines;
        n = a.n;
        m = a.m;
        lines = new Sublines[m];
        for (int i = 0; i < m; ++i){
            lines[i] = Sublines(n);
        }
        for (int i = 0; i < m; ++i){
            for (int j = 0; j < n; ++j){
                (*this)[i][j] = a[i][j];
            }
        }
        return *this;
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

    Matrix operator*(int x){
        Matrix tmp(m, n);
        for (int i = 0; i < m; ++i){
            for (int j = 0; j < n; ++j){
                tmp[i][j] = (*this)[i][j]*x;
            }
        }
        return tmp;
    }

    ~Matrix() {
        A::cnt--;
        for (int i = 0; i < m; ++i) {
            if (lines[i].line != nullptr)
                delete[] (lines[i].line);
        }
        if (lines != nullptr)
            delete[] lines;
    }
    void matrix_counter() {
        cout << cnt;
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

int main() {
    int m, n;
    cout << "Enter number of lines in matrix" << endl;
    cin >> m;
    cout << "Enter number of columns in matrix" << endl;
    cin >> n;
    Matrix a(m, n);
    cout << "Enter elements of matrix" << endl;
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
    cout << "Enter elements of matrix" << endl;    
    for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            cin >> b[i][j];
        }
    }
    cout << "Result" << endl;
    Matrix c = a*b;
    cout << c;
    Matrix d = c*15;
    cout << d;
    a = b;
    cout << a;
    cout << "There are ";
    a.matrix_counter();
    cout << " Matrix" << endl;
    return 0;
}