#### DQL语言（数据查询语言）
  
- 1.基础查询
  
  - 语法：`select 查询列表 from 表名;`
  
  - 特点：
    
    - 1.查询列表可以是：表中的字段、常量值、表达式、函数
    
    - 2.查询的结果是一个虚拟的表格
  
  - 1.查询表中的单个字段
    
    ```sql
    SELECT last_name FROM employees;
    ```
  
  - 2.查询表中的多个字断
    
    - ```sql
      SELECT `last_name`,`salsry`,email FROM employees;
      ```
  
  - 3.查询表中的所有字断
    
    ```sql
    select * from employees;
    ```
    
    - 细节：字断可以添加着重号``,防止理解成关键字
  
  - 4.查询常量值
    
    ```sql
    SELECT 100;
    SELECT 'john';
    #查询表达式
    SELECT 100*98;
    #查询函数
    SELECT version();
    ```
  
  - 5.起别名
    
    ```sql
    #方式一 使用as
    SELECT 100%98 AS result;
    select last_name as 姓,first_name as 名 from employees;
    #方式二 使用空格
    select last_name 姓,first_name 名 from employees;
    #案例：查询salary，显示结果为out put
    select salary as `out put` from employees;
    #单引号，双引号，着重号都可以
    ```
    
    - 好处：便于理解；如果要查询的字段有重名的情况，使用别名可以区分开来
  
  - 6.去重
    
    ```sql
    #案例：查询员工表中涉及到的所有的部门编号
    select department_id from employees;
    #加上关键字DISTINCT，可以实现去重
    select distinct department_id from employees;
    ```
  
  - 7.+号的作用与字符串拼接
    
    ```sql
    /*
    mysql的+只有一个功能：运算符
    select 100+90;#两个操作数都为数值型，则做加法运算
    select '123'+90;#其中一方为字符型，会试图将字符型数值转换成数值型，
            如果转换成功，则继续做加法运算，
            如果转换失败，则将字符型数值转换成0
    select null+10;#只要其中一方为null，则结果肯定为null
    */
    #案例：查询员工名和姓连接成一个字段，并显示为姓名
    #使用CONCAT()进行拼接
    select concat(last_name,first_name) as 姓名 from employees;
    ```
    
    ```sql
    select ifnull(commission_pct,0) as 奖金率 from employees;
    #ifnull(),如果commission_pct为null，则用0替代
    /*
    案例：显示出表employees的全部列，
    各个列之间用逗号连接，
    列头显示成OUT_PUT
    */
    SELECT 
        CONCAT(
        last_name,
        ',',
        first_name,
        ',',
        job_id,
        ',',
        IFNULL(commission_pct,0)) AS OUT_PUT 
    FROM
        employees;
    ```

- 2.条件查询
  
  - 语法
    
    ```sql
    select
        查询列表
    from     
        表名
    where 
        筛选条件    
    ```
  
  - 按条件表达式筛选
    
    - 条件运算符：> < = != <>(等价于!=) >= <=
      
      ```sql
      #案例：查询工资>12000的员工信息
      SELECT
          * 
      FROM
          employees 
      WHERE
          salary > 12000;
      #案例：查询部门编号不等于90号的员工名和部门编号
      SELECT
      first_name,
      department_id 
      FROM
      employees 
      WHERE
      department_id <> 90;
      ```
  
  - 按逻辑表达式筛选
    
    - 逻辑运算符： && || ! and or not
      
      ```sql
      #案例：查询工资在10000到20000之间的员工名、工资及奖金
      SELECT
          first_name,
          salary,
          salary * IFNULL( commission_pct, 0 ) AS commission 
      FROM
          employees 
      WHERE
          salary >= 10000 
          AND salary <= 20000;
      #案例：查询部门编号不实在90到110之间，或者工资高于15000的员工信息
      SELECT
          * 
      FROM
          employees 
      WHERE
          department_id < 90 OR department_id > 110 
          OR salary > 15000;
      #使用not
      SELECT
          * 
      FROM
          employees 
      WHERE
          NOT ( department_id >= 90 AND department_id <= 110 ) 
          OR salary > 15000;
      ```
    
    - 模糊查询
      
      - like
        
        - 一般和同配合搭配使用
          
          - 通配符：
            
            - % 可以匹配任意多个字符，包含0个字符
            
            - _ (下划线)任意单个字符
              
              ```sql
              #案例1:查询员工名中包含字符a的员工信息
                    SELECT
                        * 
                    FROM
                        employees 
                    WHERE
                        last_name LIKE '%a%';
                    #案例2:查询员工名中第三个字符为n，第五个字符为l的员工名和工资
                    SELECT
                        last_name,
                        salary 
                    FROM
                        employees 
                    WHERE
                        last_name LIKE '__n_l%';
                    #案例3:查询员工名中第二个字符为_的员工名，可以使用转义字符\
                    SELECT
                        last_name
                    FROM
                        employees 
                    WHERE
                        last_name LIKE '_\_%';
                    #ESCAPE指定$为转义字符
                    SELECT
                        last_name,
                        salary 
                    FROM
                        employees 
                    WHERE
                        last_name LIKE '_$_%' ESCAPE '$';
              ```
      
      - between and
        
        - 可以提高语句的简洁度
        
        - 包含两个临界值
        
        - 两个临界值不要调换顺序，完全等价于>=左边的值并且<=右边的值
          
          ```sql
              #案例：查询员工编号在100到200之间的员工信息
                SELECT
          
                FROM
                employees 
                WHERE
                employee_id BETWEEN 100 
                AND 120;
          ```
      
      - in
        
        - 判断某字段的值是否属于in列表中的某一项
        
        - in列表的值类型必须统一或兼容
        
        - in列表内不支持使用通配符
          
              /*
                案例：查询员工的工种编号是IT_PROG,AD_VP,AD_PRES
                中的一个的员工名和工种编号
                */
                SELECT
                last_name,
                job_id 
              FROM
                employees 
              WHERE
                job_id IN ( 'IT_PROG', 'AD_VP', 'AD_PRES' );
      
      - is null
        
        - =或<>不难用于判断null值
        
        - is null或is not null可以判断null值
          
          ```sql
          #案例：查询没有奖金的员工名和奖金率
            SELECT
            last_name,
            commission_pct
            FROM
            employees 
          WHERE
            commission_pct IS NULL;
            #有奖金的
            SELECT
            last_name,
            commission_pct
            FROM
            employees 
          WHERE
            commission_pct IS NOT NULL;
          ```
      
      - 安全等于 <=>
        
        ```sql
        #案例：查询没有奖金的员工名和奖金率
          SELECT
          last_name,
          commission_pct
          FROM
          employees 
        WHERE
          commission_pct <=> NULL;
          #案例：查询工资为12000的
          SELECT
          last_name,
          salary 
        FROM
          employees 
        WHERE
          salary <=> 12000;
        ```
        
        - is null仅仅可以判断null值，可读性较高，建议使用
        
        - <=>既可以判断null值，也可以判断普通的值，但是可读性较差

- 3.排序查询
  
  - 语法：
    
    - asc表示升序，desc表示降序，不写默认升序
    
    - order by 子句中可以支持单个字段、多个字段、表达式、函数、别名
    
    - order by 子句一般放在查询语句的最后面，limit子句除外
      
      ```sql
        select 查询列表 
        from 表 
        where 筛选条件 
        order by 排序列表 [asc|desc] ;
        #默认ASC升序
        select * from employees order by salary desc;
        #案例2:查询部门编号>=90的员工信息，按入职时间的先后进行排序
        SELECT * from employees where department_id >= 90 
        order by hiredate;
        #案例3:按年薪的高低显示员工的信息和年薪【按表达式排序】
        #可以按表达式排序
        SELECT
        *,salary*12*(1+IFNULL(commission_pct,0)) AS 年薪
        from employees  
        order by salary*12*(1+IFNULL(commission_pct,0)) DESC;
        #可以直接使用别名【按别名排序】
        SELECT
        *,salary*12*(1+IFNULL(commission_pct,0)) AS 年薪
        FROM
        employees 
        ORDER BY
        年薪 DESC;
        #案例4:按姓名的长度显示员工的姓名和工资【按函数排序】
        SELECT
        LENGTH(last_name) AS 字节长度,
        last_name,
        salary
        FROM
        employees 
        ORDER BY
        LENGTH( last_name ) DESC;
        #案例6:查询员工信息，要求先按工资升序排序，再按员工编号降序排序【按多个字段排序】
        SELECT *
        FROM
        employees 
        ORDER BY
        salary ASC,employee_id DESC;    
      ```

- 4.常见函数
  
  - 调用：select 函数名(实参列表) [from 表];
  
  - 单行函数：如concat、length、ifnull等；
    
    - 字符函数
      
      - length:获取参数值的字节个数
      
      - concat:拼接字符串
      
      - upper,lower:大小写转换
      
      - substr、substring:
        
        ```sql
        #索引从1开始
        #截取从指定索引处后面所有字符，输出莫展元
        select substr('李莫愁爱上了莫展元'，7) out_put;
        #截取从指定索引处指定字符长度的字符，输出李莫愁
        select substr('李莫愁爱上了莫展元'，1，3) out_put;
        ```
      
      - instr(str,substr):返回子串在原字符串的第一次出现的索引，如果找不到，返回0
      
      - trim:
        
        - trim(str):去除字符串前后空格
        
        - trim(remstr from str):去除字符串str的前后remstr字符串
      
      - lpad(str,len,padstr):用指定的字符实现左填充指定长度,若指定长度不足原字符长度，则进行裁切字符串操作，如果指定长度超过原字符串，则超出部分用padstr填充，允许出现padstr不完整
        
        ```sql
        #输出：'***ax'
        select lpad('ax',5,'*') as out_put;
        ```
      
      - rpad(str,len,padstr):用指定的字符实现右填充指定长度,若指定长度不足原字符长度，则进行裁切字符串操作
      
      - replace(str,from_str,to_str):替换字符串str中的全部from_str为to_str;
    
    - 数学函数
      
      - round:四舍五入
        
        - round(x):只保留整数
        
        - round(x,D):四舍五入至保留D位小数，如果不足D位不做操作，也不补0
      
      - ceil:向上取整，返回>=该参数的最小整数
      
      - floor:向下取整，返回<=该参数的最大整数
      
      - truncate(x,D):将x截断至D位小数
      
      - mod(n,m):取余，n对m取余,结果为n-n/m*m
      
      - rand():获取随机数，返回0-1之间的小数
    
    - 日期函数
      
      - now():返回当前系统日期+时间
      
      - curdate():返回当前系统日期，不包含时间
      
      - curtime():返回当前系统时间，不包含日期
      
      - year(date):返回年
      
      - month(date):返回月
      
      - monthname(date):返回月的英文
      
      - day(),hour(),minute(),second()
      
      - str_to_date(str,format):将日期格式的字符串转换成指定格式的日期
        
        - STR_TO_DATE('9-13-1999','%m-%d-%Y'):1999-09-13
      
      - date_format(date,format):将日期转换成字符
        
        - DATE_FORMAT('2018/6/6','%Y年%m月%d日'):2018年06月06日
    
    - 其他函数
      
      - version():查看版本
      
      - database():查看当前数据库
      
      - user():查看当前用户
      
      - password('字符'):返回该字符的密码形式（mysql8中已经被弃用，使用sha1和md5（返回md5的加密形式，即哈希）替代）
    
    - 流程控制函数
      
      - if(expr1,expr2,expr3):如果expr1的结果为ture，则返回expr2的结果，否则返回expr3的结果
      
      - case使用一：类似于switch case
        
        ```sql
        /*
        case 要判断的字段或表达式
        when 常量1 then 要显示的值1或语句1;(语句加分号，值不加分号)
        when 常量2 then 要显示的值2或语句2;
        ...
        else 要显示的值n或语句
        end
        */
        SELECT
            salary AS 原始工资,
            department_id,
        CASE
                department_id 
                WHEN 30 THEN
                salary * 1.1 
                WHEN 40 THEN
                salary * 1.2 
                WHEN 50 THEN
                salary * 1.3 ELSE salary 
            END AS 新工资 
        FROM
            employees;
        ```
      
      - case使用二：类似于多重if
        
        ```sql
        /*
        case
        when 条件1 then 要显示的值1或语句1
        when 条件2 then 幺显示的值2或语句2
        ...
        else 要显示的值n或语句 
        end
        */
        SELECT
            salary,
        CASE
        
                WHEN salary > 20000 THEN
                'A' 
                WHEN salary > 15000 THEN
                'B' 
                WHEN salary > 10000 THEN
                'C' ELSE 'D' 
            END AS 工资级别 
        FROM
            employees
        ```

  - 分组函数
    
    - 做统计使用，又称为统计函数、聚合函数、组函数
    
    - 分类：sum()求和、avg()平均值、max()最大值、min()最小值、count()计算非空个数
    
    - 特定：sum、avg一般用于处理数值类型，max、min、count可以处理任何类型
    
    - 以上分组函数都忽略null值
    
    - 可以和distinct搭配使用，实现去重，如：sum(DISTINCT salary);
    
    - 统计行数：
      
      ```sql
      select count(*) from employess;
      #count中使用常量值，相当于在表中加了一列该常量值，然后统计个数
      #因此也能用来统计行数
      select count(1) from employess;
      ```
    
    - 和分组函数一同查询的字段要求是group by后的字段

- 5.分组查询
  
  - 分组数据：group by 子句可以将表中数据分成若干组
  
  - 查询列表必须特殊，要求是分组函数和group by后出现的字段
    
    ```sql
    #where一定放在from后面
    select column,group_function(column)
    from table
    [where condition]
    [group by group_by_expression]
    [order by column];
    #案例1:查询每个工种的最高工资
    SELECT
        job_id,
        MAX( salary ) 
    FROM
        employees 
    GROUP BY
        job_id;
    #案例2:查询每个位置上的部门个数
    SELECT
        COUNT( * ),
        location_id 
    FROM
        departments 
    GROUP BY
        location_id;
    #添加分组前的筛选条件
    #案例3:查询邮箱中包含字符a的，每个部门的平均工资
    SELECT
    AVG( salary ),
    department_id 
    FROM
    employees 
    WHERE
    email LIKE '%a%' 
    GROUP BY
    department_id;
    #案例4:查询每个领导手下的有奖金的员工的最高工资
    SELECT
        MAX( salary ),
        manager_id 
    FROM
        employees 
    WHERE
        commission_pct IS NOT NULL 
    GROUP BY
        manager_id;
    ```
    
    ```sql
    #添加分组后的筛选条件:HAVING
    #案例1:查询哪个部门的员工个数>2
    #第一步，查询每个部门的员工个数
    SELECT
        COUNT(*),
        department_id 
    FROM
        employees 
    GROUP BY
        department_id；
    #第二步，根据第一步的结果进行筛选，查询哪个部门的员工个数>2
    SELECT
        COUNT(*),
        department_id 
    FROM
        employees 
    GROUP BY
        department_id 
    HAVING
        COUNT(*)> 2;
    #案例2:查询每个工种有奖金的员工的最高工资大于12000的工种编号和最高工资
    SELECT
        job_id,
        MAX( salary ) 
    FROM
        employees 
    WHERE
        commission_pct IS NOT NULL 
    GROUP BY
        job_id 
    HAVING
        MAX( salary )> 12000;
    #案例3:查询领导编号大于102的每个领导手下最低工资大于5000的领导编号以及最低工资
    SELECT
    manager_id,
    MIN( salary ) 
    FROM
    employees 
    WHERE
    manager_id > 102 
    GROUP BY
    manager_id 
    HAVING
    MIN( salary )> 5000;
    ```
  
  - 分组前筛选，数据源为原始表，位置为group by子句的前面，关键字为where
  
  - 分组后筛选，数据源为分组后的结果集，位置为group by子句的后面，关键字为having，分组函数做筛选条件肯定是放在having子句中
    
    ```sql
    #按表达式或函数分组
    #group by和having后面都支持别名，where后不支持字段别名
    #案例：按员工姓名的长度分组，查询每一组的员工个数，筛选员工个数大于5的有哪些
    SELECT
        LENGTH( last_name ),
        COUNT(*) 
    FROM
        employees 
    GROUP BY
        LENGTH( last_name ) 
    HAVING
        COUNT(*)> 5;
    #按多个字段分组,多个字段之间用逗号隔开，没有先后顺序要求
    #案例：查询每个部门每个工种的员工的平均工资
    SELECT
        AVG( salary ),
        department_id,
        job_id 
    FROM
        employees 
    GROUP BY
        department_id,
        job_id;
    #添加排序，排序放在整个分组查询最后
    #案例：查询每个部门每个工种的员工的平均工资,并且按平均工资的高低显示
    SELECT
        AVG( salary ),
        department_id,
        job_id 
    FROM
        employees 
    GROUP BY
        department_id,
        job_id 
    ORDER BY
        AVG( salary ) DESC;
    ```

- 6.连接查询：
  
  - sql92标准：
    
    - 等值连接：
      
      - 多表的等值连接为多表的交集部分
      
      - n表连接至少需要n-1个连接条件
      
      - 多表的顺序没有要求
      
      - 一般需要为表起别名
      
      - 可以搭配前面介绍的所有字句使用，如排序、分组、筛选
      
      ```sql
      #案例1:查询女神名和对应的男神名
      SELECT 
          name,
          boyName 
      FROM
          boys,
          beauty 
      WHERE
          beauty.boyfriend_id = boys.id;
      #案例2:查询员工名和对应的部门名
      SELECT
          last_name,
          department_name 
      FROM
          employees,
          departments 
      WHERE
          employees.department_id = departments.department_id;
      ```
      
      - 为表起别名
        
        - 区分多个重名的字段
        
        - 如果为表起了别名，则查询的字段就不能使用原来的表名来限定了
          
          ```sql
          #查询员工名、工种号，工种名
          SELECT
              last_name,
              e.job_id,
              job_title 
          FROM
              employees AS e,
              jobs 
          WHERE
              e.job_id = jobs.job_id;
          ```
      
      - 加筛选
        
        ```sql
        #查询有奖金的员工名、部门名
        SELECT
            last_name,
            job_title 
        FROM
            employees e,
            jobs 
        WHERE
            e.job_id = jobs.job_id 
            AND e.commission_pct IS NOT NULL;
        #查询城市名中第二个字符为o的部门名和城市名
        SELECT
        department_name,
        city 
        FROM
        locations AS l,
        departments AS d 
        WHERE
        d.location_id = l.location_id 
        AND l.city LIKE '_o%';
        ```
      
      - 加分组
        
        ```sql
        #案例1:查询每个城市的部门个数
        SELECT
            city,
            COUNT(*) 
        FROM
            locations AS l,
            departments AS d 
        WHERE
            d.location_id = l.location_id 
        GROUP BY
            city;
        ```
      
      - 加排序
        
        ```sql
        #案例：查询每个工种的工种名和员工的个数，并且按员工个数降序
        SELECT
        job_title,
        COUNT(*) AS 个数 
        FROM
        employees AS e,
        jobs AS j 
        WHERE
        e.job_id = j.job_id 
        GROUP BY
        job_title 
        ORDER BY
        个数 DESC;
        ```
      
      - 三表连接
        
        ```sql
        #案例：查询员工名、部门名和所在的城市
        SELECT
        last_name,
        department_name,
        city 
        FROM
        employees AS e,
        locations AS l,
        departments AS d 
        WHERE
        e.department_id = d.department_id 
        AND d.location_id = l.location_id;
        ```
    
    - 非等值连接
      
      ```sql
      #案例：查询员工的工资和工资级别
      SELECT
      last_name,
      salary,
      grade_level 
      FROM
      employees AS e,
      job_grades AS g 
      WHERE
      e.salary BETWEEN g.lowest_sal 
      AND g.highest_sal;
      ```
    
    - 自连接
      
      ```sql
      #案例：查询员工名和上级的名称
        SELECT
          e.employee_id,
          e.last_name,
          m.employee_id,
          m.last_name 
      FROM
          employees AS e,
          employees AS m 
      WHERE
          e.manager_id = m.employee_id;
      ```
  
  - sql99标准【推荐使用】：
    
    - 语法：
      
      ```sql
      select 查询列表
      from 表1 别名 [连接类型]
      join 表2 别名 
      on 连接条件 
      where 筛选条件
      group by 分组
      having 筛选条件
      order by 排序列表;
      ```
    
    - 内连接【*】：inner
      
      ```sql
      select 查询列表
      from 表1 别名 
      inner join 表2 别名 
      on 连接条件 ;
      ```
      
      - 等值连接
        - 可以添加排序、分组、筛选
        -  inner可以省略
      ```sql
      #案例：查询员工名、部门名
      SELECT
        last_name,
        department_name 
      FROM 
        employees e
      INNER JOIN departments d 
      ON e.department_id = d.department_id;
      #案例：查询员工名、部门名、工种名，并按部门名降序
      SELECT
        last_name,
        department_name,
        job_title 
      FROM
        employees e
        INNER JOIN departments d ON e.department_id = d.department_id
        INNER JOIN jobs j ON e.job_id = j.job_id 
      ORDER BY
        department_name DESC;
      ``` 
      - 非等值连接
      ```sql
      #案例：查询员工的工资级别
      SELECT
        grade_level,
        salary
      FROM
      employees e
      INNER JOIN job_grades g ON e.salary BETWEEN g.lowest_sal 
      AND g.highest_sal;
      ``` 
      
      - 自连接
      ```sql
      SELECT
        e.last_name,
        g.last_name 
      FROM
        employees e
        INNER JOIN employees g ON e.manager_id = g.employee_id 
      WHERE
        e.last_name LIKE '%k%';
      ``` 
    
    - 外连接
      - 应用场景：用于查询一个表中有，另一个表没有的记录
      - 特点：
        - 外连接的查询结果为主表中的所有记录，如果从表中有何它匹配的，则显示匹配的值，如果从表中没有和它匹配的，则显示null，外连接查询结果=内连接结果+主表中有而从表中没有的记录
        - 左外连接中，left join左边的是主表，如果是右外连接，right join右边的是主表
        - 左外和右外交换两个表的顺序，可以实现同样的效果
        - 全外连接的查询结果=内连接的结果+表1中有但是表2没有的+表2中有但是表1中没有的
      
      - 左外连接【*】:left [outer]
      ```sql
      #查询男朋友不在男神表的女神名
      SELECT
        b.`name`
      FROM
        beauty b
        LEFT OUTER JOIN boys bo ON b.boyfriend_id = bo.id 
      WHERE
        bo.id IS NULL;
      #查询哪个部门没有员工
      SELECT
        d.department_name
      FROM
        departments d
        LEFT JOIN employees e ON d.department_id = e.department_id 
      WHERE
        e.employee_id IS NULL;
      ``` 
      
      - 右外连接【*】:right [outer]
      ```sql
      SELECT
        b.`name`
      FROM
        boys bo 
        RIGHT OUTER JOIN beauty b ON b.boyfriend_id = bo.id 
      WHERE
        bo.id IS NULL;
      ``` 
      
      - 全外连接(mysql不支持):full [outer]
      ```sql
      SELECT
        b.*,
        bo.* 
      FROM
        beauty b
        FULL JOIN boys bo ON b.boyfriend_id = bo.id;
      ```
    - 交叉连接:cross
      - 实验sql99语法的标准实现笛卡尔乘积
      ```sql
      SELECT
        b.*,
        bo.* 
      FROM
        beauty b
        CROSS JOIN boys bo;
      ``` 
- 7.子查询
  - 含义：出现在其他语句中的select语句，称为子查询或内查询，外部的查询语句，称为主查询或外查询
  - 按子查询出现的位置分类：
    - select后面
      - 仅仅支持标量子查询
      ```sql
      #案例：查询每个部门的员工个数
      SELECT d.*,(
        SELECT COUNT(*) FROM employees e
        WHERE e.department_id=d.department_id
        ) 个数 FROM departments d;
      ``` 
    - from后面
      - 支持表子查询
      ```sql
      #案例：查询每个部门的平均工资的工资等级
      SELECT
        av.*,
        g.grade_level 
      FROM
        ( SELECT AVG( salary ) avg, department_id FROM employees GROUP BY department_id ) av
        INNER JOIN job_grades g ON av.avg BETWEEN g.lowest_sal 
        AND g.highest_sal;
      ``` 
    - where或者having后面（*）
      - 特点：
        - 子查询放在小括号内
        - 子查询一般放在条件的右侧
        - 标量子查询，一般搭配着单行操作符使用：> < >= <= = <>
        - 列子查询，一般搭配这多行操作符使用：in , any/some , all
        - 子查询的执行优先于主查询执行，主查询的条件用到了子查询的结果
      - 标量子查询（单行）（*）
      ```sql
      #案例1：谁的工资比Abel高
      SELECT * 
      FROM employees 
      WHERE salary >( 
        SELECT salary 
        FROM employees 
        WHERE last_name = 'Abel' 
        );
      #案例2：返回job_id与141号员工相同，salary比143号员工多的员工姓名，job_id和工资
      SELECT last_name, job_id, salary 
      FROM employees 
      WHERE job_id =( 
        SELECT job_id 
        FROM employees 
        WHERE employee_id = 141 
      ) 
      AND salary > ( 
        SELECT salary 
        FROM employees 
        WHERE employee_id = 143 
      );
      #案例3：返回公司工资最少的员工的last_name，job_id和salary
      SELECT last_name,job_id,salary
      FROM employees 
      WHERE salary=(
        SELECT MIN(salary) FROM employees
      );
      #案例4：查询最低工资大于50号部门最低工资的部门id和其最低工资
      SELECT
        department_id,
        MIN( salary ) 
      FROM
        employees 
      GROUP BY
        department_id 
      HAVING
        MIN( salary )>(
        SELECT
          MIN( salary )
        FROM
          employees 
        WHERE
        department_id = 50 
        );
      ``` 
      - 列子查询（多行）（*）
        - 多行操作符
          - in/not in 等于列表中的任意一个 
            - in等价于 =ANY
            - not in 等价于 <>ALL
          - any|some 和子查询返回的某一个值比较
          - all 和子查询返回的所有值比较
        ```sql
        #案例1：返回location_id是1400或1700的部门中的所有员工姓名
        SELECT
          last_name,department_id
        FROM
          employees 
        WHERE
          department_id IN (
          SELECT
            department_id 
          FROM
            departments
        WHERE
          location_id IN ( 1400, 1700 ));
        #案例2：返回其他工种中比job_id为'IT_PROG'工种任意工资低的员工的员工号、姓名、job_id以及salary
        SELECT
          employee_id,
          last_name,
          job_id,
          salary 
        FROM
          employees 
        WHERE
          salary < ANY ( SELECT DISTINCT salary FROM employees WHERE job_id = 'IT_PROG' ) 
          AND job_id <> 'IT_PROG';
        #案例2：返回其他工种中比job_id为'IT_PROG'工种所有工资低的员工的员工号、姓名、job_id以及salary
        SELECT
          employee_id,
          last_name,
          job_id,
          salary 
        FROM
          employees 
        WHERE
          salary < ALL ( SELECT DISTINCT salary FROM employees WHERE job_id = 'IT_PROG' );
        ``` 
      - 行子查询（使用较少）：结果集一行多列或多行多列
        - 有局限性，要求这些筛选条件都是使用一样的操作符(<,>,=等)
      ```sql
      #案例：查询员工编号最小并且工资最高的员工信息
      #使用标量子查询
      SELECT * FROM employees WHERE employee_id =( SELECT MIN( employee_id ) FROM employees ) 
      AND salary = ( SELECT MAX( salary ) FROM employees );
      #使用行子查询
      SELECT * FROM employees WHERE ( employee_id, salary )=( SELECT MIN( employee_id ), MAX( salary ) FROM employees );
      ``` 
    - exists后面（相关子查询）
      - 表子查询
  - 按结果集的行列数不同分类：
    - 标量子查询（结果集只有一行一列）
    - 列子查询（结果集是一列多行）
    - 行子查询（结果集是一行多列）
    - 表子查询（结果集一般为多行多列）