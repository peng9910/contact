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
extern int PersonSortNum;           //联系人的排序规则数字
extern int GroupSortNum;               //分组的排序规则数字

//*******************************************************************************************************
/*
 *   show_info:通用的显示通话记录的函数。
*/
//*******************************************************************************************************
void show_info(person* pp,info* ip,int sz)
{
    int i = 0;
    printf("%-*.*s\t%-*.*s\t%-*.*s\t%-*.*s\n",  __P_NAME,__P_NAME,"联系人名称",
                                            __P_TELE,__P_TELE,"电话",
                                            __I_DATE,__I_DATE,"通话时间",
                                            __I_TTIME,__I_TTIME,"通话时长");
    for (i = 0; i < sz;i++)
    {
        info* cur_ip = ip + i;
        int cur_pid = cur_ip->pid;
        person *cur_pp = pid_match(pp,cur_pid);
        printf("%-*.*s\t%-*.*s\t%-*.*s\t%-*.*s\n",  __P_NAME,__P_NAME,cur_pp->name,
                                                    __P_TELE,__P_TELE,cur_pp->tele,
                                                    __I_DATE,__I_DATE,cur_ip->date,
                                                    __I_TTIME,__I_TTIME,cur_ip->ttime);
    }
    printf("共 %d 条结果\n",sz);
}

//*******************************************************************************************************
/*
 *   my_group_sort:联系人排序函数
*/
//*******************************************************************************************************
static void my_group_sort(group* gp,int *offset,int sz,int mode)
{
    int i = 0;
    int j = 0;
    int ret = 0;
    for (i = 0; i < sz;i++)
    {
        int maxcount = 0;
        for (j = 0; j < sz;j++)
        {
            if(i == j)
            {
                continue;
            }
            ret = strcmp((gp+i)->name,(gp+j)->name);
            if(ret > 0)
            {
                maxcount++;
            }
        }
        if(mode == 1)   // * 从a-z排序
        {
            *(offset + maxcount) = i;
        }
        else
        {
            *(offset + sz - maxcount-1) = i;
        }
    }
}

//*******************************************************************************************************
/*
 *   show_groups:顺序展示sz个分组信息
*/
//*******************************************************************************************************
void show_groups(group* gp,int sz)
{
    int i = 0;
    printf("%-*.*s\t%s\n",__G_NAME,__G_NAME,"分组名称","联系人数量");

    int *offset = (int *)malloc(sizeof(int)*sz);
    if(GroupSortNum == 2)
    {
        my_group_sort(gp,offset,sz,1);
    }
    else if(GroupSortNum == 3)
    {
        my_group_sort(gp,offset,sz,0);
    }
    else
    {
        for (i = 0; i < sz;i++)
        {
            *(offset + i) = i;
        }
    }

    for (i = 0; i < sz;i++)
    {
        printf("%-*.*s\t%d\n",__G_NAME,__G_NAME,(gp+*(offset+i))->name,(gp+*(offset+i))->count);
    }
    printf("共有%d条结果\n",sz);
}

//*******************************************************************************************************
/*
 *   my_person_sort:联系人排序函数
*/
//*******************************************************************************************************
static void my_person_sort(person* pp,int *offset,int sz,int mode)
{
    int i = 0;
    int j = 0;
    int ret = 0;
    for (i = 0; i < sz;i++)
    {
        int maxcount = 0;
        for (j = 0; j < sz;j++)
        {
            if(i == j)
            {
                continue;
            }
            ret = strcmp((pp+i)->name,(pp+j)->name);
            if(ret > 0)
            {
                maxcount++;
            }
        }
        if(mode == 1)   // * 从a-z排序
        {
            *(offset + maxcount) = i;
        }
        else
        {
            *(offset + sz - maxcount-1) = i;
        }
    }
}

//*******************************************************************************************************
/*
 *   show_person:顺序展示sz个联系人信息
*/
//*******************************************************************************************************
void show_person(person* pp,int sz)
{
    if(pp == NULL)
    {
        printf("读取出现错误，已返回\n");
        return;
    }
    // %-10s\t                                  "编号",
    printf("%-*.*s\t%-*.*s\t%-*.*s\t%-*.*s\n",
                                                __P_NAME,__P_NAME,"联系人名称",
                                                __P_TELE,__P_TELE,"电话",
                                                __P_ADDR,__P_ADDR,"地址",
                                                __P_EMAIL,__P_EMAIL,"电子邮箱");
    int i = 0;
    int *offset = (int *)malloc(sizeof(int)*sz);
    if(PersonSortNum == 2)
    {
        my_person_sort(pp,offset,sz,1);
    }
    else if(PersonSortNum == 3)
    {
        my_person_sort(pp,offset,sz,0);
    }
    else
    {
        for (i = 0; i < sz;i++)
        {
            *(offset + i) = i;
        }
    }
    for (i = 0;i<sz;i++)
    {
        // %-10d\t                                  (pp+*(offset+i))->pid,
        printf("%-*.*s\t%-*.*s\t%-*.*s\t%-*.*s\n",
                                                __P_NAME,__P_NAME,(pp+*(offset+i))->name,
                                                __P_TELE,__P_TELE,(pp+*(offset+i))->tele,
                                                __P_ADDR,__P_ADDR,(pp+*(offset+i))->addr,
                                                __P_EMAIL,__P_EMAIL,(pp+*(offset+i))->email);
    }
    free(offset);
    offset = NULL;
    printf("共有 %d 条结果\n",sz);
}

//*******************************************************************************************************
/*
 *     show_person_info:显示与某个联系人的通话记录。
*/
//*******************************************************************************************************
void show_person_info(person* pp,info* ip)
{
    person *match_p = NULL;
    if((match_p = person_name_combine_match(pp)))
    {
        info* match_i = pid_get_info(ip,match_p->pid,0);
        if(match_i != NULL)
        {
            show_info(pp,match_i,imatch_num);
        }
    }
    else
    {
        printf("未匹配到联系人\n");
    }
}

//*******************************************************************************************************
/*
 *   show_recent_info:打印最近的通话记录。
*/
//*******************************************************************************************************
void show_recent_info(person* pp,info* ip)
{
    int limit = MIN(iArr_size,__RECENT_INFO);
    info *cur_ip = ip + iArr_size - limit;
    show_info(pp,cur_ip,limit);
}

//*******************************************************************************************************
/*
 *   show_group_person:顺序展示分组中的各联系人信息
*/
//*******************************************************************************************************
void show_group_person(group* match_gp,person* pp)
{
    int i = 0;
    int pidCount = 0;   // *记录有效的pid个数，因为前面的pid有可能为0
    int p_num = match_gp->count;
    printf("分组：%-*.*s\n",__G_NAME,__G_NAME,match_gp->name);
    person *pArr;
    if(p_num == 0)
    {
        printf("该分组暂无联系人\n");
        return;
    }
    person *ptr = (person *)malloc(p_num*sizeof(person));
    for (i = 0;((pidCount < p_num) && (i < __G_PID));i++)
    {
        if((match_gp->pid)[i] == 0)
        {
            continue;
        }
        pArr = pid_match(pp,(match_gp->pid)[i]);
        (ptr+pidCount)->pid = pArr->pid;
        strcpy((ptr+pidCount)->name,pArr->name);
        strcpy((ptr+pidCount)->tele,pArr->tele);
        strcpy((ptr+pidCount)->addr,pArr->addr);
        strcpy((ptr+pidCount)->email,pArr->email);
        pidCount++;
    }
    show_person(ptr,p_num);
    free(ptr);
    ptr = NULL;
}

//*******************************************************************************************************
/*
 *  show_group：打印分组。
*/
//*******************************************************************************************************
void show_group(group *gp,person* pp,int sz)
{
    show_groups(gp,sz);
    printf("请输入要查询的分组的名称>>");
    char name[__G_NAME] = {0};
    scanf("%s",name);
    fflush(stdin);
    if(sz != 0)
    {
        int offset = group_name_exact_match(gp,name);
        if(offset != -1)   // *分组名匹配成功
        {
            show_group_person((gp+offset),pp);
        }
        else
        {
            printf("未找到该分组...\n");
        }
    }
}