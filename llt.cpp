#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

#include <gmpxx.h>

#include "vshlib_scoped_stopwatch.hpp"
#ifdef PROGRESS
#    include "vshlib_progress_indicator.hpp"
#endif

// IWYU pragma: no_include <gmp.h>

typedef bool (*llt_function)(const uint64_t &);

static const std::vector<std::vector<uint64_t>>
    mersenne{{2LU, 3LU, 5LU, 7LU},
             {13LU, 17LU, 19LU, 31LU, 61LU, 89LU},
             {107LU, 127LU, 521LU, 607LU},
             {1279LU, 2203LU, 2281LU, 3217LU, 4253LU, 4423LU, 9689LU, 9941LU},
             {11213LU, 19937LU, 21701LU, 23209LU, 44497LU, 86243LU},
             {110503LU, 132049LU, 216091LU, 756839LU, 859433LU},
             {1257787LU, 1398269LU, 2976221LU, 3021377LU, 6972593LU},
             {13466917LU,
              20996011LU,
              24036583LU,
              25964951LU,
              30402457LU,
              32582657LU,
              37156667LU,
              42643801LU,
              43112609LU,
              57885161LU,
              74207281LU,
              77232917LU,
              82589933LU}};

static char mod_str[6972593LU + 2] = {0};

mpz_class make_mod(long n)
{
    mpz_class t;
    mpz_ui_pow_ui(t.get_mpz_t( ), 2, n);
    t = t - 1;
    return t;
}

mpz_class make_mask(long n)
{
    return make_mod(2 * (n + 1)) - make_mod(n);
}

bool llt_divided(long n)
{
    long            odd = 1;
    mpz_class       t;
    const mpz_class mod       = make_mod(n);
    const mpz_class minus_one = -1;
    const mpz_class mask      = make_mask(n);

    mpz_class h = 3;
    for (long i = 2; i <= n - 2; ++i)
    {
        h = ((h * h) << 3) + ((odd) ? ((h << 3) + 1) : minus_one);

        // t=h>>n;
        // h -= (t<<n)-t;

        mpz_and(t.get_mpz_t( ), h.get_mpz_t( ), mask.get_mpz_t( ));
        h = h - (t - (t >> n));

        while (h > mod)
        {
            h -= mod;
        }
        odd = mpz_tstbit(h.get_mpz_t( ), 0);
        h >>= 1;
    }
    return ((h << 1) + odd) == mod;
}

bool llt_shifted(long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;

    mpz_class h(2);
    mpz_class q;

    for (long i = 1; i <= n - 2; ++i)
    {
        size_t sh{0U};
        while (mpz_tstbit(h.get_mpz_t( ), sh) == 0)
        {
            ++sh;
        }

        if (sh)
        {
            h >>= sh;
        }

        h *= h;
        sh <<= 1;
        ++sh;
        h <<= sh;
        --h;

        q = h >> n;
        h += q;
        q <<= n;
        h -= q;
        while (mod < h)
        {
            h -= mod;
        }
    }

    return h == mod;
}

bool llt_2(const long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;

    mpz_class h(2);
    mpz_class q;

    for (long i{0}; i < n - 2; ++i)
    {
        h *= h;
        h <<= 1;
        --h;

        q = h >> n;
        h += q;
        q <<= n;
        h -= q;
        while (mod < h)
        {
            h -= mod;
        }
    }

    return h == mod;
}

bool llt_initial(const long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;

    mpz_class h(2);
    mpz_class q;

    for (long i{0}; i < n - 2; ++i)
    {
        h *= h;
        h <<= 1;
        --h;

        q = h >> n;
        h += q;
        q <<= n;
        h -= q;
        while (mod < h)
        {
            h -= mod;
        }
    }

    return h == mod;
}

bool llt_3(const uint64_t &n)
{
    mod_str[n] = 0;
    mpz_class mod, h, q;
    mpz_init_set_str(mod.get_mpz_t( ), mod_str, 2);
    mpz_init2(h.get_mpz_t( ), n << 1);
    mpz_init2(q.get_mpz_t( ), n);
    mpz_set_ui(h.get_mpz_t( ), 4);

#ifdef PROGRESS
    vsh::progress_indicator pb(n - 2);
#endif

    for (uint64_t i{2}; i < n; ++i)
    {
#ifdef PROGRESS
        pb(i);
#endif
        h *= h;
        h -= 2;
        while (mod < h)
        {
            // h = (h >> n) + (h & mod);
            q = h & mod;
            h >>= n;
            h += q;
        }
    }
    mod_str[n] = '1';
    return h == mod;
}

// test driver
void test(const uint64_t &n, llt_function llt)
{
    // <std::chrono::seconds>
    vsh::instrumentation::scoped_stopwatch sw("llt(" + std::to_string(n) + ")", true);
    bool                                   result{llt(n)};
    std::cout << n << '\t' << std::boolalpha << result << std::noboolalpha << '\t';
}

// main driver
int main( )
{
    memset(mod_str, '1', 6972593LU + 1);
    constexpr uint8_t exponent{7};
    for (auto i : mersenne[exponent - 1])
        test(i, &llt_3);
}
