# cv::Mat

> 参考链接：
>
> - [OpenCV: cv::Mat Class Reference](https://docs.opencv.org/4.5.1/d3/d63/classcv_1_1Mat.html)

## 简介

OpenCV的`Mat`类可以表示一个N维的数组，数组的每个位置可以是单通道或多通道。可以用来存储图片（灰度或彩色）、向量、直方图等很多东西。

我们把一个存储在`Mat`中的数组记作`M`，其中的数据分布由`M.step[]`定义。`M.step[n]`可以理解为数组`M`第`n`维数据在内存中的步长（step或stride），比如二维数组的一个位于`(i,j)`处的数据的地址可以用下面的式子表示：

$$
\tt{addr(M_{i,j})=M.data+M.step[0]*i+M.step[1]*j}
$$

由此可见，二维数组是逐行存储的，而三维数组是逐面（plane）存储（一个二维数组挨着一个，一个二维数组可以理解成一个面）。

## 创建 Mat 对象

### 1. 创建空白 Mat 对象

#### 创建二维数组

要创建一个`Mat`对象，**最常见的**便是使用`create(nrows, ncols, type)`方法或者与之相似的构造函数`Mat(nrows, ncols, type[, fillValue])`。

顾名思义，`nrows`表示二维数组的行数，`ncols`表示二维的列数，`type`表示数据类型，`fillValue`表示用来填充数组的值。

简单地说，就是“**数组大小** + **数据类型** + **填充值**（可选）”。

- 数组大小除了用两个`int`值（**先行后列**）表示；也可以用`cv::Size`表示，但要注意`Size`的构造函数是**先列后行**；

- 数据类型`type`的取值可以是像`CV_8UC1`、`CV_32FC2`这样的值，其中`U`前的数字表示数据的位数，比如`8U`表示8位无符号整型数（`unsigned char`），`32F`表示32位浮点数（`float`）；`C`以及其后的数字表示通道（Channel）数，比如`C3`就表示有3个通道。
- 填充值用`cv::Scalar`类型表示，只写一个数会被自动类型转换；预填充值会调用赋值运算符对`Scalar`类型的重载：`Mat::operator=(const Scalar& value)`

代码示例：

```cpp
using cv::Point, cv::Rect, cv::Size, cv::Scalar; // C++17

cv::Mat M(cv::Size(100,100), CV_8UC1, 255);
M.create(500,500,CV_32FC1);
M = 0.5;

cv::Mat white = cv::Mat(Size(100,100), CV_8UC3, Scalar(255,255,255));
cv::Mat black = cv::Mat(Size(100,100), CV_8UC3);
black = Scalar(0,0,0);
```

注意，`create`成员方法只有当大小或者类型与当前数组不同时，才会分配新的空间。

> `type`值实际上也是`int`类型；
>
> 也有其他表示`type`的方法，比如利用`cv::traits::Type<>`，由于不常用到，时不展开介绍（补充链接：[cv::DataType<> versus cv::traits::Type<> - Stack Overflow](https://stackoverflow.com/questions/48858328/cvdatatype-versus-cvtraitstype)）。

#### 创建多维数组

构造函数原型是这样的：

```cpp
Mat(int ndims, const int* sizes, int type);
```

`ndims`表示数组有几维；`sizes`是一个数组，描述一个$n$维数组的形状（也就是每个维度的长度）；`type`表示数据类型，同前所述。

```cpp
Mat(const std::vector<int>& sizes, int type);	
```

用一个`std::vector`对象描述数组的各个维度，由于`vector`对象本身记录数据的个数，因此不再需要`ndims`参数。

以上两个构造函数也都可以在构造时指定预填充值。



### 2. 创建与其他对象共享数据的Mat

当使用拷贝构造函数或者赋值运算符时，只会拷贝等号右端`Mat`对象的**头信息**（Header），并且增加这块空间的引用计数，因此该生成`Mat`对象的操作是$O(1)$的；换句话说，这时的两个对象共享一块空间，也就是所谓的**浅拷贝**（shallow copy）。如果需要**深拷贝**（full/deep copy），则需要使用`clone()`方法，如`M2 = M1.clone();`。

此外，还可以生成头信息指向**其他数组的一部分**的`Mat`，可以是一（些）行、一（些）列、或者是一块矩形区域。新生成的头信息**共享**原数组的数据，因此耗时仍然是$O(1)$的。

可以利用这项特性**使用**或**修改**一个数组的部分区域。

`row()`方法可以生成指向某一行的`Mat`；同理，`col()`方法可以生成指向某一列的`Mat`。

```cpp
M.row(3) = M.row(3) + M.row(5) * 3; // 将第5行乘以3倍，加在第3行上
```

要将**某一行**（列）的值设置为**另一行**（列）的值，直接使用`M.row(1) = M.row(2)`这样的表达式是不行的，需要使用`copyTo`方法：

```cpp
// M.col(1) = M.col(7) // 这样做是不行的
Mat M1 = M.col(1);
M.col(7).copyTo(M1);
```

若要表示一些行（列），使用`Range()`类型来表示：

```cpp
Mat M1(M, Range(2,5), Range(3,6));
```

如果第二个参数留空，则采用默认值`Range::all()`。

矩形区域也是可以的，我们可以用`Rect`类描述一块“**感兴趣区域**（**ROI**, Region of Interest）”，然后使用`Mat`类相应的构造函数生成`Mat`对象。

```cpp
Mat img(Size(320,320), CV_8UC3);
Mat roi(img, Rect(10,10,100,100));
roi = Scalar(0,255,0);
```

经过上面的操作，我们将`roi`对象在`img`上“圈”出的区域填充为了`(0,255,0)`。

还可以使用`Mat`类对`operator()`的重载，写法更方便。

```cpp
#include <opencv2/opencv.hpp>

int main() {
    using cv::Point, cv::Rect, cv::Size; // 逗号分隔是C++17的标准
    cv::Mat white = cv::Mat(Size(100,100), CV_8U, 255);
    cv::Mat black = cv::Mat::zeros(Size(100,100), CV_8U);
    auto area = Rect(Point(10,10), Point(30,30));
    black(area).copyTo(white(area));
    cv::namedWindow("black", cv::WINDOW_FREERATIO);
    cv::namedWindow("white", cv::WINDOW_FREERATIO);
    cv::imshow("black", black);
    cv::imshow("white", white);
    cv::waitKey(0);
}
```



> :memo: **备注**：
>
> 对`cv::Rect`进行`&`操作不仅可以取交集，还可以用来确保某一个`Rect`在`Mat`内部，如：
>
> ````cpp
> rect = rect & cv::Rect(cv::Point(0,0), M.size());
> ````
>
> **参考链接**：[copy small part of mat to another - OpenCV Q&A Forum](https://answers.opencv.org/question/79889/copy-small-part-of-mat-to-another/)

### 3. 特殊矩阵

`Mat`对象有`zeros()`、`ones()`、`eyes()`三个静态成员方法，可以生成全1、全0以及单位矩阵。

```cpp
// 创建一个精度为 double（双精度）的单位矩阵，并加在 M 上
M += Mat::eye(M.rows, M.cols, CV_64F);
```

### 4. 逗号初始化

```cpp
cv::Mat M = (cv::Mat_<double>(3,3) << 1, 0, 0, 0, 1, 0, 0, 0, 1);
```

可能会好奇为什么`<<`运算符后边跟了一串逗号表达式，但依然能够正常工作，因为在我们的印象中，逗号表达式的值取决于第一个元素。

其实，OpenCV 在这里同时重载了`<<`和`,`这两个运算符，以便实现这种写法；关于具体的参数和返回值类型暂时不多作介绍，可以利用 IDE 查看函数的参数和返回值类型。

这种写法只适于这个场景，在使用`cout`的时候，`<<`和`,`搭配默认是并不会有这种效果的。

### 5. 创建对外部数据的包装（Wrapper）

可以创建一个`Mat`来对一块外部数据进行包装，这多用于接收摄像机或者其他应用程序传来的数据的场景。需要指定数组的**大小**、数据的**类型**、**数据地址**、以及可选的**步长**（step）。

```cpp
double data[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
cv::Mat M(3, 3, CV_64F, data);
```

注意，因为`data`不是`Mat`对象管理的数据，所以`Mat`对象被销毁时，`data`不会被释放。

```cpp
#include <opencv2/opencv.hpp>

auto MatWrapper(const unsigned char * addr, int rown, int coln, int step) {
    cv::Mat M(rown, coln, CV_64F, (unsigned char*)addr, step);
    return M;
}

int main() {
    double data[] = {0.1, 0.2, 0.3, -1,  0.4, 0.5, 0.6, -1, 0.7, 0.8, 0.9, -1};
    auto M = MatWrapper((unsigned char*)data, 3, 3, (3+1)*8);
    cv::namedWindow("M", cv::WINDOW_FREERATIO);
    cv::imshow("M", M);
    cv::waitKey(0);
}
```

参数`step`表示的是每一行占用的的**字节数**，包括用来标记每一行末尾的字节（如果有的话）；如不指定`step`（默认值为`AUTO_STEP`），则认为没有padding bytes，而`step`将通过`cols * elemSize()`计算。

针对多维数组的包装与创建普通数组类似，只不过`step`变成了表示n-1个维度的`step`的数组。

## 访问Mat对象中的元素

上一个章节已经介绍了`Mat`对象在内存中是如何存储的，并且有一个计算元素地址的公式，但是一般情况下我们不需要在代码中直接使用该公式。

在知道`Mat`中数据类型的情况下，对于一个2维数组的`(i,j)`号元素，可以用`at`方法访问；假设`M`是一个双精度浮点数数组：

```cpp
M.at<double>(i,j) += 1.0F;
```

注意，为了更高的性能，`at`方法只在编译选项为“调试（Debug）”时才会进行越界判断。

对于只有一行或一列的数组，`at`方法可以只接收一个参数。

`at`方法的类型标识符不能随意选择，需要根据`Mat`对象的类型来决定：

| Matrix Type | `Mat::at<T>`   |
| :---------- | :------------- |
| `CV_8U`     | `M.at<uchar>`  |
| `CV_8S`     | `M.at<schar>`  |
| `CV_16U`    | `M.at<ushort>` |
| `CV_16S`    | `M.at<short>`  |
| `CV_32S`    | `M.at<int>`    |
| `CV_32F`    | `M.at<float>`  |
| `CV_64F`    | `M.at<double>` |

如果一次要处理一整行，最高效的方法是先取得指向行首的指针，之后使用C语言中的`[]`运算符访问相应的元素：

```cpp
// 计算矩阵中正值的和（假设M为双精度矩阵）
double sum = 0;
for (int i = 0; i < M.rows; ++i) {
    const double* Mi = M.ptr<double>(i);
    for (int j = 0; j < M.cols; ++j) {
        sum += std::max(Mi[j], 0.);
    }
}
```

有些操作实际上并不取决于数组的形状或大小。比如**逐个**处理数组中的元素（比如上面这个），或者是处理不同数组中**坐标相同**的元素（比如数组加法）。在这之前，有必要确保输入/输出数组是连续的，也就是说每一行末没有间隔。如果是这样的话，那么把他们当作一个很长的单行来处理。

```cpp
// 计算矩阵中正值的和，优化版本
int cols = M.cols, rows = M.rows;
if (M.isContinuous()) { cols *= rows; rows = 1; }

double sum=0;
for (int i = 0; i < rows; ++i) {
    const double* Mi = M.ptr<double>(i);
    for(int j = 0; j < cols; ++j) {
        sum += std::max(Mi[j], 0.);
    }
}
```

上面的这种写法，在`M`是连续存储的矩阵时，外层循环体只会被执行一次，这样的开销会更小，当操作的矩阵比较小的时候尤为明显。

最后，`Mat`类所提供的STL风格的迭代器足够聪明，能够跳过相邻两行之间的间隔。

```cpp
double sum=0;
MatConstIterator_<double> 
    it     = M.begin<double>(), 
    it_end = M.end<double>()
;
for(; it != it_end; ++it) sum += std::max(*it, 0.);
```

这些迭代器支持随机访问，因此也适用于一切STL算法，包括`std::sort()`。



那么，如果是**多通道的图像**，该如何访问呢？

当然还是可以逐行访问，并通过计算下标，用`[]`运算符取得相应元素的地址；比如3通道的图像，那么某一行第`j`个（从0开始）像素的下标就是`3 * j`，也是第0通道的下标；同理，第1通道、第2通道的下标为`3 * j + 1`、`3 * j + 2`；也就是说，n通道的图像每行有`n * cols`个连续存储的值。

如果觉得这样麻烦，可以用`cv::Vec<T, n>`这个类型来表示一个“像素”，还搭配迭代器使用，如：

```cpp
typedef cv::Vec<uchar, 3> VecUc3; 
// 或者: using VecUc3 = cv::Vec<uchar, 3>;

img.at<VecUc3>(x,y)[0] = 64;
img.at<VecUc3>(x,y)[1] = 128;

auto it = img.begin<VecUc3>(), it_end = img.end<VecUc3>();
for(; it != it_end; ++it) {
    VecUc pix = *it;
    std::cout << std::max(pix[0], pix[1], pix[2]) << ' ';
}
```

实际上，针对 `Vec<uchar, 3>`，OpenCV已经有一个定义的别名叫做`Vec3b`可以使用。

## Mat使用

`Mat`还可以像STL容器一样支持`push_back`。

未完待续……



## Mat存储图像

未完待续……





---

**扩展阅读**：

- [OpenCV: cv::MatExpr Class Reference](https://docs.opencv.org/4.5.1/d1/d10/classcv_1_1MatExpr.html)

- `CV_MAKETYPE` in [OpenCV: Interface](https://docs.opencv.org/4.5.1/d1/d1b/group__core__hal__interface.html)

- [OpenCV: cv::Vec< _Tp, cn > Class Template Reference](https://docs.opencv.org/4.5.1/d6/dcf/classcv_1_1Vec.html)

