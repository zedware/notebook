#include <assert.h>
#include <stdio.h>
#include <string.h>

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

void signed_char_to_string(signed char c, char *s)
{
    byte_to_string((unsigned char)c, s);
}

void unsigned_char_to_string(unsigned char c, char *s)
{
    byte_to_string(c, s);
}

void signed_int_to_string(signed int si, char *s)
{
    unsigned char *p = (unsigned char *)(&si);
    // size_t 是无符号数，循环不能用 i >= 0，因为 i = 0 时，i-- 还会满足 i > 0。
    // 导致死循环。
    for (size_t i = sizeof(int); i > 0; i--) {
        byte_to_string(p[i - 1], s);
        s += 8;
    }
}

void unsigned_int_to_string(unsigned int ui, char *s)
{
    unsigned char *p = (unsigned char *)(&ui);
    // size_t 是无符号数，循环不能用 i >= 0，因为 i = 0 时，i-- 还会满足 i > 0。
    // 导致死循环。
    for (size_t i = sizeof(int); i > 0; i--) {
        byte_to_string(p[i - 1], s);
        s += 8;
    }
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
        printf("int=%s, char=%s uchar=%s\n", s_si, s_sc, s_uc);
        // printf 其实又把 char 给转换回 int 了
        // 严谨的做法是查看字节的具体内容
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }

    {
        int i = -32768;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s uchar=%s\n", s_si, s_sc, s_uc);
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }

    {
        int i = -1;
        signed char c = i;
        unsigned char uc = i;
        signed_int_to_string(i, s_si);
        signed_char_to_string(c, s_sc);
        unsigned_char_to_string(uc, s_uc);
        printf("int=%s, char=%s uchar=%s\n", s_si, s_sc, s_uc);
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }
}

void unsigned_wide_to_narrow()
{
    {
        unsigned int i = 0xFFFFFFFF;
        signed char c = i;
        unsigned char uc = i;
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }

    {
        unsigned int i = 0x7FFFFFFF;
        signed char c = i;
        unsigned char uc = i;
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }

    {
        unsigned int i = 1;
        signed char c = i;
        unsigned char uc = i;
        printf("int=%d, char=%d uchar=%d\n", i, c, uc);
    }
}

void signed_narrow_to_wide()
{
    {
        signed char c = 0x1;
        signed int i = c;
        unsigned int ui = c;
        printf("char=%d, int=%d uint=%u\n", c, i, ui);
    }

    {
        signed char c = 0xFF;
        signed int i = c;
        unsigned int ui = c;
        printf("char=%d, int=%d uint=%u\n", c, i, ui);
    }
}

void unsigned_narrow_to_wide()
{
    {
        unsigned char c = 0x1;
        signed int i = c;
        unsigned int ui = c;
        printf("char=%u int=%d uint=%u\n", c, i, ui);
    }

    {
        unsigned char c = 0xFF;
        signed int i = c;
        unsigned int ui = c;
        printf("char=%u int=%d uint=%u\n", c, i, ui);
    }
}

void test_to_string()
{
    {
        unsigned char uc = 0x1;
        char s[sizeof(unsigned char) * 8 + 1];
        unsigned_char_to_string(uc, s);
        printf("%s\n", s);
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
