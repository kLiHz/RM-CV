# CMake入门



> 参考链接：
>
> - [CMake 如何入门？ - 知乎](https://www.zhihu.com/question/58949190)
> - [如何评价 CMake？ - 知乎](https://www.zhihu.com/question/276415476)
> - CLion的CMake快速入门教程：[Quick CMake Tutorial](https://www.jetbrains.com/help/clion/quick-cmake-tutorial.html)
> - Visual Studio关于CMake项目的讲解：[CMake projects in Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio)



## 基本的CMake项目

先从最简单的单文件做起，假设现在目录下有一个`main.cpp`文件：

```cpp
#include <iostream>
int main() {
    std::cout << "Hello!";
}
```

我们可以在**和源代码相同的目录下**新建一个`CMakeLists.txt`，然后写入这样的内容：

```cmake
cmake_minimum_required(VERSION 3.13)
project(cmake_test)
set(CMAKE_CXX_STANDARD 14)
add_executable(cmake_testapp main.cpp)
```

这个的例子来自CLion的教程。下面是每一行的解释：

| 命令                                     | 解释                                                         |
| ---------------------------------------- | ------------------------------------------------------------ |
| `cmake_minimum_required(VERSION 3.13)`   | 指定CMake的最低版本要求，如果不加会有警告                    |
| `project(cmake_test)`                    | `project`命令定义项目的名称                                  |
| `set(CMAKE_CXX_STANDARD 14)`             | `set`命令用来给变量赋值，这里`CMAKE_CXX_STANDARD`变量用来指定所需最低的C++版本，这里要求C++版本最低为C++14 |
| `add_executable(cmake_testapp main.cpp)` | `add_executable`会添加一个生成可执行文件的“**目标**（target）”，目标的名字为`cmake_testapp`，其将从`main.cpp`被构建（build） |

请读者留意“目标（target）”的概念，这在现代CMake中很是重要。

保存`CMakeLists.txt`文件，然后在命令行中输入`cmake .`，便会生成相应的构建文件。

不过通常我们不会在源代码的文件夹里直接构建，因为构建过程会生成很多文件，和源代码会在一起，显得比较杂乱。通常会新建一个目录，可以在当前目录，也可以在任何喜欢的地方，只要在这个目录下将`CMakeLists.txt`的位置告诉CMake就可以，然后在这个文件夹里面执行构建的操作，称为“**Out of source building**”。

比如我们在当前目录下新建一个build目录，然后在其中进行构建操作。这样，因为`CMakeLists.txt`的位置在**上一级目录**，可以用`..`表示。

Unix下会默认生成Makefile，生成后使用make命令执行。

```bash
mkdir build && cd build
cmake ..
make
```

Windows下默认生成Visual Studio的项目文件`*.vcxproj`。

也可以使用`-G`命令指定生成的构建文件。如果之前生成的是其他构建文件，需要先删除构建目录下的`CMakeCache.txt`。

```bash
cmake -G "MinGW Makefiles" ..
```

一个项目中可以有多个目标，我们可以指定**多个**生成可执行文件的目标。

比如我们有`test1.cpp`和`test2.cpp`两个cpp，它们分别会生成不同的程序。可以新建两个名为`test1`和`test2`的目标（名字随意），修改后的`CMakeLists.txt`如下所示：

```cmake
cmake_minimum_required(VERSION 3.13)
project(cmake_test)
set(CMAKE_CXX_STANDARD 14)
add_executable(test1 test1.cpp)
add_executable(test2 test2.cpp)
```

修改过`CMakeLists.txt`之后，需要重新执行`cmake`操作。如果使用的是IDE，那么可能会自动进行这个过程，或者提示用户进行这个操作。

注：使用`make`时，可以用`make <target>`指定要生成的目标，比如`make test1`，默认全部执行。

## 多文件

首先新建一个目录，这个目录就是我们的**项目目录**，比如“cmake_test”，我们的操作都在这个目录下执行，而`CMakeLists.txt`一般放在项目根目录下。

```
cmake_test
|   CMakeLists.txt
|   main.cpp
|   solution.cpp
|   solution.h
|
\---build
```



现在，我们的可执行文件的目标需要多个源代码文件生成，比如下面这样，其中`solution.cpp`、`solution.h`、`main.cpp`都在同一级目录（项目根目录）下：

**solution.h**

```cpp
#ifndef SOLUTION
#define SOLUTION

class Solution {
public:
    static int add(int a, int b);
    static int subtract(int a, int b);
    static int multiply(int a, int b);
};

#endif
```

**solution.cpp**

```cpp
#include "solution.h"

int Solution::add(int a, int b) {
    return a + b;
}
int Solution::subtract(int a, int b) {
    return a - b;
}
int Solution::multiply(int a, int b) {
    return a * b;
}
```



**main.cpp**

```cpp
#include <iostream>
#include "solution.h"

int main() {
    std::cout << Solution::add(3, 5) << ' ';
    std::cout << Solution::multiply(3, 3);
}
```



注意`solution.cpp`和`main.cpp`中引用我们自定义的头文件的方式`#include "solution.h"`。因为目前和源代码处在同一目录下，编译器默认会搜索这个路径，所以可以这样写，编译器也能正确的包含（include）这份头文件到源代码文件里。

然后我们的`CMakeLists.txt`这样写：

```cmake
cmake_minimum_required(VERSION 3.13)
project(cmake_test)
set(CMAKE_CXX_STANDARD 14)
add_executable(solution_test main.cpp solution.cpp)
```

这样`solution_test`就会由`solution.cpp`、`main.cpp`构建出来。
