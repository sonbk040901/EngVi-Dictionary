#include "btree.h"
#include <stdlib.h>
int main(int argc, char const *argv[])
{
    BTA *btfile;
    btinit();
    /*Tao mot file BTree moi*/
    btfile = btcrt("test2", 0, FALSE);
    btthresh(btfile, 100);
    if (!btfile)
    {
        printf("Khong the tao B Tree!\n");
        return 1;
    }
    /*Mo file BTree*/
    // btfile = btopn("test2", 0, FALSE);
    /*Các điều khiển btdups hỗ trợ các khóa trùng lặp trong thư mục gốc hiện tại của tệp
     chỉ mục được liên kết với con trỏ ngữ cảnh btact. Đặt giá trị của dups thành khác 0 (TRUE)
      sẽ cho phép hỗ trợ các khóa trùng lặp trong thư mục gốc hiện tại. Giá trị 0 (FALSE)
       sẽ vô hiệu hóa hỗ trợ khóa trùng lặp cho gốc hiện tại. Không cho phép bật hỗ trợ 
       phím trùng lặp trên superroot.
       Việc vô hiệu hóa hỗ trợ khóa trùng lặp trên thư mục gốc mà trước đó đã cho 
phép chúng chỉ ngăn chặn các khóa trùng lặp tiếp tục được chèn vào chỉ mục BTree gốc.
 Các bản sao hiện có sẽ vẫn còn và phải được ứng dụng quản lý.*/
    btdups(btfile, FALSE);
    /*Hàm binsky chèn một khóa mới và giá trị số nguyên được liên kết vào thư mục
     gốc hiện tại của tệp được liên kết với con trỏ ngữ cảnh btact. Khóa, một chuỗi
      ký tự, được chuyển vào khóa, trong khi giá trị giữ giá trị số nguyên được liên kết.
       giá trị được khai báo dưới dạng BTint, thường là một typedef cho int, nhưng với Hỗ trợ
        tệp lớn sẽ là một typedef trong thời gian dài.

Nếu khóa đã được chèn thành công, binsky trả về 0, nếu không sẽ trả về mã lỗi.

Các khóa dài hơn độ dài khóa tối đa (hằng số BT ZKYLEN = 32) sẽ bị cắt ngắn theo chiều dài khóa tối đa.*/
    binsky(btfile, "Son", 1073741825);
    binsky(btfile, "Duc", 2147483647);
    binsky(btfile, "Son", 101);
    binsky(btfile, "Tam", 101);
    binsky(btfile, "Hoan", 98);
    binsky(btfile, "Truong", 97);
    binsky(btfile, "Ha", 102);

    int a = 999;
    /*Hàm btins chèn một khóa và bản ghi dữ liệu vào một tệp được liên kết với
     con trỏ ngữ cảnh btact. Cả khóa và dữ liệu đều là con trỏ ký tự. Vì dữ liệu
      có thể chứa các ký tự null (x'00 ') một cách hợp pháp, độ dài của dữ liệu,
       tính bằng byte, được chuyển bằng dsize. dsize phải bằng 0 hoặc lớn hơn. 
       Nếu khóa và dữ liệu được lưu trữ thành công trong tệp B Tree, btins trả về số không.*/
    // btins(btfile, "Bao", (char *)&a, sizeof(int));
    bupdky(btfile, "Duc", 999999);
    char key[] = "Ha";
    char *key1;
    key1 = (char *)malloc(sizeof(char) * ZKYLEN);
    BTint data = 0;
    /*Hàm bfndky tìm kiếm một khóa trong thư mục gốc hiện tại của tệp được liên kết
     với con trỏ ngữ cảnh btact. Khóa, một chuỗi ký tự, được chuyển dưới dạng một 
     con trỏ trong khóa. Nếu khóa được tìm thấy, giá trị được liên kết sẽ được 
     trả về ở vị trí số nguyên được xác định bởi giá trị. giá trị được khai báo 
     dưới dạng BTint, thường là một typedef cho int, nhưng với Hỗ trợ tệp lớn sẽ
      là một typedef trong thời gian dài.

Nếu khóa được tìm thấy, bfndky trả về số không. Nếu không tìm thấy khóa,
 bfndky sẽ trả về mã lỗi QNOKEY.

Cho dù khóa có được định vị hay không, ngữ cảnh Cây B vẫn được để ở 
khóa cao nhất tiếp theo trong tệp B Tree. Một cuộc gọi đến bnxtky sẽ 
trả về khóa này. Hàm bprvky có thể được gọi để trả về khóa trước đó.

Nếu gốc hiện tại hỗ trợ các khóa trùng lặp (được kích hoạt bởi lệnh 
gọi đến btdups và đích của hàm bfndky có các khóa trùng lặp, ngữ cảnh 
của chỉ mục B Tree được định vị ở đầu bộ khóa trùng lặp.*/
    int i = bfndky(btfile, key, &data);
    if (i == 0)
        printf("Data cua ... la: %d\n", data);
    i = bnxtky(btfile, key1, &data);
    if (i == 0)
        printf("Data cua ... la: %d\n", data);
    printf("%s", key1);
    i = bnxtky(btfile, key1, &data);
    if (i == 0)
        printf("Data cua ... la: %d\n", data);
    printf("%s", key1);
    free(key1);
    btcls(btfile);
    return 0;
}
