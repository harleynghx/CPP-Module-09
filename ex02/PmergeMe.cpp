#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <cstdlib>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}
PmergeMe::~PmergeMe() {}

void PmergeMe::sortVector(std::vector<int>& arr) {
    if (arr.size() <= 1) return;
    std::vector<int> mainChain, pending;
    int odd = -1;
    bool hasOdd = false;

    if (arr.size() % 2 != 0) {
        odd = arr.back();
        hasOdd = true;
        arr.pop_back();
    }

    for (size_t i = 0; i < arr.size(); i += 2) {
        if (arr[i] > arr[i+1]) {
            mainChain.push_back(arr[i]);
            pending.push_back(arr[i+1]);
        } else {
            mainChain.push_back(arr[i+1]);
            pending.push_back(arr[i]);
        }
    }

    sortVector(mainChain);

    for (size_t i = 0; i < pending.size(); ++i) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[i]);
        mainChain.insert(pos, pending[i]);
    }
    if (hasOdd) {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), odd);
        mainChain.insert(pos, odd);
    }
    arr = mainChain;
}

void PmergeMe::sortDeque(std::deque<int>& arr) {
    if (arr.size() <= 1) return;
    std::deque<int> mainChain, pending;
    int odd = -1;
    bool hasOdd = false;

    if (arr.size() % 2 != 0) {
        odd = arr.back();
        hasOdd = true;
        arr.pop_back();
    }

    for (size_t i = 0; i < arr.size(); i += 2) {
        if (arr[i] > arr[i+1]) {
            mainChain.push_back(arr[i]);
            pending.push_back(arr[i+1]);
        } else {
            mainChain.push_back(arr[i+1]);
            pending.push_back(arr[i]);
        }
    }

    sortDeque(mainChain);

    for (size_t i = 0; i < pending.size(); ++i) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[i]);
        mainChain.insert(pos, pending[i]);
    }
    if (hasOdd) {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), odd);
        mainChain.insert(pos, odd);
    }
    arr = mainChain;
}

void PmergeMe::execute(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg.empty() || arg.find_first_not_of("0123456789 ") != std::string::npos) {
            std::cerr << "Error" << std::endl;
            return;
        }
        std::istringstream iss(arg);
        long val;
        while (iss >> val) {
            if (val < 0 || val > 2147483647) {
                std::cerr << "Error" << std::endl;
                return;
            }
            _vec.push_back(static_cast<int>(val));
            _deq.push_back(static_cast<int>(val));
        }
    }

    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
    std::cout << "\n";

    clock_t startVec = clock();
    sortVector(_vec);
    clock_t endVec = clock();
    double timeVec = static_cast<double>(endVec - startVec) / CLOCKS_PER_SEC * 1000000;

    clock_t startDeq = clock();
    sortDeque(_deq);
    clock_t endDeq = clock();
    double timeDeq = static_cast<double>(endDeq - startDeq) / CLOCKS_PER_SEC * 1000000;

    std::cout << "After: ";
    for (size_t i = 0; i < _vec.size(); ++i) std::cout << _vec[i] << " ";
    std::cout << "\n";

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vec.size() << " elements with std::vector : " << timeVec << " us\n";
    std::cout << "Time to process a range of " << _deq.size() << " elements with std::deque  : " << timeDeq << " us\n";
}