#include <stdio.h>

void main()
{
    int weekend,cigars;
    printf("Enter the day in number Weekend[1] or not Weekend[0]:\n");
    scanf("%d",&weekend);
    printf("Enter the no. of cigars:\n");
    scanf("%d",&cigars);
    if (weekend==1)
    {
        if (cigars>=40)
        {
            printf("Weekend(true)");
        }
        else
        {
            printf("Weekend(fales)");
        }
    }
    else
    {
        if (cigars>=40 && cigars<=60)
        {
            printf("Weekend(true)");
        }
        else
        {
            printf("Weekend(false)");
        }
    }
}