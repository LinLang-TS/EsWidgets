/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 宏解释:
**              E_Q_XXXX 和 E_D_XXX
**              - E是项目作用域
**              - Q是写在公共类里的
**              - D是写在私有类里的
** @file EsGlobal.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESGLOBAL_H
#define ESGLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>

//#define ES_COMPILE_DLL  // 编译Es dll
#define ES_COMPILE_EXE  // 用源码编译Es lib或exe
//#define ES_USE_DLL  // 自己使用Es的dll

#if defined(ES_COMPILE_DLL)
#define ES_EXPORT Q_DECL_EXPORT
#elif defined(ES_USE_DLL)
#define ES_EXPORT Q_DECL_IMPORT
#elif defined(ES_COMPILE_EXE)
#define ES_EXPORT
#endif


// 头文件里声明get和set方法
#define E_Q_CREATE_GET_SET_H(TYPE, T_funcName) \
public:                               \
    void set##T_funcName(TYPE);              \
    TYPE get##T_funcName() const;

// 源文件里实现get和set方法
#define E_Q_CREATE_GET_SET_CPP(CLASS, TYPE, T_funcName, T_paramName)    \
void CLASS::set##T_funcName(TYPE T_paramName)                           \
{                                                                       \
    d_ptr->_##T_paramName = T_paramName;                                \
}                                                                       \
TYPE CLASS::get##T_funcName() const                                     \
{                                                                       \
    return d_ptr->_##T_paramName;                                       \
}
// ==================


// 源文件里实现get和set方法, set方法设置后加update()
#define E_Q_CREATE_GET_SET_UPDATE_CPP(CLASS, TYPE, T_funcName, T_paramName)   \
void CLASS::set##T_funcName(const TYPE &T_paramName)                           \
{                                                                       \
    Q_D(CLASS);                                                          \
    d->T_paramName = T_paramName;                                   \
    update();                                                           \
}                                                                       \
TYPE CLASS::get##T_funcName() const                                     \
{                                                                       \
    return d_ptr->T_paramName;                                         \
}
// ==================



// 创建pimpl-公共类(给用户用的)
#define E_Q_CREATE_PIMPL(CLASS)                                        \
protected:                                                       \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr); \
    QScopedPointer<CLASS##Private> d_ptr;                        \
                                                                 \
private:                                                         \
    Q_DISABLE_COPY(CLASS)                                        \
    Q_DECLARE_PRIVATE(CLASS);

// 创建pimpl-私有类(xxxPrivate类里用的)
#define E_D_CREATE_PIMPL(CLASS) \
protected:                \
    CLASS* q_ptr;         \
                          \
private:                  \
    Q_DECLARE_PUBLIC(CLASS);



#endif //ESGLOBAL_H
