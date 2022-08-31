#include <iostream>
using namespace std;
int main()
{
	int (*result)[640] = new int[480][640] ; // Matrix of integers
	//
	int j , k ;
	//
	for( j = 0 ; j < 480 ; j++ )
		for( k = 0 ; k < 640 ; k++ )
			result[j][k] = 1 ; // Initialize to be all 1's 
	//
    cout << "P3 640 480 255" << endl ;
    //
	for( j = 0 ; j < 480 ; j++ )
		for( k = 0 ; k < 640 ; k++ )
            if( result[j][k] == 0 )
                cout << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else
                cout << "10 250 210" << endl ; // if 1 is found, then print 0 1 0
	//
	return 0 ;
}

// make a method
/*
int f(int a[][640]) {return 0;} only have to specify one of the dimensions, to do the i*col+col_idx


(int)(640 + x); 
*/
/* 
void f(int &x, const image &i) {
	x = 8;  this is passing BY REFERENCE. if you omit the & it gets passed by VALUE. 
	Passing by REFERENCE is useful for passing in large objects, to prevent it being copied.
	The "const" means "I don't mean to change it." So even though you are passing by reference you don't want to edit it. 
	^^ it cannot be edited. That reference, whatever is there, is immutable  
}

int x;
int* p;
p = &x; NOW p will be pointing to x

Thing* i;
i = new Thing() <-- gets placed in HEAP
______________________________________________________
NEW EXAMPLE
int x;
int* p;
p = &x; 
--> We can either say:
x = 8;
*p = 8; DEREFERENCING THE POINTER

Creating arrays ___ --- ___ --- ___ --- ___
int a[10];
p = new int[10]; MAKE A NEW OBJECT and have ME point to it
const int* a; THIS POINTER ALWAYS is stuck to the array
*(p+1) = 5; <-- DON'T go too far!
Arrays start at 0 because the pointer starts at 0 
int*b = a - 1;
b[1] is NOW the FIRST ELEMENT of a
*(b+1) = 7
*/

/*
Random Numbers
#include <stdlib.h>
#include <time.h>
0 < rand() < RAND_MAX (returns an integer)
double randunumber = rand()/RAND_MAX
srand(int something) sets the random seed. 
calling rand() multiple times gives you different number 

Using time approach
n = time(NULL); <-- NULL pointer is not actually pointing to anyhting
Epoch (January 1st, 1970)
You can use time as the seed for srand(). Call srand() ONCE. 
*/

