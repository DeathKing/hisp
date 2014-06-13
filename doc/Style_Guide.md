# Coding Style Guide in Hisp


1. 自定义类型大写，且以`_T`结尾。这类都是typedef出的结构体别名。例如：
	+ `HPair`
	+ `HStr`
	+ `HProc`
2. 结构体转换通常以`H`加类型大写命名。例如：
	+ `HPAIR`：转换为HPair *，即struct hp_pair *；
	+ `HSTRING`：转换为HSTring *，即struct hp_string *；
	+ `HOBJECT`：转换为HObject，即unsigned long，此例属于特例。
3. 宏全部采用大写。例如：
	+ `HBASIC`
	+ `TYPE`
	+ `FIX2SLONG`
4. 判定性的谓词宏以`_P`结尾。例如：
	+ `NIL_P`
	+ `TRUE_P`
	+ `FALSE_P`
5. 需要暴露给Hisp的API称为“外部API”，通常以hp开头。例如：
	+ `hp_cons`
	+ `hp_car`
	+ `hp_cdr`

## 关于代码格式

1. 请使用空格缩进。
2. 缩进值为4。

```
{
	// Use 4 for tab size
	"tab_size": 4,

    // Set to true to insert spaces when tab is pressed
    "translate_tabs_to_spaces": true
}
```