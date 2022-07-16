
#ifndef __MENU_H__
#define __MENU_H__

typedef void(*MENU)();  //* 将void(*)() 函数指针类型重命名为 MENU


void main_menu();

enum 
{
    EXIT,
    SEARCH_PERSON,
    NEW_PERSON,
    PERSON_LIST,
    NEW_GROUP,
    GROUP_LIST,
    SORT_SET
};

void person_menu();

enum 
{
    RETURN1,
    CHECK_ALL_PERSON,
    CHANGE_PERSON,
    CALL,
    RECENT_INFO,
    PERSON_INFO,
    DELETE_INFO,
    DELETE_PERSON
};

void group_menu();

enum
{
    RETURN2,
    CHECK_GROUP,
    ADD_TO_GROUP,
    CHANGE_GROUP_NAME,
    REMOVE_GROUP_PERSON,
    DELETE_GROUP
};

void sort_menu();

enum
{
    RETURN3,
    PERSON_SORT,
    GROUP_SORT,
    INFO_SORT
};

void person_option();
enum
{
    RETURN4,
    PERSON_NAME,
    PERSON_TELE,
    PERSON_ADDR,
    PERSON_EMAIL
};

void person_sort();

void group_sort();

int menu(MENU);
#endif