
#ifndef __MATCH_H__
#define __MATCH_H__

#include "stddata.h"

int str_match(char *s1, char *s2);//

int get_pid(person *pp);

person *pid_match(person *pp, int pid);//

int name_include(char *give_s, char *std_s);//

person *person_name_fuzzy_match(person *pp, char *give_s);//

person *person_name_exact_match(person *pp, char *name_p);//

person *person_name_combine_match(person *pp);//

void search_person(person* pp);//

info *pid_get_info(info *ip, int pid, int mode);//

int group_name_exact_match(group* gp,char* name);//

int pid_remove_group_person(group *pGroup, int pid);

#endif
