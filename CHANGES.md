## [3279be9](https://github.com/ADD-SP/MiniOS/commit/3279be984483efb8a88e56bc31c65aadb16858ae)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: Allocate 64MB for kernel
* 变动：
    * 映射线性地址 0x0000_0000 ~ 0x03ff_ffff 到物理地址 0x0000_0000 ~ 0x03ff_ffff，共 64MB。
    * 映射线性地址 0x8000_0000 ~ 0x83ff_ffff 到物理地址 0x0000_0000 ~ 0x03ff_ffff，共 64MB。
    * 内核加载器（loader.asm）的入口线性地址由 0x8003_1000 更改为 0x8003_2000，原因是页目录的第十六项中指向的页表线性地址为 0x8003_1000，对应的物理地址为 0x0003_1000，
    * 内核加载器入口地址更改。原入口的线性地址为 0x8003_1000，对应物理地址 0x0003_1000。新的入口的线性地址为 0x8003_2000，对应物理地址为 0x0003_2000。原因是页目录第十六项指向的页表的基址为 0x8003_1000，对应的物理地址为 0x0003_1000。两个地址重合，所以更改内核加载器的入口地址。
* 备注：
    * 线性地址 0x8000_0000 ~ 0x800f_ffff 被乱七八糟的东西占用，还是不要乱动了。
    * 现在内核占用线性地址 0x8010_0000 ~ 0x81ff_ffff，绝对够用了。
    * 线性地址 0x8200_0000 ~ 0x83ff_ffff 共 32 MB 暂定留给内核的 malloc 函数使用。


## [15f3a28](https://github.com/ADD-SP/MiniOS/commit/15f3a28836fee64884018b9f0a392bdae0b2e634)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: BitMap is implemented.
* 变动：
    * 实现了 BitMap。

## [5e1d286](https://github.com/ADD-SP/MiniOS/commit/5e1d286bcef4f27b77985e3b67626825cbc75894)

* 时间：2020-11-14
* 信息：:truck: Move some functions, macro to stdio
* 变动：
    * 从 `string.h` 和 `string.c` 中移动下列函数/宏到 `stdio.h` 和 `stdio.c `中。
        * `#define RGB_BALCK 0`
        * `#define RGB_BULE 1`
        * `#define RGB_GREEN 2`
        * `#define RGB_CYAN 3`
        * `#define RGB_RED 4`
        * `#define RGB_MAGENTA 5`
        * `#define RGB_BROWN 6`
        * `#define RGB_WHITE 7`
        * `#define NO_FLASH 0`
        * `#define FLASH 1`
        * `#define NO_LIGHT 0`
        * `#define LIGHT 1`
        * `#define CONBIN_COLOR(isFlash, isLight, backColor, charColor) ((isFlash << 7) | (isLight << 3) | (backColor < 6) | (charColor))`
        * `extern unsigned int _asm_get_cursor();`
        * `extern void _asm_set_cursor(Uint16 pos);`
        * `void putchar(const char c);`
        * `void putcharWitchColor(const char c, Byte color);`
        * `void puts(const char *str);`
        * `void putsWitchColor(const char *str, Byte *color);`

## [cfa39e4](https://github.com/ADD-SP/MiniOS/commit/cfa39e4b3e7f7c03cdcf53edf72164231202ed96)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: Creating common functions
* 变动：
    * 添加了一些工具函数
        * `int memcmp(const void *a, const void *b, unsigned n);`
        * `Uint32 strlen(const char *str);`
        * `int strcpy(const char *a, const char *b);`
        * `char* strchr(const char* s, char c);`

## [b139cd3](https://github.com/ADD-SP/MiniOS/commit/b139cd39847203e54482ac53c5fcc85e81fd98cf)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: Add some ASSERT
* 变动：
    * 为下列函数添加断言，检查一些参数的合法性。
        * `char *itoa(int value, char *str, int base)`
        * `int atoi(const char *str)`
        * `void puts(const char *str)`
        * `void *memcpy(void *destin, void *source, unsigned n)`
        * `void *memset(void *s, int ch, unsigned n)`

## [297a12c](https://github.com/ADD-SP/MiniOS/commit/297a12c1f96ba6a8e834691309cff6f0687b487b)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: Improved installation of interrupt
* 变动：
    * 主要使用 C 语言安装中断，包括中断向量表的初始化和 IDTR 的装载。

## [e6212f5](https://github.com/ADD-SP/MiniOS/commit/e6212f5aedcaf1c0c9411a1b4141bd44585dc16c)

* 时间：2020-11-14
* 信息：:triangular_flag_on_post: ASSERT is implemented.
* 变动：
    * 实现了 `ASSERT` 断言，主要用来调试。



## [1e65f92](https://github.com/ADD-SP/MiniOS/commit/1e65f9244b1e11d915aed745c6373d90dd079500)

* 时间：2020-11-13
* 信息：:triangular_flag_on_post: IDT is installed.
* 变动：
    * 安装了 0x00 ~ 0x1f、0x20 ~ 0x27、0x70 ~ 0x77 的中断描述符。
    * 初始化了 8259A 芯片。
    * 启用了 IDTR 寄存器。
    * 开放的中断。
* 遗留问题：
    * 中断例程除了调用中断调度器以外并不包含有意义的代码，只是做了必要的工作后直接返回。