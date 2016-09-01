tiny-cnn: A header-only, dependency-free deep learning framework for C++11
========

| **Linux/Mac OS** | **Windows** |
|------------------|-------------|
|[![Build Status](https://travis-ci.org/nyanp/tiny-cnn.svg?branch=master)](https://travis-ci.org/nyanp/tiny-cnn)|[![Build status](https://ci.appveyor.com/api/projects/status/s4mow1544tvoqeeu?svg=true)](https://ci.appveyor.com/project/nyanp/tiny-cnn)|

tiny-cnn is a C++11 implementation of deep learning. It is suitable for deep learning on limited computational resource, embedded systems and IoT devices.

* [Features](#features)
* [Comparison with other libraries](#comparison-with-other-libraries)
* [Supported networks](#supported-networks)
* [Dependencies](#dependencies)
* [Build](#build)
* [Examples](#examples)
* [References](#references)
* [License](#license)
* [Mailing list](#mailing-list)

Check out the [documentation](doc/readme.md) for more info.

## What's New
- [tiny-cnn v0.1.0 released!](https://github.com/nyanp/tiny-cnn/releases/tag/v0.1.0)

## Features
- reasonably fast, without GPU
    - with TBB threading and SSE/AVX vectorization
    - 98.8% accuracy on MNIST in 13 minutes training (@Core i7-3520M)
- portable & header-only
    - Run anywhere as long as you have a compiler which supports C++11
    - Just include tiny_cnn.h and write your model in C++. There is nothing to install.
- easy to integrate with real applications
    - no output to stdout/stderr
    - a constant throughput (simple parallelization model, no garbage collection)
    - work without throwing an exception
    - [can import caffe's model](https://github.com/nyanp/tiny-cnn/tree/master/examples/caffe_converter)
- simply implemented
    - be a good library for learning neural networks

## Comparison with other libraries

||tiny-cnn|[caffe](https://github.com/BVLC/caffe)|[Theano](https://github.com/Theano/Theano)|[TensorFlow](https://www.tensorflow.org/)|
|---|---|---|---|---|
|Prerequisites|__Nothing__(Optional:TBB,OpenMP)|BLAS,Boost,protobuf,glog,gflags,hdf5, (Optional:CUDA,OpenCV,lmdb,leveldb etc)|Numpy,Scipy,BLAS,(optional:nose,Sphinx,CUDA etc)|numpy,six,protobuf,(optional:CUDA,Bazel)|
|Modeling By|C++ code|Config File|Python Code|Python Code|
|GPU Support|No|Yes|Yes|Yes|
|Installing|Unnecessary|Necessary|Necessary|Necessary|
|Windows Support|Yes|No*|Yes|No*|
|Pre-Trained Model|Yes(via caffe-converter)|Yes|No*|No*|

*unofficial version is available

## Supported networks
### layer-types
* fully-connected layer
* convolutional layer
* average pooling layer
* max-pooling layer
* contrast normalization layer
* dropout layer
* linear operation layer
* deconvolution layer
* unpooling layer
* elementwise-add layer
* concat layer

### activation functions
* tanh
* sigmoid
* softmax
* rectified linear(relu)
* leaky relu
* identity
* exponential linear units(elu)

### loss functions
* cross-entropy
* mean-squared-error

### optimization algorithms
* stochastic gradient descent (with/without L2 normalization and momentum)
* stochastic gradient levenberg marquardt
* adagrad
* rmsprop
* adam

## Dependencies
##### Minimum requirements
Nothing. All you need is a C++11 compiler.

##### Requirements to build sample/test programs
[OpenCV](http://opencv.org/)

## Build
tiny-cnn is header-ony, so *there's nothing to build*. If you want to execute sample program or unit tests, you need to install [cmake](https://cmake.org/) and type the following commands:

```
cmake .
```

Then open .sln file in visual studio and build(on windows/msvc), or type ```make``` command(on linux/mac/windows-mingw).

Some cmake options are available:

|options|description|default|additional requirements to use|
|-----|-----|----|----|
|USE_TBB|Use [Intel TBB](https://www.threadingbuildingblocks.org/) for parallelization|OFF*|[Intel TBB](https://www.threadingbuildingblocks.org/)|
|USE_OMP|Use OpenMP for parallelization|OFF*|[OpenMP Compiler](http://openmp.org/wp/openmp-compilers/)|
|USE_SSE|Use Intel SSE instruction set|ON|Intel CPU which supports SSE|
|USE_AVX|Use Intel AVX instruction set|ON|Intel CPU which supports AVX|
|USE_OPENCV|Use OpenCV for sample/test programs|ON|[Open Source Computer Vision Library](http://opencv.org/)|
|BUILD_TESTS|Build unist tests|OFF|-**|
|BUILD_EXAMPLES|Build example projects|ON|-|
|BUILD_DOCS|Build documentation|OFF|[Doxygen](http://www.doxygen.org/)|

*tiny-cnn use c++11 standard library for parallelization by default
**to build tests, type `git submodule update --init` before build

For example, type the following commands if you want to use intel TBB and build tests:
```bash
cmake -DUSE_TBB=ON -DBUILD_EXAMPLES=ON .
```

## Customize configurations
You can edit include/config.h to customize default behavior.

## Examples
construct convolutional neural networks

```cpp
#include "tiny_cnn/tiny_cnn.h"
using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace tiny_cnn::layers;

void construct_cnn() {
    using namespace tiny_cnn;

    network<sequential> net;

    // add layers
    net << conv<tan_h>(32, 32, 5, 1, 6)  // in:32x32x1, 5x5conv, 6fmaps
        << ave_pool<tan_h>(28, 28, 6, 2) // in:28x28x6, 2x2pooling
        << fc<tan_h>(14 * 14 * 6, 120)   // in:14x14x6, out:120
        << fc<identity>(120, 10);        // in:120,     out:10

    assert(net.in_data_size() == 32 * 32);
    assert(net.out_data_size() == 10);

    // load MNIST dataset
    std::vector<label_t> train_labels;
    std::vector<vec_t> train_images;

    parse_mnist_labels("train-labels.idx1-ubyte", &train_labels);
    parse_mnist_images("train-images.idx3-ubyte", &train_images, -1.0, 1.0, 2, 2);

    // declare optimization algorithm
    adagrad optimizer;

    // train (50-epoch, 30-minibatch)
    net.train<mse>(optimizer, train_images, train_labels, 30, 50);

    // save
    std::ofstream ofs("weights");
    ofs << net;

    // load
    // std::ifstream ifs("weights");
    // ifs >> net;
}
```
construct multi-layer perceptron(mlp)

```cpp
#include "tiny_cnn/tiny_cnn.h"
using namespace tiny_cnn;
using namespace tiny_cnn::activation;
using namespace tiny_cnn::layers;

void construct_mlp() {
    network<sequential> net;

    net << fc<sigmoid>(32 * 32, 300)
        << fc<identity>(300, 10);

    assert(net.in_data_size() == 32 * 32);
    assert(net.out_data_size() == 10);
}
```

another way to construct mlp

```cpp
#include "tiny_cnn/tiny_cnn.h"
using namespace tiny_cnn;
using namespace tiny_cnn::activation;

void construct_mlp() {
    auto mynet = make_mlp<tan_h>({ 32 * 32, 300, 10 });

    assert(mynet.in_data_size() == 32 * 32);
    assert(mynet.out_data_size() == 10);
}
```

more sample, read examples/main.cpp or [MNIST example](https://github.com/nyanp/tiny-cnn/tree/master/examples/mnist) page.

## References
[1] Y. Bengio, [Practical Recommendations for Gradient-Based Training of Deep Architectures.](http://arxiv.org/pdf/1206.5533v2.pdf) 
    arXiv:1206.5533v2, 2012

[2] Y. LeCun, L. Bottou, Y. Bengio, and P. Haffner, [Gradient-based learning applied to document recognition.](http://yann.lecun.com/exdb/publis/pdf/lecun-01a.pdf)
    Proceedings of the IEEE, 86, 2278-2324.
    
other useful reference lists:
- [UFLDL Recommended Readings](http://deeplearning.stanford.edu/wiki/index.php/UFLDL_Recommended_Readings)
- [deeplearning.net reading list](http://deeplearning.net/reading-list/)

## License
The BSD 3-Clause License

## Mailing list
google group for questions and discussions:

https://groups.google.com/forum/#!forum/tiny-cnn-users
