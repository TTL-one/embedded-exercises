# InStone：Linux 视频系统项目驱动课程

这是一个从 C/C++ 基础逐步扩展到 Linux 视频采集、编码、录像和网络传输的学习工程。每一课都包含可运行代码、观察目标、实验任务和复盘问题，最终组合成一个可写入简历的 Linux 视频采集与远程录像系统。

## 学习主线

1. `vector` 的 `size`、`capacity`、扩容和连续内存
2. 对象生命周期、拷贝、移动、右值引用和 `std::move`
3. RAII 与 Linux 文件描述符
4. `write`、页缓存、`fsync` 与持久化
5. `mmap`、虚拟内存和共享映射
6. V4L2 采集与缓冲区复用
7. FFmpeg 编码、时间戳和封装
8. 线程、有界队列和背压
9. socket、`poll`、`epoll` 与多客户端传输
10. 中断、等待队列和驱动配套实验
11. 性能测量、故障恢复和项目收尾

Linux 内核驱动实验继续使用 C；用户态主项目使用 C++。底层仍调用 `open`、`read`、`write`、`ioctl`、`mmap`、`poll` 和 socket 等 Linux/POSIX 接口。

## 目录

- `lesson01_vector/`：假视频帧、`vector` 内存与 PGM 文件
- `lesson02_move/`：帧对象的复制、移动与容器扩容
- `archive/`：从个人笔记目录中筛选出的 C 语言与嵌入式 Linux 小练习；不参与根目录 CMake 构建

后续课程按学习进度逐课加入，不提前堆放完整成品。

## 构建全部课程

在 WSL/Ubuntu 中执行：

```bash
cd /mnt/e/CodeProject/InStone
cmake -S . -B build
cmake --build build
```

运行第一课：

```bash
cd lesson01_vector
../../InStone/build/lesson01_vector/frame_generator
```

运行第二课：

```bash
./build/lesson02_move/move_demo
```

也可以进入各课目录，按照该目录的 `README.md` 使用 `g++` 单独编译。

## 学习规则

- 先预测运行结果，再编译运行。
- 保留警告选项：`-Wall -Wextra -pedantic`。
- 每课记录输入、处理动作、输出和原因。
- 编译通过只证明语法和链接完成；功能结论以运行和实验结果为准。
- 简历只记录实际完成、实际测量并能解释的内容。
