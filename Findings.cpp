/*
 * MIT License
 *
 * Copyright (c) 2018-2025 Valerii Koniushenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Exporter.h"

#define SUB_SIZE 1 << 17

struct SomeStruct
{
    const char name[8] = "name";
    float f = 3.1415f;
    int i = 0;

    [[nodiscard]] bool operator==(const SomeStruct& other) const { return other.i == i; }
};

static int whatToFind = 0;
static const std::vector<SomeStruct> structs = []
{
    std::vector<SomeStruct> out;
    out.reserve(SUB_SIZE);

    for (std::size_t i = 0; i < SUB_SIZE; ++i)
    {
        out.push_back(SomeStruct());
        out.back().i = i * 10;
        out.back().f += i;
    }

    whatToFind = out.at(out.size() - 10).i;

    return out;
}();

static void FindFirstStruct_CFor(benchmark::State& state)
{
    for (auto _ : state)
    {
        const int* p = nullptr;
        for (std::size_t i = 0; i < structs.size(); ++i)
        {
            if (structs.at(i).i == whatToFind)
            {
                p = &structs.at(i).i;
                break;
            }
        }

        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_RangeBasedFor_RefCompression(benchmark::State& state)
{
    for (auto _ : state)
    {
        const int* p = nullptr;
        for (auto&& obj : structs)
        {
            if (obj.i == whatToFind)
            {
                p = &obj.i;
                break;
            }
        }
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_RangeBasedFor_NonRefCompression(benchmark::State& state)
{
    for (auto _ : state)
    {
        const int* p = nullptr;
        for (auto& obj : structs)
        {
            if (obj.i == whatToFind)
            {
                p = &obj.i;
                break;
            }
        }
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_FindIf(benchmark::State& state)
{
    for (auto _ : state)
    {
        const int* p = nullptr;
        const auto it = std::find_if(structs.begin(), structs.end(),
                                     [&](const SomeStruct& obj)
                                     {
                                         return obj.i == whatToFind;
                                     });
        p = &it->i;
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_FindIfWithStaticPred(benchmark::State& state)
{
    static auto pred = [&](const SomeStruct& obj)
    {
        return obj.i == whatToFind;
    };

    for (auto _ : state)
    {
        const int* p = nullptr;
        const auto it = std::find_if(structs.begin(), structs.end(), pred);
        p = &it->i;
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_RangesFindIf(benchmark::State& state)
{
    for (auto _ : state)
    {
        const int* p = nullptr;
        const auto it = std::ranges::find_if(structs,
                                             [&](const SomeStruct& obj)
                                             {
                                                 return obj.i == whatToFind;
                                             });
        p = &it->i;
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_Find(benchmark::State& state)
{
    SomeStruct s;
    s.i = whatToFind;
    for (auto _ : state)
    {
        const int* p = nullptr;
        const auto it = std::ranges::find(structs, s);
        p = &it->i;
        benchmark::DoNotOptimize(*p + 333);
    }
}

static void FindFirstStruct_RangesFindIfWithStaticPred(benchmark::State& state)
{
    static auto pred = [&](const SomeStruct& obj)
    {
        return obj.i == whatToFind;
    };

    for (auto _ : state)
    {
        const int* p = nullptr;
        const auto it = std::ranges::find_if(structs, pred);
        p = &it->i;
        benchmark::DoNotOptimize(*p + 333);
    }
}

BENCHMARK(FindFirstStruct_CFor);
BENCHMARK(FindFirstStruct_RangeBasedFor_RefCompression);
BENCHMARK(FindFirstStruct_RangeBasedFor_NonRefCompression);
BENCHMARK(FindFirstStruct_Find);
BENCHMARK(FindFirstStruct_FindIf);
BENCHMARK(FindFirstStruct_RangesFindIf);
BENCHMARK(FindFirstStruct_FindIfWithStaticPred);
BENCHMARK(FindFirstStruct_RangesFindIfWithStaticPred);

FUNCTION_MAIN(Findings)