#include <stdio.h>

void main()
{
    int you , your_date;
    printf("Enter the Rating[0-10] of Your and Your_Date;\n");
    scanf("%d",&you);
    scanf("%d",&your_date);
    if (you<=2 || your_date<=2)
    {
        printf("0");
    }
    else if(you>=8 || your_date>=8)
    {
        printf("2");
    }
    else
    {
        printf("1");
    }
}