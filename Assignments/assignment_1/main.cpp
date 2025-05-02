/*
Name: Jose Miguel Ortiz
Date: 03/09/2025
Email: jose.ortiz60@myhunter.cuny.edu

 This program measures the time it takes to find the next prime number for a given vector of numbers.
*/

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cmath>
#include <numeric>

using namespace std;

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

int next_prime(int n) {
    if (n < 2) return 2;
    int candidate = n + 1;
    if (candidate % 2 == 0)
        candidate++;
    while (true) {
        if (is_prime(candidate))
            return candidate;
        candidate += 2;
    }
}

void NextPrime(vector<int>& nums) {
    for (auto& num : nums) {
        if (!is_prime(num))
            num = next_prime(num);
    }
}

void ParallelNextPrime(vector<int>& nums, int numOfThreads) {
    vector<thread> threads;
    int n = nums.size();
    int chunk_size = n / numOfThreads;

    for (int i = 0; i < numOfThreads; ++i) {
        int start = i * chunk_size;
        int end = (i == numOfThreads - 1) ? n : start + chunk_size;
        threads.emplace_back([start, end, &nums]() {
            for (int j = start; j < end; ++j) {
                if (!is_prime(nums[j]))
                    nums[j] = next_prime(nums[j]);
            }
        });
    }

    for (auto& t : threads)
        t.join();
}

int main() {
    const int VECTOR_SIZE = 10000;
    const int TEST_VALUE = 1000000; // Composite number
    const string TIME_UNIT = "milliseconds";

    vector<int> original_data(VECTOR_SIZE, TEST_VALUE);

    // Measure NextPrime
    vector<long long> next_prime_times;
    for (int i = 0; i < 5; ++i) {
        auto data = original_data;
        auto start = chrono::high_resolution_clock::now();
        NextPrime(data);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        next_prime_times.push_back(duration);
    }

    // Measure ParallelNextPrime with 2,4,8 threads
    vector<int> thread_counts = {2, 4, 8};
    vector<vector<long long>> parallel_times(3, vector<long long>(5));
    for (int t = 0; t < thread_counts.size(); ++t) {
        int threads = thread_counts[t];
        for (int i = 0; i < 5; ++i) {
            auto data = original_data;
            auto start = chrono::high_resolution_clock::now();
            ParallelNextPrime(data, threads);
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            parallel_times[t][i] = duration;
        }
    }

    // Measure thread creation time
    vector<long long> thread_creation_times;
    for (int i = 0; i < 5; ++i) {
        auto start = chrono::high_resolution_clock::now();
        thread t([](){});
        t.join();
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
        thread_creation_times.push_back(duration);
    }

    // Output results
    cout << "Test results with vector size " << VECTOR_SIZE << " and time unit " << TIME_UNIT << " (except thread creation in microseconds):\n";

    cout << "\nNextPrime Times:\n";
    for (int i = 0; i < 5; ++i)
        cout << "Test " << i+1 << ": " << next_prime_times[i] << " " << TIME_UNIT << "\n";
    cout << "Average: " << accumulate(next_prime_times.begin(), next_prime_times.end(), 0LL) / 5 << " " << TIME_UNIT << "\n";

    vector<string> thread_titles = {"2 threads", "4 threads", "8 threads"};
    for (int t = 0; t < thread_counts.size(); ++t) {
        cout << "\nParallelNextPrime " << thread_titles[t] << " Times:\n";
        for (int i = 0; i < 5; ++i)
            cout << "Test " << i+1 << ": " << parallel_times[t][i] << " " << TIME_UNIT << "\n";
        cout << "Average: " << accumulate(parallel_times[t].begin(), parallel_times[t].end(), 0LL) / 5 << " " << TIME_UNIT << "\n";
    }

    cout << "\nThread Creation Times (microseconds):\n";
    for (int i = 0; i < 5; ++i)
        cout << "Test " << i+1 << ": " << thread_creation_times[i] << " microseconds\n";
    cout << "Average: " << accumulate(thread_creation_times.begin(), thread_creation_times.end(), 0LL) / 5 << " microseconds\n";

    return 0;
}