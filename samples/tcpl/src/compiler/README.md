问题：编译器对源代码文件都做了哪些处理？

学究一点的说法是词法分析、语法分析、语义检查、中间代码生成、目标代码生成。从实用的角度出发，理解 gcc 的处理对初学者就够用了。
1. 预处理（preprocess）
2. 编译（compile）
3. 汇编（assemble）
4. 链接（link）

其中：
1. gcc -E 只做预处理，主要是处理那些以#打头的语句，包括#include、#define等。输出的文件还是个 C 源代码。
2. gcc -S 只做预处理和编译，把源代码处理成一个汇编语言写成的文件。
3. gcc -c 会做预处理、编译和汇编，把源代码处理成一个特殊格式的目标文件。
4. 平常大家用的不加上述任何一个命令行开关的 gcc 命令会做预处理、编译、汇编和链接所有的事情，产生一个可执行文件。

例如：
1.  gcc -E hello_world.c > hello_world.E # 默认输出到 stdout，所有需要重定向到文件
2.  gcc -S hello_world.c  # 默认输出到 *.s
3.  gcc -c hello_world.c  # 默认输出到 *.o
4.  gcc  hello_world.c      # 默认输出到 a.out

gcc 的常用命令行参数：https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html 。

