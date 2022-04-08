/*
g++ -march=corei7 -O2 /mnt/c/test/gmp_test/ll.cpp -lgmpxx -lgmp -oll
*/

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#include <gmpxx.h>

#include "vshlib_scoped_stopwatch.hpp"

// IWYU pragma: no_include <gmp.h>

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

bool ll2(long n)
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

long init_while{0}, new_while{0};

bool ll_shifted(long n)
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

bool ll_2(const long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;

    mpz_class       h(2);
    mpz_class       q;
    const mpz_class two(2);

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

bool ll(const long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;

    mpz_class h(2);
    mpz_class q;

    for (long i{0}; i < n - 2; ++i)
    {
        // std::cout << h << std::endl;
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

bool ll_3(const long n)
{
    mpz_class mod(1);
    mod <<= n;
    --mod;
    mpz_class h(4), q;

    for (long i{0}; i < n - 2; ++i)
    {
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

    return h == mod;
}

typedef bool (*ll_function)(long);

static const std::vector<long> exp1e1 = {2, 3, 5, 7};
static const std::vector<long> exp1e2 = {13, 17, 19, 31, 61, 89};
static const std::vector<long> exp1e3 = {107, 127, 521, 607};
static const std::vector<long> exp1e4 = {1279, 2203, 2281, 3217, 4253, 4423, 9689, 9941};
static const std::vector<long> exp1e5 = {11213, 19937, 21701, 23209, 44497, 86243};
static const std::vector<long> exp1e6 = {110503, 132049, 216091, 756839, 859433};
static const std::vector<long> exp1e7 = {1257787, 1398269, 2976221, 3021377, 6972593};
static const std::vector<long> exp1e8 = {13466917,
                                         20996011,
                                         24036583,
                                         25964951,
                                         30402457,
                                         32582657,
                                         37156667,
                                         42643801,
                                         43112609,
                                         57885161,
                                         74207281,
                                         77232917,
                                         82589933};

void test(long n, ll_function ll)
{
    vsh::instrumentation::scoped_stopwatch<std::chrono::seconds> sw("ll(" + std::to_string(n) + ")");
    std::cout << n << '\t' << std::boolalpha << ll(n) << std::noboolalpha << '\t';
}

int main( )
{
    for (auto i : exp1e7)
        test(i, &ll);
}
