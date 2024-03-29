# 刷题时遇到的`class Solution`怎么理解？



`class`表示声明一个类，单词`Solution`是“解决方案”的意思。那么`class Soulution` 就是声明出一个解决方案的类，类中用来存放关于解决这一问题所需要的各种函数/方法、变量。

为什么要这样做呢，简单说来就是：

- 抽象层面上更符合逻辑——针对某一问题的解决方案，方案里可以有若干成员变量和成员函数
- 生成若干实例互不干扰——因为每个类的实例内部的（非静态）成员变量是独立的
- 更“干净”，即不会污染到程序其他地方的代码——类中的成员不会与类外其他标识符冲突

那么这个类（的对象），可以理解成**函数+函数运行需要的环境**。如果函数不需要额外的运行环境，也可以把函数设置成**静态**的。

具体来说，比如我们有一个问题，需要用函数`funcA()`解决，而`funcA()`又需要调用函数`funcX()`， 而完成这个问题可能还需要一个`int` 型的变量`val`。

简单的C语言文件可能会这样写：

```c
// solution.c

int val;

void funcX() { /* ... */ }

void funcA() { /* ... */ }
```

这样的话变量`val`还有函数`funcA()`和`funcX()`都是全局的，如果和其他很多很多的代码放在一起就有名称冲突的可能。不过在C++中，我们可以把它们放在一个类里面：

```cpp
// solution.cpp

class Solution {
private:
  int val;
  void funcX() { /* ... */ }
public:
  void funcA() { /* ... */ }
};
```

因为需要对外可用的只有`funcA`这个接口，所以只把它设置成`public`就好了 。

然后在需要调用的地方调用就好了，比如`main` 函数：

```cpp
int main() {
  Solution s;
  s.funcA();
}
```

当然，如果是解决某一问题的解决方案，也不一定用`Solution`作为类名，比如`SolveAngle`、`MatrixAdd`之类的也可以。



> 以上内容也发布在知乎：[C++中的class Solution是什么意思? - 斑马的回答 - 知乎 (zhihu.com)](https://www.zhihu.com/question/443211709/answer/1718958336)



