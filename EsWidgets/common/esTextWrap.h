/****************************************************************************
** @brief 这家伙很懒, 什么都没写
** @details 这家伙很懒, 什么都没写
** @file esTextWrap.h
** @date 2025/11/16
** @author 瑶瑶
** @copyright (C) 2024 瑶瑶. All rights reserved.
****************************************************************************/

#ifndef ES_ESTEXTWRAP_H
#define ES_ESTEXTWRAP_H

#include "esDef.h"
#include <QString>
#include <QRegularExpression>
#include <QList>
#include <QMap>
#include <QChar>
#include <QTextStream>

class ES_EXPORT EsTextWrap
{
public:
    enum class CharType
    {
        SPACE,
        ASIAN,
        LATIN
    };

    /**
     * @brief 获取字符的宽度
     *
     * @param char 字符
     * @return 字符的宽度，1为单字节字符，2为双字节字符
     */
    static int getWidth(QChar character)
    {
        // 使用东亚字符宽度表来决定字符的宽度
        static const QMap<QString, int> EAST_ASIAN_WIDTH_TABLE = {
            {"F", 2},
            {"W", 2},
            {"A", 1},
            {"N", 1},
            {"Na", 1}
        };

        // 获取东亚字符宽度类型
        QString widthType =  eastAsianWidth(character);
        return EAST_ASIAN_WIDTH_TABLE.value(widthType, 1); // 默认返回1
    }

    /**
     * @brief 计算一段文本的总宽度
     *
     * @param text 输入文本
     * @return 文本的总宽度
     */
    static int getTextWidth(const QString& text)
    {
        int width = 0;
        for (const QChar& chara : text)
        {
            width += getWidth(chara);
        }
        return width;
    }

    /**
     * @brief 获取字符类型
     *
     * @param character 字符
     * @return 字符类型，空格、拉丁字符或亚洲字符
     */
    static CharType getCharType(QChar character)
    {
        if (character.isSpace())
        {
            return CharType::SPACE;
        }
        else if (getWidth(character) == 1)
        {
            return CharType::LATIN;
        }
        return CharType::ASIAN;
    }

    /**
     * @brief 处理字符串中的多余空格并去除首尾空白
     *
     * @param text 输入文本
     * @return 处理后的文本
     */
    static QString processTextWhitespace(const QString& text)
    {
        return text.simplified(); // Qt 提供的简化函数自动去除多余空格
    }

    /**
     * @brief 将长单词分割为多个小的单词
     *
     * @param token 需要分割的单词
     * @param width 最大宽度
     * @return 分割后的单词列表
     */
    static QList<QString> splitLongToken(const QString& token, int width)
    {
        QList<QString> result;
        for (int i = 0; i < token.length(); i += width)
        {
            result.append(token.mid(i, width));
        }
        return result;
    }

    /**
     * @brief 将文本分割为一系列的token
     *
     * @param text 输入的文本
     * @return 文本的token列表
     */
    static QList<QString> tokenizer(const QString& text)
    {
        QList<QString> tokens;
        QString buffer;
        CharType lastCharType = CharType::SPACE;

        for (const QChar& chara : text)
        {
            CharType charType = getCharType(chara);

            if (!buffer.isEmpty() && (charType != lastCharType || charType != CharType::LATIN))
            {
                tokens.append(buffer);
                buffer.clear();
            }

            buffer += chara;
            lastCharType = charType;
        }

        if (!buffer.isEmpty())
        {
            tokens.append(buffer);
        }

        return tokens;
    }

    /**
     * @brief 根据最大宽度进行文本换行
     *
     * @param text 待换行的文本
     * @param width 每行的最大字符宽度
     * @param once 是否只换一次行
     * @return 包含换行后的文本和是否进行了换行的标志
     */
    static QPair<QString, bool> wrap(const QString& text, int width, bool once = true)
    {
        QStringList lines = text.split('\n');
        bool isWrapped = false;
        QString wrappedText;
        QStringList wrappedLines;

        for (const QString& line : lines)
        {
            QString processedLine = processTextWhitespace(line);
            if (getTextWidth(processedLine) > width)
            {
                QString wrappedLine;
                bool lineWrapped;
                QPair<QString, bool> wrapResult = _wrapLine(processedLine, width, once);
                wrappedLine = wrapResult.first;
                lineWrapped = wrapResult.second;

                wrappedLines.append(wrappedLine);
                isWrapped = lineWrapped;

                if (once)
                {
                    wrappedLines.append(text.mid(wrappedLine.length()).trimmed());
                    return QPair<QString, bool>(wrappedLines.join("\n"), isWrapped);
                }
            }
            else
            {
                wrappedLines.append(line);
            }
        }

        return QPair<QString, bool>(wrappedLines.join("\n"), isWrapped);
    }

private:
    /**
     * @brief 实际换行操作
     *
     * @param text 输入文本
     * @param width 每行的最大宽度
     * @param once 是否只换一次行
     * @return 包含换行后的文本和是否进行了换行的标志
     */
    static QPair<QString, bool> _wrapLine(const QString& text, int width, bool once)
    {
        QString lineBuffer;
        QStringList wrappedLines;
        int currentWidth = 0;

        QList<QString> tokens = tokenizer(text);

        for (const QString& token : tokens)
        {
            int tokenWidth = getTextWidth(token);

            if (token == " " && currentWidth == 0)
            {
                continue;
            }

            if (currentWidth + tokenWidth <= width)
            {
                lineBuffer += token;
                currentWidth += tokenWidth;

                if (currentWidth == width)
                {
                    wrappedLines.append(lineBuffer.trimmed());
                    lineBuffer.clear();
                    currentWidth = 0;
                }
            }
            else
            {
                if (currentWidth != 0)
                {
                    wrappedLines.append(lineBuffer.trimmed());
                }

                QList<QString> chunks = splitLongToken(token, width);
                for (int i = 0; i < chunks.length() - 1; ++i)
                {
                    wrappedLines.append(chunks[i].trimmed());
                }

                lineBuffer = chunks.last();
                currentWidth = getTextWidth(chunks.last());
            }
        }

        if (currentWidth != 0)
        {
            wrappedLines.append(lineBuffer.trimmed());
        }

        if (once)
        {
            return QPair<QString, bool>(wrappedLines.first() + "\n" + wrappedLines.join(" "), true);
        }

        return QPair<QString, bool>(wrappedLines.join("\n"), true);
    }
    /**
    * @brief 判断字符的东亚宽度
    *
    * @param character 字符
    * @return 字符的宽度类型
    */
    static QString eastAsianWidth(QChar character)
    {
        // 获取字符的 Unicode 值
        ushort code = character.unicode();

        // 判断字符的 Unicode 范围，适配东亚字符宽度
        if ((code >= 0x1100 && code <= 0x11FF) ||  // Hangul (韩文)
            (code >= 0x4E00 && code <= 0x9FFF) ||  // CJK Ideographs (汉字)
            (code >= 0xAC00 && code <= 0xD7AF) ||  // Hangul Syllables (韩文音节)
            (code >= 0x20000 && code <= 0x2A6DF))  // Extension A, B, C (扩展区)
        {
            return "F"; // 全宽字符
        }
        else if ((code >= 0x3000 && code <= 0x303F) ||  // CJK Punctuation (中文标点)
                 (code >= 0xFF00 && code <= 0xFFEF))    // Full-width ASCII (全宽ASCII)
        {
            return "W"; // 宽字符
        }
        else if ((code >= 0x0020 && code <= 0x007E) ||  // ASCII characters (常见ASCII字符)
                 (code >= 0xFF61 && code <= 0xFF9F))    // Half-width characters (半宽字符)
        {
            return "N"; // 半宽字符
        }
        else if ((code >= 0xFF01 && code <= 0xFF5E))  // Full-width punctuation
        {
            return "W"; // 宽字符
        }
        return "Na"; // 默认值，表示无法分类的字符
    }
};


#endif //ES_ESTEXTWRAP_H
