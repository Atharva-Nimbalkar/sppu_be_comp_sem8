#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace chrono;

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; ++i) {
        #pragma omp parallel for
        for (int j = i % 2; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Merge function
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> left(arr.begin() + l, arr.begin() + m + 1);
    vector<int> right(arr.begin() + m + 1, arr.begin() + r + 1);
    size_t i = 0, j = 0;
    int k = l;
    while (i < left.size() && j < right.size()) {
        arr[k++] = (left[i] <= right[j]) ? left[i++] : right[j++];
    }
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

// Sequential Merge Sort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, l, m);
            #pragma omp section
            parallelMergeSort(arr, m + 1, r);
        }
        merge(arr, l, m, r);
    }
}

// Utility to print array
void printArray(const vector<int>& arr) {
    for (int val : arr) {
        cout << val << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    vector<int> arr1 = arr;
    vector<int> arr2 = arr;
    vector<int> arr3 = arr;
    vector<int> arr4 = arr;

    // Sequential Bubble Sort
    auto start1 = high_resolution_clock::now();
    bubbleSort(arr1);
    auto end1 = high_resolution_clock::now();
    auto dur1 = duration_cast<microseconds>(end1 - start1).count();
    cout << "\nSequential Bubble Sort Time: " << dur1 << " µs\n";
    cout << "Sorted Array: ";
    printArray(arr1);

    // Parallel Bubble Sort
    auto start2 = high_resolution_clock::now();
    parallelBubbleSort(arr2);
    auto end2 = high_resolution_clock::now();
    auto dur2 = duration_cast<microseconds>(end2 - start2).count();
    cout << "\nParallel Bubble Sort Time: " << dur2 << " µs\n";
    cout << "Sorted Array: ";
    printArray(arr2);

    // Sequential Merge Sort
    auto start3 = high_resolution_clock::now();
    mergeSort(arr3, 0, n - 1);
    auto end3 = high_resolution_clock::now();
    auto dur3 = duration_cast<microseconds>(end3 - start3).count();
    cout << "\nSequential Merge Sort Time: " << dur3 << " µs\n";
    cout << "Sorted Array: ";
    printArray(arr3);

    // Parallel Merge Sort
    auto start4 = high_resolution_clock::now();
    parallelMergeSort(arr4, 0, n - 1);
    auto end4 = high_resolution_clock::now();
    auto dur4 = duration_cast<microseconds>(end4 - start4).count();
    cout << "\nParallel Merge Sort Time: " << dur4 << " µs\n";
    cout << "Sorted Array: ";
    printArray(arr4);

    return 0;
}


// g++ -fopenmp -Wall -Wextra -g3 sorting.cpp -o sorting
// ./sorting
// Enter number of elements: 5
// Enter 5 elements:
// 34
// 67
// 12
// 43
// 22

// Sequential Bubble Sort Time: 1 µs
// Sorted Array: 12 22 34 43 67

// Parallel Bubble Sort Time: 372 µs
// Sorted Array: 12 22 34 43 67

// Sequential Merge Sort Time: 10 µs
// Sorted Array: 12 22 34 43 67

// Parallel Merge Sort Time: 38 µs
// Sorted Array: 12 22 34 43 67