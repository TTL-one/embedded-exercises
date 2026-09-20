# 第二课：拷贝、移动与 `std::move`

## 本课目标

- 区分对象复制和资源所有权转移。
- 理解左值、右值和右值引用的实际用途。
- 通过地址和构造日志观察复制构造与移动构造。
- 理解 `std::move()`只进行类型转换，移动构造函数才转移资源。
- 观察 `push_back`、移动后的 `push_back`和 `emplace_back`的区别。

## 关键结论

```text
Frame b = a;
```

`a`是有名字的左值，调用复制构造函数。`b.pixels`申请独立内存并复制像素。

```text
Frame c = std::move(a);
```

`std::move(a)`把表达式转换为允许移动的值类别。移动构造函数接管`a`所拥有的像素缓冲区。移动后的`a`仍可析构或重新赋值，但具体内容处于有效但未指定状态。

移动构造函数声明为`noexcept`后，`std::vector<Frame>`在扩容时更有条件使用移动而不是复制，从而避免复制整帧像素。

## 实验一：单个对象

运行默认程序，比较：

- `a`构造后的像素地址；
- `b`复制后的像素地址；
- `c`移动后的像素地址；
- 移动后`a`的状态。

预期关系：`b`拥有独立地址；`c`通常接管`a`原先的地址。

## 实验二：放入容器

把`main()`替换为文件底部注释中的实验二版本，比较：

```cpp
frames.push_back(first);              // 复制
frames.push_back(std::move(second));  // 移动
frames.emplace_back(320, 240, 3);     // 原位构造
```

先保留`frames.reserve(3)`观察三种插入方式，再删除它，观察外层`vector`扩容产生的额外移动。

## 编译运行

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o move_demo
./move_demo
```

## 复盘问题

1. `Frame b = a`为什么要复制像素数据？
2. `std::move()`本身是否搬运数据？
3. 真正转移资源的是哪个函数？
4. 移动后的对象还能否析构和重新赋值？
5. `push_back(first)`和`push_back(std::move(first))`有什么区别？
6. 为什么移动构造函数通常声明为`noexcept`？


