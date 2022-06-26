#include <cstddef>
#include<iostream>
#include<cstdlib>
using namespace std;
int main() {
   int max;
   max = 2; //set the upper bound to generate the random number
    srand(time(NULL));
   cout << "The random number is: "<<rand()%max<<endl;
   if(rand()%max == 0)
   {
    cout<<"Is valid"<<endl;
   }
}
