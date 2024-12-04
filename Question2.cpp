#include <iostream>
#include <vector>
using namespace std;

// Function to sort the array using Bubble Sort
void sort(vector<int>& numbers) {
    int n = numbers.size();
    // Bubble Sort Algorithm
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (numbers[j] > numbers[j + 1]) {
                swap(numbers[j], numbers[j + 1]);
            }
        }
    }
}

// Function to find and return the median of the sorted numbers
double sortAndFindMedian(vector<int>& numbers) {
    sort(numbers);
    int n = numbers.size();
    if (n % 2 == 0) {
        return (numbers[n / 2 - 1] + numbers[n / 2]) / 2.0;
    } 
    else {
        return numbers[n / 2];
    }
}

int main() {
    // Example test case
    vector<int> numbers = {12, 5, 7, 3, 9, 15};
    double median = sortAndFindMedian(numbers);
    cout << "The median is: " << median << endl;
    
    return 0;
}
