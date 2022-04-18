#ifndef _VSHLIB_PROGRESS_INDICATOR_HPP_
#define _VSHLIB_PROGRESS_INDICATOR_HPP_

#include <iostream>
#include <string>

namespace vsh {

template<typename T, unsigned int length = 120, std::ostream &os = std::cout>
class progress_indicator
{
    static constexpr unsigned int percents{100U};

public:
    progress_indicator(const T &iterations)
        : mIterations(iterations)
        , mIterationsPerPercent(mIterations / percents)
        , bar("[")
    {
        bar.reserve(length);
    }

    progress_indicator( )                                        = delete;
    progress_indicator(const progress_indicator &rhs)            = delete;
    progress_indicator &operator=(const progress_indicator &rhs) = delete;

    ~progress_indicator( ) { os << std::endl; }

    void operator( )(const T &i)
    {
        if ((i + 1) % mIterationsPerPercent)
            return;
        const float        r{float(i) / mIterations};
        const unsigned int inner  = length - 3;
        unsigned int       filled = inner * r + 1;
        bar += std::string(filled - bar.length( ), '=');
        os << bar << '>' << std::string(inner - filled, ' ') << "] " << int(r * percents) + 1 << '%' << '\r';
        os.flush( );
    }

private:
    const T     mIterations;
    const T     mIterationsPerPercent;
    std::string bar;
};

} /* namespace vsh */
#endif /* _VSHLIB_PROGRESS_INDICATOR_HPP_ */