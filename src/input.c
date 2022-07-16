#include <stdio.h>
#include <string.h>
#include "stddata.h"
#include "match.h"

//*******************************************************************************************************
/*
 *   input_person_name:输入联系人名称
*/
//*******************************************************************************************************
void input_person_name(person* pp,char* pName)
{
    char newName[__P_NAME] = {0};
    printf("请输入新的联系人名称>>");
    while(1)
    {
        scanf("%s",newName);
        fflush(stdin);
        if(newName[__P_NAME-1] != 0)
        {
            int i = 0;
            for (i = 0; i < __P_NAME;i++)   //清除掉newName数组
            {
                newName[i] = 0;
            }
            printf("名字超过字数限制，请重新输入>>");
            continue;
        }
        if (person_name_exact_match(pp, newName) != NULL)
        {
            printf("已有该联系人，请重新输入联系人名称>>");
        }
        else
        {
            strcpy(pName, newName);
            return;
        }
    }
}

//*******************************************************************************************************
/*
 *   input_person_tele:输入联系人电话
*/
//*******************************************************************************************************
void input_person_tele(char* pTele)
{
    char newTele[__P_TELE] = {0};
    printf("请输入新的电话号码>>");
    while(1)
    {
        scanf("%s",newTele);
        fflush(stdin);
        if(newTele[__P_TELE-1] != 0)
        {
            int i = 0;
            for (i = 0; i < __P_TELE;i++)
            {
                newTele[i] = 0;
            }
            printf("号码位数超过位数限制，请检查并重新输入>>");
        }
        else
        {
            strcpy(pTele, newTele);
            return;
        }
    }
}

//*******************************************************************************************************
/*
 *   input_person_addr:输入联系人地址
*/
//*******************************************************************************************************
void input_person_addr(char* pAddr)
{
    char newAddr[__P_ADDR] = {0};
    printf("请输入新的联系人地址>>");
    while(1)
    {
        scanf("%s",newAddr);
        fflush(stdin);
        if(newAddr[__P_ADDR-1] != 0)
        {
            int i = 0;
            for (i = 0; i < __P_ADDR;i++)
            {
                newAddr[i] = 0;
            }
            printf("地址超过字数限制，请重新输入>>");
        }
        else
        {
            strcpy(pAddr, newAddr);
            return;
        }
    }
}

//*******************************************************************************************************
/*
 *   input_person_email:输入联系人邮箱
*/
//*******************************************************************************************************
void input_person_email(char* pEmail)
{
    char newEmail[__P_EMAIL] = {0};
    printf("请输入新的联系人邮箱>>");
    while(1)
    {
        scanf("%s",newEmail);
        fflush(stdin);
        if(newEmail[__P_EMAIL-1] != 0)
        {
            int i = 0;
            for (i = 0; i < __P_EMAIL;i++)
            {
                newEmail[i] = 0;
            }
            printf("邮箱超过位数限制，请检查并重新输入>>");
        }
        else
        {
            strcpy(pEmail, newEmail);
            return;
        }
    }
}

//*******************************************************************************************************
/*
 *   sure_to_change：对比较重要的操作进行进一步确认。
 *                       确认则返回1，否则返回0。
*/
//*******************************************************************************************************
int sure_to_change()
{
    printf("输入y/Y确认,n/N返回>>");
    while(1)
    {
        char ret = getchar();
        if(ret == 'n' || ret == 'N')
        {
            fflush(stdin);
            return 0;
        }
        else if(ret == 'y' || ret == 'Y')
        {
            fflush(stdin);
            return 1;
        }
    }
}