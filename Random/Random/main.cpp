//
//  main.cpp
//  Random
//
//  Created by Andrew Musk on 2017/02/02.
//  Copyright © 2017 Andrew Musk. All rights reserved.
//

#include <iostream>
using namespace std;


void swap(int *pa, int *pb)
{
    int temp = *pa;
    *pa= *pb;
    *pb=temp;
}


int main()
{
    int a=5, b=6;
    
    swap(&a,&b);
    
    cout << a << endl;
    cout << b << endl;
    
}
