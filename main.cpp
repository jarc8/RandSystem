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


class symNumber//I dont know if symbolic number exists in c++ so I made my own
{
    public: 
        float number;

        symNumber(){}

        symNumber(float number)
        {
            this->number = number;
        }

        symNumber operator +(const symNumber &a)
        {
            return symNumber(this->number + a.number);
        }

        symNumber operator -(const symNumber &a)
        {
            return symNumber(this->number - a.number);
        }

        symNumber operator *(const symNumber &a)
        {
            return symNumber(this->number * a.number);
        }

        symNumber operator /(const symNumber &a)
        {
            if (a.number == 0)
            {
                cout << "Error: Division by zero!" << '\n';
                return symNumber(0);
            }
            return symNumber(this->number / a.number);
        }

        friend ostream& operator <<(ostream &out, symNumber &obj)
        {
            out << obj.number;
            return out;
        }

        friend istream& operator >>(istream &in, symNumber &obj)
        {
            in >> obj.number;
            return in;
        }

        friend ostream& operator <<(ostream &out, const symNumber &obj)
        {
            out << obj.number;
            return out;
        }

        bool operator ==(symNumber &a)
        {
            return this->number == a.number;
        }

        bool operator !=(symNumber &a)
        {
            return this->number != a.number;
        }

        bool operator >(symNumber &a)
        {
            return this->number > a.number;
        }

        bool operator <(symNumber &a)
        {
            return this->number < a.number;
        }

        bool operator >=(symNumber &a)
        {
            return this->number >= a.number;
        }

        bool operator <=(const symNumber &a)
        {
            return this->number <= a.number;
        }

        symNumber operator ^(int exponent)
        {
            symNumber result(1);
            for (int i = 0; i < exponent; i++)
                result = result * (*this);
            return result;
        }

        float sqrt()
        {
            if (number < 0)
            {
                cout << "Error: Cannot compute square root of a negative number!" << '\n';
                return 0; // Return zero or handle error as needed
            }
            return std::sqrt(number);
        }
};

class symFunction
{
    public:
        symNumber a, b, c, d;

        symFunction(symNumber a, symNumber b, symNumber c, symNumber d)
        {
            this->a = a;
            this->b = b;
            this->c = c;
            this->d = d;
        }

        symNumber evaluate(symNumber x)
        {
            return abs(symNumber(a * (x ^ 3) + b * (x ^ 2) + c * x + d).number);
        }

        float maxY(symNumber minX, symNumber maxX)
        {
            symNumber maxY = symNumber(0);
            int samples = 10000;
            float step = (maxX.number - minX.number) / samples;
            for (int i = 0; i <= samples; i++)
            {
                float xval = minX.number + i * step;
                symNumber y = evaluate(symNumber(xval));
                if (y > maxY)
                    maxY = y;
            }
            return maxY.number;
        }

        float minY(symNumber minX, symNumber maxX)
        {
            symNumber minY = symNumber(0);
            int samples = 10000;
            float step = (maxX.number - minX.number) / samples;
            for (int i = 0; i <= samples; i++)
            {
                float xval = minX.number + i * step;
                symNumber y = evaluate(symNumber(xval));
                if (y < minY)
                    minY = y;
            }
            return minY.number;
        }
};

class symPoint
{
    public:
        symNumber x, y;

        symPoint(symNumber x, symNumber y)
        {
            this->x = x;
            this->y = y;
        }

        symPoint()
        {
            this->x = 0;
            this->y = 0;
        }

        void generate(float minX, float maxX, float minY, float maxY)
        {
            x = symNumber(minX + (maxX - minX) * (static_cast<float>(rand()) / RAND_MAX));
            y = symNumber(minY + (maxY - minY) * (static_cast<float>(rand()) / RAND_MAX));
        }
};

bool menu();
void throwDie();
void pi();
void area();
void piSplit(long long int toToss);

int main()
{
    srand(time(0));
    while(menu());
    system("pause");
    return 0;
}

bool menu()
{
    system("cls");
    cout <<"Welcome to RandSystem" << '\n';
    cout << "select an option using nums" << '\n';
    cout << "1-throw a die" << '\n';
    cout << "2-calculate pi" << '\n';
    cout << "3-area below a function (scuffed)" << '\n';
    cout << "4-exit" << '\n';
    cout << "select an option using nums: ";

    switch (getch())
    {
        case '1':
            system("cls");
            throwDie();
            break;

        case '2':
            system("cls");
            pi();
            break;

        case '3':
            system("cls");
            area();
            break;

        case '4':
            return false;
            break;
        
        default:
            cout << "this is not an option" << '\n';
            break;
    }
    return true;
}

void throwDie()
{
    int n;
    cout << "number of dies to throw: ";
    cin >> n;

    int tries;
    cout << "insert number of throws: ";
    cin >> tries;

    int results[6*n - n + 1] = {0};

    int sum = 0;
    for (int i = 0; i < tries; i++)
    {
        sum = 0;
        for (int j = 0; j < n; j++)
            sum += rand() % 6 + 1;
        results[sum - n]++;
    }

    for (int i = 0; i < sizeof(results) / sizeof(int); i++)
        cout << "the number " << setw(10) << i + n - 1 << setw(15) << " appeared " << setw(10) << results[i] << setw(10) <<" times" << '\n'; 
    
    getch();
    cout << '\n' << "press again to confirm" << '\n';
    getch();
}

void pi()
{
    long long int totalTosses;
    cout << "insert number of tries: ";
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

    cout << "out of " << totalTosses << " tries, " << piHits << " were inside the quarter of the circle" << '\n';
    cout << fixed << setprecision(15);
    cout << "the approximation of pi is: " << 4*(double)piHits/totalTosses << '\n';
    cout << "the error is: " << abs(3.14159265358979323846 - 4*(double)piHits/totalTosses) << '\n';
    cout << "the error in percent is: " << abs((3.14159265358979323846 - 4*(double)piHits/totalTosses) / 3.14159265358979323846 * 100) << "%" << '\n';
    getch();
    cout << '\n' << "press again to confirm" << '\n';
    getch();
}

void area()
{
    cout << "this version can only calculate the area below functions alike y = ax^3 + bx^2 + cx + d" << '\n';

    symNumber coefficients[4] = {0};
    for (int i = 0; i < sizeof(coefficients)/sizeof(symNumber); i++)
    {
        cout << "insert coefficient " << (char)(i+97) << ": ";
        cin >> coefficients[i];
    }
    symFunction f(coefficients[0], coefficients[1], coefficients[2], coefficients[3]);

    symNumber minX, maxX;
    cout << "insert the first x: ";
    cin >> minX;
    cout << '\n';
    cout << "insert the second x: ";
    cin >> maxX;
    cout << '\n';

    symNumber minY = f.minY(minX, maxX);
    symNumber maxY = f.maxY(minX, maxX);

    symPoint p;

    long long int tries = 0;
    long long int hit = 0;
    long long int toTry = 0;

    cout << "insert the number of tries: ";
    cin >> toTry;
    cout << '\n';

    while (tries < toTry)
    {
        p.generate(minX.number, maxX.number, minY.number, maxY.number);
        if (p.y <= f.evaluate(p.x))
            hit++;
        tries++;
        if(!(tries%1000000))
            cout << "tried: " << tries << " times" << '\n';
    }

    cout << "out of " << toTry << " tries, " << hit << " were inside the area" << '\n';
    cout << "the approximation of the area is: " << (maxX.number - minX.number) * (maxY.number - minY.number) * (double)hit/tries << '\n';
    getch();
    cout << '\n' << "press again to confirm" << '\n';
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