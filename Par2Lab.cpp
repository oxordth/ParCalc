#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include <omp.h>
#include <random>
#include <fstream>
#include <iostream>
#include <mutex>
using namespace std;

vector<int> get_numMass(int M, int N);  //Создание массива с случайным заполнением
void no_parallel_sort(vector<int>& a, int N); //Однопоточный поиск
void parallel_sort(vector<int>& a, int N, int num_threads);   //Параллельный поиск

int main() {
    setlocale(LC_ALL, "rus");
    int M = 0, num_threads;
    /*cout << "Максимальное кол-во потоков - " << thread::hardware_concurrency() << endl;
    cout << "Введите размер массива (10^6): " << endl;
    cin >> M;
    M = M * 1000000;
    cout << "Введите число потоков: " << endl;
    cin >> num_threads;*/
    vector<int> a;
    int N = 100000000;
    do
    {
        num_threads = 6;
        M = M + (500 * 1000000);
        cout << "КОЛ-ВО ПОТОКОВ: " << num_threads << endl;
        cout << "КОЛ-ВО СИМВОЛОВ В МАССИВЕ (10^6): " << M / 1000000 << endl;
        a = get_numMass(M, N);
        // Без распараллеливания
        no_parallel_sort(a, N);
        // С распараллеливанием
        parallel_sort(a, N, num_threads);
        a.clear();
    } while (M < 2000000000);
    M = 0;
    do
    {
        num_threads = 12;
        M = M + (500 * 1000000);
        cout << "КОЛ-ВО ПОТОКОВ: " << num_threads << endl;
        cout << "КОЛ-ВО СИМВОЛОВ В МАССИВЕ (10^6): " << M / 1000000 << endl;
        a = get_numMass(M, N);
        // Без распараллеливания
        no_parallel_sort(a, N);
        // С распараллеливанием
        parallel_sort(a, N, num_threads);
        a.clear();
    } while (M < 2000000000);
    return 0;

}

vector<int> get_numMass(int M, int N) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, N);
    vector<int> numMass(M);
    for (int i = 0; i < M; i++)
        numMass[i] = dist(gen);
    return numMass;
}
void no_parallel_sort(vector<int>& a, int N)
{
    double start;
    double end;
    atomic<int> uniques;
    vector<int> b(N, 0);
    start = omp_get_wtime();
    for (int i = 0; i < a.size(); i++)
    {
        ++b[a[i]];
    }
    for (int i = 0; i < N; i++)
        if (b[i] == 1)
        {
            uniques++;
        }
    end = omp_get_wtime();
    ofstream fout("no_parallel.txt", ios_base::app);
    fout << end - start << "\n";
    fout.close();
    cout << "Уникальных символов: " << uniques << endl;
    cout << "Однопоточный режим: " << end - start << endl;
}
void parallel_sort(vector<int>& a, int N, int num_threads) {
    int chunk_size = a.size() / num_threads;
    double start;
    double enda;
    mutex mtx;
    vector<thread> threads;
    vector<int> b(N, 0);
    atomic<int> uniques = 0;
    start = omp_get_wtime();
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? a.size() : (i + 1) * chunk_size;
        threads.emplace_back([&, start, end]() {
            for (int j = start; j < end; ++j) {

                ++b[a[j]];
            }
            });
    }
    for (auto& t : threads)
        t.join();
    threads.clear();
    chunk_size = b.size() / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? b.size() : (i + 1) * chunk_size;
        threads.emplace_back([&, start, end]() {

            for (int j = start; j < end; ++j) {
                if (b[j] == 1) {
                    mtx.lock();
                    uniques++;
                    mtx.unlock();
                }
            }
            });
    }
    for (auto& k : threads)
        k.join();
    enda = omp_get_wtime();
    ofstream fout("parallel.txt", ios_base::app);
    fout << enda - start << "\n";
    fout.close();
    cout << "Уникальных символов: " << uniques << endl;
    cout << "Многопоточный режим: " << enda - start << endl;
}