# 关于 C++ 继承的理解

继承，可以理解成子类内部**包含**了一个父类、或者说在父类外套了一层，用来扩充父类的功能；

因为要保证类的**封装性**，所以子类也只能访问父类的公有成员、调用父类的公有接口。

这样，子类不能访问父类的私有成员就能够很好理解了。而子类新增的成员，在子类范围中都可见；

因为是“包含”，所以父类也需要初始化，一般在子类的构造函数处用列表初始化的方式初始化父类；

至于继承方式，是用来决定父类的成员是否还对外界可见。



```cpp
#include <iostream>
#include <string>

class Human {
private:
    std::string name;
    int age;
public: 
    Human(const std::string& name_, int age_) : name(name_), age(age_) {}
    std::string self_intro() { 
        return "My name's " + name + ". I'm " + std::to_string(age) + "-year-old. "; 
    }
};

class Teacher : public Human {
private:
    std::string subject;
public: 
    Teacher(const std::string& name_, int age_, std::string && sub_) : 
        Human(name_, age_), 
        subject(sub_) 
    {}
    std::string self_intro() {
        return Human::self_intro() + "I'm a teacher and I teach " + subject + ". ";
    }
};

int main() {
    Teacher Mary("Mary", 25, "History"), Henry("Henry", 27, "Math");
    std::cout << Mary.self_intro() << "\n";
    std::cout << Henry.self_intro() << "\n";
}
```

