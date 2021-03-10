#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>
#include <utility>
#include <ctime>

using namespace std;

constexpr long long s  = 100000000;

constexpr long long fir = 25000000;
constexpr long long sec = 50000000;
constexpr long long thi = 75000000;


std::mutex myMutex;
long long maxmin = INT_MIN;
int id = 1;

void minLoc(const vector<long long>& val, unsigned long long beg, unsigned long long end) {
    long long minL = val[beg];
    for (auto it = beg; it < end; ++it) {
        if (val[it] < minL)
            minL = val[it];
    }
    lock_guard<mutex> myLock(myMutex);
    cout << "Поток: " << id << " Минимум: " << minL << endl;
    id++;
    if (maxmin < minL) {
        maxmin = minL;
    }
}

long long min1(const vector<long long>& val, unsigned long long beg, unsigned long long end){
    long long minL = val[beg];
    for (auto it = beg; it < end; ++it) {
        if (val[it] < minL)
            minL = val[it];
    }
    return minL;
}

int main() {
    srand(time(NULL));
    
    int count;
    cout << "Введите кол-во потоков: ";
    cin >> count;
    
    vector<long long> randValues;
    randValues.reserve(s);
    
    for (long long i = 0; i < s; ++i) {
        if (i < fir)
            randValues.push_back(rand() % fir);
        else if (i < sec)
            randValues.push_back(fir + rand() % (sec - fir));
        else if (i < thi)
            randValues.push_back(sec + rand() % (thi - sec));
        else
            randValues.push_back(800 + rand() % s);
    }
    
    unsigned long h = randValues.size() / count;
    vector<thread> ths;
    
    auto start = chrono::system_clock::now();
    
    for (long long i = 0; i < count-1; i++) {
        ths.push_back(thread(minLoc, std::ref(randValues), i * h, ((i+1) * h)-1));
    }
    ths.push_back(thread(minLoc, std::ref(randValues), h*(count - 1), randValues.size()-1));
    
    for (long long i = 0; i < count; i++) {
        ths[i].join();
    }
    
    
    long long totalMax = INT_MIN;
    cout << "       Максимум: " << maxmin << endl;
    
    chrono::duration<double> dur = chrono::system_clock::now() - start;
    cout << "Время нескольких потоков " << dur.count() << " секунд\n\n";
    
     //один поток
    totalMax = 0;
    start = chrono::system_clock::now();
    for (long long i = 0; i < count; i++) {
        long long r = min1(randValues, i * h, (i+1) * h-1);
        if(r > totalMax)
            totalMax = r;
    }
    cout << "       Максимум: " << totalMax << endl;
    
    dur = chrono::system_clock::now() - start;
    cout << "Время одного потока " << dur.count() << " секунд\n\n";
    return 0;
}
