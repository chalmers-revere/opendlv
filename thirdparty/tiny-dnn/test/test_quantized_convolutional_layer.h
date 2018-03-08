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

TEST(quantized_convolutional, setup_internal) {
  quantized_convolutional_layer l(5, 5, 3, 1, 2, padding::valid, true, 1, 1,
                                  core::backend_t::internal);

  EXPECT_EQ(l.parallelize(), true);          // if layer can be parallelized
  EXPECT_EQ(l.in_channels(), 3u);            // num of input tensors
  EXPECT_EQ(l.out_channels(), 1u);           // num of output tensors
  EXPECT_EQ(l.in_data_size(), 25u);          // size of input tensors
  EXPECT_EQ(l.out_data_size(), 18u);         // size of output tensors
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
  EXPECT_STREQ(l.layer_type().c_str(), "q_conv");  // string with layer type
}

TEST(quantized_convolutional, fprop) {
  network<sequential> nn;

  quantized_convolutional_layer l(5, 5, 3, 1, 2);

  // layer::forward_propagation expects tensors, even if we feed only one
  // input at a time
  auto create_simple_tensor = [](size_t vector_size) {
    return tensor_t(1, vec_t(vector_size));
  };

  // create simple tensors that wrap the payload vectors of the correct size
  tensor_t in_tensor     = create_simple_tensor(25),
           out_tensor    = create_simple_tensor(18),
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

    for (auto o : out) EXPECT_NEAR(0.0, o, 1e-3);
  }

  // clang-format off
    // TODO(edgarriba): apparently when using double type, test crashes.
    weight[0] = 0.3f;  weight[1] = 0.1f; weight[2] = 0.2f;
    weight[3] = 0.0f;  weight[4] = -0.1f; weight[5] = -0.1f;
    weight[6] = 0.05f; weight[7] = -0.2f; weight[8] = 0.05f;

    weight[9] = 0.0f; weight[10] = -0.1f; weight[11] = 0.1f;
    weight[12] = 0.1f; weight[13] = -0.2f; weight[14] = 0.3f;
    weight[15] = 0.2f; weight[16] = -0.3f; weight[17] = 0.2f;

    in[0] = 3;  in[1] = 2;  in[2] = 1;  in[3] = 5; in[4] = 2;
    in[5] = 3;  in[6] = 0;  in[7] = 2;  in[8] = 0; in[9] = 1;
    in[10] = 0; in[11] = 6; in[12] = 1; in[13] = 1; in[14] = 10;
    in[15] = 3; in[16] =-1; in[17] = 2; in[18] = 9; in[19] = 0;
    in[20] = 1; in[21] = 2; in[22] = 1; in[23] = 5; in[24] = 5;

    {
        l.forward_propagation(in_data, out_data);

        EXPECT_NEAR(-0.043426, out[0], 2e-2);
        EXPECT_NEAR(1.6769816, out[1], 2e-2);
        EXPECT_NEAR(1.4731820, out[2], 2e-2);
        EXPECT_NEAR(1.0822733, out[3], 2e-2);
        EXPECT_NEAR(0.0415336, out[4], 2e-2);
        EXPECT_NEAR(-1.997466, out[5], 2e-2);
        EXPECT_NEAR(0.4238461, out[6], 2e-2);
        EXPECT_NEAR(1.1756143, out[7], 2e-2);
        EXPECT_NEAR(0.8273983, out[8], 2e-2);
        EXPECT_NEAR(-0.192101, out[9], 2e-2);
        EXPECT_NEAR(1.2309504, out[10], 2e-2);
        EXPECT_NEAR(2.2292108, out[11], 2e-2);
        EXPECT_NEAR(0.5300441, out[12], 2e-2);
        EXPECT_NEAR(1.7407004, out[13], 2e-2);
        EXPECT_NEAR(1.6345025, out[14], 2e-2);
        EXPECT_NEAR(0.6362420, out[15], 2e-2);
        EXPECT_NEAR(3.4186277, out[16], 2e-2);
        EXPECT_NEAR(-0.489455, out[17], 2e-2);
    }
  // clang-format on
}

#ifdef CNN_USE_NNPACK
TEST(quantized_convolutional, fprop_npp) {
  using network = network<sequential>;
  network nn;

  quantized_convolutional_layer l(5, 5, 3, 1, 2, padding::valid, true, 1, 1,
                                  backend_t::nnpack);

  // layer::forward_propagation expects tensors, even if we feed only one
  input at a time auto create_simple_tensor = [](size_t vector_size) {
    return tensor_t(1, vec_t(vector_size));
  };

  // create simple tensors that wrap the payload vectors of the correct size
  tensor_t in_tensor     = create_simple_tensor(25),
           out_tensor    = create_simple_tensor(18),
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

    for (auto o : out) EXPECT_NEAR(0.5, o, 1e-3);
  }

  // clang-format off
  weight[0] = 0.3;  weight[1] = 0.1;   weight[2] = 0.2;
  weight[3] = 0.0;  weight[4] = -0.1;  weight[5] = -0.1;
  weight[6] = 0.05; weight[7] = -0.2;  weight[8] = 0.05;

  weight[9]  = 0.0; weight[10] = -0.1; weight[11] = 0.1;
  weight[12] = 0.1; weight[13] = -0.2; weight[14] = 0.3;
  weight[15] = 0.2; weight[16] = -0.3; weight[17] = 0.2;

  in[0]  = 3;  in[1]  = 2;  in[2]  = 1;  in[3]  = 5;
  in[4]  = 2;  in[5]  = 3;  in[6]  = 0;  in[7]  = 2;
  in[8]  = 0;  in[9]  = 1;  in[10] = 0;  in[11] = 6;
  in[12] = 1;  in[13] = 1;  in[14] = 10; in[15] = 3;
  in[16] = -1; in[17] = 2;  in[18] = 9;  in[19] = 0;
  in[20] = 1;  in[21] = 2;  in[22] = 1;  in[23] = 5;
  in[24] = 5;
  // clang-format on

  {
    l.forward_propagation(in_data, out_data);

    EXPECT_NEAR(-0.043426, out[0], 2e-2);
    EXPECT_NEAR(1.6769816, out[1], 2e-2);
    EXPECT_NEAR(1.4858254, out[2], 2e-2);
    EXPECT_NEAR(1.0822733, out[3], 2e-2);
    EXPECT_NEAR(0.0415336, out[4], 2e-2);
    EXPECT_NEAR(-1.997466, out[5], 2e-2);
    EXPECT_NEAR(0.4238461, out[6], 2e-2);
    EXPECT_NEAR(1.1884713, out[7], 2e-2);
    EXPECT_NEAR(0.8273983, out[8], 2e-2);
    EXPECT_NEAR(-0.192101, out[9], 2e-2);
    EXPECT_NEAR(1.2309504, out[10], 2e-2);
    EXPECT_NEAR(2.2292108, out[11], 2e-2);
    EXPECT_NEAR(0.5300441, out[12], 2e-2);
    EXPECT_NEAR(1.7407004, out[13], 2e-2);
    EXPECT_NEAR(1.6345025, out[14], 2e-2);
    EXPECT_NEAR(0.6362420, out[15], 2e-2);
    EXPECT_NEAR(3.4186277, out[16], 2e-2);
    EXPECT_NEAR(-0.489455, out[17], 2e-2);
  }
}
#endif

/*
TEST(quantized_convolutional, gradient_check) { // tanh - mse
    network<sequential> nn;
    nn << quantized_convolutional_layer(5, 5, 3, 1, 1) << tanh();

    vec_t a(25, 0.0);
    label_t t = 3;

    uniform_rand(a.begin(), a.end(), -1, 1);
    nn.init_weight();
    EXPECT_TRUE(nn.gradient_check<mse>(&a, &t, 1, epsilon<float_t>(),
GRAD_CHECK_ALL));
}

TEST(quantized_convolutional, gradient_check2) { // sigmoid - mse
    network<sequential> nn;
    nn << quantized_convolutional_layer(5, 5, 3, 1, 1) << sigmoid();

    vec_t a(25, 0.0);
    label_t t = 3;

    uniform_rand(a.begin(), a.end(), -1, 1);
    nn.init_weight();
    EXPECT_TRUE(nn.gradient_check<mse>(&a, &t, 1, epsilon<float_t>(),
GRAD_CHECK_ALL));
}

TEST(quantized_convolutional, gradient_check3) { // rectified - mse
    network<sequential> nn;
    nn << quantized_convolutional_layer(5, 5, 3, 1, 1) << relu();

    vec_t a(25, 0.0);
    label_t t = 3;

    uniform_rand(a.begin(), a.end(), -1, 1);
    nn.init_weight();
    EXPECT_TRUE(nn.gradient_check<mse>(&a, &t, 1, epsilon<float_t>(),
GRAD_CHECK_ALL));
}

TEST(quantized_convolutional, gradient_check4) { // identity - mse
    network<sequential> nn;
    nn << quantized_convolutional_layer(5, 5, 3, 1, 1);

    vec_t a(25, 0.0);
    label_t t = 3;

    uniform_rand(a.begin(), a.end(), -1, 1);
    nn.init_weight();
    EXPECT_TRUE(nn.gradient_check<mse>(&a, &t, 1, epsilon<float_t>(),
GRAD_CHECK_ALL));
}

TEST(quantized_convolutional, gradient_check5) { // sigmoid - cross-entropy
    network<sequential> nn;
    nn << quantized_convolutional_layer(5, 5, 3, 1, 1) << sigmoid();

    vec_t a(25, 0.0);
    label_t t = 3;

    uniform_rand(a.begin(), a.end(), -1, 1);
    nn.init_weight();
    EXPECT_TRUE(nn.gradient_check<cross_entropy>(&a, &t, 1, epsilon<float_t>(),
GRAD_CHECK_ALL));
}

TEST(quantized_convolutional, read_write)
{
    quantized_convolutional_layer l1(5, 5, 3, 1, 1);
    quantized_convolutional_layer l2(5, 5, 3, 1, 1);

    l1.init_weight();
    l2.init_weight();

    serialization_test(l1, l2);
}

TEST(quantized_convolutional, read_write2) {
#define O true
#define X false
    static const bool connection[] = {
        O, X, X, X, O, O,
        O, O, X, X, X, O,
        O, O, O, X, X, X
    };
#undef O
#undef X
    quantized_convolutional_layer layer1(14, 14, 5, 3, 6,
connection_table(connection, 3, 6));
    quantized_convolutional_layer layer2(14, 14, 5, 3, 6,
connection_table(connection, 3, 6));
    layer1.init_weight();
    layer2.init_weight();

    serialization_test(layer1, layer2);
}
*/
}  // namespace tiny_dnn
