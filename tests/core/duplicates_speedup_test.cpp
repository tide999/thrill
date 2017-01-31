/*******************************************************************************
 * tests/core/duplicates_speedup_test.cpp
 *
 * Part of Project Thrill - http://project-thrill.org
 *
 * Copyright (C) 2016 Alexander Noe <aleexnoe@gmail.com>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#include <thrill/api/all_gather.hpp>
#include <thrill/api/dia.hpp>
#include <thrill/api/generate.hpp>
#include <thrill/api/reduce_by_key.hpp>
#include <thrill/api/size.hpp>
#include <thrill/common/cmdline_parser.hpp>
#include <thrill/common/logger.hpp>
#include <thrill/common/stats_timer.hpp>
#include <thrill/common/string_view.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <string>
#include <utility>

using namespace thrill; // NOLINT

void SpeedupTest(api::Context& ctx, size_t equal, size_t elements) {

    static constexpr bool debug = false;

    auto in = api::Generate(
        ctx, elements,
        [&equal](size_t n) {
            std::array<size_t, 128> value;
            for (size_t i = 0; i < 128; ++i) {
                value[i] = i + n;
            }
            return std::make_pair(n / equal, value);
        }).Keep().Execute();

    ctx.net.Barrier();
    common::StatsTimerStart timer;
    auto out = in.ReducePair(
        [](const std::array<size_t, 128>& in1, const std::array<size_t, 128>& in2) {
            std::array<size_t, 128> value_out;
            for (size_t i = 0; i < 128; ++i) {
                value_out[i] = in1[i] + in2[i];
            }
            return value_out;
        });
    out.Size();
    ctx.net.Barrier();
    timer.Stop();

    if (debug) {
        auto vec = out.AllGather();
        std::sort(vec.begin(), vec.end(), [](auto i1, auto i2) {
                      return i1.first < i2.first;
                  });
        if (ctx.my_rank() == 0) {
            LOG1 << "Checking results!";
            ASSERT_EQ(elements / equal, vec.size());
            for (size_t i = 0; i < vec.size(); ++i) {
                for (size_t j = 0; j < 128; ++j) {
                    ASSERT_EQ(vec[i].second[j],
                              equal * (equal - 1) / 2
                              + equal * j
                              + equal * equal * i);
                }
            }
            LOG1 << "Result checking successful.";
        }
    }
    else {
        auto traffic = ctx.net_manager().Traffic();
        LOG1 << "RESULT" << " benchmark=duplicates detection=ON"
             << " elements=" << elements
             << " time=" << timer.Milliseconds()
             << " traffic=" << traffic.first + traffic.second
             << " machines=" << ctx.num_hosts();
    }
}

int main(int argc, char* argv[]) {

    common::CmdlineParser clp;

    size_t equal = 5;
    clp.AddOptParamSizeT("e", equal, "Number of equal elements reduced together");

    size_t elements = 1000;
    clp.AddOptParamSizeT("n", elements, "Number of elements in total.");

    if (!clp.Process(argc, argv)) {
        return -1;
    }

    clp.PrintResult();

    api::Run([&](api::Context& ctx) {
                 return SpeedupTest(ctx, equal, elements);
             });
}

/******************************************************************************/
