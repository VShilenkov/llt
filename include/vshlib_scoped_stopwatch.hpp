#ifndef _VSH_LIB_INCLUDE_INSTRUMENTATION_VSHLIB_SCOPED_STOPWATCH_HPP_
#define _VSH_LIB_INCLUDE_INSTRUMENTATION_VSHLIB_SCOPED_STOPWATCH_HPP_

// IWYU pragma: begin_exports
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
// IWYU pragma: end_exports

namespace vsh {
namespace instrumentation {

template<typename duration_t = std::chrono::milliseconds, std::ostream &os = std::cout>
class scoped_stopwatch
{
    using clock_type      = std::chrono::steady_clock;
    using time_point_type = std::chrono::time_point<clock_type>;
    using lap_type        = std::pair<std::string, time_point_type>;

public:
    scoped_stopwatch(const std::string &name = "", bool celebrate_birthday = false)
        : mName(name)
        , mLaps( )
    {
        mLaps.reserve(16);
        lap( );
        if(celebrate_birthday)
        {
            const std::time_t t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::cout << std::put_time(std::localtime(&t_c), "%F %T.\n") << std::flush;
        }
    }
    ~scoped_stopwatch( )
    {
        time_point_type   end_time   = clock_type::now( );
        auto              i          = mLaps.cbegin( );
        time_point_type   begin_time = i->second;
        const std::string prefix(mName.empty( ) ? "" : mName + ": ");
        for (auto j = next(i); j < mLaps.cend( ); ++i, ++j)
        {
            const std::string lap_prefix(j->first.empty( ) ? "" : j->first + ": ");
            os << prefix << lap_prefix << std::chrono::duration_cast<duration_t>(j->second - i->second).count( )
               << std::endl;
        }

        os << prefix << ((mLaps.size( ) > 1) ? "last: " : "total: ");
        os << std::chrono::duration_cast<duration_t>(end_time - i->second).count( );
        if (mLaps.size( ) > 1)
        {
            os << std::endl
               << prefix << "total: " << std::chrono::duration_cast<duration_t>(end_time - begin_time).count( )
               << std::endl;
        }
        else
        {
            os << std::endl;
        }
    }

    void lap(const std::string &name = "") { mLaps.push_back(std::make_pair(name, clock_type::now( ))); }

private:
    std::string           mName;
    std::vector<lap_type> mLaps;
};

} /* namespace instrumentation */
} /* namespace vsh */

#endif /* _VSH_LIB_INCLUDE_INSTRUMENTATION_VSHLIB_SCOPED_STOPWATCH_HPP_ */