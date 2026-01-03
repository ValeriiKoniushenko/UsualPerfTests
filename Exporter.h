#pragma once

#include <benchmark/benchmark.h>
#include <benchmark/export.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>

#define FUNCTION_MAIN(FileName)                                                                    \
    int main(int argc, char** argv)                                                                \
    {                                                                                              \
        benchmark::MaybeReenterWithoutASLR(argc, argv);                                            \
        char arg0_default[] = "benchmark";                                                         \
        char* args_default = reinterpret_cast<char*>(arg0_default);                                \
        if (!argv)                                                                                 \
        {                                                                                          \
            argc = 1;                                                                              \
            argv = &args_default;                                                                  \
        }                                                                                          \
        ::benchmark::Initialize(&argc, argv);                                                      \
        if (::benchmark::ReportUnrecognizedArguments(argc, argv))                                  \
        {                                                                                          \
            return 1;                                                                              \
        }                                                                                          \
                                                                                                   \
        TimeCollectorReporter reporter(#FileName);                                                 \
        ::benchmark::RunSpecifiedBenchmarks(&reporter);                                            \
        ::benchmark::Shutdown();                                                                   \
        return 0;                                                                                  \
    }                                                                                              \
                                                                                                   \
    int main(int, char**);

class TimeCollectorReporter : public benchmark::ConsoleReporter
{
public:
    TimeCollectorReporter(const char* name)
        : _fileName(name)
    {
    }

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

        std::filesystem::path path = "dumps";
        std::error_code ec;
        std::filesystem::create_directory(path, ec);
        if (ec)
        {
            std::cerr << "Error while creating a directory '" << path.generic_string() << "': " << ec.message() << std::endl;
            return;
        }
        path /= (_fileName + ".html");

        std::ofstream out(path);
        if (!out)
        {
            std::cerr << "Error while opening a file '" << path.generic_string() << "' for writing." << std::endl;
            return;
        }

        out << html;

        std::cout << "To open " << path.generic_string() << " in your browser:" << std::endl;
        std::cout
            << "file://"
                   + (std::filesystem::current_path() / path.generic_string()).generic_string()
            << std::endl;
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
    std::string _fileName;
};