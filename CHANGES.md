## [b139cd3](https://github.com/ADD-SP/MiniOS/commit/b139cd39847203e54482ac53c5fcc85e81fd98cf)

* 时间:2020-11-14
* 信息：:triangular_flag_on_post: Add some ASSERT
* 变动：
    * 为下列函数添加断言，检查一些参数的合法性。
        * `char *itoa(int value, char *str, int base)`
        * `int atoi(const char *str)`
        * `void puts(const char *str)`
        * `void *memcpy(void *destin, void *source, unsigned n)`
        * `void *memset(void *s, int ch, unsigned n)`

## [297a12c](https://github.com/ADD-SP/MiniOS/commit/297a12c1f96ba6a8e834691309cff6f0687b487b)

* 时间:2020-11-14
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
    * 修复了 bug#0
    * 安装了 0x00 ~ 0x1f、0x20 ~ 0x27、0x70 ~ 0x77 的中断描述符。
    * 初始化了 8259A 芯片。
    * 启用了 IDTR 寄存器。
    * 开放的中断。
* 遗留问题：
    * 中断例程除了调用中断调度器以外并不包含有意义的代码，只是做了必要的工作后直接返回。