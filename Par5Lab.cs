using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Threading;

class Program
{
    static List<int> GetNumMass(int M, int N)
    {
        Random rand = new Random();
        List<int> numMass = new List<int>(M);
        for (int i = 0; i < M; i++)
        {
            numMass.Add(rand.Next(0, N));
        }
        return numMass;
    }

    static void NoParallelSort(List<int> a, int N)
    {
        Stopwatch stopwatch = Stopwatch.StartNew();
        int[] b = new int[N];
        foreach (int num in a)
        {
            b[num]++;
        }
        int uniques = Array.FindAll(b, count => count == 1).Length;
        stopwatch.Stop();
        Console.WriteLine($"Уникальных символов: {uniques}");
        Console.WriteLine($"Однопоточный режим: {stopwatch.Elapsed.TotalSeconds.ToString("0.000") + " секунд"}");
    }

    static void ParallelSort(List<int> a, int N, int numThreads)
    {
        Stopwatch stopwatch = Stopwatch.StartNew();
        int[] b = new int[N];
        int uniques = 0;
        int chunkSize = a.Count / numThreads;
        List<Thread> threads = new List<Thread>();
        object lockObj = new object();
        for (int i = 0; i < numThreads; i++)
        {
            int startIdx = i * chunkSize;
            int endIdx = (i == numThreads - 1) ? a.Count : (i + 1) * chunkSize;
            Thread thread = new Thread(() =>
            {
                for (int j = startIdx; j < endIdx; j++)
                {
                    Interlocked.Increment(ref b[a[j]]);
                }
            });
            threads.Add(thread);
            thread.Start();
        }
        foreach (Thread thread in threads)
        {
            thread.Join();
        }
        foreach (int count in b)
        {
            if (count == 1)
            {
                lock (lockObj)
                {
                    uniques++;
                }
            }
        }
        stopwatch.Stop();
        Console.WriteLine($"Уникальных символов: {uniques}");
        Console.WriteLine($"Многопоточный режим: {stopwatch.Elapsed.TotalSeconds.ToString("0.000") + " секунд"}");
    }

    static void Main(string[] args)
    {
        int M = 500000000;
        List<int> a;
        int N = 100000000;
        a = GetNumMass(M, N);
        NoParallelSort(new List<int>(a), N);
        for (int numThreads = 2; numThreads < 13; numThreads++)
        {
            Console.WriteLine($"Кол-во потоков: {numThreads}");
            ParallelSort(new List<int>(a), N, numThreads);
        }
    }
}
