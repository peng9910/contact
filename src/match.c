#include <stdio.h>
#include "stddata.h"
#include "file.h"
#include "match.h"
#include "show.h"
#include <stdlib.h>
#include <string.h>

extern int gArr_size;              //gArr_size来记录分组的总个数
extern int pArr_size;           //引入外部变量，pArr_size记录的是全部联系人的数量
extern int fmatch_size;         //引入外部变量，fmatch_size记录的是模糊匹配到的联系人的数量
extern int iArr_size;           //引入外部变量，iArr_size来记录通话记录的总条数
extern int imatch_num;              //match_num来记录pid匹配通话记录的条数
extern int iunmatch_num;            //unmatch_num来记录pid未匹配通话记录的条数

//*******************************************************************************************************
/*
    str_match:将两个字符串从开头匹配到'\0'处。
              严格匹配成功返回2，包含匹配成功返回1，匹配失败返回0
*/
//*******************************************************************************************************
int str_match(char* s1,char* s2)
{
    int i = 0;
    while(1)
    {
        if((*(s1+i) != 0) && (*(s2+i) != 0) && (*(s1+i) == *(s2+i)))
        {
            i++;
        }
        else
        {
            break;
        }
    }
    if(*(s1+i) == 0 && *(s2+i) == 0)
    {
        return 2;
    }
    else if(*(s1+i) == 0 || *(s2+i) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//*******************************************************************************************************
/*
    get_pid:获取一个最小可用的id值
*/
//*******************************************************************************************************
int get_pid(person* pp)
{
    int i = 1;
    while(1)
    {
        if(!pid_match(pp,i))
        {
            return i;
        }
        i++;
    }
}

//*******************************************************************************************************
/*
    pid_match:看传入的pid是否存在。
              如果存在则返回指向该pid的联系人信息的指针，
              如果不存在则返回NULL。
*/
//*******************************************************************************************************
person* pid_match(person* pp,int pid)
{
    int i = 0;
    for (i = 0; i < pArr_size;i++)
    {
        if(pid == (pp+i)->pid)
        {
            return (pp+i);
        }
    }
    return NULL;
}

//*******************************************************************************************************
/*
    name_include:判断已有的名字(std_s)是否包含给定的名字(give_s)信息。
                如果完全相同，则返回2
                如果包含，则返回1
                如果没有，则返回0
*/
//*******************************************************************************************************
int name_include(char* give_s,char* std_s)
{
    int g_len = strlen(give_s);
    int s_len = strlen(std_s);
    int position_flag = 0;
    if(g_len > s_len)
    {
        return 0;
    }
    else
    {
        for (position_flag = 0; position_flag <= (s_len - g_len);position_flag++)
        {
            switch(str_match(give_s, std_s + position_flag))
            {
                case 2:
                    if(position_flag == 0)
                    {
                        return 2;
                    }
                    else
                    {
                        return 1;
                    }
                    break;
                case 1:
                    return 1;
            }
        }
        return 0;
    }
}

//*******************************************************************************************************
/*
 *   name_fuzzy_match:模糊匹配。给定字符串，返回由 有该字符串的所有名字 组成的地址的指针。
 *                               如果都匹配不到，返回NULL。
 *               fmatch_size保存着匹配到的联系人数量 
 !              用完记得free掉开辟的内存
*/
//*******************************************************************************************************
person* person_name_fuzzy_match(person* pp,char* give_s)
{
    int i = 0;
    fmatch_size = 0;         //在每次搜索前初始化为0
    person* fmatch_p = (person*)malloc(sizeof(person));
    if(fmatch_p != NULL)
    {
        person *ptr = NULL;
        for (i = 0; i < pArr_size;i++)
        {
            if(name_include(give_s, (pp + i)->name))     //匹配到之后
            {
                *(fmatch_p + fmatch_size) = *(pp+i);     //拷贝数据过来
                fmatch_size++;
                ptr = (person*)realloc(fmatch_p,(fmatch_size+1)*sizeof(person));
                if(ptr != NULL)
                {
                    fmatch_p = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("名字匹配：查询扩容失败\n");
                    return fmatch_p;
                }
            }
        }
        printf("名字匹配：匹配完成\n");
        return fmatch_p;
    }
    else
    {
        printf("名字匹配：开辟内存失败\n");
        return NULL;
    }
}

//*******************************************************************************************************
/*
 *   name_exact_match:精确匹配。
 *                    只有名字和给定的字符完全匹配，才返回保存着该联系人信息的地址的指针。
 *                    否则返回NULL
*/
//*******************************************************************************************************
person* person_name_exact_match(person* pp,char* name_p)
{
    int i = 0;
    for (i = 0; i < pArr_size;  i++)
    {
        if(str_match(name_p, (pp+i)->name) == 2)
        {
            return pp+i;
        }
    }
    return NULL;
}

//*******************************************************************************************************
/*
 *   name_combine_match:结合精确匹配和模糊匹配。
 *                       如果一次精确匹配成功，则返回保存着该联系人信息的地址的指针
 *                       否则进行模糊匹配，给出参考选项
 *                       并再一次进行精确匹配，成功则返回保存着该联系人信息的地址的指针，否则返回NULL
*/
//*******************************************************************************************************
person* person_name_combine_match(person* pp)
{
    printf("请输入联系人名称>");
    char name[__P_NAME] = {0};
    scanf("%s",name);
    fflush(stdin);
    person *match_p = NULL;
    if(!(match_p = person_name_exact_match(pp,name)))
    {
        if((match_p = person_name_fuzzy_match(pp,name)))
        {
            printf("没有该联系人，或许您指的是以下联系人\n");
            show_person(match_p,fmatch_size);
            free(match_p);
            match_p = NULL;
            printf("再次输入联系人名称>");
            scanf("%s",name);
            fflush(stdin);
            if((match_p = person_name_exact_match(pp,name)))
            {
                return match_p;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
    return match_p;
}

//*******************************************************************************************************
/*
 *   search_person:按照名字搜索联系人。
 *                 将所有的联系人的名字与输入的字符进行匹配，
 *                 匹配成功则打印出来。
*/
//*******************************************************************************************************
void search_person(person* pp)
{
    printf("请输入要查询的联系人的名称>");
    char name[__P_NAME] = {0};
    scanf("%s",name);
    fflush(stdin);
    person *match_p = NULL;
    match_p = person_name_fuzzy_match(pp,name);
    show_person(match_p,fmatch_size);
    free(match_p);
    match_p = NULL;
}

//*******************************************************************************************************
/*
    pid_get_info:在通话记录中匹配pid。
                 如果mode为0,则返回该pid的所有通话记录的地址。
                 否则返回非该pid的所有通话记录的地址。
                 如果malloc失败，则返回NULL。
        ##记得free掉动态内存##
*/
//*******************************************************************************************************
info* pid_get_info(info* ip,int pid,int mode)
{
    info *match_i = (info *)malloc(sizeof(info));
    info *unmatch_i = (info *)malloc(sizeof(info));
    if(match_i != NULL && unmatch_i != NULL)
    {
        int i = 0;
        imatch_num = 0;
        iunmatch_num = 0;
        for (i = 0; i < iArr_size;i++)
        {
            if(pid == (ip+i)->pid)
            {
                (match_i + imatch_num)->pid = (ip + i)->pid;
                strcpy((match_i+imatch_num)->date,(ip+i)->date);
                strcpy((match_i+imatch_num)->ttime,(ip+i)->ttime);   //拷贝内容
                imatch_num++;
                info *ptr = NULL;
                ptr = realloc(match_i,(imatch_num+1)*sizeof(info));
                if(ptr != NULL)
                {
                    match_i = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("通话记录：扩容失败\n");
                }
            }
            else
            {
                (unmatch_i + iunmatch_num)->pid = (ip + i)->pid;
                strcpy((unmatch_i+iunmatch_num)->date,(ip+i)->date);
                strcpy((unmatch_i+iunmatch_num)->ttime,(ip+i)->ttime);   //拷贝内容
                iunmatch_num++;
                info *ptr = NULL;
                ptr = realloc(unmatch_i,(iunmatch_num+1)*sizeof(info));
                if(ptr != NULL)
                {
                    unmatch_i = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("通话记录：扩容失败\n");
                }
            }
        }
        if(mode == 0)
        {
            free(unmatch_i);
            unmatch_i = NULL;
            return match_i;
        }
        else
        {
            free(match_i);
            match_i = NULL;
            return unmatch_i;
        }
    }
    else
    {
        printf("通话记录：内存开辟失败\n");
        return NULL;
    }
}

//****************************************************************
/*
 * 分组名称的精确匹配。
 * 如果给定的字符串和分组的名称完全匹配，则返回与gp的偏移量
 * 否则返回-1
*/
//****************************************************************
int group_name_exact_match(group* gp,char* name)
{
    int offset = 0;
    for (offset = 0; offset < gArr_size;offset++)
    {
        if(str_match(name,(gp+offset)->name) == 2)
        {
            return offset;
        }
    }
    return -1;
}

//*******************************************************************************************************
/*
 *   pid_remove_group_person:删除pGroup的pid[__G_PID]中的pid
 */
//*******************************************************************************************************
int pid_remove_group_person(group *pGroup,int pid)
{
    int i = 0;
    for (i = 0; i < __G_PID; i++)
    {
        if((pGroup->pid)[i] == pid)
        {
            (pGroup->pid)[i] = 0;
            (pGroup->count)--;
            return 1;
        }
    }
    return 0;
}