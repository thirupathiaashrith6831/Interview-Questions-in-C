#include <stdio.h>

void main()
{
    int size;
    printf("Enter the no. of elements to store:\n");
    scanf("%d",&size);
    int a[size];
    for (int i=0;i<=size-1;i++)
    {
        printf("Enter the %d elemnet:\n",i+1);
        scanf("%d",&a[i]);
    }
    int sum=0;
    for (int i=0;i<=size-1;i++)
    {
        if(a[i]!=13)
        {
            sum=sum+a[i];
        }
        else
        {
            break;
        }
    }
    printf("the sum is %d\n",sum);
    if (sum==0)
    { 
        printf("-1");
    }
    else
    {
        printf("%d\n",sum);
    }   
    
}