#include <stdio.h>
#include "stddata.h"
#include "file.h"
#include "match.h"
#include "show.h"
#include "menu.h"
#include "input.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

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
 *   init_file:文件初始化，如果文件不存在就新建一个
*/
//*******************************************************************************************************
void init_file()
{
    FILE *fp = fopen("../data/group.txt","a");
    fclose(fp);
    fp = NULL;
    fp = fopen("../data/person.txt","a");
    fclose(fp);
    fp = NULL;
    fp = fopen("../data/info.txt","a");
    fclose(fp);
    fp = NULL;
    fp = fopen("../data/setting.txt","a");
    fclose(fp);
    fp = NULL;
    fp = fopen("../data/setting.txt","r");
    sortset psort = {0};
    sortset *pSort = &psort;
    fread(pSort,sizeof(sortset),1,fp);
    if(pSort->groupsort != 0 && pSort->groupsort != 0)
    {
        PersonSortNum = pSort->personsort;
        GroupSortNum = pSort->groupsort;
    }
    fclose(fp);
    fp = NULL;
}


//*******************************************************************************************************
/*
 *   get_person:获得联系人文件中的信息，返回person类型的指针,里面存放着所有的联系人信息。
 *             还能得到联系人的总个数 pArr_size
    !    ##使用该函数一定记得free掉内存。##
*/
//*******************************************************************************************************
person* get_person()
{
    FILE *fp = NULL;
    char src_log[] = "联系人";
    pArr_size = 0;      //每次查询都初始化长度为0
    fp = fopen("../data/person.txt","r");
    if(fp != NULL)
    {
        person *pArr = (person*)malloc(sizeof(person));       //先动态开辟一个person大小的空间
        if(pArr != NULL)    
        {
            int i = 0;
            while(fread(pArr+i,sizeof(person),1,fp))
            {
                i++;
                pArr_size++;
                person *ptr = NULL;
                ptr = realloc(pArr,(pArr_size+1)*sizeof(person));
                if(ptr != NULL)
                {
                    pArr = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("%s：扩容失败\n",src_log);
                    fclose(fp);
                    return pArr;       //扩容失败，返回已查询到的信息
                }
            }
            if(feof(fp))    //判断文件是到文件末尾了，还是读取失败导致结束的。
            {
                printf("%s：文件读取完成\n",src_log);
            }
            else
            {
                printf("%s：文件意外中断\n",src_log);
                fclose(fp);
                return pArr;        //文件未读取完整，返回已查询到的信息
            }
        }
        else
        {
            printf("%s：内存开辟失败\n",src_log);
            fclose(fp);
            return NULL;
        }
        fclose(fp);
        return pArr;        //正常返回
    }
    else
    {
        printf("%s：首次创建联系人或文件打开失败\n",src_log);
        fclose(fp);
        return NULL;
    }
}

//*******************************************************************************************************
/*
    get_info:将通话记录读取到动态内存中。返回info类型的指针，里面存放所有的通话信息。
             还能得到通话信息总个数 iArr_size
    !        ##使用该函数一定记得free掉内存。##
*/
//*******************************************************************************************************
info* get_info()
{
    FILE *fp = NULL;
    char src_log[] = "通话记录";
    iArr_size = 0;      //每次查询都初始化长度为0
    fp = fopen("../data/info.txt","r");
    if(fp != NULL)
    {
        info *pArr = (info*)malloc(sizeof(info));       //先动态开辟一个info大小的空间
        if(pArr != NULL)    
        {
            int i = 0;
            while(fread(pArr+i,sizeof(info),1,fp))
            {
                i++;
                iArr_size++;
                info *ptr = NULL;
                ptr = realloc(pArr,(pArr_size+1)*sizeof(info));
                if(ptr != NULL)
                {
                    pArr = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("%s：扩容失败\n",src_log);
                    fclose(fp);
                    return pArr;       //扩容失败，返回已查询到的信息
                }
            }
            if(feof(fp))    //判断文件是到文件末尾了，还是读取失败导致结束的。
            {
                printf("%s：文件读取完成\n",src_log);
            }
            else
            {
                printf("%s：文件意外中断\n",src_log);
                fclose(fp);
                return pArr;        //文件未读取完整，返回已查询到的信息
            }
        }
        else
        {
            printf("%s：内存开辟失败\n",src_log);
            fclose(fp);
            return NULL;
        }
        fclose(fp);
        return pArr;        //正常返回
    }
    else
    {
        printf("%s：文件打开失败\n",src_log);
        fclose(fp);
        return NULL;
    }
}

//*******************************************************************************************************
/*
 *  get_group:获得联系人文件中的信息，返回group类型的指针,里面存放着所有的分组信息。
 *              还能得到分组的总个数 gArr_size
 !   ##使用该函数一定记得free掉内存。##
*/
//*******************************************************************************************************
group* get_group()
{
    FILE *fp = NULL;
    char src_log[] = "联系人";
    gArr_size = 0;              //*每次查询都初始化长度为0
    fp = fopen("../data/group.txt","r");
    if(fp != NULL)
    {
        group *pArr = (group*)malloc(sizeof(group));       //先动态开辟一个person大小的空间
        if(pArr != NULL)    
        {
            int i = 0;
            while(fread(pArr+i,sizeof(group),1,fp))
            {
                i++;
                gArr_size++;
                group *ptr = NULL;
                ptr = realloc(pArr,(gArr_size+1)*sizeof(group));
                if(ptr != NULL)
                {
                    pArr = ptr;
                    ptr = NULL;
                }
                else
                {
                    printf("%s：扩容失败\n",src_log);
                    fclose(fp);
                    return pArr;       //扩容失败，返回已查询到的信息
                }
            }
            if(feof(fp))    //判断文件是到文件末尾了，还是读取失败导致结束的。
            {
                printf("%s：文件读取完成\n",src_log);
            }
            else
            {
                printf("%s：文件意外中断\n",src_log);
                fclose(fp);
                return pArr;        //文件未读取完整，返回已查询到的信息
            }
        }
        else
        {
            printf("%s：内存开辟失败\n",src_log);
            fclose(fp);
            return NULL;
        }
        fclose(fp);
        return pArr;        //正常返回
    }
    else
    {
        printf("%s：首次创建分组或文件打开失败\n",src_log);
        fclose(fp);
        return NULL;
    }
}

//*******************************************************************************************************
/*
 *   create_person:新建一个联系人
*/
//*******************************************************************************************************
void create_person()
{
    person p1 = {0};
    person *ppi = &p1;
    person* pp = get_person();
    input_person_name(pp, ppi->name);
    int pid = get_pid(pp);
    free(pp);
    pp = NULL;
    ppi->pid = pid;
    input_person_tele(ppi->tele);
    input_person_addr(ppi->addr);
    input_person_email(ppi->email);
    show_person(ppi,1);
    printf("联系人的信息如上，是否确认创建？\n");
    int ret = sure_to_change();
    if(ret == 0)
    {
        printf("取消创建联系人，已返回\n");
        return;
    }
    FILE* fp = fopen("../data/person.txt","a");
    if(fp != NULL)
    {
        fwrite(ppi,sizeof(person),1,fp);
        printf("联系人信息保存成功\n");
    }
    fclose(fp);
    fp = NULL;
}

//******************************************************************************************************
/*
    create_group:创建一个分组。
*/
//******************************************************************************************************
void create_group()
{
    group g = {0};
    group *ggp = &g;
    group *gp = get_group();
    printf("请输入分组的名称>>");
    while(1)
    {
        scanf("%s",ggp->name);
        fflush(stdin);
        if(group_name_exact_match(gp,ggp->name) != -1)  //完全匹配
        {
            printf("已有该分组，请重新输入分组的名称>>");
        }
        else
        {
            break;
        }
    }
    free(gp);
    gp = NULL;
    FILE *fg = fopen("../data/group.txt","a");
    if(fg != NULL)
    {
        fwrite(ggp,sizeof(group),1,fg);
        printf("分组保存成功\n");
    }
    fclose(fg);
    fg = NULL;
}

//*******************************************************************************************************
/*
 *   time_form_change:实现x秒到x时x分x秒的格式转换。
 *       从时到分，如果是0时，则时不返回，如果是0时0分，则时分不返回。
*/
//*******************************************************************************************************
static void time_form_change(long totalTime,char* ttime)
{
    int hour = 0,minute = 0,second = 0;
    second = totalTime % 60;
    minute = (totalTime / 60) % 60;
    hour = (totalTime / 60) / 60;
    if(minute == 0 && hour == 0)    //不到一分钟
    {
        sprintf(ttime,"%d秒",second);
    }
    else if(hour == 0)      //满了一分钟但不到一个小时
    {
        sprintf(ttime,"%d分%d秒",minute,second);
    }
    else        //一小时以上
    {
        sprintf(ttime,"%d小时%d分%d秒",hour,minute,second);
    }
}

//*******************************************************************************************************
/*
 *   call_process:通话过程，产生通话记录，保存通话记录。
*/
//*******************************************************************************************************
void call_process(int pid)
{
    char date[__I_DATE] = {0};        //date  用来保存日期时间
    char ttime[__I_TTIME] = {0};       //ttime 用来保存通话用时
    time_t start = time(NULL);          //通话开始时间
    printf("通话开始,输入q/Q退出通话>");
    char input = {0};
    while(1)
    {
        Sleep(100);         //每0.1秒检测一次是否退出通话
        input = getchar();
        if(input == 'q' || input == 'Q')
        {
            fflush(stdin);      //退出前清空标准输入流，防止影响信息输入
            break;
        }
    }
    time_t end = time(NULL);            //通话结束时间
    printf("通话结束\n");
    time_t total_time = (end - start);
    struct tm *tm_start = localtime(&start);    //拿到时间结构体
    sprintf(date,"%d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",tm_start->tm_year+1900, tm_start->tm_mon+1, tm_start->tm_mday, 
                                            tm_start->tm_hour, tm_start->tm_min, tm_start->tm_sec);
    time_form_change(total_time,ttime);
    info l = {0};
    info *log = &l;         //创建一个info类型的指针
    strcat(log->date,date);
    strcat(log->ttime,ttime);
    log->pid = pid;
    FILE *fip = fopen("../data/info.txt","a");
        if(fip != NULL)
        {
            if(fwrite(log,sizeof(info),1,fip))
                printf("通话记录：本次记录保存成功。\n");
            else
                printf("通话记录：记录写入失败。\n");
        }
        else
        {
            printf("通话记录：文件打开失败，本次记录未保存。\n");
        }
    fclose(fip);
}

//*******************************************************************************************************
/*
 *   call:模拟与联系人通信。
*/
//*******************************************************************************************************
void call(person* pp)
{
    person *match_p = NULL;
    if((match_p = person_name_combine_match(pp)))
    {
        printf("准备与 %s %s 进行通信...\n",match_p->name,match_p->tele);
        int ret = sure_to_change();
        if(ret == 0)
        {
            printf("取消通话，已返回\n");
        }
        else
        {
            call_process(match_p->pid);
        }
    }
    else
    {
        printf("无匹配结果，此次通话已返回\n");
    }
}

//*******************************************************************************************************
/*
 *    base_add_to_group:通过pid添加联系人到分组
*/
//*******************************************************************************************************
int base_add_to_group(group* gp,int offset,int pid)
{
    int i = 0;
    for (i = 0; i < __G_PID;i++)
    {
        if(((gp+offset)->pid)[i] == 0)
        {
            ((gp+offset)->pid)[i] = pid;
            (gp+offset)->count += 1;
            FILE *fp = fopen("../data/group0.txt","w");
            int i = 0;
            for (i = 0; i < gArr_size;i++)
            {
                fwrite(gp+i, sizeof(group), 1, fp);
            }
            fclose(fp);
            fp = NULL;
            remove("../data/group.txt");
            rename("../data/group0.txt","../data/group.txt");
            return 1;
        }
    }
    return 0;
}

//*******************************************************************************************************
/*
 *     add_to_group:添加联系人到分组
*/
//*******************************************************************************************************
void add_to_group(group* gp,person* pp)
{
    person* ptr = person_name_combine_match(pp);
    if(ptr == NULL)
    {
        printf("无匹配结果，已返回\n");
    }
    else
    {
        show_groups(gp,gArr_size);
        char gname[__G_NAME];
        printf("想将 %s %s 添加到哪个分组>>",ptr->name,ptr->tele);
        scanf("%s",gname);
        fflush(stdin);
        int offset = group_name_exact_match(gp,gname);
        if(offset == -1)
        {
            printf("%s 分组无匹配结果，已返回\n",gname);
        }
        else
        {
            int i = 0;
            for (i = 0; i < __G_PID;i++)
            {
                if(((gp+offset)->pid)[i] == ptr->pid)
                {
                    printf("该联系人已在分组 %s 中\n",(gp+offset)->name);
                    return;
                }
            }
            if (base_add_to_group(gp, offset, ptr->pid))
            {
                printf("已将 %s %s 添加到 %s 分组\n", ptr->name, ptr->tele, (gp + offset)->name);
            }
            else
            {
                printf("分组 %s 中联系人已达到 %d 上限\n", ptr->name, __G_PID);
            }
        }
    }
}

//*******************************************************************************************************
/*
 *   del_person_info：删除联系人的通话记录。
*/
//*******************************************************************************************************
void del_person_info(person* pp,info* ip)
{
    person* match_p = person_name_combine_match(pp);
    if(match_p != NULL)
    {
        printf("即将删除 %s %s 的通话记录\n",match_p->name,match_p->tele);
        info* match_i = pid_get_info(ip,match_p->pid,0);
        if(match_i != NULL)
        {
            show_info(pp,match_i,imatch_num);
        }
        else
        {
            printf("无 %s %s 的通话记录\n");
            return;
        }
        if(sure_to_change() == 0)
        {
            printf("已取消，开始返回\n");
            return;
        }
        info *unmatch_i = pid_get_info(ip,match_p->pid,1);
        FILE *fp = fopen("../data/info0.txt","w");
        int i = 0;
        for (i = 0; i < iunmatch_num;i++)
        {
            fwrite(unmatch_i+i,sizeof(info),1,fp);
        }
        fclose(fp);
        fp = NULL;
        remove("../data/info.txt");
        rename("../data/info0.txt","../data/info.txt");
        printf("%s %s 的通话记录删除成功\n",match_p->name,match_p->tele);
    }
    else
    {
        printf("未匹配到联系人\n");
    }
}

//*******************************************************************************************************
/*
 *   del_person：删除联系人。
*/
//*******************************************************************************************************
int del_person(person* pp)
{
    int offset = 0;
    person *pMatch = person_name_combine_match(pp);
    if(pMatch == NULL)
    {
        printf("未匹配到联系人\n");
        return 0;
    }
    else
    {
        show_person(pMatch,1);
        printf("你将要删除该联系人..\n");
        for (offset = 0; offset < pArr_size;offset++)
        {
            if((pp+offset)->pid == pMatch->pid)
            {
                break;
            }
        }
        if(sure_to_change())    // *确认
        {
            int i = 0;
            FILE *fp = fopen("../data/person1.txt","w");
            for (i = 0; i < pArr_size;i++)
            {
                if(i == offset)     // *跳过选择的联系人
                {
                    continue;
                }
                fwrite(pp+i,sizeof(person),1,fp);
            }
            fclose(fp);
            fp = NULL;
            remove("../data/person.txt");
            rename("../data/person1.txt","../data/person.txt");
            printf("联系人 %s %s 删除成功\n",pMatch->name,pMatch->tele);
            return 1;
        }
        else
        {
            printf("取消删除，已返回\n");
            return 0;
        }
    }
}

//*******************************************************************************************************
/*
 *   change_person：修改联系人信息。
*/
//*******************************************************************************************************
void change_person(person *pp)
{
    person *pMatch = person_name_combine_match(pp);
    if(pMatch == NULL)
    {
        printf("未匹配到联系人\n");
    }
    else
    {
        int offset = 0;
        int returnFlag = 0;
        int actionFlag = 0;
        for (offset = 0; offset < pArr_size;offset++)
        {
            if((pp+offset)->pid == pMatch->pid)
            {
                break;
            }
        }
        show_person(pMatch,1);
        while (1)
        {
            if(returnFlag == 1)
            {
                return;
            }
            else if(actionFlag == 1)
            {
                break;
            }
            int input = menu(person_option);
            int i = 0;
            switch(input)
            {
                case PERSON_NAME:
                    input_person_name(pp, (pp + offset)->name);
                    actionFlag = 1;
                    break;
                case PERSON_TELE:
                    for (i = 0; i < __P_TELE;i++)
                    {
                        ((pp + offset)->tele)[i] = 0;
                    }
                    input_person_tele((pp + offset)->tele);
                    actionFlag = 1;
                    break;
                case PERSON_ADDR:
                    for (i = 0; i < __P_ADDR;i++)
                    {
                        ((pp + offset)->addr)[i] = 0;
                    }
                    input_person_tele((pp + offset)->addr);
                    actionFlag = 1;
                    break;
                case PERSON_EMAIL:
                    for (i = 0; i < __P_EMAIL;i++)
                    {
                        ((pp + offset)->email)[i] = 0;
                    }
                    input_person_tele((pp + offset)->email);
                    actionFlag = 1;
                    break;
                case RETURN4:
                    returnFlag = 1;
                    break;
                default:
                    printf("输入有误，请重新输入..\n");
            }
        }
        printf("修改后联系人信息:\n");
        show_person((pp+offset),1);
        printf("你将要修改该联系人..\n");
        int ret = sure_to_change();
        if(ret)
        {
            FILE *fp = fopen("../data/person0.txt","w");
            int i = 0;
            for (i = 0; i < pArr_size;i++)
            {
                fwrite(pp+i,sizeof(person),1,fp);
            }
            fclose(fp);
            fp = NULL;
            remove("../data/person.txt");
            rename("../data/person0.txt","../data/person.txt");
            printf("联系人信息修改成功\n");
        }
        else
        {
            printf("取消修改，已返回\n");
        }
    }
}

//*******************************************************************************************************
/*
 *   change_group_name：修改分组名称。
*/
//*******************************************************************************************************
void change_group_name(group *gp)
{
    char groupName[__G_NAME] = {0};
    show_groups(gp,gArr_size);
    printf("你将要修改分组的名称..\n");
    printf("请输入分组的现名称>>");
    scanf("%s",groupName);
    fflush(stdin);
    int offset = group_name_exact_match(gp, groupName);
    if(offset != -1)
    {
        show_groups(gp+offset,1);
        printf("请输入该分组的新名称>>");
        while(1)
        {
            scanf("%s",groupName);
            fflush(stdin);
            if(group_name_exact_match(gp, groupName) != -1)  //完全匹配
            {
                printf("已有该分组，请重新输入分组的新名称>>");
            }
            else
            {
                break;
            }
        }
        printf("你将要把分组 %s 的名称修改为 %s ..\n",(gp+offset)->name,groupName);
        if(sure_to_change())
        {
            int i = 0;
            strcpy((gp+offset)->name,groupName);
            FILE *fp = fopen("../data/group1.txt","w");
            for (i = 0; i < gArr_size;i++)
            {
                fwrite(gp+i,sizeof(group),1,fp);
            }
            fclose(fp);
            fp = NULL;
            remove("../data/group.txt");
            rename("../data/group1.txt","../data/group.txt");
            printf("分组名称修改成功\n");
        }
        else
        {
            printf("修改取消，已返回\n");
        }
    }
    else
    {
        printf("未匹配到该分组\n");
    }
}

//*******************************************************************************************************
/*
 *   remove_group_person:移除分组中的联系人
*/
//*******************************************************************************************************
void remove_group_person(group *gp,person *pp)
{
    char personName[__P_NAME] = {0};
    char groupName[__G_NAME] = {0};
    show_groups(gp,gArr_size);
    printf("你将要移除分组中的联系人..\n");
    printf("请输入分组名称>>");
    scanf("%s",groupName);
    fflush(stdin);
    int offset = group_name_exact_match(gp,groupName);
    if(offset != -1)
    {
        show_group_person(gp+offset,pp);
        printf("请输入要移除的联系人名称>>");
        scanf("%s",personName);
        fflush(stdin);
        person *pMatch = person_name_exact_match(pp,personName);
        if(pMatch == NULL)
        {
            printf("未匹配到该联系人名称\n");
        }
        else
        {
            int i = 0;
            if(pid_remove_group_person(gp+offset,pMatch->pid) == 1)
            {
                printf("你将要从 %s 分组移除 %s 联系人..\n",(gp+offset)->name,pMatch->name);
                if(sure_to_change() == 1)
                {
                    FILE *fp = fopen("../data/group2.txt","w");
                    for (i = 0; i < gArr_size;i++)
                    {
                        fwrite(gp+i,sizeof(group),1,fp);
                    }
                    fclose(fp);
                    fp = NULL;
                    remove("../data/group.txt");
                    rename("../data/group2.txt","../data/group.txt");
                    printf("成功移除 %s 分组的 %s 联系人\n",(gp+offset)->name,pMatch->name);
                }
                else
                {
                    printf("取消移除，已返回\n");
                }
            }
            else
            {
                printf("该分组中没有该联系人\n");
            }
        }
    }
    else
    {
        printf("未匹配到该分组\n");
    }
}

//*******************************************************************************************************
/*
 *   delete_group:删除分组。
*/
//*******************************************************************************************************
void delete_group(group *gp)
{
    char groupName[__G_NAME] = {0};
    show_groups(gp,gArr_size);
    printf("请输入想要删除的分组的名称>>");
    scanf("%s",groupName);
    fflush(stdin);
    int offset = group_name_exact_match(gp,groupName);
    if(offset == -1)
    {
        printf("未匹配到该分组\n");
    }
    else
    {
        printf("你将要删除分组 %s ..\n",(gp+offset)->name);
        int ret = sure_to_change();
        if(ret ==  0)
        {
            printf("取消删除，已返回\n");
        }
        else
        {
            int i = 0;
            FILE *fp = fopen("../data/group3.txt","w");
            for (i = 0; i < gArr_size;i++)
            {
                if(i == offset)
                {
                    continue;
                }
                fwrite(gp+i,sizeof(group),1,fp);
            }
            fclose(fp);
            fp = NULL;
            remove("../data/group.txt");
            rename("../data/group3.txt","../data/group.txt");
            printf("分组已经成功删除\n");
        }
    }
}

//*******************************************************************************************************
/*
 *   set_sort_rule:设置排序规则
*/
//*******************************************************************************************************
void set_sort_rule(int kindOftype)
{
    int ret = 0;
    FILE *fp = NULL;
    switch(kindOftype)
    {
        case 1: // * 1：为联系人排序
            while(1)
            {
                ret = menu(person_sort);
                if(ret == 0 || ret == 1 || ret == 2 || ret == 3)
                {
                    break;
                }
                else
                {
                    printf("输入有误，请重新输入..\n");
                }
            }
            if(ret == 0)
            {
                printf("取消联系人排序设置，已返回\n");
                break;
            }
            PersonSortNum = ret;
            sortset psort = {0};
            sortset *pSort = &psort;
            pSort->personsort = PersonSortNum;
            pSort->groupsort = GroupSortNum;
            fp = fopen("../data/setting.txt","w");
            fwrite(pSort,sizeof(sortset),1,fp);
            fclose(fp);
            fp = NULL;
            printf("联系人排序设置成功\n");
            break;
        case 2: // * 2: 为分组排序
            while(1)
            {
                ret = menu(group_sort);
                if(ret == 0 || ret == 1 || ret == 2 || ret == 3)
                {
                    break;
                }
                else
                {
                    printf("输入有误，请重新输入..\n");
                }
            }
            if(ret == 0)
            {
                printf("取消分组排序设置，已返回\n");
                break;
            }
            GroupSortNum = ret;
            sortset gsort = {0};
            sortset *gSort = &gsort;
            gSort->groupsort = GroupSortNum;
            gSort->personsort = PersonSortNum;
            fp = fopen("../data/setting.txt","w");
            fwrite(gSort,sizeof(sortset),1,fp);
            fclose(fp);
            fp = NULL;
            printf("分组排序设置成功\n");
            break;
    }
}