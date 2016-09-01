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
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <map>
#include <set>

#include "tiny_cnn/nodes.h"
#include "tiny_cnn/util/util.h"
#include "tiny_cnn/lossfunctions/loss_function.h"
#include "tiny_cnn/activations/activation_function.h"

namespace tiny_cnn {

struct result {
    result() : num_success(0), num_total(0) {}

    double accuracy() const {
        return num_success * 100.0 / num_total;
    }

    template <typename Char, typename CharTraits>
    void print_summary(std::basic_ostream<Char, CharTraits>& os) const {
        os << "accuracy:" << accuracy() << "% (" << num_success << "/" << num_total << ")" << std::endl;
    }

    template <typename Char, typename CharTraits>
    void print_detail(std::basic_ostream<Char, CharTraits>& os) {
        print_summary(os);
        auto all_labels = labels();

        os << std::setw(5) << "*" << " ";
        for (auto c : all_labels) 
            os << std::setw(5) << c << " ";
        os << std::endl;

        for (auto r : all_labels) {
            os << std::setw(5) << r << " ";           
            for (auto c : all_labels) 
                os << std::setw(5) << confusion_matrix[r][c] << " ";
            os << std::endl;
        }
    }

    std::set<label_t> labels() const {
        std::set<label_t> all_labels;
        for (auto r : confusion_matrix) {
            all_labels.insert(r.first);
            for (auto c : r.second)
                all_labels.insert(c.first);
        }
        return all_labels;
    }

    int num_success;
    int num_total;
    std::map<label_t, std::map<label_t, int> > confusion_matrix;
};

enum grad_check_mode {
    GRAD_CHECK_ALL, ///< check all elements of weights
    GRAD_CHECK_RANDOM ///< check 10 randomly selected weights
};

template <typename NetType>
class network;

template <typename Layer>
network<sequential>& operator << (network<sequential>& n, Layer&& l);

void construct_graph(network<graph>& graph, const std::vector<std::shared_ptr<layer>>& inputs, const std::vector<std::shared_ptr<layer>>& outputs);
void construct_graph(network<graph>& graph, const std::vector<layer*>& inputs, const std::vector<layer*>& outputs);

/**
 * A model of neural networks in tiny-cnn
 *
 * There are two types of network model available: sequential and graph.
 * A graph representation describe network as computational graph - 
 * each node of graph is layer, and each directed edge holds tensor and
 * its gradients. Sequential representation describe network as linked list -
 * each layer has at most one predecessor and one successor layer.
 *
 * Two types of network is represented as network<sequential> and network<graph> class.
 * These two classes have same API, except for its construction.
 *
 *     using namespace tiny_cnn;
 *     using namespace tiny_cnn::layers;
 *
 *     std::vector<vec_t> data;
 *     std::vector<label_t> label;
 *
 *     network<sequential> net("foo");
 *     std::cout << net.name(); // "foo"
 *
 *     // simply stack layers by operator <<
 *     net << fc<tan_h>(50, 200) << fc<tan_h>(200, 10);
 *
 *     // prepare optimizer
 *     adagrad opt;
 *
 *     // then train
 *     net.train<mse>(opt, data, label, 10, 20);
 *
 *
 * @param NetType specify the network is "sequential" or "graph".
 *                "sequential" means the network doesn't have any branch or merge pass.
 *                if the network has branch/merge, "graph" can be used.
 **/
template<typename NetType>
class network
{
public:
    typedef typename std::vector<layerptr_t>::iterator iterator;
    typedef typename std::vector<layerptr_t>::const_iterator const_iterator;

    explicit network(const std::string& name = "") : name_(name) {}

    /**
     * name of the network
     **/
    std::string  name() const           { return name_; }

    /**
     * explicitly initialize weights of all layers
     **/
    void         init_weight()          { net_.setup(true, 1); }

    /**
     * executes forward-propagation and returns output
     **/
    vec_t        predict(const vec_t& in) { return fprop(in); }

    /**
     * executes forward-propagation and returns output
     **/
    tensor_t predict(const tensor_t& in) { return fprop(in); }

    /**
     * executes forward-propagation and returns maximum output
     **/
    float_t      predict_max_value(const vec_t& in) {
        return fprop_max(in);
    }

    /**
     * executes forward-propagation and returns maximum output index
     **/
    label_t      predict_label(const vec_t& in) {
        return fprop_max_index(in);
    }

    /**
     * executes forward-propagation and returns output
     *
     * @param in input value range(double[], std::vector<double>, std::list<double> etc)
     **/
    template <typename Range>
    vec_t        predict(const Range& in) {
        using std::begin; // for ADL
        using std::end;
        return predict(vec_t(begin(in), end(in)));
    }


    /**
     * trains the network for a fixed number of epochs (for classification task)
     *
     * The difference between train and fit method is how to specify desired output.
     * This method takes label_t argument and convert to target vector automatically.
     * To train correctly, output dimension of last layer must be greater or equal to
     * number of label-ids.
     *
     * @param optimizer          optimizing algorithm for training
     * @param inputs             array of input data
     * @param class_labels       array of label-id for each input data(0-origin)
     * @param batch_size         number of samples per parameter update
     * @param epoch              number of training epochs
     * @param on_batch_enumerate callback for each mini-batch enumerate
     * @param on_epoch_enumerate callback for each epoch
     * @param reset_weights      set true if reset current network weights
     * @param n_threads          number of tasks
     * @param t_cost             target costs (leave to nullptr in order to assume equal cost for every target)
     */
    template <typename Error, typename Optimizer, typename OnBatchEnumerate, typename OnEpochEnumerate>
    bool train(Optimizer&                  optimizer,
               const std::vector<vec_t>&   inputs,
               const std::vector<label_t>& class_labels,
               size_t                      batch_size,
               int                         epoch,
               OnBatchEnumerate            on_batch_enumerate,
               OnEpochEnumerate            on_epoch_enumerate,             
               const bool                  reset_weights = false,
               const int                   n_threads = CNN_TASK_SIZE,
               const std::vector<vec_t>&   t_cost = std::vector<vec_t>()
               )
    {
        std::vector<tensor_t> input_tensor, output_tensor, t_cost_tensor;
        normalize_tensor(inputs, input_tensor);
        normalize_tensor(class_labels, output_tensor);
        if (!t_cost.empty()) normalize_tensor(t_cost, t_cost_tensor);

        return fit<Error>(optimizer, input_tensor, output_tensor, batch_size, epoch, on_batch_enumerate, on_epoch_enumerate, reset_weights, n_threads, t_cost_tensor);
    }

    /**
    * trains the network for a fixed number of epochs to generate desired output.
    *
    * This method execute fixed number of training steps and invoke callbacks for each mini-batch/epochs.
    * The network is trained to minimize given loss function(specified by template parameter).
    *
    * Shape of inputs and desired_outputs must be same to network inputs. For example, if your network
    * has 2 input layers that takes N dimensional array, for each element of inputs must be [2xN]
    * array.
    *
    * @code
    * network<sequential> net;
    * adagrad opt;
    *
    * net << layers::fc<tan_h>(2,3) << layers::fc<relu>(3,1);
    *
    * // 2training data, each data is float_t[2]
    * std::vector<vec_t> data { { 1, 0 }, { 0, 2 } };
    * std::vector<vec_t> out  {    { 2 },    { 1 } };
    *
    * net.fit<mse>(opt, data, out, 1, 1);
    *
    * // 2training data, each data is float_t[1][2]
    * // this form is also valid
    * std::vector<tensor_t> data2{ { { 1, 0 } }, { { 0, 2 } } };
    * std::vector<tensor_t> out2 { {    { 2 } }, {    { 1 } } };
    *
    * net.fit<mse>(opt, data2, out2, 1, 1);
    * @endcode
    *
    *
    * @param optimizer          optimizing algorithm for training
    * @param inputs             array of input data
    * @param desired_outputs    array of desired output
    * @param batch_size         number of samples per parameter update
    * @param epoch              number of training epochs
    * @param on_batch_enumerate callback for each mini-batch enumerate
    * @param on_epoch_enumerate callback for each epoch
    * @param reset_weights      set true if reset current network weights
    * @param n_threads          number of tasks
    * @param t_cost             target costs (leave to nullptr in order to assume equal cost for every target)
    */
    template <typename Error, typename Optimizer, typename OnBatchEnumerate, typename OnEpochEnumerate, typename T, typename U>
    bool fit(Optimizer&            optimizer,
             const std::vector<T>& inputs,
             const std::vector<U>& desired_outputs,
             size_t                batch_size,
             int                   epoch,
             OnBatchEnumerate      on_batch_enumerate,
             OnEpochEnumerate      on_epoch_enumerate,
             const bool            reset_weights = false,
             const int             n_threads = CNN_TASK_SIZE,
             const std::vector<U>& t_cost = std::vector<U>()
             )
    {
        std::vector<tensor_t> input_tensor, output_tensor, t_cost_tensor;
        normalize_tensor(inputs, input_tensor);
        normalize_tensor(desired_outputs, output_tensor);
        if (!t_cost.empty()) normalize_tensor(t_cost, t_cost_tensor);

        return fit<Error>(optimizer, input_tensor, output_tensor, batch_size, epoch, on_batch_enumerate, on_epoch_enumerate, reset_weights, n_threads, t_cost_tensor);
    }

    /**
     * @param optimizer          optimizing algorithm for training
     * @param inputs             array of input data
     * @param desired_outputs    array of desired output
     * @param batch_size         number of samples per parameter update
     * @param epoch              number of training epochs
     **/
    template<typename Error, typename Optimizer, typename T, typename U>
    bool fit(Optimizer&            optimizer,
             const std::vector<T>& inputs, 
             const std::vector<U>& desired_outputs,
             size_t                batch_size = 1,
             int                   epoch = 1) {
        return fit<Error>(optimizer, inputs, desired_outputs, batch_size, epoch, nop, nop);
    }

    /**
     * @param optimizer          optimizing algorithm for training
     * @param inputs             array of input data
     * @param class_labels       array of label-id for each input data(0-origin)
     * @param batch_size         number of samples per parameter update
     * @param epoch              number of training epochs
     **/
    template<typename Error, typename Optimizer>
    bool train(Optimizer&                  optimizer,
               const std::vector<vec_t>&   inputs,
               const std::vector<label_t>& class_labels,
               size_t                      batch_size = 1,
               int                         epoch = 1) {
        return train<Error>(optimizer, inputs, class_labels, batch_size, epoch, nop, nop);
    }

    /**
     * @deprecated use fit instead for regression task
     **/
    template<typename Error, typename Optimizer>
    bool train(Optimizer& optimizer, const std::vector<vec_t>& in, const std::vector<vec_t>& t, size_t batch_size = 1, int epoch = 1) {
        return fit<Error>(optimizer, in, t, batch_size, epoch, nop, nop);
    }

    /**
     * set the netphase to train or test
     * @param phase phase of network, could be train or test
     */
    void set_netphase(net_phase phase) {
        for (auto n : net_) {
            n->set_context(phase);
        }
    }

    /**
     * test and generate confusion-matrix for classification task
     **/
    result test(const std::vector<vec_t>& in, const std::vector<label_t>& t) {
        result test_result;
        set_netphase(net_phase::test);
        for (size_t i = 0; i < in.size(); i++) {
            const label_t predicted = fprop_max_index(in[i]);
            const label_t actual = t[i];

            if (predicted == actual) test_result.num_success++;
            test_result.num_total++;
            test_result.confusion_matrix[predicted][actual]++;
        }
        return test_result;
    }

    /**
     * generate output for each input
     **/
    std::vector<vec_t> test(const std::vector<vec_t>& in) {
        std::vector<vec_t> test_result(in.size());
        set_netphase(net_phase::test);
        for (size_t i = 0; i < in.size(); i++) {
            test_result[i] = predict(in[i]);
        }
        return test_result;
    }

    /**
     * calculate loss value (the smaller, the better) for regression task
     **/
    template <typename E>
    float_t get_loss(const std::vector<vec_t>& in, const std::vector<vec_t>& t) {
        float_t sum_loss = float_t(0);

        for (size_t i = 0; i < in.size(); i++) {
            const vec_t predicted = predict(in[i]);
            sum_loss += get_loss<E>(predicted, t[i]);
        }
        return sum_loss;
    }

    /**
     * calculate loss value (the smaller, the better) for regression task
     **/
    template <typename E, typename T>
    float_t get_loss(const std::vector<T>& in, const std::vector<tensor_t>& t) {
        float_t sum_loss = float_t(0);
        std::vector<tensor_t> in_tensor;
        normalize_tensor(in, in_tensor);

        for (size_t i = 0; i < in.size(); i++) {

            const tensor_t predicted = predict(in_tensor[i]);
            for (size_t j = 0; j < predicted.size(); j++)
                sum_loss += get_loss<E>(predicted[j], t[i][j]);
        }
        return sum_loss;
    }

    /**
     * save network weights into stream
     * @attention this saves only network *weights*, not network configuration
     **/
    void save(std::ostream& os) const {
        os.precision(std::numeric_limits<tiny_cnn::float_t>::digits10);
        net_.save(os);
    }

    /**
     * load network weights from stream
     * @attention this loads only network *weights*, not network configuration
     **/
    void load(std::istream& is) {
        is.precision(std::numeric_limits<tiny_cnn::float_t>::digits10);
        net_.load(is);
    }
    
    /**
     * load network weights from filepath, 30 times faster than stream reading
     * @attention this loads only network *weights*, not network configuration
     **/
    void fast_load(const char* filepath) {
		FILE* stream = fopen(filepath, "r");
		std::vector<double> data;
		double temp;
		while (fscanf(stream, "%lf", &temp) > 0)
			data.push_back(temp);
		fclose(stream);

		net_.load(data);
	}

    /**
    * checking gradients calculated by bprop
    * detail information:
    * http://ufldl.stanford.edu/wiki/index.php/Gradient_checking_and_advanced_optimization
    **/
    template <typename E>
    bool gradient_check(const vec_t* in, const label_t* t, int data_size, float_t eps, grad_check_mode mode) {
        std::vector<vec_t> v;
        net_.label2vec(t, data_size, &v);

        for (auto current : net_) { // ignore first input layer
            vec_t& w = *current->get_weights()[0];
            vec_t& b = *current->get_weights()[1];
            vec_t& dw = *current->get_weight_grads()[0];
            vec_t& db = *current->get_weight_grads()[1];

            if (w.empty()) continue;

            switch (mode) {
            case GRAD_CHECK_ALL:
                for (int i = 0; i < (int)w.size(); i++)
                    if (!calc_delta<E>(in, &v[0], data_size, w, dw, i, eps)) return false;
                for (int i = 0; i < (int)b.size(); i++)
                    if (!calc_delta<E>(in, &v[0], data_size, b, db, i, eps)) return false;
                break;
            case GRAD_CHECK_RANDOM:
                for (int i = 0; i < 10; i++)
                    if (!calc_delta<E>(in, &v[0], data_size, w, dw, uniform_idx(w), eps)) return false;
                for (int i = 0; i < 10; i++)
                    if (!calc_delta<E>(in, &v[0], data_size, b, db, uniform_idx(b), eps)) return false;
                break;
            default:
                throw nn_error("unknown grad-check type");
            }
        }
        return true;
    }

    /**
     * return number of layers
     **/
    size_t layer_size() const {
        return net_.size();
    }

    /**
     * @deprecated use layer_size() instread.
     **/
    size_t depth() const {
        return layer_size();
    }

    /**
     * return raw pointer of index-th layer
     **/
    const layer* operator [] (size_t index) const {
        return net_[index];
    }

    /**
     * return raw pointer of index-th layer
     **/
    layer* operator [] (size_t index) {
        return net_[index];
    }

    /**
     * return index-th layer as <T>
     * throw nn_error if index-th layer cannot be converted to T
     **/
    template <typename T>
    const T& at(size_t index) const {
        return net_.template at<T>(index);
    }
    
    /**
     * return total number of elements of output data
     **/
    cnn_size_t out_data_size() const {
        return net_.out_data_size();
    }

    /**
     * return total number of elements of input data
     */
    cnn_size_t in_data_size() const {
        return net_.in_data_size();
    }

    /**
    * set weight initializer to all layers
    **/
    template <typename WeightInit>
    network& weight_init(const WeightInit& f) {
        auto ptr = std::make_shared<WeightInit>(f);
        for (auto& l : net_)
            l->weight_init(ptr);
        return *this;
    }

    /**
    * set bias initializer to all layers
    **/
    template <typename BiasInit>
    network& bias_init(const BiasInit& f) {
        auto ptr = std::make_shared<BiasInit>(f);
        for (auto& l : net_)
            l->bias_init(ptr);
        return *this;
    }

    /**
     * returns if 2 networks have almost(<eps) the same weights
     **/
    template <typename T>
    bool has_same_weights(const network<T>& rhs, float_t eps) const {
        auto first1 = net_.begin();
        auto first2 = rhs.net_.begin();
        auto last1 = net_.end();
        auto last2 = rhs.net_.end();

        for (; first1 != last1 && first2 != last2; ++first1, ++first2)
            if (!(*first1)->has_same_weights(**first2, eps)) return false;
        return true;
    }

    iterator begin() { return net_.begin(); }
    iterator end() { return net_.end(); }
    const_iterator begin() const { return net_.begin(); }
    const_iterator end() const { return net_.end(); }

protected:
    float_t fprop_max(const vec_t& in, int idx = 0) {
        const vec_t& prediction = fprop(in, idx);
        return *std::max_element(std::begin(prediction), std::end(prediction));
    }

    label_t fprop_max_index(const vec_t& in, int idx = 0) {
        return label_t(max_index(fprop(in, idx)));
    }
private:

    template <typename Layer>
    friend network<sequential>& operator << (network<sequential>& n, Layer&& l);

    friend void construct_graph(network<graph>& graph, const std::vector<std::shared_ptr<layer>>& inputs, const std::vector<std::shared_ptr<layer>>& outputs);
    friend void construct_graph(network<graph>& graph, const std::vector<layer*>& inputs, const std::vector<layer*>& outputs);

    template <typename Error, typename Optimizer, typename OnBatchEnumerate, typename OnEpochEnumerate>
    bool fit(Optimizer&                   optimizer,
        const std::vector<tensor_t>& inputs,
        const std::vector<tensor_t>& desired_outputs,
        size_t                       batch_size,
        int                          epoch,
        OnBatchEnumerate             on_batch_enumerate,
        OnEpochEnumerate             on_epoch_enumerate,
        const bool                   reset_weights = false,
        const int                    n_threads = CNN_TASK_SIZE,
        const std::vector<tensor_t>& t_cost = std::vector<tensor_t>()
    )
    {
        //check_training_data(in, t);
        check_target_cost_matrix(desired_outputs, t_cost);
        set_netphase(net_phase::train);
        net_.setup(reset_weights, std::min(n_threads, (int)batch_size));

        for (auto n : net_)
            n->set_parallelize(batch_size < CNN_TASK_SIZE);
        optimizer.reset();
        for (int iter = 0; iter < epoch; iter++) {

            for (size_t i = 0; i < inputs.size(); i += batch_size) {
                train_once<Error>(optimizer, &inputs[i], &desired_outputs[i],
                    static_cast<int>(std::min(batch_size, inputs.size() - i)),
                    n_threads,
                    get_target_cost_sample_pointer(t_cost, i));
                on_batch_enumerate();

                //if (i % 100 == 0 && layers_.is_exploded()) {
                //    std::cout << "[Warning]Detected infinite value in weight. stop learning." << std::endl;
                //    return false;
                //}
            }
            on_epoch_enumerate();
        }
        set_netphase(net_phase::test);
        return true;
    }

    /**
     * train on one minibatch
     *
     * @param size is the number of data points to use in this batch
     */
    template <typename E, typename Optimizer>
    void train_once(Optimizer& optimizer,
                    const tensor_t* in,
                    const tensor_t* t,
                    int size,
                    const int nbThreads,
                    const tensor_t* t_cost) {
        if (size == 1) {
            bprop<E>(fprop(in[0]), t[0], 0, t_cost);
            net_.update_weights(&optimizer, 1, 1);
        } else {
            train_onebatch<E>(optimizer, in, t, size, nbThreads, t_cost);
        }
    }   

    /** 
     * trains on one minibatch, i.e. runs forward and backward propagation to calculate
     * the gradient of the loss function with respect to the network parameters (weights),
     * then calls the optimizer algorithm to update the weights
     *
     * @param batch_size the number of data points to use in this batch 
     */
    template <typename E, typename Optimizer>
    void train_onebatch(Optimizer&      optimizer,
                        const tensor_t* in,
                        const tensor_t* t,
                        int             batch_size,
                        const int       num_tasks,
                        const tensor_t* t_cost) {
        int num_threads = std::min(batch_size, num_tasks);

        net_.set_worker_count(num_threads);

        // number of data points to use in each thread
        int data_per_thread = (batch_size + num_threads - 1) / num_threads;

        // i is the thread / worker index
        for_i(num_threads, [&](int i) {
            int start_index = i * data_per_thread;
            int end_index = std::min(batch_size, start_index + data_per_thread);

            // loop over data points in this batch assigned to thread i
            for (int j = start_index; j < end_index; ++j)
                bprop<E>(fprop(in[j], i), t[j], i, t_cost ? &(t_cost[j]) : nullptr);
        }, 1);
        
        // merge all dW and update W by optimizer
        net_.update_weights(&optimizer, num_threads, batch_size);
    }

    vec_t fprop(const vec_t& in, int idx = 0) {
        if (in.size() != (size_t)in_data_size())
            data_mismatch(**net_.begin(), in);

        return net_.forward({in}, idx)[0];
    }

    std::vector<vec_t> fprop(const std::vector<vec_t>& in, int idx = 0) {
        return net_.forward(in, idx);
    }

    template <typename E>
    float_t get_loss(const vec_t& out, const vec_t& t) {
        float_t e = float_t(0);
        assert(out.size() == t.size());
        for(size_t i = 0; i < out.size(); i++){ e += E::f(out[i], t[i]); }
        return e;
    }

    template <typename E>
    bool calc_delta(const vec_t* in, const vec_t* v, int data_size, vec_t& w, vec_t& dw, int check_index, double eps) {
        static const float_t delta = 1e-10;

        std::fill(dw.begin(), dw.end(), float_t(0));

        // calculate dw/dE by numeric
        float_t prev_w = w[check_index];

        w[check_index] = prev_w + delta;
        float_t f_p = float_t(0);
        for (int i = 0; i < data_size; i++) { f_p += get_loss<E>(fprop(in[i]), v[i]); }

        float_t f_m = float_t(0);
        w[check_index] = prev_w - delta;
        for (int i = 0; i < data_size; i++) { f_m += get_loss<E>(fprop(in[i]), v[i]); }

        float_t delta_by_numerical = (f_p - f_m) / (float_t(2) * delta);
        w[check_index] = prev_w;

        // calculate dw/dE by bprop
        for (int i = 0; i < data_size; i++) {
            bprop<E>(fprop(tensor_t{in[i]}), tensor_t{v[i]}, 0, nullptr);
        }

        float_t delta_by_bprop = dw[check_index];
        net_.clear_grads(1);

        return std::abs(delta_by_bprop - delta_by_numerical) <= eps;
    }

    template <typename E>
    void bprop(const tensor_t& out, const tensor_t& t, int idx, const tensor_t* t_cost) {
        tensor_t delta = gradient<E>(out, t);

        if (t_cost) {
            for_i(delta.size(), [&](int i) {
                for (size_t j = 0; j < delta[i].size(); j++)
                    delta[i][j] *= (*t_cost)[i][j];
            });
        }

        net_.backward(delta, idx);
    }

    void check_t(size_t i, label_t t, cnn_size_t dim_out) {
        if (t >= dim_out) {
            std::ostringstream os;
            os << format_str("t[%u]=%u, dim(network output)=%u", i, t, dim_out) << std::endl;
            os << "in classification task, dim(network output) must be greater than max class id." << std::endl;
            if (dim_out == 1)
                os << std::endl << "(for regression, use vector<vec_t> instead of vector<label_t> for training signal)" << std::endl;

            throw nn_error("output dimension mismatch!\n " + os.str());
        }
    }

    void check_t(size_t i, const vec_t& t, cnn_size_t dim_out) {
        if (t.size() != dim_out)
            throw nn_error(format_str("output dimension mismatch!\n dim(target[%u])=%u, dim(network output size=%u", i, t.size(), dim_out));
    }

    template <typename T>
    void check_training_data(const std::vector<vec_t>& in, const std::vector<T>& t) {
        cnn_size_t dim_in = in_data_size();
        cnn_size_t dim_out = out_data_size();

        if (in.size() != t.size())
            throw nn_error("number of training data must be equal to label data");

        size_t num = in.size();

        for (size_t i = 0; i < num; i++) {
            if (in[i].size() != dim_in)
                throw nn_error(format_str("input dimension mismatch!\n dim(data[%u])=%d, dim(network input)=%u", i, in[i].size(), dim_in));

            check_t(i, t[i], dim_out);
        }
    }

    void check_target_cost_matrix(const std::vector<tensor_t>& t, const std::vector<tensor_t>& t_cost) {
        if (!t_cost.empty()) {
            if (t.size() != t_cost.size()) {
                throw nn_error("if target cost is supplied, its length must equal that of target data");
            }

            for (size_t i = 0, end = t.size(); i < end; i++) {
                check_target_cost_element(t[i], t_cost[i]);
            }
        }
    }

    // regression
    void check_target_cost_element(const vec_t& t, const vec_t& t_cost) {
        if (t.size() != t_cost.size()) {
            throw nn_error("if target cost is supplied for a regression task, its shape must be identical to the target data");
        }
    }
    void check_target_cost_element(const tensor_t& t, const tensor_t& t_cost) {
        if (t.size() != t_cost.size()) {
            throw nn_error("if target cost is supplied for a regression task, its shape must be identical to the target data");
        }
        for (size_t i = 0; i < t.size(); i++)
            check_target_cost_element(t[i], t_cost[i]);
    }

    const tensor_t* get_target_cost_sample_pointer(const std::vector<tensor_t>& t_cost, size_t i) {
        if (!t_cost.empty()) {
            assert(i < t_cost.size());
            return &(t_cost[i]);
        }
        else {
            return nullptr;
        }
    }

    void normalize_tensor(const std::vector<tensor_t>& inputs, std::vector<tensor_t>& normalized) {
        normalized = inputs;
    }

    void normalize_tensor(const std::vector<vec_t>& inputs, std::vector<tensor_t>& normalized) {
        normalized.reserve(inputs.size());
        for (size_t i = 0; i < inputs.size(); i++)
            normalized.emplace_back(tensor_t{ inputs[i] });
    }

    void normalize_tensor(const std::vector<label_t>& inputs, std::vector<tensor_t>& normalized) {
        std::vector<vec_t> vec;
        normalized.reserve(inputs.size());
        net_.label2vec(&inputs[0], inputs.size(), &vec);
        normalize_tensor(vec, normalized);
    }

    std::string name_;
    NetType net_;
};

/**
 * @brief [cut an image in samples to be tested (slow)]
 * @details [long description]
 * 
 * @param data [pointer to the data]
 * @param rows [self explained]
 * @param cols [self explained]
 * @param sizepatch [size of the patch, such as the total number of pixel in the patch is sizepatch*sizepatch ]
 * @return [vector of vec_c (sample) to be passed to test function]
 */
inline std::vector<vec_t> image2vec(const float_t* data, const unsigned int  rows, const unsigned int cols, const unsigned int sizepatch, const unsigned int step=1)
{
    assert(step>0);
    std::vector<vec_t> res((cols-sizepatch)*(rows-sizepatch)/(step*step),vec_t(sizepatch*sizepatch));
        for_i((cols-sizepatch)*(rows-sizepatch)/(step*step), [&](int count)
        {
            const int j = step*(count / ((cols-sizepatch)/step));
            const int i = step*(count % ((cols-sizepatch)/step));

            //vec_t sample(sizepatch*sizepatch);

            if (i+sizepatch < cols && j+sizepatch < rows)
            for (unsigned int k=0;k<sizepatch*sizepatch;k++)
            //for_i(sizepatch*sizepatch, [&](int k)
            {
                unsigned int y = k / sizepatch + j;
                unsigned int x = k % sizepatch + i;
                res[count][k] = data[x+y*cols];
            }
            //);
            //res[count] = (sample);
        });


    return res;
}

template <typename Layer>
network<sequential>& operator << (network<sequential>& n, Layer&& l) {
    n.net_.add(std::forward<Layer>(l));
    return n;
}

template <typename NetType, typename Char, typename CharTraits>
std::basic_ostream<Char, CharTraits>& operator << (std::basic_ostream<Char, CharTraits>& os, const network<NetType>& n) {
    n.save(os);
    return os;
}

template <typename NetType, typename Char, typename CharTraits>
std::basic_istream<Char, CharTraits>& operator >> (std::basic_istream<Char, CharTraits>& os, network<NetType>& n) {
    n.load(os);
    return os;
}

inline void construct_graph(network<graph>& graph, const std::vector<layer*>& inputs, const std::vector<layer*>& outputs)
{
    graph.net_.construct(inputs, outputs);
}

inline void construct_graph(network<graph>& graph, const std::vector<std::shared_ptr<layer>>& inputs, const std::vector<std::shared_ptr<layer>>& outputs)
{
    std::vector<layer*> in_ptr, out_ptr;
    auto shared2ptr = [](std::shared_ptr<layer> l) { return l.get(); };

    std::transform(inputs.begin(), inputs.end(), std::back_inserter(in_ptr), shared2ptr);
    std::transform(outputs.begin(), outputs.end(), std::back_inserter(out_ptr), shared2ptr);

    graph.net_.construct(in_ptr, out_ptr);
}

} // namespace tiny_cnn
