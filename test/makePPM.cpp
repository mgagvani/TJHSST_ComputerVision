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
    cout << "P3 640 480 1" << endl ;
    //
	for( j = 0 ; j < 480 ; j++ )
		for( k = 0 ; k < 640 ; k++ )
            if( result[j][k] == 0 )
                cout << "0 0 0" << endl ; // if 0 is found, then print 0 0 0
            else
                cout << "0 1 0" << endl ; // if 1 is found, then print 0 1 0
	//
	return 0 ;
}
