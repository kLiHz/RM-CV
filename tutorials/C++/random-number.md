# 用C++的random库生成更好的随机数

## 传统C语言的`rand()`和`srand()`

你可能还熟悉用`rand()`生成随机数的方法，`rand()`会返回一个伪随机数，范围在`[0, RAND_MAX)`之间；根据实现的不同，`RAND_MAX`的值也会不一样，但至少应为32767。

```cpp
#include <stdio.h>
#include <stdlib.h>

int main()
{
    for(int i = 0; i < 5; i++) printf("%d ", rand());  
    return 0;
}
```

同时，如果不事先调用`srand()`，那么每次程序执行时都假设调用了`srand(1)`，结果就是每次得到的随机数序列都是一样的。

也就是，给`srand()`不同的参数值，`rand()`生成随机数的起点就会不一样。

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(0));
    for(int i = 0; i < 5; i++) printf("%d ", rand());
    return 0;
}
```

`time()`函数每次会返回一个`time_t`值，这样，根据调用的时间不同，生成的随机数也就不同了。

假如想要一个3~8的随机值，你会怎么做呢？也许你会用`3 + rand() % （8 - 3 + 1）`，这样看起来其实是够用了，但是其实这样是不对的。

为了方便说明，我们假设随机数发生的范围是1到32，且1到32每个数字出现的概率是相等的 ，那么`rand() % 5`会得到6个0、7个1、7个2、6个3、6个4，可见，0到4出现的概率并不是相等的。

那该怎么写呢？[std::rand - cppreference.com](https://en.cppreference.com/w/cpp/numeric/random/rand) 这里给出了一个示例，下面直接复制粘贴：

```cpp
#include <cstdlib>
#include <iostream>
#include <ctime>
 
int main() 
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    int random_variable = std::rand();
    std::cout << "Random value on [0 " << RAND_MAX << "]: " 
              << random_variable << '\n';
 
    // roll 6-sided dice 20 times
    for (int n=0; n != 20; ++n) {
        int x = 7;
        while(x > 6) 
            x = 1 + std::rand()/((RAND_MAX + 1u)/6);
            // Note: 1+rand()%6 is biased
        std::cout << x << ' ';
    }
}
```

即，记总能产生的随机数数目的最大值为$N$，在这里$N$的值应该为`RAND_MAX + 1`，所以，`rand()/N`可以得到0到1之间的浮点数，再乘以一个数$k$就可以得到$0$到$k$之间的数了。

生成$[a,b]$之间的随机数：

$$
a + \cfrac{ \tt rand()}{N} \times (b-a+1) = a + \cfrac{ \tt rand()}{ \cfrac{N}{(b-a+1)}}
$$

**可悲的是**，写这篇文章时，“C++随机数”关键字的搜索结果中，基本没有提到这些问题，而且很多都是只提到了使用`rand()`来产生随机数（因为有C++，所以更应该使用C++的random库）。笔者自己也后知后觉，瞬间感觉自己**被骗了**很多年。

> 其实多往后边翻几页就能看到了，这里放几个搜索到的**个人博客**的入口：
>
> - [C++11 的随机数 | 拾荒志 (murphypei.github.io)](https://murphypei.github.io/blog/2019/10/cpp-random)
> - [C++随机数 | Sail (sail.name)](https://www.sail.name/2018/08/07/random-number-of-C++/)
> - [C++ 生成随机数 (ccyg.studio)](http://blog.ccyg.studio/article/91e20a65-45d1-49e8-b28a-a33b6ac0f96b/)
>
> 还有微软的文档；如果你到微软的文档看`rand()`，就会发现其实是[不推荐使用](https://docs.microsoft.com/zh-cn/cpp/c-runtime-library/reference/rand?view=msvc-160)的：
>
> - [\<random\> | Microsoft Docs](https://docs.microsoft.com/en-us/cpp/standard-library/random?view=msvc-160)，也有机翻的[中文版](https://docs.microsoft.com/zh-cn/cpp/standard-library/random?view=msvc-160)



## C++的random library 

那么我们用C++该怎么生成随机数呢？

如果你在CLion中使用`rand()`生成随机数，那你可能会收到Clang-d的警告，它会建议使用C++的random number library。况且，用`rand()`确实不好。

可以最后再回来看下面的链接，它们包含更多信息，也有更多实用场景下的例子：

- [How to generate a random number in C++? - Stack Overflow](https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c)：怎样在C++中生成随机数？
- 👉[c++ - Generate random numbers using C++11 random library - Stack Overflow](https://stackoverflow.com/questions/19665818/generate-random-numbers-using-c11-random-library)：使用C++11的random库生成随机数

---

以下内容主要来自[Pseudo-random number generation - cppreference.com][Pseudo-random number generation]：

C++有一个随机数的库，定义在头文件`<random>`中，提供可以生成随机数和伪随机数的类，这些类可以分为两类：

- 均匀随机数生成器（uniform random bit generators, URBGs），包括用来生成伪随机数的随机数生成引擎，以及，真正的随机数生成器，如果有可用的设备的话；
- 随机数分布（random number distributions），将URBGs输出的数据转换到多种多样的统计学分布上（如均匀分布、正态分布等）。

一个URBG是一个函数对象；有好几种随机数引擎，比如线性同余法（linear congruential algorithm）、梅森旋转算法（Mersenne twister algorithm）、时滞斐波那契算法（lagged Fibonacci algorithm），这些引擎使用种子（seed）作为熵源生成伪随机数；也有好几种随机数引擎配接器，它们使用另一个随机数引擎作为自己的熵源，通常用来改变原来引擎的频谱特性（spectral characteristics）。详细的内容可以看上边的[链接][Pseudo-random number generation]。

有一些预定义的随机数生成器，比如`mt19937`，是一个32位的梅森旋转算法（32-bit Mersenne Twister by Matsumoto and Nishimura, 1998）。

也可以用`std::random_device`生成不确定的随机数，也就是真随机数；当没有随机数的发生条件（比如随机数发生器）时，也可以用伪随机数生成引擎实现它。

至于随机数的分布，就又有很多种了，就不一一列举了，还是在上面的[链接][Pseudo-random number generation]里面看。

[Pseudo-random number generation]:https://en.cppreference.com/w/cpp/numeric/random

那具体怎么在代码中使用呢？下面是从[std::uniform_int_distribution - cppreference.com](https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution)复制粘贴的一个生成均匀随机数的例子（模拟掷骰子）：

```cpp
#include <random>
#include <iostream>
 
int main()
{
    std::random_device rd;  //如果可用的话，从一个随机数发生器上获得一个真正的随机数
    std::mt19937 gen(rd()); //gen是一个使用rd()作种子初始化的标准梅森旋转算法的随机数发生器
    std::uniform_int_distribution<> distrib(1, 6);
 
    for (int n=0; n<10; ++n)
        //使用`distrib`将`gen`生成的unsigned int转换到[1, 6]之间的int中
        std::cout << distrib(gen) << ' ';
    std::cout << '\n';
}
```

`std::random_device`会向操作系统请求随机数。

还有一个正态分布的例子，从上面的介绍随机数的[页面][Pseudo-random number generation]复制而来。

```cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
 
int main()
{
    // 如果可用，从一个真实的随机数发生设备获得种子
    std::random_device r;
 
    // 在1和6之间随机的选择一个均值mean，用来生成正态分布
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(1, 6);
    int mean = uniform_dist(e1);
    std::cout << "Randomly-chosen mean: " << mean << '\n';
 
    // 围绕刚刚选择到的“均值”mean生成一个正态分布
    std::seed_seq seed2{r(), r(), r(), r(), r(), r(), r(), r()}; 
    std::mt19937 e2(seed2);
    std::normal_distribution<> normal_dist(mean, 2);
 
    std::map<int, int> hist;
    for (int n = 0; n < 10000; ++n) {
        ++hist[std::round(normal_dist(e2))];
    }
    std::cout << "Normal distribution around " << mean << ":\n";
    for (auto p : hist) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
                  << p.first << ' ' << std::string(p.second/200, '*') << '\n';
    }
}
```

大家可以自己编译运行一下上边的代码，看会输出什么样的结果。

综上，想必大家都已经清楚怎么用C++的方式生成随机数了，更多的使用方法就点击上边的链接吧（看不懂英文？那就快去学！）

