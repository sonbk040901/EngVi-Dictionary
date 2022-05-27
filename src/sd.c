#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "sd.h"
#include "../lib/bt-5.0.1/inc/btree.h"

char exchange(char c)
{
    char ch;
    switch (c)
    {
    case 'u':
    case 'e':
    case 'o':
    case 'a':
    case 'i':
    case 'w':
    case 'h':
    case 'y':
        ch = '0';
        break;
    case 'b':
    case 'f':
    case 'p':
    case 'v':
        ch = '1';
        break;
    case 'c':
    case 'g':
    case 'j':
    case 'k':
    case 'q':
    case 's':
    case 'x':
    case 'z':
        ch = '2';
        break;
    case 'd':
    case 't':
        ch = '3';
        break;
    case 'l':
        ch = '4';
        break;
        break;
    case 'm':
    case 'n':
        ch = '5';
        break;
    case 'r':
        ch = '6';
        break;
    default:
        ch = '0';
        break;
    }
    return ch;
}
void back(char *word, int n)
{
    for (int i = n; word[i] != '\0'; i++)
    {
        word[i] = word[i + 1];
    }
}
char *getcode(const char *word)
{
    int index = 0, i;
    char *code = strdup(word);
    char first = toupper(*word), flag = 0;
    for (i = 0; i < strlen(code); i++)
    {
        *(code + i) = tolower(*(code + i));
        *(code + i) = exchange(*(code + i));
    }
    for (i = 0; code[i] != 0;)
    {
        if (flag != code[i] && code[i] != 48 || !i)
        {
            flag = code[i];
            i++;
            continue;
        }
        if (flag == code[i])
        {
            back(code, i);
        }
        else if (word[i] == 119 || word[i] == 104 || word[i] == 72 || word[i] == 87)
        {
            back(code, i);
        }
        else if (code[i] == 48)
        {
            flag = 0;
            back(code, i);
        }
    }
    while (index <= 3)
    {
        flag = code[index];
        code[index++] = flag == '\0' ? '0' : flag;
    }
    code[index] = '\0';
    *code = first;
    return code;
}
char *find(const char *code1, BTA *BT)
{
    BTA *index;
    btinit();
    if (availability("index.dat"))
        index = btopn("index.dat", 0, FALSE);
    else
    {
        index = btcrt("index.dat", 0, FALSE);
        convert_index(index, BT);
    }
    btdups(index, TRUE);
    char *suggest;
    suggest = (char *)malloc(sizeof(char) * MAX_SIZE);
    char word[256], code[256];
    strcpy(code, code1);
    int rsize, dsize = MAX_SIZE, i = 0;
    btsel(index, code, word, dsize, &rsize);
    strcpy(suggest, "Do you mean: \n\n");
    do
    {
        strcat(suggest, word);
        strcat(suggest, "\n");
        i++;
    } while (i < 20 && !btseln(index, code, word, dsize, &rsize));
    btcls(index);
    return suggest;
}
void convert_index(BTA *fbt, BTA *BT)
{
    int dsize;
    char word[256], *code;
    btpos(BT, 1);
    while (!bnxtky(BT, word, &dsize))
    {
        code = getcode(word);
        btins(fbt, code, word, 256);
    }
}
int availability(const char *filename)
{
    FILE *f;
    f = fopen(filename, "r");
    if (!f)
        return 0;
    else
    {
        fclose(f);
        return 1;
    }
}
