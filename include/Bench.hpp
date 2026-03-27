/**
 * @file Bench.hpp
 * @author Daniele Giardino
 * @brief Lightweight benchmarking helpers for rawdsp tests and examples.
 *
 * This header provides two small utilities:
 * - `bench::time_ms` to measure the average execution time of a zero-argument
 *   callable over many iterations (with optional warmup).
 * - `bench::measure_ms` to measure the elapsed time of a single invocation
 *   of a callable (with arbitrary arguments), returning milliseconds.
 *
 * Usage examples are in `tests/test_primitive_simd.cpp`.
 */

#pragma once

#include <chrono>
#include <cstddef>
#include <functional>
#include <utility>

namespace bench
{
    /**
     * @brief Result of an averaged timing run.
     *
     * - `total_ms` is the total elapsed time for all iterations in milliseconds.
     * - `avg_ms` is the average time per iteration in milliseconds.
     * - `iterations` is the number of iterations performed.
     */
    struct TimerResult
    {
        double total_ms;
        double avg_ms;
        std::size_t iterations;
    };

    /**
     * @brief Measure the average execution time of a zero-argument callable.
     *
     * The callable `f` is invoked `iterations` times and the total and average
     * durations are returned as a `TimerResult`. If `warmup` is true, `f` is
     * executed a small number of times before the timed loop to reduce cold-start
     * noise (e.g., cache effects, lazy initialization).
     *
     * @tparam F Callable type (must be invocable with no arguments).
     * @param iterations Number of timed iterations to perform.
     * @param warmup If true, perform a small number of warmup calls before timing.
     * @param f The zero-argument callable to time.
     * @return TimerResult with total and average milliseconds.
     */
    template <typename F>
    TimerResult time_ms(std::size_t iterations, bool warmup, F &&f)
    {
        if (warmup)
        {
            for (int i = 0; i < 16; ++i)
                f();
        }

        using clock = std::chrono::high_resolution_clock;
        auto t0 = clock::now();
        for (std::size_t i = 0; i < iterations; ++i)
            f();
        auto t1 = clock::now();

        double total_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
        return {total_ms, total_ms / static_cast<double>(iterations), iterations};
    }

    /**
     * @brief Measure the elapsed time of a single invocation of a callable.
     *
     * This utility invokes `f(args...)` once and returns the elapsed time in
     * milliseconds as a `double`. Use `std::ref`/`std::cref` when passing
     * references to containers or large objects.
     *
     * @tparam F Callable type.
     * @tparam Args Argument types for the callable.
     * @param f Callable to invoke.
     * @param args Arguments to forward to the callable.
     * @return Elapsed time in milliseconds for the single invocation.
     */
    template <typename F, typename... Args>
    double measure_ms(F &&f, Args &&... args)
    {
        using clock = std::chrono::high_resolution_clock;
        auto t0 = clock::now();
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        auto t1 = clock::now();
        return std::chrono::duration<double, std::milli>(t1 - t0).count();
    }

} // namespace bench

