/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "tiny_dnn/tiny_dnn.h"

void sample1_convnet(const string& data_dir = "../data");
void sample2_mlp(const string& data_dir = "../data");
void sample3_dae();
void sample4_dropout(const string& data_dir = "../data");
void sample5_unbalanced_training_data(const string& data_dir = "../data");
void sample6_graph();

int main(int argc, char** argv) {
  try {
    if (argc == 2) {
      sample1_convnet(argv[1]);
    } else {
      sample1_convnet();
    }
  } catch (const nn_error& e) {
    std::cout << e.what() << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////
// learning convolutional neural networks (LeNet-5 like architecture)
void sample1_convnet(const std::string& data_dir) {
  // construct LeNet-5 architecture
  tiny_dnn::network<sequential> nn;
  tiny_dnn::adagrad optimizer;

// connection table [Y.Lecun, 1998 Table.1]
#define O true
#define X false
  // clang-format off
static const bool connection[] = {
  O, X, X, X, O, O, O, X, X, O, O, O, O, X, O, O,
  O, O, X, X, X, O, O, O, X, X, O, O, O, O, X, O,
  O, O, O, X, X, X, O, O, O, X, X, O, X, O, O, O,
  X, O, O, O, X, X, O, O, O, O, X, X, O, X, O, O,
  X, X, O, O, O, X, X, O, O, O, O, X, O, O, X, O,
  X, X, X, O, O, O, X, X, O, O, O, O, X, O, O, O
};
// clang-format on
#undef O
#undef X

  using conv     = tiny_dnn::layers::conv;
  using ave_pool = tiny_dnn::layers::ave_pool;
  using fc       = tiny_dnn::layers::fc;
  using tanh     = tiny_dnn::activation::tanh;
  using tiny_dnn::core::connection_table;

  nn << conv(32, 32, 5, 1, 6) /* 32x32 in, 5x5 kernel, 1-6 fmaps conv */
     << tanh(28, 28, 6)
     << ave_pool(28, 28, 6, 2) /* 28x28 in, 6 fmaps, 2x2 subsampling */
     << tanh(14, 14, 6)
     << conv(14, 14, 5, 6, 16, connection_table(connection, 6, 16))
     << tanh(10, 10, 16) << ave_pool(10, 10, 16, 2) << tanh(5, 5, 16)
     << conv(5, 5, 5, 16, 120) << tanh(1, 1, 120) << fc(120, 10) << tanh(10);

  std::cout << "load models..." << std::endl;

  // load MNIST dataset
  std::vector<label_t> train_labels, test_labels;
  std::vector<vec_t> train_images, test_images;

  std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
  std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
  std::string test_labels_path  = data_dir + "/t10k-labels.idx1-ubyte";
  std::string test_images_path  = data_dir + "/t10k-images.idx3-ubyte";

  parse_mnist_labels(train_labels_path, &train_labels);
  parse_mnist_images(train_images_path, &train_images, -1.0, 1.0, 2, 2);
  parse_mnist_labels(test_labels_path, &test_labels);
  parse_mnist_images(test_images_path, &test_images, -1.0, 1.0, 2, 2);

  std::cout << "start learning" << std::endl;

  progress_display disp(train_images.size());
  timer t;
  int minibatch_size = 10;

  optimizer.alpha *= std::sqrt(minibatch_size);

  // create callback
  auto on_enumerate_epoch = [&]() {
    std::cout << t.elapsed() << "s elapsed." << std::endl;

    tiny_dnn::result res = nn.test(test_images, test_labels);

    std::cout << res.num_success << "/" << res.num_total << std::endl;

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_minibatch = [&]() { disp += minibatch_size; };

  // training
  nn.train<mse>(optimizer, train_images, train_labels, minibatch_size, 20,
                on_enumerate_minibatch, on_enumerate_epoch);

  std::cout << "end training." << std::endl;

  // test and show results
  nn.test(test_images, test_labels).print_detail(std::cout);

  // save networks
  std::ofstream ofs("LeNet-weights");
  ofs << nn;
}

///////////////////////////////////////////////////////////////////////////////
// learning 3-Layer Networks
void sample2_mlp(const string& data_dir) {
  const size_t num_hidden_units = 500;

#if defined(_MSC_VER) && _MSC_VER < 1800
  // initializer-list is not supported
  int num_units[] = {28 * 28, num_hidden_units, 10};
  auto nn         = make_mlp<tanh_layer>(num_units, num_units + 3);
#else
  auto nn = make_mlp<tanh_layer>({28 * 28, num_hidden_units, 10});
#endif
  gradient_descent optimizer;

  // load MNIST dataset
  std::vector<label_t> train_labels, test_labels;
  std::vector<vec_t> train_images, test_images;

  std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
  std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
  std::string test_labels_path  = data_dir + "/t10k-labels.idx1-ubyte";
  std::string test_images_path  = data_dir + "/t10k-images.idx3-ubyte";

  parse_mnist_labels(train_labels_path, &train_labels);
  parse_mnist_images(train_images_path, &train_images, -1.0, 1.0, 0, 0);
  parse_mnist_labels(test_labels_path, &test_labels);
  parse_mnist_images(test_images_path, &test_images, -1.0, 1.0, 0, 0);

  optimizer.alpha = 0.001;

  progress_display disp(train_images.size());
  timer t;

  // create callback
  auto on_enumerate_epoch = [&]() {
    std::cout << t.elapsed() << "s elapsed." << std::endl;

    tiny_dnn::result res = nn.test(test_images, test_labels);

    std::cout << optimizer.alpha << "," << res.num_success << "/"
              << res.num_total << std::endl;

    optimizer.alpha *= 0.85;  // decay learning rate
    optimizer.alpha = std::max((tiny_dnn::float_t)0.00001, optimizer.alpha);

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_data = [&]() { ++disp; };

  nn.train<mse>(optimizer, train_images, train_labels, 1, 20, on_enumerate_data,
                on_enumerate_epoch);
}

///////////////////////////////////////////////////////////////////////////////
// denoising auto-encoder
void sample3_dae() {
#if defined(_MSC_VER) && _MSC_VER < 1800
  // initializer-list is not supported
  int num_units[] = {100, 400, 100};
  auto nn         = make_mlp<tanh>(num_units, num_units + 3);
#else
  auto nn = make_mlp<tanh_layer>({100, 400, 100});
#endif

  std::vector<vec_t> train_data_original;

  // load train-data

  std::vector<vec_t> train_data_corrupted(train_data_original);

  for (auto& d : train_data_corrupted) {
    d = corrupt(move(d), 0.1, 0.0);  // corrupt 10% data
  }

  gradient_descent optimizer;

  // learning 100-400-100 denoising auto-encoder
  nn.train<mse>(optimizer, train_data_corrupted, train_data_original);
}

///////////////////////////////////////////////////////////////////////////////
// dropout-learning

void sample4_dropout(const string& data_dir) {
  using network = network<sequential>;
  network nn;
  size_t input_dim    = 28 * 28;
  size_t hidden_units = 800;
  size_t output_dim   = 10;
  gradient_descent optimizer;

  fully_connected_layer f1(input_dim, hidden_units);
  tanh_layer th1(hidden_units);
  dropout_layer dropout(hidden_units, 0.5);
  fully_connected_layer f2(hidden_units, output_dim);
  tanh_layer th2(output_dim);
  nn << f1 << th1 << dropout << f2 << th2;

  optimizer.alpha  = 0.003;  // TODO(nyanp): not optimized
  optimizer.lambda = 0.0;

  // load MNIST dataset
  std::vector<label_t> train_labels, test_labels;
  std::vector<vec_t> train_images, test_images;

  std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
  std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
  std::string test_labels_path  = data_dir + "/t10k-labels.idx1-ubyte";
  std::string test_images_path  = data_dir + "/t10k-images.idx3-ubyte";

  parse_mnist_labels(train_labels_path, &train_labels);
  parse_mnist_images(train_images_path, &train_images, -1.0, 1.0, 0, 0);
  parse_mnist_labels(test_labels_path, &test_labels);
  parse_mnist_images(test_images_path, &test_images, -1.0, 1.0, 0, 0);

  // load train-data, label_data
  progress_display disp(train_images.size());
  timer t;

  // create callback
  auto on_enumerate_epoch = [&]() {
    std::cout << t.elapsed() << "s elapsed." << std::endl;

    dropout.set_context(net_phase::test);
    tiny_dnn::result res = nn.test(test_images, test_labels);
    dropout.set_context(net_phase::train);

    std::cout << optimizer.alpha << "," << res.num_success << "/"
              << res.num_total << std::endl;

    optimizer.alpha *= 0.99;  // decay learning rate
    optimizer.alpha = std::max((tiny_dnn::float_t)0.00001, optimizer.alpha);

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_data = [&]() { ++disp; };

  nn.train<mse>(optimizer, train_images, train_labels, 1, 100,
                on_enumerate_data, on_enumerate_epoch);

  // change context to enable all hidden-units
  // f1.set_context(dropout::test_phase);
  // std::cout << res.num_success << "/" << res.num_total << std::endl;
}

#include "tiny_dnn/util/target_cost.h"

///////////////////////////////////////////////////////////////////////////////
// learning unbalanced training data

void sample5_unbalanced_training_data(const string& data_dir) {
  // keep the network relatively simple
  const size_t num_hidden_units = 20;
  auto nn_standard = make_mlp<tanh_layer>({28 * 28, num_hidden_units, 10});
  auto nn_balanced = make_mlp<tanh_layer>({28 * 28, num_hidden_units, 10});
  gradient_descent optimizer;

  // load MNIST dataset
  std::vector<label_t> train_labels, test_labels;
  std::vector<vec_t> train_images, test_images;

  std::string train_labels_path = data_dir + "/train-labels.idx1-ubyte";
  std::string train_images_path = data_dir + "/train-images.idx3-ubyte";
  std::string test_labels_path  = data_dir + "/t10k-labels.idx1-ubyte";
  std::string test_images_path  = data_dir + "/t10k-images.idx3-ubyte";

  parse_mnist_labels(train_labels_path, &train_labels);
  parse_mnist_images(train_images_path, &train_images, -1.0, 1.0, 0, 0);
  parse_mnist_labels(test_labels_path, &test_labels);
  parse_mnist_images(test_images_path, &test_images, -1.0, 1.0, 0, 0);

  {  // create an unbalanced training set
    std::vector<label_t> train_labels_unbalanced;
    std::vector<vec_t> train_images_unbalanced;
    train_labels_unbalanced.reserve(train_labels.size());
    train_images_unbalanced.reserve(train_images.size());

    for (size_t i = 0, end = train_labels.size(); i < end; ++i) {
      const label_t label = train_labels[i];

      // drop most 0s, 1s, 2s, 3s, and 4s
      const bool keep_sample = label >= 5 || bernoulli(0.005);

      if (keep_sample) {
        train_labels_unbalanced.push_back(label);
        train_images_unbalanced.push_back(train_images[i]);
      }
    }

    // keep the newly created unbalanced training set
    std::swap(train_labels, train_labels_unbalanced);
    std::swap(train_images, train_images_unbalanced);
  }

  optimizer.alpha = 0.001;

  progress_display disp(train_images.size());
  timer t;

  const int minibatch_size = 10;

  auto nn = &nn_standard;  // the network referred to by the callbacks

  // create callbacks - as usual
  auto on_enumerate_epoch = [&]() {
    std::cout << t.elapsed() << "s elapsed." << std::endl;

    tiny_dnn::result res = nn->test(test_images, test_labels);

    std::cout << optimizer.alpha << "," << res.num_success << "/"
              << res.num_total << std::endl;

    optimizer.alpha *= 0.85;  // decay learning rate
    optimizer.alpha =
      std::max(static_cast<tiny_dnn::float_t>(0.00001), optimizer.alpha);

    disp.restart(train_images.size());
    t.restart();
  };

  auto on_enumerate_data = [&]() { disp += minibatch_size; };

  // first train the standard network (default cost - equal for each sample)
  // - note that it does not learn the classes 0-4
  nn_standard.train<mse>(optimizer, train_images, train_labels, minibatch_size,
                         20, on_enumerate_data, on_enumerate_epoch, true,
                         CNN_TASK_SIZE);

  // then train another network, now with explicitly
  // supplied target costs (aim: a more balanced predictor)
  // - note that it can learn the classes 0-4 (at least somehow)
  nn                     = &nn_balanced;
  optimizer              = gradient_descent();
  const auto target_cost = create_balanced_target_cost(train_labels, 0.8);
  nn_balanced.train<mse>(optimizer, train_images, train_labels, minibatch_size,
                         20, on_enumerate_data, on_enumerate_epoch, true,
                         CNN_TASK_SIZE, target_cost);

  // test and show results
  std::cout << "\nStandard training (implicitly assumed equal "
            << "cost for every sample):\n";
  nn_standard.test(test_images, test_labels).print_detail(std::cout);

  std::cout << "\nBalanced training "
            << "(explicitly supplied target costs):\n";
  nn_balanced.test(test_images, test_labels).print_detail(std::cout);
}

void sample6_graph() {
  // declare node
  auto in1   = std::make_shared<input_layer>(shape3d(3, 1, 1));
  auto in2   = std::make_shared<input_layer>(shape3d(3, 1, 1));
  auto added = std::make_shared<add>(2, 3);
  auto out   = std::make_shared<linear_layer>(3);

  // connect
  (in1, in2) << added;
  added << out;

  network<graph> net;
  construct_graph(net, {in1, in2}, {out});

  auto res = net.predict({{2, 4, 3}, {-1, 2, -5}})[0];

  // relu({2,4,3} + {-1,2,-5}) = {1,6,0}
  std::cout << res[0] << "," << res[1] << "," << res[2] << std::endl;
}
