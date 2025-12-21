# EsWidgets/cmake/EsWidgetsConfig.cmake

# 1. 声明这是一个包配置文件（可选但推荐）
include(CMakeFindDependencyMacro)

# 2. 声明你的库依赖 Qt5（必须和你 find_package(Qt5 ...) 一致！）
find_dependency(Qt5 COMPONENTS Core Gui Widgets Xml Svg REQUIRED)

# 3. 加载实际的库目标（由 install(EXPORT) 自动生成）
include("${CMAKE_CURRENT_LIST_DIR}/EsWidgetsTargets.cmake")