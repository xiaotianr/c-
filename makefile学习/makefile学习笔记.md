#### 1.Make简介：
- Make工程管理器也就是个“自动编译管理器”，这里的“自动”是指它能够根据文件时间戳自动发现更新过的文件而减少编译的工作量，同时，它通过读入Makefile文件的内容来执行大量的编译工作 。
  - 工程管理器，顾名思义，是指管理较多的文件 。
  - Make将只编译改动的代码文件(根据时间戳)，而不用完全编译。

##### Makefile基本结构
- Makefile是Make读入的**唯一**配置文件 。
  - 1.由make工具创建的目标体（target），通常是目标文件或可执行文件。（生成什么）
  - 2.要创建的目标体所依赖的文件（dependency_file）。（由谁生成）
  - 3.创建每个目标体时需要运行的命令（command）。（怎么生成）
  - **注意**：命令行前面必须是一个”TAB键”,否则编译错误为:`*** missing separator. Stop`.

##### Makefile格式
```
target : dependency_files
<TAB>  command
```
- example1:
  ```
  hello.o :  hello.c hello.h
  	gcc  –c  hello.c  –o  hello.o
  ```
- example2:
  ```
  sunq:kang.o yul.o
  	gcc kang.o yul.o -o sunq
  kang.o : kang.c kang.h 
  	gcc –Wall –O -g –c kang.c -o kang.o
  yul.o : yul.c 
  	gcc -Wall –O -g –c yul.c -o yul.o
  ```
- gcc参数解释：
  - -Wall:表示允许发出gcc所有有用的报警信息.
  - -c:只是编译不链接,生成目标文件”.o”
  - -o file:表示把输出文件输出到file里

##### make clean —— 伪目标
```
.PHONY:clean
clean:
       rm *.o test
```
- 这样每次`make clean`都会无条件执行，而不会因为文件下有同名文件而不执行。

#### 2.Make变量
##### 创建和使用变量
- 创建变量的目的:用来代替一个文本字符串
- 可代替的文本字符串包含：
  - 系列文件的名字
  - 传递给编译器的参数
  - 需要运行的程序
  - 需要查找源代码的目录
  - 你需要输出信息的目录
  - 你想做的其它事情

##### 变量定义的方式
- 递归展开方式 VAR=var
- 简单方式 VAR：=var
##### 变量使用
- 变量使用：$(VAR)

##### 递归展开方式 VAR=var
```
OBJS = kang.o yul.o
CC = gcc
CFLAGS = -Wall -O -g
sunq : $(OBJS)
	$(CC) $(OBJS) -o sunq
kang.o : kang.c kang.h
	$(CC) $(CFLAGS) -c kang.c -o kang.o
yul.o : yul.c yul.h
	$(CC) $(CFLAGS) -c yul.c -o yul.o
```
- 优点：它可以向后引用变量
- 缺点：不能对该变量进行任何扩展，例如`CFLAGS = $(CFLAGS) -O`会造成死循环

##### 简单方式 VAR：=var
```
m := mm
x := $(m)
y := $(x) bar
x := later 
echo $(x) $(y) 
```
- 这种定义变量的方式更适合在大的编程项目中使用，因为它更像我们一般的编程语言

##### 用?=定义变量
```
FOO ?= bar
```
- 条件定义：如果变量存在不做调整，如果不存在就定义对应的值为bar
- 等价于：
  ```
  ifeq ($(origin FOO), undefined)
  	FOO = bar
  endif
  ```

##### 为变量添加值
- 可以通过+=为已定义的变量添加新的值
  ```
  Main=hello.o hello-1.o
  Main+=hello-2.o
  ```

##### 预定义变量
- AR： 库文件维护程序的名称，默认值为ar。AS汇编程序的名称，默认值为as。
- CC： C编译器的名称，默认值为cc。CPP C预编译器的名称，默认值为$(CC) –E。
- CXX： C++编译器的名称，默认值为g++。
- FC： FORTRAN编译器的名称，默认值为f77
- RM： 文件删除程序的名称，默认值为rm -f
- ARFLAGS： 库文件维护程序的选项，无默认值。
- ASFLAGS： 汇编程序的选项，无默认值。
- CFLAGS： C编译器的选项，无默认值。
- CPPFLAGS： C预编译的选项，无默认值。
- CXXFLAGS： C++编译器的选项，无默认值。
- FFLAGS： FORTRAN编译器的选项，无默认值。

##### 自动变量
- $* 不包含扩展名的目标文件名称
- $+ 所有的依赖文件，以空格分开，并以出现的先后为序，可能 包含重复的依赖文件
- $< 第一个依赖文件的名称
- $? 所有时间戳比目标文件晚的的依赖文件，并以空格分开
- $@ 目标文件的完整名称
- $^ 所有不重复的目标依赖文件，以空格分开
- $% 如果目标是归档成员，则该变量表示目标的归档成员名称

##### 环境变量
- make在启动时会自动读取系统当前已经定义了的环境变量，并且会创建与之具有相同名称和数值的变量
- 如果用户在Makefile中定义了相同名称的变量，那么用户自定义变量将会覆盖同名的环境变量

#### 3.Make命令选项和隐含规则
##### Make使用
- 选项：
  - -C dir读入指定目录下的Makefile
  - -f file读入当前目录下的file文件作为Makefile
  - -i 忽略所有的命令执行错误（尽可能执行命令）
  - -I dir指定被包含的Makefile所在目录
  - -n 只打印要执行的命令，但不执行这些命令
  - -p 显示make变量数据库和隐含规则
  - -s 在执行命令时不显示命令
  - -w 如果make在执行过程中改变目录，打印当前目录名

##### Makefile隐含规则(编译C程序的隐含规则为例)
- `*.o`的目标的依赖目标会自动推导为`*.c`，并且其生成命令是`$(CC) –c $(CPPFLAGS) $(CFLAGS)`
  - example：（生成f1.o和f2.o的命令被省略）
    ```
    CFLAGS=-c - Wall - I - include
    f1.o:f1.c
    f2.o:f2.c
    ```
  - **`CFLAGS`默认只有-c 所以我们要自定义`CFLAGS`**
- `*`目标依赖于`*.o`，通过运行C的编译器来运行链接程序生成（一般是“ld”），其生成命令是：`$(CC) $(LDFLAGS) *.o $(LOADLIBES) $(LDLIBS)`。这个规则对于只有一个源文件的工程有效，同时也对多个Object文件（由不同的源文件生成）的也有效。
  - example:会自动查找`f1.c`、`f2.c`然后一步步生成
    ```
    f1:f1.o f2.o
    ```
  - 注意：如果没有一个源文件（如上例中的`x.c`）和目标名字（如上例中的x）相关联，那么，最好写出自己的生成规则，不然，隐含规则会报错的。
    - example:即下面的例子不会执行
      ```
      test:f1.o f2.o
      ```

#### VPATH用法和Makefile嵌套
##### VPATH : 虚路径
- 当make需要去找寻文件的依赖关系时，可以在文件前加上路径，但最好的方法是把一个路径告诉make，让make在自动去找。Makefile文件中的特殊变量`VPATH`就是完成这个功能的，如果没有指明这个变量，make只会在当前的目录中去找寻依赖文件和目标文件。如果定义了这个变量，那么，make就会在当当前目录找不到的情况下，到所指定的目录中去找寻文件了。
  ```
  VPATH = src:../headers
  ```
- 上面的的定义指定两个目录，“src”和“…/headers”，make会按照这个顺序进行搜索,目录由“冒号”分隔。（当然，当前目录永远是最高优先搜索的地方）

##### Makefile的嵌套
- 模板：
  ```
  CC=gcc
  SUBDIRS=f1 \
  		f2 \
  		main \
  		obj
  OBJS=f1.o f2.o main.o
  BIN=myapp
  OBJS_DIR=obj
  BIN_DIR=bin
  export CC OBJS BIN OBJS_DIR BIN_DIR

  all:CHECK_DIR $(SUBDIRS)
  CHECK_DIR:
  	mkdir -p $(BIN_DIR)
  $(SUBDIRS):ECHO
  	make -C $@
  ECHO:
  	@echo $(SUBDIRS)
  	@echo begin compile
  CLEAN:
  	@$(RM) $(OBJS_DIR)/*.o
  	@rm -rf $(BIN_DIR)
  ```
  - `@echo $(SUBDIRS)`命令本身不显示 只是打印
  - `make -C $@`就是调入子makefile
  - `export CC OBJS BIN OBJS_DIR BIN_DIR`用于子目录也能使用对应的变量