#include<stdio.h>

void function1()
{
    int i =0;
   for(i=0; i< 1000; i++);
}


void function2()
{
    int i =0;
   for(i=0; i< 2000; i++);
}

int main()
{
    int j=0;

    for (j=0; j< 1000000; j++)
    {
        function1();
        function2();
    }

}

