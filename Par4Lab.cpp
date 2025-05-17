#include <iostream>
#include <vector>
#include <omp.h>
#include <random>
#include <algorithm>
#include <thread>

using namespace std;
int partition(vector<int>& arr, int low, int high);
vector<int> quickSort(vector<int> arr, int low, int high, bool outerCall);
vector<int> quickSort_OMP_sect(vector<int> arr, int low, int high, int depth, bool outerCall);
vector<int> get_numMass(int N);
string equalCh(vector<int>& mass1, vector<int>& mass2);
int main()
{
    setlocale(LC_ALL, "rus");
    vector<int> a;
    vector<int>par;
    vector<int>no_par;
    int M = 150000;
    int N = 100000;
    int d = 0;
    cout << "!--------------------------------------Чисел в массиве: " << N << "--------------------------------------!" << endl;
    a = get_numMass(N);
    omp_set_nested(1);
    no_par = quickSort(a, 0, N - 1, true);
    par = quickSort_OMP_sect(a, 0, N - 1, d, true);
    cout << equalCh(no_par, par) << endl;
    d = 0;
    cout << "!--------------------------------------Чисел в массиве: " << M << "--------------------------------------!" << endl;
    a = get_numMass(M);
    no_par = quickSort(a, 0, M - 1, true);
    par = quickSort_OMP_sect(a, 0, M - 1, d, true);
    cout << equalCh(no_par, par) << endl;
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
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}
vector<int> quickSort(vector<int> arr, int low, int high, bool outerCall = false) {
    double start = omp_get_wtime();
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
    if (outerCall)
    {
        double end = omp_get_wtime();
        cout << "Однопоточный режим: " << end - start << endl;
    }
    return arr;
}
vector<int> quickSort_OMP_sect(vector<int> arr, int low, int high, int depth, bool outerCall = false) {
    double start = omp_get_wtime();
    if (low < high) {
        int pi = partition(arr, low, high);
        if (depth <= 3)
        {
#pragma omp parallel sections
            {
#pragma omp section
                {
                    quickSort_OMP_sect(arr, low, pi - 1, depth+1);
                }
#pragma omp section
                {
                    quickSort_OMP_sect(arr, pi + 1, high, depth+1);
                }
            }
        }
        else
        {
                    quickSort(arr, low, pi - 1);
                    quickSort(arr, pi + 1, high);
        }
    }
    if (outerCall)
    {
        double end = omp_get_wtime();
        cout << "Многопоточный режим: " << end - start << endl;
    }
    return arr;
}
string equalCh(vector<int>& mass1, vector<int>& mass2)
{
    if (mass1 == mass2)
    {
        return "Равные векторы";
    }
    return "НЕ равные векторы";
}