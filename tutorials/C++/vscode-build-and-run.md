# VS Code 使用

VSCode，全称Visual Studio Code（在Visual Studio后边加了一个Code），有时也简称为 VSC，是一个编辑器。



## 在 VS Code 中构建和运行

> **TL;DR**: VS Code 商店里有 CMake Tools 插件；使用 CMake 生成构建档，再搭配插件，build & run 可以一气呵成。

VS Code 是一个可以搭配很多插件、因此很好用的代码**编辑器**；既然是编辑器，那么它本身其实不是为了调试程序而设计的。

不过，编写一些小的程序，甚至小型的工程，使用 IDE 会稍显庞大，一般也可以选择使用 VS Code 编写；写好之后，想顺手在 VS Code 中编译运行的时候，可能会遇到一些问题。

首先，对于单文件或几个文件的小程序，可以在终端中通过命令行进行编译链接和调试；商店中也有 Code Runner 之类的插件也有帮助，但本质上也是替用户在 Shell 中执行命令，二选一的话还是建议自己手动输入命令。

> 惊喜的是，写文章的时候我测试了一下，不知道为何，设置断点后直接点击`Start Debugging` / 按`F5`可以进入调试的，断点和变量查看都没有问题，猜想应该是 cpp-tool 插件帮助配置好了`lanuch.json`等文件
>
> 笔者的环境是 Windows 下，使用 msys2 工具链，能够提供类似 Linux 的环境，编译器是MinGW gcc；在 VS Code 中把编译器的路径设置为了 msys2 MinGW 64bit 中的`g++`；除此之外没有启用其他插件。



但是当程序需要链接外部库的时候，事情就变得复杂了。

将`g++`添加到 Windows 环境变量后，在 cmd 或者 PowerShell 里面都能使用，而这个 MinGW gcc 也能找到 msys2 环境中的系统标头文件路径，以及链接 C++ 标准库文件。因此，仅编译源代码文件，以及编译链接使用 C++ 标准库的程序，是没有问题的。

但是要链接外部库，就要指定库文件路径以及库文件名；直接点击`F5`是不行了，因为没有指定要链接的库；好在 msys2 也有`pkg-config`可供使用，调用它可以获取外部库的标头文件路径以及库文件路径，比如有一个`show_img.cpp`使用了 OpenCV 这个库，理论上，这样写命令便能实现目的：

```cpp
g++ show_img.cpp -o show_img $(pkg-config --cflags --libs opencv4)
```

> `$()`为命令替换符，作用是获取其他命令的输出。

不过可惜的是，在 PowerShell 中输入上述命令，`g++`并不能完成链接操作；只有在 msys2 的窗口中能够正常使用。索性放弃手动输入命令，转向使用 CMake 进行程序的构建。

关于 CMake 的使用，见另一篇文章。这里只做简单介绍。

要使用 CMake，首先安装 CMake，并把二进制文件添加到环境变量，这样就能在命令行中使用了；然后，需要写`CMakeLists.txt`，大概像这样：

```cmake
cmake_minimum_required(VERSION 3.13)
project(opencv_test)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(show_img show_img.cpp)
target_link_libraries(show_img ${OpenCV_LIBS})
```

> 虽然成功找到 OpenCV 的安装之后，会写入`OpenCV_DIRS`变量，但并不需要使用`include_directories(${OpenCV_INCLUDE_DIRS})`命令。

之后，随便进入某个文件夹（一般还是选择在项目文件夹下新建一个`build`目录），然后在目录下执行`cmake`并指定`CMakeLists.txt`所在的目录，若在 Windows 下想要使用 msys2 的工具链进行构建（使用 msys2 环境下安装的库），还要指定 CMake 生成的构建档的类型，如：

```bash
cmake .. -G "MinGW Makefiles"
```

然后开始构建（默认以 Release 生成；路径需要指定为`CMakeCache.txt`所在目录，也就是构建目录下）：

```bash
cmake --build .
```

之后就会看到生成的`show_img`可执行文件出现在`build`目录下。

而 VS Code 商店中是有 CMake Tools 插件的，安装后我们会发现 VS Code 界面上出现了 CMake 相关的选项；选择好构建类型、构建项、启动项之后，只需要点击调试或者运行按钮就可以了。

综上，个人感觉 CMake 是目前最好的解决方案，写一次`CMakeLists.txt`，到处都可以用。



---

扩展阅读：

- https://www.bilibili.com/video/BV1sW411v7VZ/

- https://www.bilibili.com/video/BV1nt4y1r7Ez/

- [vscode配置opencv c++开发环境 - walker_bin - 博客园 (cnblogs.com)](https://www.cnblogs.com/walker-lin/p/11111226.html)

- [（三）Visual Studio Code中设置OpenCV(MinGW-w64) - 简书 (jianshu.com)](https://www.jianshu.com/p/bc6017331646)
