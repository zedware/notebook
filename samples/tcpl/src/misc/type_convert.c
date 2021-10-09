#include <assert.h>
#include <stdio.h>
#include <string.h>

//
// 整数转成二进制以字符串的形式输出。有两种办法：
// 1. 用移位实现单字节的输出，例如 0x05 输出为 "00000101"。多字节的对每个字节调用它。
//    byte_to_string
// 2. 用移位分别实现每种整数的输出。
//    char_to_string, short_to_string, int_to_string, ...
// 第一种方法可以复用代码，但是多字节整数的处理跟 CPU 的字节顺序有关，可移植性不好。
// 第二种方法需要写多遍类似的代码，比较冗长。
//
// 总结：
// 1. 字节多的往字节少的转换，实际上是截掉了高位字节。汇编代码中就是用指令 mov ，拷贝
//    对应的字节。
// 2. 字节少的往字节多的转换，实际上是高位字节补了全零或全一。补的其实是字节少的类型
//    的符号位。下面的汇编代码中对应的分别是 movsbl 和 movzbl 指令。

// 下面的汇编代码是用命令产生的：
//  $ gcc -Wall -g foo.c
//  $ objdump -S a.out > foo.s
// 也可以用下面的命令，输出格式有所不同：
//  $ gcc -S foo.c  # 默认输出文件为 foo.s
// x86 汇编指令的含义：
//  https://web.stanford.edu/class/archive/cs/cs107/cs107.1166/guide_x86-64.html
//
/*
    {
        signed char c = 0xFF;
    1849:   c6 45 83 ff             movb   $0xff,-0x7d(%rbp)
        signed int i = c;
    184d:   0f be 45 83             movsbl -0x7d(%rbp),%eax
    1851:   89 45 8c                mov    %eax,-0x74(%rbp)
        unsigned int ui = c;
    1854:   0f be 45 83             movsbl -0x7d(%rbp),%eax
    1858:   89 45 90                mov    %eax,-0x70(%rbp)
<snipped>
    {
        unsigned char c = 0x1;
    18fe:   c6 45 82 01             movb   $0x1,-0x7e(%rbp)
        signed int i = c;
    1902:   0f b6 45 82             movzbl -0x7e(%rbp),%eax
    1906:   89 45 84                mov    %eax,-0x7c(%rbp)
        unsigned int ui = c;
    1909:   0f b6 45 82             movzbl -0x7e(%rbp),%eax
    190d:   89 45 88                mov    %eax,-0x78(%rbp)
*/

// 命名习惯一般为 x_to_y 或 x2y
void byte_to_string(unsigned char byte, char *s)
{
    char *p = s + 7;
    for (int i = 0; i < 8; i++) {
        if ((byte & 0x1) == 0x1) {
            *p-- = '1';
        } else {
            *p-- = '0';
        }

        byte >>= 1;
    }

    s[8] = '\0';
}

// 作业：signed/unsigned short 都可以用它
void short_to_string(unsigned short us, char *s)
{
}

// 作业：signed/unsigned int 都可以用它
void int_to_string(unsigned int i, char *s)
{
}

void signed_char_to_string(signed char c, char *s)
{
    byte_to_string((unsigned char)c, s);
}

void unsigned_char_to_string(unsigned char c, char *s)
{
    byte_to_string(c, s);
}

void unsigned_int_to_string(unsigned int ui, char *s)
{
    unsigned char *p = (unsigned char *)(&ui);
    // size_t 是无符号数，循环不能用 i >= 0，因为 i = 0 时，i-- 还会满足 i > 0。
    // 导致死循环和访问越界。
    // 在 x86 机器上整数的四个字节是低位字节在低地址，高位字节在高地址。
    // 例如 0x01020304 在内存中是 0x04 0x03 0x02 0x01。所以我们先转 0x01。
    for (size_t i = sizeof(int); i > 0; i--) {
        byte_to_string(p[i - 1], s);
        s += 8;
    }
}

void signed_int_to_string(signed int si, char *s)
{
    unsigned_int_to_string((unsigned int)si, s);
}

void signed_wide_to_narrow()
{
    char s_si[sizeof(int) * 8 + 1];
    char s_sc[sizeof(signed char) * 8 + 1];
    char s_uc[sizeof(unsigned char) * 8 + 1];

    {
        int i = -2147483648;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s, uchar=%s\n", s_si, s_sc, s_uc);
        // printf 其实又把 char 给转换回 int 了
        // 严谨的做法是查看字节的具体内容
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        int i = -32768;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s, uchar=%s\n", s_si, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        int i = -1;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s, uchar=%s\n", s_si, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        int i = 0x01020304;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s, uchar=%s\n", s_si, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }
}

void unsigned_wide_to_narrow()
{
    char s_ui[sizeof(unsigned int) * 8 + 1];
    char s_sc[sizeof(signed char) * 8 + 1];
    char s_uc[sizeof(unsigned char) * 8 + 1];

    {
        unsigned int i = 0xFFFFFFFF;
        signed char c = i;
        unsigned char uc = i;
        unsigned_int_to_string(i, s_ui);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("uint=%s, char=%s, uchar=%s\n", s_ui, s_sc, s_uc);
        printf("uint=%u, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        unsigned int i = 0x7FFFFFFF;
        signed char c = i;
        unsigned char uc = i;
        unsigned_int_to_string(i, s_ui);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("uint=%s, char=%s, uchar=%s\n", s_ui, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        unsigned int i = 1;
        signed char c = i;
        unsigned char uc = i;
        unsigned_int_to_string(i, s_ui);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("uint=%s, char=%s, uchar=%s\n", s_ui, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }

    {
        unsigned int i = 0x01020304;
        signed char c = i;
        unsigned char uc = i;
        unsigned_int_to_string(i, s_ui);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("uint=%s, char=%s, uchar=%s\n", s_ui, s_sc, s_uc);
        printf("int=%d, char=%d, uchar=%d\n", i, c, uc);
    }
}

void signed_narrow_to_wide()
{
    char s_sc[sizeof(signed char) * 8 + 1];
    char s_si[sizeof(signed int) * 8 + 1];
    char s_ui[sizeof(unsigned int) * 8 + 1];

    {
        signed char c = 0x1;
        signed int i = c;
        unsigned int ui = c;
        signed_char_to_string(c, s_sc);
        signed_int_to_string(i, s_si);
        unsigned_int_to_string(ui, s_ui);
        printf("char=%s, int=%s, uint=%s\n", s_sc, s_si, s_ui);
        printf("char=%d, int=%d, uint=%u\n", c, i, ui);
    }

    {
        signed char c = 0xFF;
        signed int i = c;
        unsigned int ui = c;
        signed_char_to_string(c, s_sc);
        signed_int_to_string(i, s_si);
        unsigned_int_to_string(ui, s_ui);
        printf("char=%s, int=%s, uint=%s\n", s_sc, s_si, s_ui);
        printf("char=%d, int=%d, uint=%u\n", c, i, ui);
    }
}

void unsigned_narrow_to_wide()
{
    char s_uc[sizeof(signed char) * 8 + 1];
    char s_si[sizeof(signed int) * 8 + 1];
    char s_ui[sizeof(unsigned int) * 8 + 1];

    {
        unsigned char c = 0x1;
        signed int i = c;
        unsigned int ui = c;
        unsigned_char_to_string(c, s_uc);
        signed_int_to_string(i, s_si);
        unsigned_int_to_string(ui, s_ui);
        printf("uchar=%s, int=%s, uint=%s\n", s_uc, s_si, s_ui);
        printf("uchar=%u int=%d, uint=%u\n", c, i, ui);
    }

    {
        unsigned char c = 0xFF;
        signed int i = c;
        unsigned int ui = c;
        unsigned_char_to_string(c, s_uc);
        signed_int_to_string(i, s_si);
        unsigned_int_to_string(ui, s_ui);
        printf("uchar=%s, int=%s, uint=%s\n", s_uc, s_si, s_ui);
        printf("uchar=%u int=%d, uint=%u\n", c, i, ui);
    }
}

void test_to_string()
{
    // tests for unsigned char
    {
        unsigned char uc = 0x1;
        char s[sizeof(unsigned char) * 8 + 1];
        unsigned_char_to_string(uc, s);
        assert(strcmp("00000001", s) == 0);
    }

    {
        unsigned char uc = 0x7F;
        char s[sizeof(unsigned char) * 8 + 1];
        unsigned_char_to_string(uc, s);
        assert(strcmp("01111111", s) == 0);
    }

    {
        unsigned char uc = 0xFF;
        char s[sizeof(unsigned char) * 8 + 1];
        unsigned_char_to_string(uc, s);
        assert(strcmp("11111111", s) == 0);
    }

    // tests for int
    {
        int i = 0x01020304;
        char s[sizeof(int) * 8 + 1];
        signed_int_to_string(i, s);
        assert(strcmp("00000001000000100000001100000100", s) == 0);
    }
}

int main(void)
{
    test_to_string();

    signed_wide_to_narrow();
    unsigned_wide_to_narrow();
    signed_narrow_to_wide();
    unsigned_narrow_to_wide();
    return 0;
}
