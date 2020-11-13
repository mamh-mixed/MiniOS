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