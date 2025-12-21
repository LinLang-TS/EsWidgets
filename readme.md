# EsWidgets

一个轻量级的 Fluent Design 风格 Qt C++ 组件库，适用于快速开发美观的桌面应用。

> [!Note]
>
> 本项目大部分参考了 [PyQt-Fluent-Widgets](https://github.com/zhiyiYo/PyQt-Fluent-Widgets) (API设计), 小部分参考了[ElaWidgetTools](https://github.com/Liniyous/ElaWidgetTools) (Pimpl模式), 因此感谢他们的开源



## 为什么做这个项目?

- PyQt-Fluent-Widgets 的 API 非常顺手，但其 C++ 版是付费的 😭; 而（ElaWidgetTools）对我来说要重新学习他的API风格, 不习惯;
- 我希望有一个**免费、开源、可自由修改**的 C++ Fluent UI 库
- 自己写的库，改起来最方便 😋



## 状态说明

- ✅ 已满足日常小工具/脚本的 UI 需求
- 🖥️ 仅在 **Windows + Qt 5.15.2** 下开发测试，未做跨平台\跨版本适配。其他环境可能存在兼容性问题
- 🚧 欢迎 PR！一起维护一个好用的组件库。

> [!Note]
>
> 至于为什么没历史git提交记录, 由于鄙人学历初中没毕业, 没什么水平,  写代码纯个人兴趣爱好, 提交比较混乱, 所以之前的记录就不给大家看了😂;  既然开源了 就重新开个库, 以后规范提交~

## 安装（CLion 构建后示例）

> [!Tip]
>
> 生成器选择`让Cmake决定` 或 `Visual Studio 你安装的vs版本`

```cmake
# 编译安装（Release）
# 1. cd cmake-build-release
# 2. cmake --install ./ --config Release

# 编译安装（Debug）
# 1. cd cmake-build-debug
# 2. cmake --install ./ --config Debug
```



## 使用

```cmake
# 引入项目
find_package(EsWidgets REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Es::EsWidgets)
```

## 贡献

欢迎提交 Issue 或 PR！

- 保持 Fluent Design 风格一致性
- 修复问题请附带复现方式
- 代码格式推荐使用 `clang-format`

## License

Licensed under the Apache License 2.0. See [LICENSE](LICENSE) for details.