#include <iostream>
#include <vector>
#include <omp.h>
#include <random>
using namespace std;
vector<int>Sort_noOMP(vector<int> values);
vector<int>Sort_OMP(vector<int> values, int N);
vector<int> get_numMass(int N);

int main()
{
    setlocale(LC_ALL, "rus");
    vector<int> a;
    vector<int>par;
    vector<int>no_par;
    int M = 50000;
    int N = 100000;
    cout << "N = " << N << endl;
    a = get_numMass(N);
    no_par = Sort_noOMP(a);
    for (int i = 1; i < 13; i++)
    {
        par = Sort_OMP(a, i);
        if (par == no_par)
        {
            cout << "Верно" << endl;
        }
        else {
            cout << "Неверно" << endl;
        }
    }
    cout << '\n';
    cout << "N = " << M << endl;
    N = 10000;
    a = get_numMass(M);
    no_par = Sort_noOMP(a);
    for (int i = 1; i < 13; i++)
    {
        par = Sort_OMP(a, i);
        if (par == no_par)
        {
            cout << "Верно" << endl;
        }
        else {
            cout << "Неверно" << endl;
        }
    }
}

vector<int> get_numMass(int N) {
    int M = 1000;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, M);
    vector<int> numMass(N);
    for (int i = 0; i < N; i++)
        numMass[i] = dist(gen);
    return numMass;
}
vector<int>Sort_noOMP(vector<int> values) {
    double start = omp_get_wtime();
    for (int i = 0; i < values.size(); i++) {
        for (int j = 0; j < values.size() - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Однопоточный режим: " << end - start << endl;
    return values;
}
vector<int>Sort_OMP(vector<int> values, int N) {
    cout << "Кол-во потоков: " << N << endl;
    double start = omp_get_wtime();
#pragma omp parallel num_threads(N)
    for (int i = 0; i < values.size(); i++) {
#pragma omp for 
        for (int j = 0; j < values.size() - 1; j++) {
            if (values[j] > values[j + 1]) {
                auto temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }
    double end = omp_get_wtime();
    cout << "Многопоточный режим: " << end - start << endl;
    return values;
}