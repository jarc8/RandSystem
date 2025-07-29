#include <iostream>
#include <conio.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

long long int piHits = 0;
mutex piMutex;

void pi();
void piSplit(long long int toToss);

int main()
{
    srand(time(0));

    bool active = true;

    while (active)
    {
        cout <<"Welcome to RandSystem piWThreads edition" << '\n';
        cout << "1 - calculate pi" << '\n';
        cout << "2 - exit" << '\n';
        cout << "choose option using nums: ";

        if(getch() == '1')
            pi();
        else if(getch() == '2')
            active = false;
        else
            cout << '\n' << "invalid option, try again" << '\n';
    }
    
    return 0;
}
    

void pi()
{
    long long int totalTosses;
    cout << '\n' << "insert number of tries: ";
    cin >> totalTosses;
    int threads;
    cout << "insert the number of threads to use(at your own risk): ";
    cin >> threads;

    vector<thread> threadPool;

    for (int i = 0; i < threads; i++)
    {
        threadPool.push_back(thread(piSplit, totalTosses / threads));
    }

    for (auto &t : threadPool)
    {
        t.join();
    }


    cout << '\n' << "out of " << totalTosses << " tries, " << piHits << " were inside the quarter of the circle" << '\n';
    cout << fixed << setprecision(15);
    cout << "the approximation of pi is: " << 4*(double)piHits/totalTosses << '\n';
    cout << "the error is: " << abs(3.14159265358979323846 - 4*(double)piHits/totalTosses) << '\n';
    cout << "the error in percentage is: " << abs((3.14159265358979323846 - 4*(double)piHits/totalTosses) / 3.14159265358979323846 * 100) << "%" << '\n';
    getch();
    cout << '\n' << "press again to confirm" << '\n' << '\n';
    getch();
}

void piSplit(long long int toToss)
{
    long long int hit = 0;
    double x, y;

    for (long long int i = 0; i < toToss; i++)
    {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0)
            hit++;
    }
    
    lock_guard<mutex> lock(piMutex);
    piHits += hit;
}