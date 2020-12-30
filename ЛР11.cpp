#include <iostream>
#include <vector>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>
#include <utility>
#include <ctime>

using namespace std;

constexpr long long s = 100000000;

constexpr long long fir = 25000000;
constexpr long long sec = 50000000;
constexpr long long thi = 75000000;


std::mutex myMutex;
vector<unsigned long long> res;
int id = 1;

void minLoc(const vector<int>& val, unsigned long long beg, unsigned long long end) {
    unsigned long long minL = val[beg];
    for (auto it = beg; it < end; ++it) {
        lock_guard<mutex> myLock(myMutex);
        if (val[it] < minL)
            minL = val[it];
    }
    cout << "Поток: " << id << " Минимум: " << minL << endl;
    id++;
    res.push_back(minL);
}

unsigned long long min1(const vector<int>& val, unsigned long long beg, unsigned long long end){
    unsigned long long minL = val[beg];
    for (auto it = beg; it < end; ++it) {
        if (val[it] < minL)
            minL = val[it];
    }
    return minL;
}

int main() {
    srand(time(NULL));
    
    int count, h;
    cout << "Введите кол-во потоков: ";
    cin >> count;
    
    vector<int> randValues;
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
    
    h = randValues.size() / count;
    vector<thread> ths;
    
    auto start = chrono::system_clock::now();
    
    for (int i = 0; i < count; i++) {
        ths.push_back(thread(minLoc, randValues, i * h, i * (h+1)));
    }
    
    for (int i = 0; i < count; i++) {
        ths[i].join();
    }
    
    
    unsigned long long totalMax = 0;
    for (int i = 0; i < res.size(); i++)
        if (res[i] > totalMax) totalMax = res[i];
    cout << "       Максимум: " << totalMax << endl;
    
    chrono::duration<double> dur = chrono::system_clock::now() - start;
    cout << "Время нескольких потоков " << dur.count() << " секунд\n\n";
    
     //один поток
    totalMax = 0;
    start = chrono::system_clock::now();
    for (int i = 0; i < count; i++) {
        unsigned long long r = min1(randValues, i * h, i * (h+1));
        if(r > totalMax)
            totalMax = r;
    }
    cout << "       Максимум: " << totalMax << endl;
    
    dur = chrono::system_clock::now() - start;
    cout << "Время одного потока " << dur.count() << " секунд\n\n";
}
