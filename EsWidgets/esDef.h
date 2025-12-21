/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esDef.h
** @date 2025/10/1
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ESDEF_H
#define ESDEF_H
#include "esGlobal.h"
#include <QDebug>


//枚举类导出  兼容QT5低版本;
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)

// 创建枚举类型-开始
#define E_BEGIN_ENUM_CREATE(CLASS) \
namespace CLASS                \
{                              \
Q_NAMESPACE_EXPORT(ES_EXPORT)

// 创建枚举类型-结束
#define E_END_ENUM_CREATE(CLASS) }

// 绑定枚举到Qt元对象系统里
#define E_ENUM_CREATE(CLASS) Q_ENUM_NS(CLASS)


#else
#define E_BEGIN_ENUM_CREATE(CLASS)          \
class ES_EXPORT CLASS : public QObject \
{                                       \
Q_OBJECT                            \
public:

#define E_END_ENUM_CREATE(CLASS) \
private:                         \
Q_DISABLE_COPY(CLASS)        \
}                            \
;

#define E_ENUM_CREATE(CLASS) Q_ENUM(CLASS)
#endif

/**
 * 枚举命名规则:
 *      - 需要把枚举转换为字符串使用的:
 *          前缀_值 (内部使用时会处理识别第一个 `_` 下划线, 切割取后面的值)
 *      - 不需要枚举转字符串的:
 *          enum class 枚举名{值, ...}
 */
E_BEGIN_ENUM_CREATE(Es)

    /**
     * 自定义Icon:
     * 案例一:
     *      1. 在我的qrc路径下添加你自己的Icon, 然后添加对应文件名的自定义枚举值;
     * 案例二:
     *      1. 替换我的Icon文件, 改为同名文件即可
     * 案例三:
     *      1. 继续往这个枚举里添加对应文件名的自定义枚举值;
     *      2. 继承 EsIconBase 实现 CustomIcon 类, 重写 path() 返回 qrc路径 或 绝对路径(此时path的参数无效, 随便传一个);
     *      3. 给组件设置Icon时,传递 CustomIcon*
     *
     * tip: 枚举值里第一个`_`之前的字符可以自定义, 如: MyIcon_IconFileName 会自动切割IconFileName
     */
    enum IconName
    {
        Icon_Up,
        Icon_Add,
        Icon_Bus,
        Icon_Car,
        Icon_Cut,
        Icon_IOT,
        Icon_Pin,
        Icon_Tag,
        Icon_VPN,
        Icon_Cafe,
        Icon_Chat,
        Icon_Copy,
        Icon_Code,
        Icon_Down,
        Icon_Edit,
        Icon_Flag,
        Icon_Font,
        Icon_Game,
        Icon_Help,
        Icon_Hide,
        Icon_Home,
        Icon_Info,
        Icon_Leaf,
        Icon_Link,
        Icon_Mail,
        Icon_Menu,
        Icon_Mute,
        Icon_More,
        Icon_Move,
        Icon_Play,
        Icon_Save,
        Icon_Send,
        Icon_Sync,
        Icon_Unit,
        Icon_View,
        Icon_Wifi,
        Icon_Zoom,
        Icon_Album,
        Icon_Brush,
        Icon_Broom,
        Icon_Close,
        Icon_Cloud,
        Icon_Embed,
        Icon_Globe,
        Icon_Heart,
        Icon_Label,
        Icon_Media,
        Icon_Movie,
        Icon_Music,
        Icon_Robot,
        Icon_Pause,
        Icon_Paste,
        Icon_Photo,
        Icon_Phone,
        Icon_Print,
        Icon_Share,
        Icon_Tiles,
        Icon_Unpin,
        Icon_Video,
        Icon_Train,
        Icon_AddTo,
        Icon_Accept,
        Icon_Camera,
        Icon_Cancel,
        Icon_Delete,
        Icon_Folder,
        Icon_Filter,
        Icon_Market,
        Icon_Scroll,
        Icon_Layout,
        Icon_GitHub,
        Icon_Update,
        Icon_Remove,
        Icon_Return,
        Icon_People,
        Icon_QRCode,
        Icon_Ringer,
        Icon_Rotate,
        Icon_Search,
        Icon_Volume,
        Icon_Frigid,
        Icon_SaveAs,
        Icon_ZoomIn,
        Icon_Connect,
        Icon_History,
        Icon_Setting,
        Icon_Palette,
        Icon_Message,
        Icon_FitPage,
        Icon_ZoomOut,
        Icon_Airplane,
        Icon_Asterisk,
        Icon_Calories,
        Icon_Calendar,
        Icon_Feedback,
        Icon_BookShelf,
        Icon_Minimize,
        Icon_CheckBox,
        Icon_Document,
        Icon_Language,
        Icon_Download,
        Icon_Question,
        Icon_Speakers,
        Icon_DateTime,
        Icon_FontSize,
        Icon_HomeFill,
        Icon_PageLeft,
        Icon_SaveCopy,
        Icon_SendFill,
        Icon_SkipBack,
        Icon_SpeedOff,
        Icon_Alignment,
        Icon_Bluetooth,
        Icon_Completed,
        Icon_Constract,
        Icon_Headphone,
        Icon_Megaphone,
        Icon_Projector,
        Icon_Education,
        Icon_LeftArrow,
        Icon_EraseTool,
        Icon_PageRight,
        Icon_PlaySolid,
        Icon_Highlight,
        Icon_FolderAdd,
        Icon_PauseBold,
        Icon_PencilInk,
        Icon_PieSingle,
        Icon_QuickNote,
        Icon_SpeedHigh,
        Icon_StopWatch,
        Icon_ZipFolder,
        Icon_Basketball,
        Icon_Brightness,
        Icon_Dictionary,
        Icon_Microphone,
        Icon_ChevronDown,
        Icon_FullScreen,
        Icon_MixVolumes,
        Icon_RemoveFrom,
        Icon_RightArrow,
        Icon_QuietHours,
        Icon_Fingerprint,
        Icon_Application,
        Icon_Certificate,
        Icon_Transparent,
        Icon_ImageExport,
        Icon_SpeedMedium,
        Icon_LibraryFill,
        Icon_MusicFolder,
        Icon_PowerButton,
        Icon_SkipForward,
        Icon_CareUpSolid,
        Icon_AcceptMedium,
        Icon_CancelMedium,
        Icon_ChevronRight,
        Icon_ClippingTool,
        Icon_SearchMirror,
        Icon_ShoppingCart,
        Icon_FontIncrease,
        Icon_BackToWindow,
        Icon_CommandPrompt,
        Icon_CloudDownload,
        Icon_DictionaryAdd,
        Icon_CareDownSolid,
        Icon_CareLeftSolid,
        Icon_ClearSelection,
        Icon_DeveloperTools,
        Icon_BackgroundColor,
        Icon_CareRightSolid,
        Icon_ChevronDownMed,
        Icon_ChevronRightMed,
        Icon_EmojiTabSymbols,
        Icon_ExpressiveInputEntry,
        // CheckBoxIcon
        IconCheckBox_Accept,
        IconCheckBox_PartialAccept,
        // InfoBarIcon
        IconInfoBar_Info,
        IconInfoBar_Success,
        IconInfoBar_Warning,
        IconInfoBar_Error
    };

    E_ENUM_CREATE(IconName)

    enum Theme
    {
        Theme_LIGHT, // = "Light"
        Theme_DARK, // = "Dark"
        Theme_AUTO //= "Auto"
    };

    E_ENUM_CREATE(Theme)


    enum StyleSheet
    {
        Style_MENU,
        Style_LABEL,
        Style_PIVOT,
        Style_BUTTON,
        Style_DIALOG,
        Style_SLIDER,
        Style_INFO_BAR,
        Style_SPIN_BOX,
        Style_TAB_VIEW,
        Style_TOOL_TIP,
        Style_CHECK_BOX,
        Style_COMBO_BOX,
        Style_FLIP_VIEW,
        Style_LINE_EDIT,
        Style_LIST_VIEW,
        Style_TREE_VIEW,
        Style_INFO_BADGE,
        Style_PIPS_PAGER,
        Style_TABLE_VIEW,
        Style_CARD_WIDGET,
        Style_TIME_PICKER,
        Style_COLOR_DIALOG,
        Style_MEDIA_PLAYER,
        Style_SETTING_CARD,
        Style_TEACHING_TIP,
        Style_FLUENT_WINDOW,
        Style_SWITCH_BUTTON,
        Style_MESSAGE_DIALOG,
        Style_STATE_TOOL_TIP,
        Style_CALENDAR_PICKER,
        Style_FOLDER_LIST_DIALOG,
        Style_SETTING_CARD_GROUP,
        Style_EXPAND_SETTING_CARD,
        Style_NAVIGATION_INTERFACE,
    };

    E_ENUM_CREATE(StyleSheet)

    // 主题颜色(这个比较特殊, 只需要值就可以区分了, 就没按规则命名, 内部也没处理)
    enum ThemeColor
    {
        ThemeColorPrimary,
        ThemeColorDark1,
        ThemeColorDark2,
        ThemeColorDark3,
        ThemeColorLight1,
        ThemeColorLight2,
        ThemeColorLight3,
    };

    E_ENUM_CREATE(ThemeColor)

    // 滚动条的显示模式
    enum class ScrollBarHandleDisplayMode
    {
        ALWAYS = 0,
        ON_HOVER = 1
    };

    enum class SmoothMode
    {
        NO_SMOOTH = 0, // 无平滑
        CONSTANT = 1, // 恒定速度
        LINEAR = 2, // 线性加减速
        QUADRATI = 3, // 二次曲线（抛物线）加减速
        COSINE = 4 // 余弦加减速
    };

    // 标题栏按钮状态
    enum class TitleBarButtonState
    {
        NORMAL = 0, // 正常
        HOVER = 1, // 鼠标悬浮
        PRESSED = 2, // 被按下
    };
    enum CheckBoxState
    {
        NORMAL,           // 默认状态（未选中，无交互）
        HOVER,            // 鼠标悬停（未选中）
        PRESSED,          // 鼠标按下（未选中）
        CHECKED,          // 已选中（默认状态）
        CHECKED_HOVER,    // 已选中 + 鼠标悬停
        CHECKED_PRESSED,  // 已选中 + 鼠标按下
        DISABLED,         // 禁用状态（未选中）
        CHECKED_DISABLED  // 禁用状态（已选中）
    };

    // SwitchButton指示器位置
    enum class IndicatorPosition
    {
        LEFT,
        RIGHT
    };

    enum class InfoBarPosition
    {
        NONE,
        TOP,
        BOTTOM,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };

    // 导航栏显示模式
    enum class NavigationDisplayMode
    {
        MINIMAL,
        COMPACT,
        EXPAND,
        MENU
    };


    // 导航项位置
    enum class NavigationItemPosition
    {
        TOP,
        SCROLL,
        BOTTOM
    };


    enum class ToolTipPosition
    {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        TOP_LEFT,
        TOP_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_RIGHT
    };

    enum class ItemViewToolTipType
    {
        LIST,
        TABLE
    };

    // 菜单动画类型
    enum class MenuAnimationType
    {
        NONE,
        DROP_DOWN,
        PULL_UP,
        FADE_IN_DROP_DOWN,
        FADE_IN_PULL_UP,
    };




E_END_ENUM_CREATE(Es)


#endif //ESDEF_H
