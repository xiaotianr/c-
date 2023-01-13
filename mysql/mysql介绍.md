#### MYSQL

- MYSQL服务启动与停止

```shell
net stop MYSQL
net start MYSQL
```

- MYSQL服务端登录和退出

```shell
mysql -h localhost -P 3306 -u root -p
mysql -h localhost -P 3306 -u root -p123456
mysql -u root -p123456
#-h 主机名 -P 端口号 -u 用户 -p 密码
#退出exit或crtl+c
```

- MYSQL常见命令

```sql
show databases;#显示数据库
use test;#use 库名; #进入数据库
show tables;#显示表
show tables from test;#显示test数据库里的表
select database();#查看当前在哪个数据库
create table stuinfo(
    ->id int,
    ->name varchar(20));#创建表,stuinfo为表名，id、name为列名，int、varchat为列类型
#desc stuinfo;#查看stuinfo表结构
select * from stuinfo;#查看stuinfo表数据
insert into stuinfo (id,name) values(1,'john');#插入数据
update stuinfo set name='lilei' where id=1;#修改数据
delete from stuinfo where id=1;#删除数据
```

```sql
select version();#查看mysql版本（sql命令）
mysql --version #shell 命令查看mysql版本 
```

- MYSQL语法规范
  
  - 不区分大小写，但是建议关键字大写，表名、列名小写
  
  - 每条命令最好用分号结尾
  
  - 每条命令根据需要，可以进行缩进或换行
  
  - 注释
    
    - 单行注释：#注释文字
    
    - 单行注释：-- 注释文字 （--后面有个空格/n）
    
    - 多行注释：/* 注释文字 */
    
    - 
