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

#include <list>
#include <unordered_set>

static void Adding_int_A_B(benchmark::State& state)
{
    for (auto _ : state)
    {
        int a = 5123;
        int b = 1111;
        volatile int c = 0;
        benchmark::DoNotOptimize((c = a + b));
        benchmark::ClobberMemory();
    }
}

static void Adding_float_A_B(benchmark::State& state)
{
    for (auto _ : state)
    {
        float a = 5123.3412f;
        float b = 1111.3412f;
        volatile float c = 0;
        benchmark::DoNotOptimize((c = a + b));
        benchmark::ClobberMemory();
    }
}

static void Adding_double_A_B(benchmark::State& state)
{
    for (auto _ : state)
    {
        double a = 5123.3412;
        double b = 1111.3412;
        volatile double c = 0;
        benchmark::DoNotOptimize((c = a + b));
        benchmark::ClobberMemory();
    }
}

static void Adding_long_long_A_B(benchmark::State& state)
{
    for (auto _ : state)
    {
        long long a = 51233412;
        long long b = 11113412;
        volatile long long c = 0;
        benchmark::DoNotOptimize((c = a + b));
        benchmark::ClobberMemory();
    }
}

static void Dividing_int_A_B(benchmark::State& state)
{
    for (auto _ : state)
    {
        int a = 51233412;
        int b = 11113412;
        volatile int c = 0;
        benchmark::DoNotOptimize((c = a / b));
        benchmark::ClobberMemory();
    }
}

static void Static_Array_100(benchmark::State& state)
{
    for (auto _ : state)
    {
        constexpr std::size_t size = 100;

        int arr[size];
        for (std::size_t i = 0; i < size; ++i)
        {
            benchmark::DoNotOptimize(arr[i] = i * 100);
        }

        long long sum = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            benchmark::DoNotOptimize(sum += arr[i]);
        }

        long long countOfSmall = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            if (arr[i] < (size * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            if (arr[i] % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        constexpr std::size_t index = size / 2 - size * 0.1;
        for (std::size_t i = size; i > index; --i)
        {
            arr[i] = arr[i - 1];
        }
        benchmark::DoNotOptimize(arr[index] = 5555);

        benchmark::ClobberMemory();
    }
}

static void Static_Array_10000(benchmark::State& state)
{
    for (auto _ : state)
    {
        constexpr std::size_t size = 10000;

        int arr[size];
        for (std::size_t i = 0; i < size; ++i)
        {
            benchmark::DoNotOptimize(arr[i] = i * 100);
        }

        long long sum = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            benchmark::DoNotOptimize(sum += arr[i]);
        }

        long long countOfSmall = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            if (arr[i] < (size * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (std::size_t i = 0; i < size; ++i)
        {
            if (arr[i] % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        constexpr std::size_t index = size / 2 - size * 0.1;
        for (std::size_t i = size; i > index; --i)
        {
            arr[i] = arr[i - 1];
        }
        benchmark::DoNotOptimize(arr[index] = 5555);

        benchmark::ClobberMemory();
    }
}

static void Vector_100(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> arr;
        for (std::size_t i = 0; i < 100; ++i)
        {
            arr.push_back(0);
            benchmark::DoNotOptimize(arr.back() = i * 100);
        }

        long long sum = 0;
        for (auto i : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto i : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto i : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        const std::size_t index = arr.size() / 2 - arr.size() * 0.1;
        arr.insert(arr.begin() + index, 5555);
        benchmark::DoNotOptimize(arr[index] == 5555);

        benchmark::ClobberMemory();
    }
}

static void Vector_10000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::vector<int> arr;
        for (std::size_t i = 0; i < 10000; ++i)
        {
            arr.push_back(0);
            benchmark::DoNotOptimize(arr.back() = i * 100);
        }

        long long sum = 0;
        for (auto i : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto i : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto i : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        const std::size_t index = arr.size() / 2 - arr.size() * 0.1;
        arr.insert(arr.begin() + index, 5555);
        benchmark::DoNotOptimize(arr[index] == 5555);

        benchmark::ClobberMemory();
    }
}

static void List_1000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::list<int> arr;
        for (std::size_t i = 0; i < 1000; ++i)
        {
            arr.push_back(0);
            benchmark::DoNotOptimize(arr.back() = i * 100);
        }

        long long sum = 0;
        for (auto i : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto i : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto i : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        const std::size_t index = arr.size() / 2 - arr.size() * 0.1;
        auto it = arr.begin();
        std::advance(it, index);

        arr.insert(it, 5555);
        benchmark::DoNotOptimize(*it == 5555);

        benchmark::ClobberMemory();
    }
}

static void Map_1000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::map<int, int> arr;
        for (std::size_t i = 0; i < 1000; ++i)
        {
            const auto it = arr.emplace(i, i * 100);
            benchmark::DoNotOptimize(123 + it.first->first);
        }

        long long sum = 0;
        for (auto [_, i] : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto [_, i] : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto [_, i] : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        arr.emplace(5555, 5555);
        benchmark::DoNotOptimize(arr[5555] == 5555);

        benchmark::ClobberMemory();
    }
}

static void UnorderedMap_1000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::unordered_map<int, int> arr;
        for (std::size_t i = 0; i < 1000; ++i)
        {
            const auto it = arr.emplace(i, i * 100);
            benchmark::DoNotOptimize(123 + it.first->first);
        }

        long long sum = 0;
        for (auto [_, i] : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto [_, i] : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto [_, i] : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        arr.emplace(5555, 5555);
        benchmark::DoNotOptimize(arr[5555] == 5555);

        benchmark::ClobberMemory();
    }
}

static void Set_1000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::set<int> arr;
        for (std::size_t i = 0; i < 1000; ++i)
        {
            const auto it = arr.emplace(i * 100);
            benchmark::DoNotOptimize(123 + *it.first);
        }

        long long sum = 0;
        for (auto i : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto i : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto i : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        arr.emplace(5555);
        benchmark::DoNotOptimize(arr.contains(5555));

        benchmark::ClobberMemory();
    }
}

static void UnorderedSet_1000(benchmark::State& state)
{
    for (auto _ : state)
    {
        std::unordered_set<int> arr;
        for (std::size_t i = 0; i < 1000; ++i)
        {
            const auto it = arr.emplace(i * 100);
            benchmark::DoNotOptimize(123 + *it.first);
        }

        long long sum = 0;
        for (auto i : arr)
        {
            benchmark::DoNotOptimize(sum += i);
        }

        long long countOfSmall = 0;
        for (auto i : arr)
        {
            if (i < (arr.size() * 100 / 2))
            {
                benchmark::DoNotOptimize(++countOfSmall);
            }
        }

        int count = 0;
        for (auto i : arr)
        {
            if (i % 2 == 0)
            {
                benchmark::DoNotOptimize(++count);
            }
        }

        arr.emplace(5555);
        benchmark::DoNotOptimize(arr.contains(5555));

        benchmark::ClobberMemory();
    }
}

static void Count_25_chars(benchmark::State& state)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::cout << "Hello world, how are you!" << '\n');
    }

    std::cout.rdbuf(old);
}

static void Count_5_chars(benchmark::State& state)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::cout << "Hello" << '\n');
    }

    std::cout.rdbuf(old);
}

static void Count_50_chars(benchmark::State& state)
{
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    for (auto _ : state)
    {
        benchmark::DoNotOptimize(std::cout << "HelloHelloHelloHelloHelloHelloHelloHelloHelloHello" << '\n');
    }

    std::cout.rdbuf(old);
}

static void Call_Lambda(benchmark::State& state)
{
    static auto func = []()
    {
        volatile long long arr[5]{};
        arr[3] = reinterpret_cast<long long>(arr);

        bool answer = false;
        if (arr[3] > 500000)
        {
            benchmark::DoNotOptimize(answer = true);
        }
    };

    for (auto _ : state)
    {
        func();
    }
}

void Func()
{
    volatile long long arr[5]{};
    arr[3] = reinterpret_cast<long long>(arr);

    bool answer = false;
    if (arr[3] > 500000)
    {
        benchmark::DoNotOptimize(answer = true);
    }
}

static void Call_Function(benchmark::State& state)
{
    for (auto _ : state)
    {
        Func();
    }
}


BENCHMARK(Call_Function);
BENCHMARK(Call_Lambda);

BENCHMARK(Count_5_chars);
BENCHMARK(Count_25_chars);
BENCHMARK(Count_50_chars);

BENCHMARK(UnorderedSet_1000);
BENCHMARK(Set_1000);
BENCHMARK(UnorderedMap_1000);
BENCHMARK(Map_1000);
BENCHMARK(List_1000);
BENCHMARK(Vector_10000);
BENCHMARK(Vector_100);

BENCHMARK(Static_Array_10000);
BENCHMARK(Static_Array_100);

BENCHMARK(Dividing_int_A_B);
BENCHMARK(Adding_int_A_B);
BENCHMARK(Adding_float_A_B);
BENCHMARK(Adding_double_A_B);
BENCHMARK(Adding_long_long_A_B);

FUNCTION_MAIN(Daily)