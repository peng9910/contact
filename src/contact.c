#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include "menu.h"
#include "file.h"
#include "match.h"
#include "show.h"
#include "stddata.h"

int pArr_size = 0;              //pArr_size来记录总联系人个数
int iArr_size = 0;              //iArr_size来记录通话记录的总条数
int gArr_size = 0;              //gArr_size来记录分组的总个数
int fmatch_size = 0;            //match_size来记录模糊匹配到的联系人个数
int imatch_num = 0;              //imatch_num来记录pid匹配通话记录的条数
int iunmatch_num = 0;            //iunmatch_num来记录pid未匹配通话记录的条数
int PersonSortNum = 0;           //联系人的排序规则数字
int GroupSortNum = 0;               //分组的排序规则数字

int main()
{
    init_file();
    int return_flag = 0;            //菜单返回标志
    while(1)
    {
        int input1 = menu(main_menu);   //执行main_menu函数并拿到输入值
        switch(input1)
        {
            person *pp;
            case SEARCH_PERSON:
                pp = get_person();

                search_person(pp);

                free(pp);
                pp = NULL;
                break;
            case NEW_PERSON:
                create_person();
                break;
            case PERSON_LIST:
                pp = get_person();  //动态内存存储联系人文件内容

                while(1)
                {
                    if(return_flag)
                    {
                        return_flag = 0;
                        break;  //标志变为1，返回主菜单
                    }
                    int input2 = menu(person_menu);
                    info *ip;    //动态内存存储通话记录文件内容
                    switch(input2)
                    {
                        case CHECK_ALL_PERSON:
                            show_person(pp,pArr_size);
                            break;
                        case CHANGE_PERSON:
                            change_person(pp);
                            break;
                        case CALL:
                            call(pp);                            
                            break;
                        case RECENT_INFO:
                            ip = get_info();
                            if(ip != NULL)
                            {
                                show_recent_info(pp,ip);
                            }
                            free(ip);
                            ip = NULL;
                            break;
                        case PERSON_INFO:
                            ip = get_info();
                            show_person_info(pp,ip);
                            free(ip);
                            ip = NULL;
                            break;
                        case DELETE_INFO:
                            ip = get_info();
                            del_person_info(pp,ip);
                            free(ip);
                            ip = NULL;
                            break;
                        case DELETE_PERSON:
                            if(del_person(pp) == 1) // *看是否执行了删除操作
                            {
                                free(pp);
                                pp = NULL;
                                pp = get_person();  // *删除完后重新加载
                            }
                            break;
                        case RETURN1:
                            return_flag = 1;
                            break;
                        default:
                            printf("输入有误，请重新输入...\n");
                    }
                }

                free(pp);       //释放动态内存
                pp = NULL;      
                break;
            case NEW_GROUP://新建分组
                create_group();
                break;
            case GROUP_LIST://分组管理
                pp = get_person();

                while(1)
                {
                    if(return_flag)
                    {
                        return_flag = 0;
                        break;  //标志变为1，返回主菜单
                    }
                    int input3 = menu(group_menu);
                    group *gp;  //保存分组文件内容
                    switch(input3)
                    {
                        case CHECK_GROUP:
                            gp = get_group();
                            if(gp != NULL)
                            {
                                show_group(gp,pp,gArr_size);
                            }
                            free(gp);
                            gp = NULL;
                            break;
                        case ADD_TO_GROUP:
                            gp = get_group();
                            if(gp != NULL)
                            {
                                add_to_group(gp,pp);
                            }
                            free(gp);
                            gp = NULL;
                            break;
                        case CHANGE_GROUP_NAME:
                            gp = get_group();
                            if(gp != NULL)
                            {
                                change_group_name(gp);
                            }
                            free(gp);
                            gp = NULL;
                            break;
                        case REMOVE_GROUP_PERSON:
                            gp = get_group();
                            if(gp != NULL)
                            {
                                remove_group_person(gp,pp);
                            }
                            free(gp);
                            gp = NULL;
                            break;
                        case DELETE_GROUP:
                            gp = get_group();
                            if(gp != NULL)
                            {
                                delete_group(gp);
                            }
                            free(gp);
                            gp = NULL;
                            break;
                        case RETURN2:
                            return_flag = 1;
                            break;
                        default:
                            printf("输入有误，请重新输入...\n");
                    }
                }

                free(pp);
                pp = NULL;
                break;
            case SORT_SET:
                while(1)
                {
                    if(return_flag)
                    {
                        return_flag = 0;
                        break;
                    }
                    int input4 = menu(sort_menu);
                    switch(input4)
                    {
                        case PERSON_SORT:
                            set_sort_rule(1);
                            break;
                        case GROUP_SORT:
                            set_sort_rule(2);
                            break;
                        case RETURN3:
                            return_flag = 1;
                            break;
                        default:
                            printf("输入有误，请重新输入...\n");
                    }
                }
                break;
            case EXIT://退出
                return 0;
            default:
                printf("输入有误，请重新输入...\n");
        }
    }
    return 0;
}