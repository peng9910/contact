#ifndef __FILE_H__
#define __FILE_H__

#include "stddata.h"

void init_file();

person *get_person();//

info *get_info();//

group *get_group();//

void create_person();//

void create_group();//

void call_process(int pid);//

void call(person *pp);//

int base_add_to_group(group *gp,int offset,int pid);//

void add_to_group(group *gp, person *pp);//

void del_person_info(person *pp, info *ip);//

int del_person(person *pp);

void change_person(person *pp);

void change_group_name(group *gp);

void remove_group_person(group *gp, person *pp);

void delete_group(group *gp);

void set_sort_rule(int kindOftype);

#endif
