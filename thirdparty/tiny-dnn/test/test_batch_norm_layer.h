/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <gtest/gtest.h>

#include <vector>

#include "test/testhelper.h"
#include "tiny_dnn/tiny_dnn.h"

namespace tiny_dnn {

TEST(batchnorm, gradient_check1) {
  size_t num         = 4;
  size_t spatial_dim = 4;
  size_t channels    = 2;
  batch_normalization_layer bn(spatial_dim, channels);

  /* following values are extracted from caffe
   * confirming that batch-norm layer is compatible with caffe's bn */

  // clang-format off
    float_t top_diff[] = {
        0.554228544,
        -0.823364496,
        -0.103415221,
        0.669684947,
        0.142640188,
        -0.171076611,
        0.292261183,
        -0.067076027,
        -0.00277741,
        0.058186941,
        0.046050139,
        -0.006042562,
        -0.004771964,
        0.025202896,
        -0.062344212,
        0.030099955,
        -0.023314178,
        -0.030725746,
        0.070954606,
        0.055909708,
        -0.019887319,
        0.076775789,
        0.014769247,
        -0.025637595,
        0.004412052,
        -0.013895055,
        -0.001271803,
        3.15E-05,
        -0.013110356,
        0.008091689,
        -0.005485342,
        0.007250476
    };


    float_t top_data[] = {
        -0.430924207,
        -2.23937607,
        1.7876749,
        1.41079676,
        0.578419685,
        0.662835836,
        -2.1911881,
        -0.002405337,
        1.49315703,
        -0.836038888,
        0.006807627,
        -0.012308626,
        0.424309582,
        -0.56077528,
        0.095194906,
        0.34416762,
        -0.755284429,
        -1.02720368,
        0.802836478,
        -0.06101859,
        2.17714667,
        -0.994640052,
        -0.497716337,
        0.397495717,
        -0.545207798,
        0.320612997,
        -0.016919944,
        0.102396645,
        0.551594019,
        -1.44724381,
        -0.530790627,
        0.993595243
    };

    float_t stddev[] = {
        5.13347721,
        6.15658283
    };

    float_t expected_gradients[] = {
        0.115063809,
        -0.100263298,
        -0.078099057,
        0.083551511,
        0.025724376,
        -0.024521608,
        0.026721604,
        -0.01322682,
        -0.049858954,
        0.030313857,
        0.003235561,
        -0.006351554,
        0.000483762,
        -0.002936667,
        -0.011636967,
        0.00547356,
        0.012069567,
        0.018599048,
        -0.015254314,
        0.007145011,
        0.01277818,
        0.001789367,
        -0.004100761,
        -0.003131026,
        0.011310737,
        -0.017643189,
        -0.005286998,
        -0.008531732,
        0.000200434,
        -0.013175356,
        -0.007668978,
        0.007226899
    };

  // clang-format on
  tensor_t outd, ing, outg;
  std::vector<tensor_t *> in_data, out_data, in_grad, out_grad;

  for (size_t i = 0; i < num; i++) {
    size_t first = i * spatial_dim * channels;
    size_t last  = first + spatial_dim * channels;

    ing.push_back(vec_t(spatial_dim * channels));
    outg.push_back(vec_t(top_diff + first, top_diff + last));
    outd.push_back(vec_t(top_data + first, top_data + last));
  }
  in_grad.push_back(&ing);
  out_grad.push_back(&outg);
  out_data.push_back(&outd);

  bn.set_context(net_phase::train);
  bn.set_stddev(vec_t(stddev, stddev + 2));
  bn.back_propagation(in_data, out_data, out_grad, in_grad);

  for (size_t i = 0; i < num; i++) {
    for (size_t j = 0; j < spatial_dim * channels; j++) {
      EXPECT_NEAR(expected_gradients[i * spatial_dim * channels + j],
                  (*in_grad[0])[i][j], 1e-4);
    }
  }
}

/*
TEST(batchnorm, gradient_check2) {
  size_t num         = 4;
  size_t spatial_dim = 4;
  size_t channels    = 2;
  batch_normalization_layer bn(spatial_dim, channels, );

  std::vector<tensor_t> input_data = generate_test_data({num},
{spatial_dim*channels});
  std::vector<tensor_t> in_grad = input_data;  // copy constructor
  std::vector<tensor_t> out_data = generate_test_data({num},
{spatial_dim*channels});
  std::vector<tensor_t> out_grad = generate_test_data({num},
{spatial_dim*channels});

  size_t trials = 100;
  for (size_t i = 0; i < trials; i++) {
    const size_t edge = 0;
    const size_t idx = uniform_idx(input_data[edge][0]);
    float_t ngrad = numeric_gradient(bn, input_data, edge, idx, out_data,
out_grad, edge, idx);
    float_t cgrad = analytical_gradient(bn, input_data, edge, idx, out_data,
out_grad, edge, idx);
    std::cout << "relative error" << relative_error(cgrad, ngrad) << std::endl;
    EXPECT_NEAR(ngrad, cgrad, epsilon<float_t>());
  }
}
*/

TEST(batchnorm, forward) {
  batch_normalization_layer bn(/*spatial-size=*/4, /*channel=*/3);

  /*
        mean   var
  ch0:  0.0    0.0
  ch1: -1.0    6.0
  ch2:  2.875 10.696
  */

  // clang-format off
    tensor_t in = {
      {
         0.0,  0.0,  0.0,  0.0,  // ch-0 of data#0
        -4.0,  0.0, -1.0,  2.0,  // ch-1 of data#0
         1.0,  0.0,  1.0,  3.0,  // ch-2 of data#0
      }, {
         0.0,  0.0,  0.0,  0.0,  // ch-0 of data#1
         2.0,  0.0, -4.0, -3.0,  // ch-1 of data#1
         2.0,  5.0,  1.0, 10.0   // ch-2 of data#1
      }
    };

    /* y = (x - mean) ./ sqrt(variance + eps) */
    tensor_t expect = {
        {
            0.0,    0.0,    0.0,   0.0,    // ch-0 of data#0
           -1.225,  0.408,  0.0,   1.225,  // ch-1 of data#0
           -0.573, -0.879, -0.573, 0.038,  // ch-2 of data#0
        }, {
            0.0,   0.0,    0.0,    0.0,    // ch-0 of data#1
            1.225, 0.408, -1.225, -0.816,  // ch-1 of data#1
           -0.268, 0.650, -0.573,  2.179f  // ch-2 of data#1
        }
    };

  // clang-format on
  std::vector<const tensor_t *> result;
  bn.forward({in}, result);

  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 3 * 4; j++) {
      EXPECT_NEAR(expect[i][j], (*result[0])[i][j], 1e-3);
    }
  }

  bn.post_update();

  // confirming that calculating the moving average doesn't affect the result
  // while we feed the same data
  bn.forward({in}, result);
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 0; j < 3 * 4; j++) {
      EXPECT_NEAR(expect[i][j], (*result[0])[i][j], 1e-3);
    }
  }
}

TEST(batchnorm, read_write) {
  batch_normalization_layer l1(100, 100);
  batch_normalization_layer l2(100, 100);

  l1.setup(true);
  l2.setup(true);

  serialization_test(l1, l2);
}

}  // namespace tiny_dnn
