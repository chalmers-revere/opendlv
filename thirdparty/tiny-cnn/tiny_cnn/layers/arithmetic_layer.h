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
#include "tiny_cnn/util/util.h"
#include "tiny_cnn/layers/layer.h"

namespace tiny_cnn {

/**
 * element-wise add N vectors
 **/
class elementwise_add_layer : public layer {
public:
    elementwise_add_layer(cnn_size_t num_args, cnn_size_t dim)
    : layer(std::vector<vector_type>(num_args, vector_type::data), {vector_type::data}), num_args_(num_args), dim_(dim) {}

    std::string layer_type() const override {
        return "elementwise-add";
    }

    std::vector<shape3d> in_shape() const override {
        return std::vector<shape3d>(num_args_, shape3d(dim_,1,1));
    }

    std::vector<shape3d> out_shape() const override {
        return{ shape3d(dim_,1,1) };
    }

    void forward_propagation(cnn_size_t worker_index,
                             const std::vector<vec_t*>& in_data,
                             std::vector<vec_t*>& out_data) override {
        const vec_t& in1 = *in_data[0];
        vec_t& out = *out_data[0];

        CNN_UNREFERENCED_PARAMETER(worker_index);

        out = in1;

        for (cnn_size_t i = 1; i < num_args_; i++) {
            std::transform(in_data[i]->begin(),
                           in_data[i]->end(),
                           out.begin(),
                           out.begin(),
                           [](float_t x, float_t y){ return x + y; });
        }
    }

    void back_propagation(cnn_size_t                 worker_index,
                          const std::vector<vec_t*>& in_data,
                          const std::vector<vec_t*>& out_data,
                          std::vector<vec_t*>&       out_grad,
                          std::vector<vec_t*>&       in_grad) override {
        CNN_UNREFERENCED_PARAMETER(worker_index);
        CNN_UNREFERENCED_PARAMETER(in_data);
        CNN_UNREFERENCED_PARAMETER(out_data);
        for (cnn_size_t i = 0; i < num_args_; i++)
            *in_grad[i] = *out_grad[0];
    }
private:
    cnn_size_t num_args_;
    cnn_size_t dim_;
};

} // namespace tiny_cnn
