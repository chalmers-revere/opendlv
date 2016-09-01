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
#include <algorithm>

namespace tiny_cnn {

/**
 * applies dropout to the input
 **/
class dropout_layer : public layer {
public:
    typedef activation::identity Activation;
    typedef layer Base;

    /**
     * @param in_dim       [in] number of elements of the input
     * @param dropout_rate [in] (0-1) fraction of the input units to be dropped
     * @param phase        [in] initial state of the dropout
     **/
    dropout_layer(cnn_size_t in_dim, float_t dropout_rate, net_phase phase = net_phase::train)
        : Base({vector_type::data}, {vector_type::data}),
          phase_(phase),
          dropout_rate_(dropout_rate),
          scale_(float_t(1) / (float_t(1) - dropout_rate_)),
          in_size_(in_dim)
    {
        clear_mask();
    }

    dropout_layer(const dropout_layer& obj) = default;
    dropout_layer(dropout_layer&& obj) = default;
    virtual ~dropout_layer(){}
    dropout_layer& operator=(const dropout_layer& obj) = default;
    dropout_layer& operator=(dropout_layer&& obj) = default;

    void set_dropout_rate(float_t rate)
    {
        dropout_rate_ = rate;
        scale_ = float_t(1) / (float_t(1) - dropout_rate_);
    }

    ///< number of incoming connections for each output unit
    size_t fan_in_size() const override
    {
        return 1;
    }

    ///< number of outgoing connections for each input unit
    size_t fan_out_size() const override
    {
        return 1;
    }

    std::vector<index3d<cnn_size_t>> in_shape() const override {
        return{ index3d<cnn_size_t>(in_size_, 1, 1) };
    }

    std::vector<index3d<cnn_size_t>> out_shape() const override {
        return{ index3d<cnn_size_t>(in_size_, 1, 1) };
    }

    void back_propagation(cnn_size_t                 index,
                          const std::vector<vec_t*>& in_data,
                          const std::vector<vec_t*>& out_data,
                          std::vector<vec_t*>&       out_grad,
                          std::vector<vec_t*>&       in_grad) override {
        vec_t&       prev_delta = *in_grad[0];
        const vec_t& curr_delta = *out_grad[0];
        const std::vector<uint8_t>& mask = dropout_layer_worker_storage_[index].mask_;

        CNN_UNREFERENCED_PARAMETER(in_data);
        CNN_UNREFERENCED_PARAMETER(out_data);

        for (size_t i = 0; i < curr_delta.size(); i++) {
            prev_delta[i] = mask[i] * curr_delta[i];
        }
    }

    void forward_propagation(cnn_size_t index,
                             const std::vector<vec_t*>& in_data,
                             std::vector<vec_t*>& out_data) override {
        const vec_t& in  = *in_data[0];
        vec_t&       out = *out_data[0];

        std::vector<uint8_t>& mask = dropout_layer_worker_storage_[index].mask_;

        if (phase_ == net_phase::train) {
            for (size_t i = 0; i < in.size(); i++)
                mask[i] = bernoulli(dropout_rate_);

            for (size_t i = 0; i < in.size(); i++)
                out[i] = mask[i] * scale_ * in[i];
        }
        else {
            for (size_t i = 0; i < in.size(); i++)
                out[i] = in[i];
        }
    }

    /**
     * set dropout-context (training-phase or test-phase)
     **/
    void set_context(net_phase ctx) override
    {
        phase_ = ctx;
    }

    std::string layer_type() const override { return "dropout"; }

    const std::vector<uint8_t>& get_mask(cnn_size_t worker_index) const {
        return dropout_layer_worker_storage_[worker_index].mask_;
    }

    virtual void set_worker_count(cnn_size_t worker_count) override {
        Base::set_worker_count(worker_count);
        dropout_layer_worker_storage_.resize(worker_count);

        for (dropout_layer_worker_specific_storage& dws : dropout_layer_worker_storage_) {
            dws.mask_.resize(in_size_);
        }
    }

    void clear_mask() {
        for (dropout_layer_worker_specific_storage& dws : dropout_layer_worker_storage_) {
            std::fill(dws.mask_.begin(), dws.mask_.end(), 0);
        }
    }

private:
    net_phase phase_;
    float_t dropout_rate_;
    float_t scale_;
    cnn_size_t in_size_;

    struct dropout_layer_worker_specific_storage {
        // binary mask, but use uint8 instead of bool to avoid the std::vector specialization for bools
        // (though it would be a good idea to profile which is actually better)
        std::vector<uint8_t> mask_;
    };

    std::vector<dropout_layer_worker_specific_storage> dropout_layer_worker_storage_;
};

} // namespace tiny_cnn
