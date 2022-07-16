#ifndef __SHOW_H__
#define __SHOW_H__

#define __RECENT_INFO 10

#include "stddata.h"

void show_info(person *pp, info *ip, int sz);//

void show_groups(group *gp, int sz);//

void show_person(person*,int);//

void show_person_info(person *pp, info *ip);//

void show_recent_info(person *pp, info *ip);//

void show_group_person(group *gp, person *pp);//

void show_group(group *gp, person *pp, int sz);//

#endif