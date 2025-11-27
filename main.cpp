#include <iostream>
#include "generator.h"
#include <random>

using namespace std;

CoroutineA printerCoroutine(shared_ptr<int> number)
{
    while (true)
    {
        cout << "Coroutine A: " << *number << '\n';
        co_await suspend_always{};
    }
}

Generator randomNumberGenerator(CoroutineA* printer)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 256);
    while (true){
        int num = dist(gen);
        co_await num;
        co_yield num;
    }
}

int main(){
    auto number = make_shared<int>(0);
    auto printer = (printerCoroutine(number));
    auto gen = randomNumberGenerator(&printer);
    
    for ( int i = 0; i < 15; ++i )
    {
        int value = gen();
        cout << "Generated value :" << value << '\n';
        cout << '\n'; 
    }
}