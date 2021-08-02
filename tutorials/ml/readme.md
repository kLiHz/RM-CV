# Machine Learning Overview



[OpenCV: Machine Learning Overview](https://docs.opencv.org/3.4/dc/dd6/ml_intro.html)



## Training Data

在机器学习算法中，有“训练数据”这么一个概念。训练数据由若干部分组成：

- 一组训练样本。每个训练样本都是一个由值组成的向量（a vector of values）（在计算机视觉中有时会用“特征向量（feature vector）”来称呼）。通常，这些向量都有相同数目的组成元素（特征）；OpenCV中的机器学习模块这样假设。每项特征要么能在彼此之间通过比较得出次序（即**能够被排序**），要么**能被归类**（即其值属于某一确定集合，值的类型可以是整数、字符串等等）。
