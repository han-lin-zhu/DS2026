# Data Structures and Algorithms

本仓库用于存放**数据结构与算法**课程的 C++ 作业和练习。课程中的数据结构和算法尽量自行实现。

## 当前状态

目前在 `My_STL/vector.hpp` 中练习实现 `my_stl::vector<T>`。已编写构造与拷贝、元素访问、正向与反向迭代器，以及 `reserve()`、`clear()`、`push_back()` 和 `emplace_back()` 的初步实现。

底层使用 `new T[]` 和 `delete[]` 管理数组。`reserve()` 通过拷贝赋值转移元素，`clear()` 将逻辑大小归零。移动构造、插入、删除和调整大小等接口仍待补全。

成员函数的花括号已补齐，头文件通过 GCC 的 C++20 语法检查。已用 `my_stl::vector<int>` 验证元素访问、正向迭代器、普通反向遍历、`reserve()` 和 `clear()`。

`const` 版 `rbegin()` 和 `rend()` 仍需改用 `const_reverse_iterator` 构造返回值。目前对常量容器调用这两个函数会编译失败。

## 目录结构

```text
.
├── .vscode/            # 本机 MinGW-w64 构建、调试与补全配置
├── README.md
├── My_STL/
│   └── vector.hpp      # 手写 vector 类模板
└── exp/                # 课程实验和练习目录
```

VS Code 配置使用 C++20，并指向本机的 `D:/mingw/mingw64/bin`。其中的 `vector` 示例调试配置需要 `exp/vector_demo.cpp` 作为入口，当前仓库尚未包含该文件。
