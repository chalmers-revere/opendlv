/*
    Copyright (c) 2015, Taiga Nomi
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
#include "tiny_cnn/util/image.h"
#include "tiny_cnn/activations/activation_function.h"

namespace tiny_cnn {

/**
 * applies max-pooing operaton to the spatial data
 **/
template <typename Activation = activation::identity>
class max_unpooling_layer : public feedforward_layer<Activation> {
public:
    CNN_USE_LAYER_MEMBERS;
    typedef feedforward_layer<Activation> Base;

    /**
     * @param in_width     [in] width of input image
     * @param in_height    [in] height of input image
     * @param in_channels  [in] the number of input image channels(depth)
     * @param unpooling_size [in] factor by which to upscale
     **/
    max_unpooling_layer(cnn_size_t in_width,
                      cnn_size_t in_height,
                      cnn_size_t in_channels,
                      cnn_size_t unpooling_size)
        : Base({vector_type::data}),
        unpool_size_(unpooling_size),
        stride_(unpooling_size),
        in_(in_width, in_height, in_channels),
        out_(in_width * unpooling_size, in_height * unpooling_size, in_channels)
    {
        //set_worker_count(CNN_TASK_SIZE);
        init_connection();
    }

    /**
     * @param in_width     [in] width of input image
     * @param in_height    [in] height of input image
     * @param in_channels  [in] the number of input image channels(depth)
     * @param unpooling_size [in] factor by which to upscale
     * @param stride       [in] interval at which to apply the filters to the input
    **/
    max_unpooling_layer(cnn_size_t in_width,
                      cnn_size_t in_height,
                      cnn_size_t in_channels,
                      cnn_size_t unpooling_size,
                      cnn_size_t stride)
        : Base({vector_type::data}),
        unpool_size_(unpooling_size),
        stride_(stride),
        in_(in_width, in_height, in_channels),
        out_(unpool_out_dim(in_width, unpooling_size, stride), unpool_out_dim(in_height, unpooling_size, stride), in_channels)
    {
        //set_worker_count(CNN_TASK_SIZE);
        init_connection();
    }

    size_t fan_in_size() const override {
        return 1;
    }

    size_t fan_out_size() const override {
        return in2out_[0].size();
    }

    void forward_propagation(cnn_size_t index,
                             const std::vector<vec_t*>& in_data,
                             std::vector<vec_t*>&       out_data)  override {
        const vec_t& in  = *in_data[0];
        vec_t&       out = *out_data[0];
        vec_t&       a   = *out_data[1];
        std::vector<cnn_size_t>& max_idx = max_unpooling_layer_worker_storage_[index].in2outmax_;

        for_(parallelize_, 0, in2out_.size(), [&](const blocked_range& r) {
            for (int i = r.begin(); i < r.end(); i++) {
                const auto& in_index = out2in_[i];
                a[i] = (max_idx[in_index] == i) ? in[in_index] : float_t(0);
            }
        });

        for_i(parallelize_, out.size(), [&](int i) {
            out[i] = h_.f(a, i);
        });
    }

    void back_propagation(cnn_size_t                 index,
                          const std::vector<vec_t*>& in_data,
                          const std::vector<vec_t*>& out_data,
                          std::vector<vec_t*>&       out_grad,
                          std::vector<vec_t*>&       in_grad) override {
        vec_t&       prev_delta = *in_grad[0];
        vec_t&       curr_delta = *out_grad[1];
        std::vector<cnn_size_t>& max_idx = max_unpooling_layer_worker_storage_[index].in2outmax_;

        CNN_UNREFERENCED_PARAMETER(in_data);

        this->backward_activation(*out_grad[0], *out_data[0], curr_delta);

        for_(parallelize_, 0, in2out_.size(), [&](const blocked_range& r) {
            for (int i = r.begin(); i != r.end(); i++) {
                cnn_size_t outi = out2in_[i];
                prev_delta[i] = (max_idx[outi] == i) ? curr_delta[outi] : float_t(0);
            }
        });
    }

    /*void back_propagation_2nd(const std::vector<vec_t>& delta_in) override {
        const vec_t& current_delta2 = delta_in[0];
        const vec_t& prev_out = prev_->output(0);
        const activation::function& prev_h = prev_->activation_function();

        max_pooling_layer_worker_specific_storage& mws = max_pooling_layer_worker_storage_[0];

        for (cnn_size_t i = 0; i < in_size_; i++) {
            cnn_size_t outi = in2out_[i];
            prev_delta2_[i] = (mws.out2inmax_[outi] == i) ? current_delta2[outi] * sqr(prev_h.df(prev_out[i])) : float_t(0);
        }
    }*/

    std::vector<index3d<cnn_size_t>> in_shape() const override { return {in_}; }
    std::vector<index3d<cnn_size_t>> out_shape() const override { return {out_, out_}; }
    std::string layer_type() const override { return "max-unpool"; }
    size_t unpool_size() const {return unpool_size_;}

    virtual void set_worker_count(cnn_size_t worker_count) override {
        Base::set_worker_count(worker_count);
        max_unpooling_layer_worker_storage_.resize(worker_count);
        for (max_unpooling_layer_worker_specific_storage& mws : max_unpooling_layer_worker_storage_) {
            mws.in2outmax_.resize(out_.size());
        }
    }

private:
    size_t unpool_size_;
    size_t stride_;
    std::vector<cnn_size_t> out2in_; // mapping out => in (N:1)
    std::vector<std::vector<cnn_size_t> > in2out_; // mapping in => out (1:N)

    struct max_unpooling_layer_worker_specific_storage {
        std::vector<cnn_size_t> in2outmax_; // mapping max_index(out) => in (1:1)
    };

    std::vector<max_unpooling_layer_worker_specific_storage> max_unpooling_layer_worker_storage_;

    index3d<cnn_size_t> in_;
    index3d<cnn_size_t> out_;

    static cnn_size_t unpool_out_dim(cnn_size_t in_size, cnn_size_t unpooling_size, cnn_size_t stride) {
        return (int) (double)in_size * stride + unpooling_size - 1;
    }

    void connect_kernel(cnn_size_t unpooling_size, cnn_size_t inx, cnn_size_t iny, cnn_size_t  c)
    {
        cnn_size_t dxmax = static_cast<cnn_size_t>(std::min((size_t)unpooling_size, inx * stride_ - out_.width_));
        cnn_size_t dymax = static_cast<cnn_size_t>(std::min((size_t)unpooling_size, iny * stride_ - out_.height_));

        for (cnn_size_t dy = 0; dy < dymax; dy++) {
            for (cnn_size_t dx = 0; dx < dxmax; dx++) {
                cnn_size_t out_index = out_.get_index(static_cast<cnn_size_t>(inx * stride_ + dx),
                                                      static_cast<cnn_size_t>(iny * stride_ + dy), c);
                cnn_size_t in_index = in_.get_index(inx, iny, c);

                if (in_index >= in2out_.size())
                    throw nn_error("index overflow");
                if (out_index >= out2in_.size())
                    throw nn_error("index overflow");
                out2in_[out_index] = in_index;
                in2out_[in_index].push_back(out_index);
            }
        }
    }

    void init_connection()
    {
        in2out_.resize(in_.size());
        out2in_.resize(out_.size());

        for (max_unpooling_layer_worker_specific_storage& mws : max_unpooling_layer_worker_storage_) {
            mws.in2outmax_.resize(in_.size());
        }

        for (cnn_size_t c = 0; c < in_.depth_; ++c)
            for (cnn_size_t y = 0; y < in_.height_; ++y)
                for (cnn_size_t x = 0; x < in_.width_; ++x)
                    connect_kernel(static_cast<cnn_size_t>(unpool_size_),
                                   x, y, c);
    }

};

} // namespace tiny_cnn
