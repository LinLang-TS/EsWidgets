cmake_minimum_required(VERSION 3.5)
project(EsWidgets VERSION 0.0.1 LANGUAGES CXX)

# region ======================================== 编译文件 ========================================

set(esPublicHeaders
        # 全局宏和枚举
        EsWidgets/esSingleton.h
        EsWidgets/esGlobal.h
        EsWidgets/esDef.h
        # common
        EsWidgets/common/esColor.h
        EsWidgets/common/esConfig.h
        EsWidgets/common/esFont.h
        EsWidgets/common/esIcon.h
        EsWidgets/common/esSmoothScroll.h
        EsWidgets/common/esStyleSheet.h
        EsWidgets/common/esTextWrap.h
        EsWidgets/common/esUtils.h
        EsWidgets/common/esAnimation.h
        EsWidgets/common/esRouter.h
        EsWidgets/common/esScreen.h

        # esWidgets组件
        # button
        EsWidgets/components/button/esPushButton.h
        EsWidgets/components/button/esPrimaryPushButton.h
        EsWidgets/components/button/esTransparentPushButton.h
        EsWidgets/components/button/esSwitchButton.h
        EsWidgets/components/button/esToggleButton.h
        EsWidgets/components/button/esTogglePushButton.h
        EsWidgets/components/button/esTransparentTogglePushButton.h
        EsWidgets/components/button/esToolButton.h
        EsWidgets/components/button/esTransparentToolButton.h
        # delegate
        EsWidgets/components/delegate/esSmoothScrollDelegate.h
        EsWidgets/components/delegate/esTableItemDelegate.h
        EsWidgets/components/delegate/esListItemDelegate.h
        EsWidgets/components/delegate/esMenuItemDelegate.h
        EsWidgets/components/delegate/esIndicatorMenuItemDelegate.h
        # subButton
        EsWidgets/components/subButton/esArrowButton.h
        EsWidgets/components/subButton/esIndicator.h
        EsWidgets/components/subButton/esLineEditButton.h
        # widget
        EsWidgets/components/widgets/esCheckBox.h
        EsWidgets/components/widgets/esInfoBar.h
        EsWidgets/components/widgets/esLineEdit.h
        EsWidgets/components/widgets/esScrollBar.h
        EsWidgets/components/widgets/esTableWidget.h
        EsWidgets/components/widgets/esTableView.h
        EsWidgets/components/widgets/esWidget.h
        EsWidgets/components/widgets/esListWidget.h
        EsWidgets/components/widgets/esListView.h
        EsWidgets/components/widgets/esOpacityAniStackedWidget.h
        EsWidgets/components/widgets/esPopUpAniStackedWidget.h
        EsWidgets/components/widgets/esComboBox.h
        EsWidgets/components/widgets/esScrollArea.h
        EsWidgets/components/widgets/esTableWidgetComboBoxItem.h

        # window
        EsWidgets/components/windows/esStackedWidget.h
        EsWidgets/components/windows/esWindowBase.h

        # es无边框窗口
        EsWidgets/framelessWindow/esFramelessWindow.h
        EsWidgets/framelessWindow/esTitleBar.h
        EsWidgets/framelessWindow/esTitleBarButtons.h
        EsWidgets/framelessWindow/esWin32Utils.h
        EsWidgets/framelessWindow/esWindowsWindowEffect.h

)

set(esSources
        EsWidgets/_rc/_esResource.qrc # qrc文件不能同名, 不同目录也不能同名, 会覆盖
        # common
        EsWidgets/common/esColor.cpp
        EsWidgets/common/esConfig.cpp
        EsWidgets/common/esFont.cpp
        EsWidgets/common/esIcon.cpp
        EsWidgets/common/esSmoothScroll.cpp
        EsWidgets/common/esStyleSheet.cpp
        EsWidgets/common/esTextWrap.cpp
        EsWidgets/common/esUtils.cpp
        EsWidgets/common/esAnimation.cpp
        EsWidgets/common/esRouter.cpp
        EsWidgets/common/esScreen.cpp

        # esWidgets组件
        # button
        EsWidgets/components/button/esPushButton.cpp
        EsWidgets/components/button/esPrimaryPushButton.cpp
        EsWidgets/components/button/esTransparentPushButton.cpp
        EsWidgets/components/button/esSwitchButton.cpp
        EsWidgets/components/button/esToggleButton.cpp
        EsWidgets/components/button/esTogglePushButton.cpp
        EsWidgets/components/button/esTransparentTogglePushButton.cpp
        EsWidgets/components/button/esToolButton.cpp
        EsWidgets/components/button/esTransparentToolButton.cpp
        # delegate
        EsWidgets/components/delegate/esSmoothScrollDelegate.cpp
        EsWidgets/components/delegate/esTableItemDelegate.cpp
        EsWidgets/components/delegate/esListItemDelegate.cpp
        EsWidgets/components/delegate/esMenuItemDelegate.cpp
        EsWidgets/components/delegate/esIndicatorMenuItemDelegate.cpp
        # subButton
        EsWidgets/components/subButton/esArrowButton.cpp
        EsWidgets/components/subButton/esIndicator.cpp
        EsWidgets/components/subButton/esLineEditButton.cpp
        # widget
        EsWidgets/components/widgets/esCheckBox.cpp
        EsWidgets/components/widgets/esInfoBar.cpp
        EsWidgets/components/widgets/esLineEdit.cpp
        EsWidgets/components/widgets/esScrollBar.cpp
        EsWidgets/components/widgets/esTableWidget.cpp
        EsWidgets/components/widgets/esTableView.cpp
        EsWidgets/components/widgets/esWidget.cpp
        EsWidgets/components/widgets/esListWidget.cpp
        EsWidgets/components/widgets/esListView.cpp
        EsWidgets/components/widgets/esOpacityAniStackedWidget.cpp
        EsWidgets/components/widgets/esPopUpAniStackedWidget.cpp
        EsWidgets/components/windows/esWindowBase.cpp
        EsWidgets/components/windows/esStackedWidget.cpp
        EsWidgets/components/widgets/esMenuActionListWidget.cpp
        EsWidgets/components/widgets/esRoundMenu.cpp
        EsWidgets/components/widgets/esMenuActionListWidget.h
        EsWidgets/components/widgets/esRoundMenu.h
        EsWidgets/components/widgets/esComboBox.cpp
        EsWidgets/components/widgets/esScrollArea.cpp
        # esWidgets的私有头
        EsWidgets/private/esArrowButton_p.h
        EsWidgets/private/esCheckBox_p.h
        EsWidgets/private/esIndicator_p.h
        EsWidgets/private/esInfoBar_p.h
        EsWidgets/private/esLinedit_p.h
        EsWidgets/private/esLineEditButton_p.h
        EsWidgets/private/esPushButton_p.h
        EsWidgets/private/esScrollBar_p.h
        EsWidgets/private/esSwitchButton_p.h
        EsWidgets/private/esTableView_p.h
        EsWidgets/private/esTableWidget_p.h
        EsWidgets/private/esToolButton_p.h
        EsWidgets/private/esWidget_p.h
        EsWidgets/private/esListWidget_p.h
        EsWidgets/private/esListView_p.h
        EsWidgets/private/esWindowBase_p.h
        EsWidgets/private/esStackedWidget_p.h
        EsWidgets/private/esOpacityAniStackedWidget_p.h
        EsWidgets/private/esPopUpAniStackedWidget_p.h
        EsWidgets/private/esRoundMenu_p.h
        EsWidgets/private/esMenuActionListWidget_p.h
        EsWidgets/private/esComboBox_p.h
        # es无边框窗口
        EsWidgets/framelessWindow/esFramelessWindow_p.h
        EsWidgets/framelessWindow/esTitleBar_p.h
        EsWidgets/framelessWindow/esTitleBarButtons_p.h
        EsWidgets/framelessWindow/esFramelessWindow.cpp
        EsWidgets/framelessWindow/esTitleBar.cpp
        EsWidgets/framelessWindow/esTitleButtons.cpp
        EsWidgets/framelessWindow/esWin32Utils.cpp
        EsWidgets/framelessWindow/esWindowsWindowEffect.cpp
        EsWidgets/components/widgets/esTableWidgetComboBoxItem.cpp

)


# endregion


# region ======================================== 常规 ========================================

# Qt 自动生成Moc, 自动生成RCC, 自动生成UIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#### ------------------------------ 使用utf8编译 ------------------------------
option(utf8Enable "是否启用utf8编译" ON)

if (utf8Enable)
    #    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

#### ------------------------------ C/C++标准 ------------------------------
#set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

#### ------------------------------ 编译类型 ------------------------------
# 如果编译静态库, 需要把 _rc 目录拷贝给用户, 然后在 add_executable() 里添加 .qrc 的文件;
# option会缓存, 所以更改后要把构建目录删掉重新生成
option(isStaticBuild "是否静态编译(默认为否, 编译动态库)" ON)

if (isStaticBuild)
    set(libType "STATIC")
else ()
    set(libType "SHARED")
endif ()


#add_library(${PROJECT_NAME} ${libType}
#        ${esPublicHeaders}
#        ${esSources}
#)
# 测试源码编译Gui(自己测试用的, 没规范写示例, 凑合看...)
add_executable(${PROJECT_NAME}
        ${esPublicHeaders}
        ${esSources}
        examples/main.cpp
)

#### ------------------------------ 编译后的文件配置 ------------------------------
######## 输出目录
set(outputDir ${CMAKE_BINARY_DIR})
######## 输出文件名(不带后缀)
set(outputFileName ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${outputDir}
        LIBRARY_OUTPUT_DIRECTORY ${outputDir}
        RUNTIME_OUTPUT_DIRECTORY ${outputDir}
        DEBUG_POSTFIX "d"                               # 在 Debug 构建配置下自动给目标文件添加后缀
        OUTPUT_NAME ${outputFileName}
)

# endregion


# region ======================================== C/C++ ========================================

#### ------------------------------ 附加包含目录 ------------------------------


# 在 add_library(...) 之后添加
target_include_directories(${PROJECT_NAME}
        PUBLIC
        # 构建时（开发本项目）的头文件路径
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EsWidgets>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EsWidgets/common>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EsWidgets/components>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/EsWidgets/framelessWindow>"
        #        "C:\\Program Files (x86)\\Visual Leak Detector\\include"
        # 安装后，用户使用的头文件路径（相对于 install prefix）
        $<INSTALL_INTERFACE:include>
)
#### ------------------------------ 运行库 ------------------------------
if (isStaticBuild)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif ()


# endregion


# region ======================================== 链接器 ========================================
#### ------------------------------ 查找模块首选目录 ------------------------------
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")  # 动态库
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/vs2019_static_x64")        # 静态库

#### ------------------------------ 查找模块 ------------------------------
find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Xml
        Svg
        REQUIRED)

#### ------------------------------ 附加库包含目录 ------------------------------
target_link_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib") # 第三方链接库的路径
target_link_directories(${PROJECT_NAME} PRIVATE "C:\\Program Files (x86)\\Visual Leak Detector\\lib\\Win64") # 第三方链接库的路径

#### ------------------------------ 链接库 ------------------------------
target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Xml
        Qt5::Svg
        dwmapi
#        vld  # debug找内存泄露用的
)

# endregion

# region ======================================== 安装 ========================================

# 生成首字母大写的转发头文件
function(generateForwardHeaders HEADER_LIST OUTPUT_DIR)
    foreach (header ${HEADER_LIST})
        # 获取文件名
        get_filename_component(filename "${header}" NAME_WE)  # NAME_WE = name without extension
        get_filename_component(ext "${header}" LAST_EXT)      # 通常是 .h

        # 首字母大写
        string(SUBSTRING "${filename}" 0 1 first_char)
        string(SUBSTRING "${filename}" 1 -1 rest_chars)
        string(TOUPPER "${first_char}" first_upper)
        set(upper_name "${first_upper}${rest_chars}")

        # 生成内容
        set(wrapper_content "#include \"${filename}${ext}\"\n")

        # 写入到构建目录
        set(wrapper_file "${CMAKE_CURRENT_BINARY_DIR}/generated_headers/${upper_name}")
        file(WRITE "${wrapper_file}" "${wrapper_content}")

        # 将生成的文件加入安装列表
        install(FILES "${wrapper_file}"
                DESTINATION "${OUTPUT_DIR}")
    endforeach ()
endfunction()


# 安装库
install(TARGETS ${PROJECT_NAME}
        EXPORT EsWidgetsTargets
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

# 安装头文件
install(FILES ${esPublicHeaders}
        DESTINATION include
)
# 生成并安装首字母大写版本
generateForwardHeaders("${esPublicHeaders}" "include")

# 安装 CMake 配置文件（让 find_package 工作）
install(EXPORT EsWidgetsTargets
        FILE EsWidgetsTargets.cmake
        NAMESPACE Es::
        DESTINATION cmake
)

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EsWidgetsConfig.cmake"
        DESTINATION cmake
)

# endregion

# 安装教程(这里我用的 Clion )
# 命令默认会安装到 "C:\Program Files\${PROJECT_NAME}"目录; 可以加上 --prefix ./install 安装到构建目录的 install 目录或其他自定义目录

# 编译Release版
# 1. cd cmake-build-release
# 2. & "C:\Program Files\JetBrains\CLion 2025.2.1\bin\cmake\win\x64\bin\cmake.exe" --install ./ --config Release

# 编译Debug版
# 1. cd cmake-build-debug
# 2. & "C:\Program Files\JetBrains\CLion 2025.2.1\bin\cmake\win\x64\bin\cmake.exe" --install ./ --config Debug

# 引用教程
# find_package(Qt5 COMPONENTS Core Gui Widgets Xml Svg REQUIRED)
# find_package(EsWidgets REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE Es::EsWidgets)
