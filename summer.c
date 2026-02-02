#include <stdio.h>

void main()
{
int summer,temp;
printf("Enter the day in number summer[1] or not summer[0]:\n");
scanf("%d",&summer);
printf("Enter the temperature on the day:\n");
scanf("%d",&temp);
if (summer==1){
    if ((temp>=60)&&(temp<=100))
    {
        printf("summer(true)");
    }
    else
    {
        printf("summer(false)");
    }
}
else{
    if ((temp>=60)&&(temp<=90))
    {
         printf("normal(true)\n");
    }
    else
    {
        printf("normal(false)\n");
    }
}
}