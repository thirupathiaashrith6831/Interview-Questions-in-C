#include <stdio.h>

void main()
{
    int a,b,c;
    printf("Enter the [a,b,c] values:\n");
    scanf("%d",&a);
    scanf("%d",&b);
    scanf("%d",&c);
    int z=a+b;
    int y=b+c;
    int x=a+c;
 //----------------------------------------------
    if (z==10 || y==10 || x==10)
    {
        printf("10");
    }
    else if(a>10 || b>10 || c>10)
    {
        printf("5");
    }
    else
    {
        printf("0");
    }
}