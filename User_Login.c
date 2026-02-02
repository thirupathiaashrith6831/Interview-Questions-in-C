#include <stdio.h>

void main()
{   int id,pwd;
    int Uid=5371;
    int Upwd=9647;
    printf("Enter the User_Id:\n");
    scanf("%d",&id);
    if (Uid==id)
    {  
        printf("Enter the Password:\n");
        scanf("%d",&pwd);
        if (Upwd==pwd)
        {
            printf("Login Sucessfull");
        }
        else
        {
            printf("Incorrect Password");
        }
    }
    else
    {
        printf("Incorrect User_Id");
    }
}