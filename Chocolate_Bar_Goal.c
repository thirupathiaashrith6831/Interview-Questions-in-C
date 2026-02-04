#include <stdio.h>

void main()
{
int big,small,goal;
printf("Enter the no. of small chocolate bars:\n");
scanf("%d",&small);
printf("Enter the no. of big chocolate bars:\n");
scanf("%d",&big);
printf("Enter the total weight of chocolate:\n");
scanf("%d",&goal);
int req=goal/5;
if (req<=big)
{
    int rem = goal-(req*5);
    if (rem<=small)
    {
        printf("%d",rem);
    }
    else 
    {
        printf("-1");
    }
}
else
{
    int rem1=goal-(big*5);
    if (rem1<=small)
    {
        printf("%d",rem1);
    }
    else
    {
        printf("-1");
    }
}
}