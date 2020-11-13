## bug#0

* 状态：已修复
* 描述：
    * 当 `char *itoa(int value, char *str, int base)` 的参数 `value` 为 0 时且 `base` 为 10 时，不会形成任何字符串。
    * 当 `char *itoa(int value, char *str, int base)` 的参数 `value` 为 0 时且 `base` 为 16 时，只会输出 `"0x"`。
    * 当 `char *itoa(int value, char *str, int base)` 的参数 `base` 为 16 时，不能正确地输出 16 进制字符串中的字母。
* 历史：
    1. 2020-11-13 发现。
    2. 2020-11-13 修复（[1e65f92](https://github.com/ADD-SP/MiniOS/commit/1e65f9244b1e11d915aed745c6373d90dd079500)）