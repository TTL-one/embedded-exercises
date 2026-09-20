# 第一课：`vector` 与假视频帧

## 本课目标

- 区分 `size()` 和 `capacity()`。
- 观察 `vector` 扩容及元素地址变化。
- 区分 `reserve()`、`resize()`、`push_back()` 和 `pop_back()`。
- 使用 `std::vector<std::uint8_t>` 表示一帧 8 位灰度图。
- 把连续像素内存保存为 PGM 文件。

## 核心模型

```text
capacity：当前分配的空间最多能容纳多少个元素
size：当前已经构造、允许访问多少个元素
有效长度：业务上本次有多少数据，需要由具体协议或数据源决定
```

`reserve(n)`只预留容量，不创建元素。`resize(n)`把元素数量变成 `n`，并初始化新增元素。合法下标始终满足：

```cpp
index < data.size()
```

容量不足时，`push_back()`可能触发重新分配：申请更大的连续空间，移动或复制原有元素，销毁旧元素并释放旧空间。重新分配后，原有元素的指针、引用和迭代器失效。

## 图像大小

8 位灰度图每像素 1 字节：

```text
字节数 = width × height
```

当前学习代码实际调用 `generate_frame(320, 320, 0)`，因此像素区为 102400 字节。若改为 320×240，则为 76800 字节。

## `ofstream::write`

```cpp
output.write(
    reinterpret_cast<const char*>(frame.pixels.data()),
    static_cast<std::streamsize>(frame.pixels.size())
);
```

- `data()`返回像素连续存储区的首地址，类型为 `std::uint8_t*`。
- `reinterpret_cast`不改变数据，只把同一地址解释为 `const char*`，以符合 `write()`接口。
- `size()`返回元素数。由于 `uint8_t`每个元素占 1 字节，此处也等于字节数。
- `static_cast`把无符号的 `std::size_t`数值显式转换为流接口需要的 `std::streamsize`。
- `std::ios::binary`使像素按原始字节写入，避免文本模式转换。

## 单独编译

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o frame_generator
./frame_generator
file frame_000.pgm
```

必须使用 `g++`。`gcc`即使按 `.cpp`编译源文件，链接时通常也不会自动加入 C++ 标准库，会出现 `std::cout`、`std::ofstream`等未定义引用。

## 复盘问题

1. 容量足够与容量不足时，`push_back()`分别做什么？
2. 为什么 `reserve(100)`之后还不能访问 `data[0]`？
3. 为什么 `resize(100)`之后可以访问 `data[0]`？
4. `vector`重新分配后，哪些对象会失效？
5. 为什么图像字节数还取决于像素格式？


