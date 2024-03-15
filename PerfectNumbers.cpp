#include <iostream>
#include <set>
#include <cmath>

using namespace std;

set<int> getAllDivisors(int);
bool isPerfectNumber(int, set<int>);
 
int main(int argc, char ** argv)
{
    if (argc < 2)
        exit(-1);

    int numsToCheck = atoi(argv[1]);

    for (int i = 1; i < numsToCheck; i++)
    {
        auto divisors = getAllDivisors(i);

        if (isPerfectNumber(i, divisors))
            cout << "Found perfect number: " << i << endl;
    }
}

set<int> getAllDivisors(int num)
{
    set<int> divisors;
    for (int i = 1; i < num / 2; i++)
    {
        if (divisors.count(i) != 0)
            continue;

        if (num % i == 0)
        {
            divisors.insert(i);
            divisors.insert(num / i);
        }
    }
    return divisors;
}

/* Sigma Function */
bool isPerfectNumber(int num, set<int> divisors)
{
    int sum = 0;
    for (int i : divisors)
        sum += i;

    if (sum == 2 * num)
        return true;
    
    return false;
}