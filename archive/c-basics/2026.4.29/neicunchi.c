#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

struct Person
{
        char name[40];
        char phone[20];
        struct Person *next;
};

struct Person *pool = NULL;
int count;

void getInput(struct Person *person);
void printPerson(struct Person *person);
void addPerson(struct Person **contacts);
void changePerson(struct Person *contacts);
void delPerson(struct Person **contacts);
struct Person *findPerson(struct Person *contacts);
void displayContacts(struct Person *contacts);
void releaseContacts(struct Person **contacts);
void releasePool(void);

void getInput(struct Person *person)
{
        printf("请输入姓名：");
        scanf("%s", person->name);
        printf("请输入电话：");
        scanf("%s", person->phone);
}

void addPerson(struct Person **contacts)
{
        struct Person *person;
        struct Person *temp;

        // 如果内存池非空，则直接从里面获取空间
        if (pool != NULL)
        {
                person = pool;
                pool = pool->next;
                count--;
        }
        // 如果内存池为空，则调用malloc函数申请新的内存空间
        else
        {
                person = (struct Person *)malloc(sizeof(struct Person));
                printf("test:malloc\n");
                if (person == NULL)
                {
                        printf("内存分配失败！\n");
                        exit(1);
                }
        }

        getInput(person);

        // 将person用头插法添加到通讯录中
        if (*contacts != NULL)
        {
                temp = *contacts;
                *contacts = person;
                person->next = temp;
        }
        else
        {
                *contacts = person;
                person->next = NULL;
        }
}

void printPerson(struct Person *person)
{
        printf("联系人：%s\n", person->name);
        printf("电话：%s\n", person->phone);
}

struct Person *findPerson(struct Person *contacts)
{
        struct Person *current;
        char input[40];

        printf("请输入联系人：");
        scanf("%s", input);

        current = contacts;
        while (current != NULL && strcmp(current->name, input))
        {
                current = current->next;
        }

        return current;
}

void changePerson(struct Person *contacts)
{
        struct Person *person;

        person = findPerson(contacts);
        if (person == NULL)
        {
                printf("找不到该联系人！\n");
        }
        else
        {
                printf("请输入新的联系电话：");
                scanf("%s", person->phone);
        }
}

void delPerson(struct Person **contacts)
{
        struct Person *temp;
        struct Person *person;
        struct Person *current;
        struct Person *previous;

        // 先找到待删除的节点指针
        person = findPerson(*contacts);
        if (person == NULL)
        {
                printf("找不到该联系人！\n");
        }
        else
        {
                current = *contacts;
                previous = NULL;

                // current定位到待删除的节点
                while (current != NULL && current != person)
                {
                        previous = current;
                        current = current->next;
                }

                if (previous == NULL)
                {
                        // 待删除的节点是第一个节点
                        *contacts = current->next;
                }
                else
                {
                        // 待删除的节点不是第一个节点
                        previous->next = current->next;
                }

                // 判断内存池中是否有空位
                if (count < MAX)
                {
                        // 使用头插法将person指向的空间插入到内存池中
                        if (pool != NULL)
                        {
                                temp = pool;
                                pool = person;
                                person->next = temp;
                        }
                        else
                        {
                                pool = person;
                                person->next = NULL;
                        }
                        count++;
                }
                // 没有空位直接释放
                else
                {
                        free(person);
                }
        }
}

void displayContacts(struct Person *contacts)
{
        struct Person *current;

        current = contacts;
        while (current != NULL)
        {
                printPerson(current);
                current = current->next;
        }
}

void releaseContacts(struct Person **contacts)
{
        struct Person *temp;

        while (*contacts != NULL)
        {
                temp = *contacts;
                *contacts = (*contacts)->next;
                free(temp);
        }
}

void releasePool(void)
{
        struct Person *temp;

        while (pool != NULL)
        {
                temp = pool;
                pool = pool->next;
                free(temp);
        }
}

int main(void)
{
        int code;
        struct Person *contacts = NULL;
        struct Person *person;

        printf("| 欢迎使用通讯录管理程序 |\n");
        printf("|--- 1:插入新的联系人 ---|\n");
        printf("|--- 2:查找已有联系人 ---|\n");
        printf("|--- 3:更改已有联系人 ---|\n");
        printf("|--- 4:删除已有联系人 ---|\n");
        printf("|--- 5:显示当前通讯录 ---|\n");
        printf("|--- 6:退出通讯录程序 ---|\n");
        printf("|- Powered by FishC.com -|\n");

        while (1)
        {
                printf("\n请输入指令代码：");
                scanf("%d", &code);
                switch (code)
                {
                        case 1: addPerson(&contacts); break;

                        case 2: person = findPerson(contacts);
                                if (person == NULL)
                                {
                                        printf("找不到该联系人！\n");
                                }
                                else
                                {
                                        printPerson(person);
                                }
                                break;

                        case 3: changePerson(contacts); break;

                        case 4: delPerson(&contacts); break;

                        case 5: displayContacts(contacts); break;

                        case 6: goto END;
                }
        }

END:
        releaseContacts(&contacts);
        releasePool();

        return 0;
}