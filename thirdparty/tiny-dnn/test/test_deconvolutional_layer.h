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

TEST(deconvolutional, setup_tiny) {
  deconvolutional_layer l(2, 2, 3, 1, 2, padding::valid, true, 1, 1,
                          core::backend_t::internal);

  EXPECT_EQ(l.parallelize(), true);          // if layer can be parallelized
  EXPECT_EQ(l.in_channels(), 3u);            // num of input tensors
  EXPECT_EQ(l.out_channels(), 1u);           // num of output tensors
  EXPECT_EQ(l.in_data_size(), 4u);           // size of input tensors
  EXPECT_EQ(l.out_data_size(), 32u);         // size of output tensors
  EXPECT_EQ(l.in_data_shape().size(), 1u);   // number of inputs shapes
  EXPECT_EQ(l.out_data_shape().size(), 1u);  // num of output shapes
  EXPECT_EQ(l.weights().size(), 2u);         // the wieghts vector size
  EXPECT_EQ(l.weights_grads().size(), 2u);   // the wieghts vector size
  EXPECT_EQ(l.inputs().size(), 3u);          // num of input edges
  EXPECT_EQ(l.outputs().size(), 1u);         // num of outpus edges
  EXPECT_EQ(l.in_types().size(), 3u);        // num of input data types
  EXPECT_EQ(l.out_types().size(), 1u);       // num of output data types
  EXPECT_EQ(l.fan_in_size(), 9u);            // num of incoming connections
  EXPECT_EQ(l.fan_out_size(), 18u);          // num of outgoing connections
  EXPECT_STREQ(l.layer_type().c_str(), "deconv");  // string with layer type
}

#ifdef CNN_USE_NNPACK
TEST(deconvolutional, setup_nnp) {
  deconvolutional_layer l(2, 2, 3, 1, 2, padding::valid, true, 1, 1,
                          backend_t::nnpack);

  EXPECT_EQ(l.parallelize(), true);         // if layer can be parallelized
  EXPECT_EQ(l.in_channels(), 3);            // num of input tensors
  EXPECT_EQ(l.out_channels(), 1);           // num of output tensors
  EXPECT_EQ(l.in_data_size(), 4);           // size of input tensors
  EXPECT_EQ(l.out_data_size(), 32);         // size of output tensors
  EXPECT_EQ(l.in_data_shape().size(), 1);   // number of inputs shapes
  EXPECT_EQ(l.out_data_shape().size(), 1);  // num of output shapes
  EXPECT_EQ(l.weights().size(), 2);         // the wieghts vector size
  EXPECT_EQ(l.weights_grads().size(), 2);   // the wieghts vector size
  EXPECT_EQ(l.inputs().size(), 3);          // num of input edges
  EXPECT_EQ(l.outputs().size(), 1);         // num of outpus edges
  EXPECT_EQ(l.in_types().size(), 3);        // num of input data types
  EXPECT_EQ(l.out_types().size(), 1);       // num of output data types
  EXPECT_EQ(l.fan_in_size(), 9);            // num of incoming connections
  EXPECT_EQ(l.fan_out_size(), 18);          // num of outgoing connections
  EXPECT_STREQ(l.layer_type().c_str(), "deconv");  // string with layer type
}
#endif

TEST(deconvolutional, fprop) {
  network<sequential> nn;

  deconvolutional_layer l(2, 2, 3, 1, 2);

  // layer::forward_propagation expects tensors, even if we feed only one
  // input at a time
  auto create_simple_tensor = [](size_t vector_size) {
    return tensor_t(1, vec_t(vector_size));
  };

  // create simple tensors that wrap the payload vectors of the correct size
  tensor_t in_tensor     = create_simple_tensor(4),
           out_tensor    = create_simple_tensor(32),
           a_tensor      = create_simple_tensor(32),
           weight_tensor = create_simple_tensor(18),
           bias_tensor   = create_simple_tensor(2);

  // short-hand references to the payload vectors
  vec_t &in = in_tensor[0], &out = out_tensor[0], &weight = weight_tensor[0];

  ASSERT_EQ(l.in_shape()[1].size(), 18u);  // weight

  uniform_rand(in.begin(), in.end(), -1.0, 1.0);

  std::vector<tensor_t *> in_data, out_data;
  in_data.push_back(&in_tensor);
  in_data.push_back(&weight_tensor);
  in_data.push_back(&bias_tensor);
  out_data.push_back(&out_tensor);
  out_data.push_back(&a_tensor);
  l.setup(false);
  {
    l.forward_propagation(in_data, out_data);

    for (auto o : out) EXPECT_DOUBLE_EQ(o, float_t(0));
  }

  // clang-format off
  weight[0] = 0.3;   weight[1] = 0.1;    weight[2] = 0.2;
  weight[3] = 0.0;   weight[4] = -0.1;   weight[5] = -0.1;
  weight[6] = 0.05;  weight[7] = -0.2;   weight[8] = 0.05;

  weight[9]  = 0.0;  weight[10] = -0.1;  weight[11] = 0.1;
  weight[12] = 0.1;  weight[13] = -0.2;  weight[14] = 0.3;
  weight[15] = 0.2;  weight[16] = -0.3;  weight[17] = 0.2;

  in[0] = 3;  in[1] = 2;  in[2] = 3;  in[3] = 0;
  // clang-format on

  {
    l.forward_propagation(in_data, out_data);

    EXPECT_NEAR(0.900, out[0], 1E-5);
    EXPECT_NEAR(0.900, out[1], 1E-5);
    EXPECT_NEAR(0.800, out[2], 1E-5);
    EXPECT_NEAR(0.400, out[3], 1E-5);
    EXPECT_NEAR(0.900, out[4], 1E-5);
    EXPECT_NEAR(0.000, out[5], 1E-5);
    EXPECT_NEAR(0.100, out[6], 1E-5);
    EXPECT_NEAR(-0.20, out[7], 1E-5);
    EXPECT_NEAR(0.150, out[8], 1E-5);
    EXPECT_NEAR(-0.80, out[9], 1E-5);
    EXPECT_NEAR(-0.55, out[10], 1E-5);
    EXPECT_NEAR(0.100, out[11], 1E-5);
    EXPECT_NEAR(0.150, out[12], 1E-5);
    EXPECT_NEAR(-0.60, out[13], 1E-5);
    EXPECT_NEAR(0.150, out[14], 1E-5);
    EXPECT_NEAR(0.000, out[15], 1E-5);
  }
}

TEST(deconvolutional, fprop2) {
  network<sequential> nn;

  deconvolutional_layer l(2, 2, 3, 1, 2, padding::same);

  auto create_simple_tensor = [](size_t vector_size) {
    return tensor_t(1, vec_t(vector_size));
  };

  tensor_t in_tensor     = create_simple_tensor(4),
           out_tensor    = create_simple_tensor(32),
           weight_tensor = create_simple_tensor(18),
           bias_tensor   = create_simple_tensor(2);

  // short-hand references to the payload vectors
  vec_t &in = in_tensor[0], &out = out_tensor[0], &weight = weight_tensor[0];

  ASSERT_EQ(l.in_shape()[1].size(), 18u);  // weight

  uniform_rand(in.begin(), in.end(), -1.0, 1.0);

  std::vector<tensor_t *> in_data, out_data;
  in_data.push_back(&in_tensor);
  in_data.push_back(&weight_tensor);
  in_data.push_back(&bias_tensor);
  out_data.push_back(&out_tensor);
  l.setup(false);
  {
    l.forward_propagation(in_data, out_data);

    for (auto o : out) EXPECT_DOUBLE_EQ(o, float_t(0));
  }

  // clang-format off
  weight[0] = 0.3;   weight[1] = 0.1;   weight[2] = 0.2;
  weight[3] = 0.0;   weight[4] = -0.1;  weight[5] = -0.1;
  weight[6] = 0.05;  weight[7] = -0.2;  weight[8] = 0.05;

  weight[9]  = 0.0;  weight[10] = -0.1; weight[11] = 0.1;
  weight[12] = 0.1;  weight[13] = -0.2; weight[14] = 0.3;
  weight[15] = 0.2;  weight[16] = -0.3; weight[17] = 0.2;

  in[0] = 3;  in[1] = 2;  in[2] = 3;  in[3] = 0;
  // clang-format on

  // resize tensor because its dimension changed in above used test case
  out_tensor[0].resize(32);

  {
    l.forward_propagation(in_data, out_data);

    EXPECT_NEAR(0.000, out[0], 1E-5);
    EXPECT_NEAR(0.100, out[1], 1E-5);
    EXPECT_NEAR(-0.80, out[2], 1E-5);
    EXPECT_NEAR(-0.55, out[3], 1E-5);
    EXPECT_NEAR(-0.70, out[4], 1E-5);
    EXPECT_NEAR(0.800, out[5], 1E-5);
    EXPECT_NEAR(-1.10, out[6], 1E-5);
    EXPECT_NEAR(0.900, out[7], 1E-5);
  }
}

/*
TEST(deconvolutional, gradient_check) {
  const size_t in_width = 2;
  const size_t in_height = 2;
  const size_t kernel_size = 3;
  const size_t in_channels = 10;
  const size_t out_channels = 5;

  deconvolutional_layer deconv(in_width, in_height, kernel_size, in_channels,
out_channels, padding::valid);
  std::vector<tensor_t> input_data = generate_test_data({1,1,1},
{in_width*in_height*in_channels,
kernel_size*kernel_size*in_channels*out_channels, out_channels});
  std::vector<tensor_t> in_grad = input_data;  // copy constructor
  std::vector<tensor_t> out_data = generate_test_data({1},
{in_width*in_width*in_height*in_height*out_channels});
  std::vector<tensor_t> out_grad = generate_test_data({1},
{in_width*in_width*in_height*in_height*out_channels});
  const size_t trials = 100;
  for (size_t i = 0; i < trials; i++) {
    const size_t in_edge = uniform_idx(input_data);
    const size_t in_idx = uniform_idx(input_data[in_edge][0]);
    const size_t out_edge = uniform_idx(out_data);
    const size_t out_idx = uniform_idx(out_data[out_edge][0]);
    float_t ngrad = numeric_gradient(deconv, input_data, in_edge, in_idx,
out_data, out_grad, out_edge, out_idx);
    float_t cgrad = analytical_gradient(deconv, input_data, in_edge, in_idx,
out_data, out_grad, out_edge, out_idx);
    EXPECT_NEAR(ngrad, cgrad, epsilon<float_t>());
  }
}
*/

/*
TEST(deconvolutional, gradient_check) {  // tanh - mse
  network<sequential> nn;
  nn << deconvolutional_layer(2, 2, 3, 1, 1) << tanh();

  const auto test_data = generate_gradient_check_data(nn.in_data_size());
  nn.init_weight();
  EXPECT_TRUE(nn.gradient_check<mse>(test_data.first, test_data.second,
                                     epsilon<float_t>(), GRAD_CHECK_ALL));
}

TEST(deconvolutional, gradient_check2) {  // sigmoid - mse
  network<sequential> nn;
  nn << deconvolutional_layer(2, 2, 3, 1, 1) << sigmoid();

  const auto test_data = generate_gradient_check_data(nn.in_data_size());
  nn.init_weight();
  EXPECT_TRUE(nn.gradient_check<mse>(test_data.first, test_data.second,
                                     epsilon<float_t>(), GRAD_CHECK_ALL));
}

TEST(deconvolutional, gradient_check3) {  // rectified - mse
  network<sequential> nn;

  nn << deconvolutional_layer(2, 2, 3, 1, 1) << relu();

  const auto test_data = generate_gradient_check_data(nn.in_data_size());
  nn.init_weight();
  EXPECT_TRUE(nn.gradient_check<mse>(test_data.first, test_data.second,
                                     epsilon<float_t>(), GRAD_CHECK_ALL));
}

TEST(deconvolutional, gradient_check4) {  // identity - mse
  network<sequential> nn;

  nn << deconvolutional_layer(2, 2, 3, 1, 1);

  const auto test_data = generate_gradient_check_data(nn.in_data_size());
  nn.init_weight();
  EXPECT_TRUE(nn.gradient_check<mse>(test_data.first, test_data.second,
                                     epsilon<float_t>(), GRAD_CHECK_ALL));
}

TEST(deconvolutional, gradient_check5) {  // sigmoid - cross-entropy
  network<sequential> nn;

  nn << deconvolutional_layer(2, 2, 3, 1, 1) << sigmoid();

  const auto test_data = generate_gradient_check_data(nn.in_data_size());
  nn.init_weight();
  EXPECT_TRUE(nn.gradient_check<cross_entropy>(
    test_data.first, test_data.second, epsilon<float_t>(), GRAD_CHECK_ALL));
}

TEST(deconvolutional, read_write)
{
    deconvolutional_layer l1(2, 2, 3, 1, 1);
    deconvolutional_layer l2(2, 2, 3, 1, 1);

    l1.init_weight();
    l2.init_weight();

    serialization_test(l1, l2);
}

TEST(deconvolutional, read_write2) {
#define O true
#define X false
    static const bool connection[] = {
        O, X, X, X, O, O,
        O, O, X, X, X, O,
        O, O, O, X, X, X
    };
#undef O
#undef X
    deconvolutional_layer layer1(14, 14, 5, 3, 6,
connection_table(connection, 3, 6));
    deconvolutional_layer layer2(14, 14, 5, 3, 6,
connection_table(connection, 3, 6));
    layer1.init_weight();
    layer2.init_weight();

    serialization_test(layer1, layer2);
}
*/
}  // namespace tiny_dnn
