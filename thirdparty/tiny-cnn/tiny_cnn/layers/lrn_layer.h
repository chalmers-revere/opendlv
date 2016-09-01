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
#include <algorithm>

namespace tiny_cnn {

    enum class norm_region {
        across_channels,
        within_channels
    };

/**
 * local response normalization
 */
template<typename Activation>
class lrn_layer : public feedforward_layer<Activation> {
public:
    CNN_USE_LAYER_MEMBERS;

    typedef feedforward_layer<Activation> Base;

    /**
    * @param layer       [in] the previous layer connected to this
    * @param local_size  [in] the number of channels(depths) to sum over
    * @param in_channels [in] the number of channels of input data
    * @param alpha       [in] the scaling parameter (same to caffe's LRN)
    * @param beta        [in] the scaling parameter (same to caffe's LRN)
    **/
    lrn_layer(layer*      prev,
              cnn_size_t  local_size,
              float_t     alpha = 1.0,
              float_t     beta  = 5.0,
              norm_region region = norm_region::across_channels)
        : Base({ vector_type::data }),
          in_shape_(prev->out_data_shape()[0]),
          size_(local_size),
          alpha_(alpha),
          beta_(beta),
          region_(region),
          in_square_(in_shape_.area()) {
    }

    /**
     * @param in_width    [in] the width of input data
     * @param in_height   [in] the height of input data
     * @param local_size  [in] the number of channels(depths) to sum over
     * @param in_channels [in] the number of channels of input data
     * @param alpha       [in] the scaling parameter (same to caffe's LRN)
     * @param beta        [in] the scaling parameter (same to caffe's LRN)
     **/
    lrn_layer(cnn_size_t  in_width,
              cnn_size_t  in_height,
              cnn_size_t  local_size,
              cnn_size_t  in_channels,
              float_t     alpha = 1.0,
              float_t     beta  = 5.0,
              norm_region region = norm_region::across_channels)
        : Base({vector_type::data}),
          in_shape_(in_width, in_height, in_channels),
          size_(local_size),
          alpha_(alpha),
          beta_(beta),
          region_(region),
          in_square_(in_shape_.area()) {
    }

    size_t fan_in_size() const override {
        return size_;
    }

    size_t fan_out_size() const override {
        return size_;
    }

    std::vector<shape3d> in_shape() const override {
        return { in_shape_ };
    }

    std::vector<shape3d> out_shape() const override {
        return { in_shape_, in_shape_ };
    }

    std::string layer_type() const override { return "norm"; }

    void forward_propagation(cnn_size_t index,
                            const std::vector<vec_t*>& in_data,
                            std::vector<vec_t*>& out_data) override {
        vec_t& out = *out_data[0];
        vec_t& a   = *out_data[1];

        CNN_UNREFERENCED_PARAMETER(index);

        if (region_ == norm_region::across_channels) {
            forward_across(*in_data[0], a);
        }
        else {
            forward_within(*in_data[0], a);
        }

        for_i(parallelize_, out.size(), [&](int i) {
            out[i] = h_.f(a, i);
        });
    }

    void back_propagation(cnn_size_t                 index,
                          const std::vector<vec_t*>& in_data,
                          const std::vector<vec_t*>& out_data,
                          std::vector<vec_t*>&       out_grad,
                          std::vector<vec_t*>&       in_grad) override {
        CNN_UNREFERENCED_PARAMETER(index);
        CNN_UNREFERENCED_PARAMETER(in_data);
        CNN_UNREFERENCED_PARAMETER(out_data);
        CNN_UNREFERENCED_PARAMETER(out_grad);
        CNN_UNREFERENCED_PARAMETER(in_grad);
        throw nn_error("not implemented");
    }

private:
    void forward_across(const vec_t& in, vec_t& out) {
        std::fill(in_square_.begin(), in_square_.end(), float_t(0));

        for (cnn_size_t i = 0; i < size_ / 2; i++) {
            cnn_size_t idx = in_shape_.get_index(0, 0, i);
            add_square_sum(&in[idx], in_shape_.area(), &in_square_[0]);
        }

        cnn_size_t head = size_ / 2;
        long tail = ((long) head) - size_;
        cnn_size_t channels = in_shape_.depth_;
        const cnn_size_t wxh = in_shape_.area();
        const float_t alpha_div_size = alpha_ / size_;

        for (cnn_size_t i = 0; i < channels; i++, head++, tail++) {
            if (head < channels)
                add_square_sum(&in[in_shape_.get_index(0, 0, head)], wxh, &in_square_[0]);

            if (tail >= 0)
                sub_square_sum(&in[in_shape_.get_index(0, 0, tail)], wxh, &in_square_[0]);

            float_t *dst = &out[in_shape_.get_index(0, 0, i)];
            const float_t *src = &in[in_shape_.get_index(0, 0, i)];
            for (cnn_size_t j = 0; j < wxh; j++)
                dst[j] = src[j] * std::pow(float_t(1) + alpha_div_size * in_square_[j], -beta_);
        }
    }

    void forward_within(const vec_t& in, vec_t& out) {
        CNN_UNREFERENCED_PARAMETER(in);
        CNN_UNREFERENCED_PARAMETER(out);
        throw nn_error("not implemented");
    }

    void add_square_sum(const float_t *src, cnn_size_t size, float_t *dst) {
        for (cnn_size_t i = 0; i < size; i++)
            dst[i] += src[i] * src[i];
    }

    void sub_square_sum(const float_t *src, cnn_size_t size, float_t *dst) {
        for (cnn_size_t i = 0; i < size; i++)
            dst[i] -= src[i] * src[i];
    }

    shape3d in_shape_;

    cnn_size_t size_;
    float_t alpha_, beta_;
    norm_region region_;

    vec_t in_square_;
};

} // namespace tiny_cnn
