/*
    Copyright (c) 2013, Taiga Nomi
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

#include "tiny_cnn/config.h"
#include "tiny_cnn/network.h"
#include "tiny_cnn/nodes.h"

#include "tiny_cnn/layers/input_layer.h"
#include "tiny_cnn/layers/feedforward_layer.h"
#include "tiny_cnn/layers/convolutional_layer.h"
#include "tiny_cnn/layers/fully_connected_layer.h"
#include "tiny_cnn/layers/average_pooling_layer.h"
#include "tiny_cnn/layers/max_pooling_layer.h"
#include "tiny_cnn/layers/linear_layer.h"
#include "tiny_cnn/layers/lrn_layer.h"
#include "tiny_cnn/layers/dropout_layer.h"
#include "tiny_cnn/layers/arithmetic_layer.h"
#include "tiny_cnn/layers/concat_layer.h"
#include "tiny_cnn/layers/deconvolutional_layer.h"
#include "tiny_cnn/layers/max_unpooling_layer.h"
#include "tiny_cnn/layers/average_unpooling_layer.h"

#include "tiny_cnn/activations/activation_function.h"
#include "tiny_cnn/lossfunctions/loss_function.h"
#include "tiny_cnn/optimizers/optimizer.h"

#include "tiny_cnn/util/weight_init.h"
#include "tiny_cnn/util/image.h"
#include "tiny_cnn/util/deform.h"
#include "tiny_cnn/util/product.h"
#include "tiny_cnn/util/graph_visualizer.h"

#include "tiny_cnn/io/mnist_parser.h"
#include "tiny_cnn/io/cifar10_parser.h"
#include "tiny_cnn/io/display.h"
#include "tiny_cnn/io/layer_factory.h"

#ifdef CNN_USE_CAFFE_CONVERTER
// experimental / require google protobuf
#include "tiny_cnn/io/caffe/layer_factory.h"
#endif

// shortcut version of layer names
namespace tiny_cnn {
namespace layers {

template <class T>
using conv = tiny_cnn::convolutional_layer<T>;

template <class T>
using max_pool = tiny_cnn::max_pooling_layer<T>;

template <class T>
using ave_pool = tiny_cnn::average_pooling_layer<T>;

template <class T>
using fc = tiny_cnn::fully_connected_layer<T>;

template <class T>
using dense = tiny_cnn::fully_connected_layer<T>;

using add = tiny_cnn::elementwise_add_layer;

using dropout = tiny_cnn::dropout_layer;

using input = tiny_cnn::input_layer;

template <class T>
using lrn = tiny_cnn::lrn_layer<T>;

using input = tiny_cnn::input_layer;

using concat = tiny_cnn::concat_layer;

template <class T>
using deconv = tiny_cnn::deconvolutional_layer<T>;

template <class T>
using max_unpooling = tiny_cnn::max_unpooling_layer<T>;

template <class T>
using average_unpooling = tiny_cnn::average_unpooling_layer<T>;

}
}
