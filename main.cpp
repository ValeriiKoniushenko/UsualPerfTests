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

#include <benchmark/benchmark.h>
#include <benchmark/export.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

class TimeCollectorReporter : public benchmark::ConsoleReporter
{
public:
    ~TimeCollectorReporter() override
    {
        std::ifstream in(PATH_TO_TEMPLATE_HTML, std::ios::ate);
        const auto size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::string html(size, ' ');
        in.read(&html[0], size);
        in.close();

        const char* labelStr = "@@LABELS";
        const char* valuesStr = "@@VALUES";

        html.replace(html.find(valuesStr), std::strlen(valuesStr), getValuesData());
        html.replace(html.find(labelStr), std::strlen(labelStr), getLabelsData());

        std::ofstream out("stats.html");
        out << html;

        std::cout << "To open stats.html in your browser:" << std::endl;
        std::cout << "file://" + (std::filesystem::current_path() / "stats.html").generic_string() << std::endl;
    }

    [[nodiscard]] std::string getValuesData()
    {
        std::string out;
        for (const auto& [label, value] : _data)
        {
            out += std::to_string(value) + ",";
        }
        out.pop_back();
        return out;
    }

    [[nodiscard]] std::string getLabelsData()
    {
        std::string out;
        for (const auto& [label, value] : _data)
        {
            out += "'" + label + "',";
        }
        out.pop_back();
        return out;
    }

    void ReportRuns(const std::vector<Run>& runs) override
    {
        benchmark::ConsoleReporter::ReportRuns(runs);

        for (const auto& run : runs)
        {
            if (run.skipped)
            {
                continue;
            }

            _data.emplace_back(run.benchmark_name(), run.GetAdjustedRealTime());
        }
    }

private:
    std::vector<std::pair<std::string, double>> _data;
};

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

const char* CaselessStrStrFunc(const char* haystack, const char* needle)
{
    if (!*needle)
    {
        return haystack;
    }

    for (const char* h = haystack; *h; ++h)
    {
        const char* h_it = h;
        const char* n_it = needle;
        while (*h_it && *n_it
               && std::tolower(static_cast<unsigned char>(*h_it))
                      == std::tolower(static_cast<unsigned char>(*n_it)))
        {
            ++h_it;
            ++n_it;
        }

        if (!*n_it)
        {
            return h;
        }
    }

    return nullptr;
}

const char* CaselessStrStrFuncWithLikely(const char* haystack, const char* needle)
{
    if (!*needle) [[unlikely]]
    {
        return haystack;
    }

    for (const char* h = haystack; *h; ++h)
    {
        const char* h_it = h;
        const char* n_it = needle;
        while (*h_it && *n_it
               && std::tolower(static_cast<unsigned char>(*h_it))
                      == std::tolower(static_cast<unsigned char>(*n_it)))
        {
            ++h_it;
            ++n_it;
        }

        if (!*n_it)
        {
            return h;
        }
    }

    return nullptr;
}

const char* CustomStrStrFunc(const char* haystack, const char* needle)
{
    if (!*needle)
    {
        return haystack;
    }
    for (const char* h = haystack; *h; ++h)
    {
        const char* h_it = h;
        const char* n_it = needle;
        while (*h_it && *n_it && *h_it == *n_it)
        {
            ++h_it;
            ++n_it;
        }
        if (!*n_it)
        {
            return h;
        }
    }
    return nullptr;
}

static void StdStrStr(benchmark::State& state)
{
    for (auto _ : state)
    {
        const char* text = "Hello, world!";
        const char* sub = "world";
        benchmark::DoNotOptimize(strstr(text, sub));
    }
}

static void CustomStrStr(benchmark::State& state)
{
    for (auto _ : state)
    {
        const char* text = "Hello, world!";
        const char* sub = "world";
        benchmark::DoNotOptimize(CustomStrStrFunc(text, sub));
    }
}

static void CustomCaseLessStrStr(benchmark::State& state)
{
    for (auto _ : state)
    {
        const char* text = "Hello, WoRlD!";
        const char* sub = "WoRld";
        benchmark::DoNotOptimize(CaselessStrStrFunc(text, sub));
    }
}

static void CustomCaseLessStrStrWithPredictions(benchmark::State& state)
{
    for (auto _ : state)
    {
        const char* text = "Hello, WoRlD!";
        const char* sub = "WoRld";
        benchmark::DoNotOptimize(CaselessStrStrFuncWithLikely(text, sub));
    }
}

BENCHMARK(CustomStrStr);
BENCHMARK(StdStrStr);
BENCHMARK(CustomCaseLessStrStr);
BENCHMARK(CustomCaseLessStrStrWithPredictions);

static void NonStaticLambda(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto func = []()
        {
            int arr[1024]{};
            for (std::size_t i = 0; i < SUB_SIZE; ++i)
            {
                benchmark::DoNotOptimize(arr[0] = 1 + i);
                benchmark::DoNotOptimize(arr[66] = 5 * i);
                benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
                benchmark::ClobberMemory();
            }
        };
        func();
    }
}

static void StaticLambda(benchmark::State& state)
{
    for (auto _ : state)
    {
        static auto func = []()
        {
            int arr[1024]{};
            for (std::size_t i = 0; i < SUB_SIZE; ++i)
            {
                benchmark::DoNotOptimize(arr[0] = 1 + i);
                benchmark::DoNotOptimize(arr[66] = 5 * i);
                benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
                benchmark::ClobberMemory();
            }
        };
        func();
    }
}

BENCHMARK(NonStaticLambda);
BENCHMARK(StaticLambda);

static void ArrWithoutDefaultZero(benchmark::State& state)
{
    for (auto _ : state)
    {
        int arr[1024];
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(arr[0] = 1 + i);
            benchmark::DoNotOptimize(arr[66] = 5 * i);
            benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
            benchmark::ClobberMemory();
        }
    }
}

static void ArrWithDefaultZero(benchmark::State& state)
{
    for (auto _ : state)
    {
        int arr[1024]{};
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(arr[0] = 1 + i);
            benchmark::DoNotOptimize(arr[66] = 5 * i);
            benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(ArrWithoutDefaultZero);
BENCHMARK(ArrWithDefaultZero);

static void StructWithoutDefaultZero(benchmark::State& state)
{
    struct Foo
    {
        int a;
        char c;
    };

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(Foo{ 1, 2 });
            Foo f;
            benchmark::DoNotOptimize(f.a = 1 + i);
            benchmark::DoNotOptimize(f.c = 5 * i);
            benchmark::ClobberMemory();
        }
    }
}

static void StructWithDefaultZero(benchmark::State& state)
{
    struct Foo
    {
        int a = 0;
        char c = 0;
    };

    for (auto _ : state)
    {
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(Foo{ 1, 2 });
            Foo f;
            benchmark::DoNotOptimize(f.a = 1 + i);
            benchmark::DoNotOptimize(f.c = 5 * i);
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(StructWithoutDefaultZero);
BENCHMARK(StructWithDefaultZero);

static void JustVariableAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        int x = 0;
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(x += i);
        }
    }
}

static void HeapPointerAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        int* x = new int;
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(*x += i);
            benchmark::ClobberMemory();
        }
        delete x;
    }
}

static void HeapSmartPointerAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        auto x = std::make_unique<int>();
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(*x += i);
            benchmark::ClobberMemory();
        }
    }
}

static void StackPointerAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        int a = 0;
        int* x = &a;
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(*x += i);
            benchmark::ClobberMemory();
        }
    }
}

static void HeadRestrictPointerAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        int a = 0;
        int* __restrict x = &a;
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(*x += i);
            benchmark::ClobberMemory();
        }
    }
}

static void StackRefAdd(benchmark::State& state)
{
    for (auto _ : state)
    {
        int a = 0;
        int& x = a;
        for (std::size_t i = 0; i < SUB_SIZE; ++i)
        {
            benchmark::DoNotOptimize(x += i);
            benchmark::ClobberMemory();
        }
    }
}

BENCHMARK(JustVariableAdd);
BENCHMARK(HeapPointerAdd);
BENCHMARK(HeadRestrictPointerAdd);
BENCHMARK(HeapSmartPointerAdd);
BENCHMARK(StackPointerAdd);
BENCHMARK(StackRefAdd);

int main(int argc, char** argv)
{
    benchmark::MaybeReenterWithoutASLR(argc, argv);
    char arg0_default[] = "benchmark";
    char* args_default = reinterpret_cast<char*>(arg0_default);
    if (!argv)
    {
        argc = 1;
        argv = &args_default;
    }
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }

    TimeCollectorReporter reporter;
    ::benchmark::RunSpecifiedBenchmarks(&reporter);
    ::benchmark::Shutdown();
    return 0;
}

int main(int, char**);