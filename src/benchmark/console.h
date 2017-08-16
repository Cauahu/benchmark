#ifndef BENCHMARK_CONSOLE_H_
#define BENCHMARK_CONSOLE_H_

#include <iostream>

#if !defined(BM_NO_COLOR)
#include <unistd.h>
#include <cstdio>
#endif

namespace benchmark {
 class Console{
public:
    /// Console text colors.
    enum TextColor
    {
        /// Default console color. Used for resets.
        TextDefault,

        /// Black.
        ///
        /// @warning Avoid using black unless it is absolutely necesssary.
        TextBlack,

        /// Blue.
        TextBlue,

        /// Green.
        TextGreen,

        /// Cyan.
        TextCyan,

        /// Red.
        TextRed,

        /// Purple.
        TextPurple,

        /// Yellow.
        TextYellow,

        /// White.
        ///
        /// @warning Avoid using white unless it is absolutely necessary.
        TextWhite
    };


    inline static Console& instance()
    {
        static Console singleton;
        return singleton;
    }

    inline static bool isFormattingEnabled()
    {
        return instance()._formattingEnabled;
    }

    inline static void setFormattingEnabled(bool enabled)
    {
        instance()._formattingEnabled = enabled;
    }

 private:
    inline Console()
        :   _formattingEnabled(true)
    {

    }
private:
    bool _formattingEnabled;
 };

#if !defined(BM_NO_COLOR)
inline std::ostream& operator <<(std::ostream& stream,
                                     const Console::TextColor& color)
{
    static const bool outputNoColor = isatty(fileno(stdout)) != 1;

    if ((!Console::isFormattingEnabled()) ||
        (outputNoColor) ||
        ((stream.rdbuf() != std::cout.rdbuf()) &&
            (stream.rdbuf() != std::cerr.rdbuf()))){
        return stream;
    }

    const char* value = "";
    switch(color) {
        case Console::TextDefault:
            value = "\033[m"; break;
        case Console::TextBlack:
            value = "\033[0;30m"; break;
        case Console::TextBlue:
            value = "\033[0;34m"; break;
        case Console::TextGreen:
            value = "\033[0;32m"; break;
        case Console::TextCyan:
            value = "\033[0;36m"; break;
        case Console::TextRed:
            value = "\033[0;31m"; break;
        case Console::TextPurple:
            value = "\033[0;35m"; break;
        case Console::TextYellow:
            value = "\033[0;33m"; break;
        case Console::TextWhite:
            value = "\033[0;37m"; break;
    }
    return stream << value;
}
#else 
    inline std::ostream& operator <<(std::ostream& stream,
                                     const Console::TextColor&)
    {
        return stream;
    }

#endif
}
#endif
