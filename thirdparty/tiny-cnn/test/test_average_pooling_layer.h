/*
    Copyright (c) 2016, Taiga Nomi
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once
#include "picotest/picotest.h"
#include "testhelper.h"
#include "tiny_cnn/tiny_cnn.h"

namespace tiny_cnn {

TEST(ave_pool, gradient_check) { // sigmoid - cross-entropy
    typedef cross_entropy loss_func;
    typedef activation::sigmoid activation;
    typedef network<sequential> network;

    network nn;
    nn << fully_connected_layer<activation>(3, 8)
        << average_pooling_layer<activation>(4, 2, 1, 2); // 4x2 => 2x1

    vec_t a(3, 0.0);
    for (int i = 0; i < 3; i++) a[i] = i;
    label_t t = 0;

    nn.init_weight();
    //for (int i = 0; i < 24; i++) nn[0]->weight()[i] = i;

    EXPECT_TRUE(nn.gradient_check<loss_func>(&a, &t, 1, 1e-5, GRAD_CHECK_ALL));
}

TEST(ave_pool, forward) {
    average_pooling_layer<identity> l(4, 4, 1, 2);
    vec_t in = {
        0, 1, 2, 3,
        8, 7, 5, 6,
        4, 3, 1, 2,
        0,-1,-2,-3
    };

    vec_t expected = {
        4, 4,
        1.5, -0.5
    };


    l.weight_init(weight_init::constant(1.0));
    l.bias_init(weight_init::constant(0.0));
    l.init_weight();

    vec_t res = l.forward({ in })[0];

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected[i], res[i]);
    }
}

TEST(ave_pool, forward_stride) {
    average_pooling_layer<identity> l(4, 4, 1, 2, 1);
    vec_t in = {
        0, 1, 2, 3,
        8, 7, 5, 6,
        4, 3, 1, 2,
        0,-1,-2,-3
    };

    vec_t expected = {
        16.0/4, 15.0/4, 16.0/4,
        22.0/4, 16.0/4, 14.0/4,
         6.0/4,  1.0/4, -2.0/4
    };

    l.weight_init(weight_init::constant(1.0));
    l.bias_init(weight_init::constant(0.0));
    l.init_weight();

    vec_t res = l.forward({ in })[0];

    for (size_t i = 0; i < expected.size(); i++) {
        EXPECT_FLOAT_EQ(expected[i], res[i]);
    }
}

TEST(ave_pool, read_write) {
    average_pooling_layer<tan_h> l1(100, 100, 5, 2);
    average_pooling_layer<tan_h> l2(100, 100, 5, 2);

    l1.setup(true, 1);
    l2.setup(true, 1);

    serialization_test(l1, l2);
}

} // namespace tiny-cnn

