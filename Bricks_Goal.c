#include <stdio.h>

void main()
{
int big,small,goal;
printf("Enter the no. of small bricks:\n");
scanf("%d",&small);
printf("Enter the no. of big bricks:\n");
scanf("%d",&big);
printf("Enter the goal:\n");
scanf("%d",&goal);
int required =goal/5;
if (required<=big)
{
    int rem = goal-(required*5);
    if (rem<=small)
    {
        printf("true");
    }
    else 
    {
        printf("false");
    }
}
else
{
    int rem=goal-(big*5);
    if (rem<=small)
    {
        printf("true");
    }
    else
    {
        printf("false");
    }
}
}