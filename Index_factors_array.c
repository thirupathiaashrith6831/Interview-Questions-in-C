#include <stdio.h>

void main()
{
    int size;
    printf("Enter the no. of elements to store:\n");
    scanf("%d",&size);
    int a[size];
    for (int i=0;i<=size-1;i++)
    {
        printf("Enter the %d element:\n",i+1);
        scanf("%d",&a[i]);
    }
    int sum=0;
    for (int i=0;i<=size-1;i++)
    {
       sum=sum+a[i];
    }
    printf("The sum is %d\n",sum);
    for (int i=1;i<=size;i++)
    {
       if((sum%i)==0)
       {
           printf("%d[%d]\n",i,a[i-1]);
       }
    }
}