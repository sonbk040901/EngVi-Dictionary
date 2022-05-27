#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../lib/bt-5.0.1/inc/btree.h"
#ifndef _SD_H
#define _SD_H
#ifndef MAX_SIZE
#define MAX_SIZE 10000
#endif
/* Thuat toan soundex
1    Giữ lại chữ cái đầu tiên của tên và loại bỏ tất cả các lần xuất hiện khác của a, e, i, o, u, y, h, w.
2    Thay các phụ âm bằng các chữ số như sau (sau chữ cái đầu tiên):
b, f, p, v → 1
c, g, j, k, q, s, x, z → 2
d, t → 3
l → 4
m, n → 5
r → 6
3    Nếu hai hoặc nhiều chữ cái với số trùng liền kề trong tên gốc (trước bước 1), chỉ giữ lại chữ cái đầu tiên; cũng có hai chữ cái có cùng một số cách nhau bởi 'h' hoặc 'w' được mã hóa thành một số duy nhất, trong khi những chữ cái được phân tách bằng một nguyên âm được mã hóa hai lần. Quy tắc này cũng áp dụng cho chữ cái đầu tiên.
4    Nếu bạn có quá ít chữ cái trong từ mà bạn không thể gán ba số, hãy thêm các số không cho đến khi có ba số. Nếu bạn có bốn số trở lên, chỉ giữ lại ba số đầu tiên.
*/
//cre: https://vi.nipponkaigi.net/wiki/Soundex
char *getcode(const char *);
char exchange(char);
void back(char *, int);
int availability(const char *);
void convert_index(BTA *, BTA *);
char *find(const char *, BTA *);

#endif