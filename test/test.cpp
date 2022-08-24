#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

long fibonacci(int n) {
    static vector<long> fib {1, 1};

    if(fib.size() < n) {
        for(int i = fib.size(); i <= n; i++) {
            fib.push_back(fib[i-1] + fib[i-2]);
        }
    }
    return fib[n-1];
}

int main()
{
   vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

   for (const string& word : msg)
   {
      cout << word << " ";
   }
   cout << endl; 

   long num = fibonacci(50);
   printf("%ld\n", num); 
   num = fibonacci(5);
   printf("%ld\n", num);
   num = fibonacci(50);
   printf("%ld\n", num);
   /**
   vector<int> fib {1, 1};
   for(int i = 2; i < 10; i++) {
    fib.push_back(fib[i-1] + fib[i-2]);
   }
   for(int value: fib) {
    printf("%d\n", value);
   }
   **/
}