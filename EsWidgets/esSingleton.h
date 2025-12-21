/****************************************************************************
** @brief 快速创建单例模式的宏
** @details 这家伙很懒, 什么都没写
** @file esSingleton.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESSINGLETON_H
#define ESSINGLETON_H
// 声明并实现单例模式, 写在头文件
#define E_SINGLETON_CREATE(Class) \
public:                                  \
static inline Class* getInstance() { \
static Class instance;           \
return &instance;                \
}                                    \
private:                                 \
Q_DISABLE_COPY(Class)

// 声明单例模式-写在头文件
#define E_SINGLETON_CREATE_H(Class) \
public:                             \
static Class* getInstance();    \
private:                            \
Q_DISABLE_COPY(Class)

// 实现单例模式-写在源文件
#define E_SINGLETON_CREATE_CPP(Class) \
Class* Class::getInstance() {         \
static Class instance;            \
return &instance;                 \
};

#endif //ESSINGLETON_H
