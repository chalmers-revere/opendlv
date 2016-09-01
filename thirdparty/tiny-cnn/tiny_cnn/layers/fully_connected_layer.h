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
#include "tiny_cnn/layers/layer.h"
#include "tiny_cnn/util/product.h"

namespace tiny_cnn {

/**
 * compute fully-connected(matmul) operation
 **/
template<typename Activation>
class fully_connected_layer : public feedforward_layer<Activation> {
public:
    typedef feedforward_layer<Activation> Base;
    CNN_USE_LAYER_MEMBERS;

    /**
     * @param in_dim [in] number of elements of the input
     * @param out_dim [in] number of elements of the output
     * @param has_bias [in] whether to include additional bias to the layer
     **/
    fully_connected_layer(cnn_size_t in_dim, cnn_size_t out_dim, bool has_bias = true)
        : Base(std_input_order(has_bias)), in_size_(in_dim), out_size_(out_dim), has_bias_(has_bias) {}

    size_t fan_in_size() const override {
        return in_size_;
    }

    size_t fan_out_size() const override {
        return out_size_;
    }

    std::vector<index3d<cnn_size_t>> in_shape() const override {
        if (has_bias_) {
            return { index3d<cnn_size_t>(in_size_, 1, 1), index3d<cnn_size_t>(in_size_, out_size_, 1), index3d<cnn_size_t>(out_size_, 1, 1) };
        }
        else {
            return { index3d<cnn_size_t>(in_size_, 1, 1), index3d<cnn_size_t>(in_size_, out_size_, 1) };
        }
    }

    std::vector<index3d<cnn_size_t>> out_shape() const override {
        return { index3d<cnn_size_t>(out_size_, 1, 1), index3d<cnn_size_t>(out_size_, 1, 1) };
    }

    void forward_propagation(cnn_size_t index,
                             const std::vector<vec_t*>& in_data,
                             std::vector<vec_t*>& out_data) override {
        const vec_t& in  = *in_data[0];
        const vec_t& W   = *in_data[1];
        vec_t&       out = *out_data[0];
        vec_t&       a   = *out_data[1];

        CNN_UNREFERENCED_PARAMETER(index);

        for_i(parallelize_, out_size_, [&](int i) {
            a[i] = float_t(0);
            for (cnn_size_t c = 0; c < in_size_; c++) {
                a[i] += W[c*out_size_ + i] * in[c];
            }

            if (has_bias_) {
                vec_t& b = *in_data[2];
                a[i] += b[i];
            }
        });

        for_i(parallelize_, out_size_, [&](int i) {
            out[i] = h_.f(a, i);
        });
    }

    void back_propagation(cnn_size_t                index,
                          const std::vector<vec_t*>& in_data,
                          const std::vector<vec_t*>& out_data,
                          std::vector<vec_t*>&       out_grad,
                          std::vector<vec_t*>&       in_grad) override {
        const vec_t& prev_out   = *in_data[0];
        const vec_t& W          = *in_data[1];
        vec_t&       dW         = *in_grad[1];
        vec_t&       prev_delta = *in_grad[0];
        vec_t&       curr_delta = *out_grad[1];

        CNN_UNREFERENCED_PARAMETER(index);

        this->backward_activation(*out_grad[0], *out_data[0], curr_delta);

        for (cnn_size_t c = 0; c < this->in_size_; c++) {
            // propagate delta to previous layer
            // prev_delta[c] += current_delta[r] * W_[c * out_size_ + r]
            prev_delta[c] += vectorize::dot(&curr_delta[0], &W[c*out_size_], out_size_);
        }

        for_(parallelize_, 0, size_t(out_size_), [&](const blocked_range& r) {
            // accumulate weight-step using delta
            // dW[c * out_size + i] += current_delta[i] * prev_out[c]
            for (cnn_size_t c = 0; c < in_size_; c++)
                vectorize::muladd(&curr_delta[r.begin()], prev_out[c], r.end() - r.begin(), &dW[c*out_size_ + r.begin()]);

            if (has_bias_) {
                vec_t& db = *in_grad[2];
                for (int i = r.begin(); i < r.end(); i++)
                    db[i] += curr_delta[i];
            }
        });
    }

    std::string layer_type() const override { return "fully-connected"; }

protected:
    cnn_size_t in_size_;
    cnn_size_t out_size_;
    bool has_bias_;
};

} // namespace tiny_cnn
