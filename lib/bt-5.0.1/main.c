#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>
#include "btree.h"
#define MAX_CHOSE 5
void menu(void)
{
    printf("\t\t\t|======================================================|\n");
    printf("\t\t\t|%25sMENU%25s|\n", " ", " ");
    printf("\t\t\t|------------------------------------------------------|\n");
    printf("\t\t\t|  1  |   %-45s|\n", "Them thong tin lien lac moi");
    printf("\t\t\t|  2  |   %-45s|\n", "Tim kiem theo ten");
    printf("\t\t\t|  3  |   %-45s|\n", "Hien thi toan bo danh ba");
    printf("\t\t\t|  4  |   %-45s|\n", "Xoa mot thong tin lien lac");
    printf("\t\t\t|  5  |   %-45s|\n", "Ket thuc chuong trinh");
    printf("\t\t\t|======================================================|\n");
}
int main()
{
    int chose;
    BTA *btfile;
    btinit();
    btfile = btcrt("btreefile", 0, FALSE);
    btthresh(btfile, 3);
    if (!btfile)
    {
        printf("Khong the tao B Tree!\n");
        return 1;
    }
    btdups(btfile, FALSE);
    char name[ZKYLEN];
    char *name1;
    int number, search;
    do
    {
        menu();
        printf("Hay nhap lua chon cua ban: ");
        while (!scanf("%d", &chose))
        {
            printf("Nhap sai: Khong duoc nhap chu!\nChi duoc nhap so tu %d - %d\n", 1, MAX_CHOSE);
            menu();
            printf("Hay nhap lua chon cua ban: ");
            __fpurge(stdin);
        }
        __fpurge(stdin);
        switch (chose)
        {
        case 1:
            /* Lua chon thu nhat */
            printf("Hay nhap ten cua lien he moi: ");
            __fpurge(stdin);
            fgets(name, ZKYLEN, stdin);
            name[strlen(name) - 1] = 0;
            name1 = strdup(name);
            printf("Hay nhap so dien thoai: ");
            scanf("%d", &number);
            binsky(btfile, name1, number);
            free(name1);
            break;
        case 2:
            /* Lua chon thu hai */
            printf("Hay nhap ten lien he ban muon tim: ");
            __fpurge(stdin);
            fgets(name, ZKYLEN, stdin);
            name[strlen(name) - 1] = '\0';
            search = bfndky(btfile, name, &number);
            if (search == 0)
            {
                printf("Lien he can tim la: %s\tstd: %d\n", name, number);
            }
            else
                printf("Khong tim thay lien he trong danh ba!!!\n");
            break;
        case 3:
            /* Lua chon thu ba */
            name1 = (char *)malloc(sizeof(char) * ZKYLEN);
            btpos(btfile, 1 /*Gia tri 1 la vi tri dau, gia tri 2 la vi tri cuoi*/);
            printf("Danh ba:\n");
            printf("%-31s%-15s\n", "    NAME", "       NUMBER");
            while (!bnxtky(btfile, name1, &number))
            {
                printf(" %-38s%d\n", name1, number);
            }
            free(name1);
            break;
        case 4:
            /* Lua chon thu tu */
            printf("Hay nhap ten lien he ban muon xoa khoi danh ba: ");
            __fpurge(stdin);
            fgets(name, ZKYLEN, stdin);
            name[strlen(name) - 1] = '\0';
            search = bdelky(btfile, name);
            if (search == 0)
            {
                printf("Da xoa lien he %s khoi danh ba\n", name);
            }
            else
                printf("Khong tim thay lien he trong danh ba!!!\n");
            break;
        case 5:
            printf("\tKet thuc chuong trinh\n");
            btcls(btfile);
            chose = 0;
            break;
        default:
            printf("Chi duoc chon cac so tu %d - %d!!!\n", 1, MAX_CHOSE);
            chose = 1;
            break;
        }
    } while (chose);
    return 0;
}