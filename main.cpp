#include <benchmark/benchmark.h>


static void ArrWithoutDefaultZero(benchmark::State &state) {
    for (auto _: state) {
        int arr[1024];
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(arr[0] = 1 + i);
            benchmark::DoNotOptimize(arr[66] = 5 * i);
            benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
        }
    }
}

static void ArrWithDefaultZero(benchmark::State &state) {
    for (auto _: state) {
        int arr[1024]{};
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(arr[0] = 1 + i);
            benchmark::DoNotOptimize(arr[66] = 5 * i);
            benchmark::DoNotOptimize(arr[1023] = 6 * i + 3);
        }
    }
}

BENCHMARK(ArrWithoutDefaultZero);
BENCHMARK(ArrWithDefaultZero);


static void StructWithoutDefaultZero(benchmark::State &state) {
    struct Foo {
        int a;
        char c;
    };

    for (auto _: state) {
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(Foo{1, 2});
            Foo f;
            benchmark::DoNotOptimize(f.a = 1 + i);
            benchmark::DoNotOptimize(f.c = 5 * i);
        }
    }
}

static void StructWithDefaultZero(benchmark::State &state) {
    struct Foo {
        int a = 0;
        char c = 0;
    };

    for (auto _: state) {
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(Foo{1, 2});
            Foo f;
            benchmark::DoNotOptimize(f.a = 1 + i);
            benchmark::DoNotOptimize(f.c = 5 * i);
        }
    }
}

BENCHMARK(StructWithoutDefaultZero);
BENCHMARK(StructWithDefaultZero);

static void JustVariableAdd(benchmark::State &state) {
    for (auto _: state) {
        int x = 0;
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(x += i);
        }
    }
}

static void HeapPointerAdd(benchmark::State &state) {
    for (auto _: state) {
        int *x = new int;
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(*x += i);
        }
        delete x;
    }
}

static void HeapSmartPointerAdd(benchmark::State &state) {
    for (auto _: state) {
        auto x = std::make_unique<int>();
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(*x += i);
        }
    }
}

static void StackPointerAdd(benchmark::State &state) {
    for (auto _: state) {
        int a = 0;
        int *x = &a;
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(*x += i);
        }
    }
}

static void HeadRestrictPointerAdd(benchmark::State &state) {
    for (auto _: state) {
        int a = 0;
        int *__restrict x = &a;
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(*x += i);
        }
    }
}

static void StackRefAdd(benchmark::State &state) {
    for (auto _: state) {
        int a = 0;
        int &x = a;
        for (std::size_t i = 0; i < 1 << 16; ++i) {
            benchmark::DoNotOptimize(x += i);
        }
    }
}

BENCHMARK(JustVariableAdd);
BENCHMARK(HeapPointerAdd);
BENCHMARK(HeadRestrictPointerAdd);
BENCHMARK(HeapSmartPointerAdd);
BENCHMARK(StackPointerAdd);
BENCHMARK(StackRefAdd);

BENCHMARK_MAIN();
