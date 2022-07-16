#ifndef __STDDATA_H__
#define __STDDATA_H__

#define __G_PID 100       //* group的pid数组的最大个数
#define __G_NAME 20       //* group的name的最大宽度
#define __P_EMAIL 20      //* person的email的最大宽度
#define __P_ADDR 30       //* person的addr的最大宽度
#define __P_TELE 15       //* person的tele的最大宽度
#define __P_NAME 20       //* person的name的最大宽度
#define __I_DATE 30       //* info的date的最大宽度
#define __I_TTIME 20      //* info的ttime的最大宽度

#define MIN(a,b) (((a)<(b))?(a):(b))    //*宏定义，求二者中的较小值

/*
 *  info为存储通话信息的结构
*/
typedef struct Info
{
    int pid;      //此次通话的联系人编号
    char ttime[__I_TTIME];          //通话时长
    char date[__I_DATE];      //通话开始的时间日期
} info;

/*
 *  person为存储联系人的结构
*/
typedef struct Person
{
    int pid;         //联系人编号
    char name[__P_NAME];  //联系人名称
    char tele[__P_TELE];   //联系人号码
    char addr[__P_ADDR];   //地址
    char email[__P_EMAIL];  //电子邮箱
}person;

/*
 * group为存储分组的结构
*/
typedef struct Group
{
    int count;      //分组的人数
    char name[__G_NAME];  //分组的名称
    int pid[__G_PID];   //存放分组内的联系人pid的数组
}group;

/*
 * sortset为存储排序设置的结构
*/
typedef struct Sortset
{
    int personsort; // * 1为默认排序，2为按名称a-z排序,3为按名称z-a排序
    int groupsort;  // * 1为默认排序，2为按名称a-z排序，3为按名称z-a排序
}sortset;

#endif