# A quick introduction to tiny-dnn
Include tiny_dnn.h:

```cpp
    #include "tiny_dnn/tiny_dnn.h"
    using namespace tiny_dnn;
    using namespace tiny_dnn::layers;
    using namespace tiny_dnn::activation;
```

Declare the model as ```network```. There are 2 types of network: ```network<sequential>``` and ```network<graph>```. The sequential model is easier to construct.

```cpp
    network<sequential> net;
```

Stack layers:

```cpp
    net << conv(32, 32, 5, 1, 6, padding::same) << tanh()  // in:32x32x1, 5x5conv, 6fmaps
        << max_pool(32, 32, 6, 2) << tanh()                // in:32x32x6, 2x2pooling
        << conv(16, 16, 5, 6, 16, padding::same) << tanh() // in:16x16x6, 5x5conv, 16fmaps
        << max_pool(16, 16, 16, 2) << tanh()               // in:16x16x16, 2x2pooling
        << fc(8*8*16, 100) << tanh()                       // in:8x8x16, out:100
        << fc(100, 10) << softmax();                       // in:100 out:10
```

Declare the optimizer:

```cpp
    adagrad opt;
```

In addition to gradient descent, you can use modern optimizers such as adagrad, adadelta, adam.

Now you can start the training:

```cpp
    int epochs = 50;
    int batch = 20;
    net.fit<cross_entropy>(opt, x_data, y_data, batch, epochs);
```

If you don't have the target vector but have the class-id, you can alternatively use ```train```.

```cpp
    net.train<cross_entropy, adagrad>(opt, x_data, y_label, batch, epochs);
```

Validate the training result:

```cpp
    auto test_result = net.test(x_data, y_label);
    auto loss = net.get_loss<cross_entropy>(x_data, y_data);
```

Generate prediction on the new data:

```cpp
    auto y_vector = net.predict(x_data);
    auto y_label = net.predict_max_label(x_data);
```

Save the trained parameter and models:

```cpp
    net.save("my-network");
```

For a more in-depth about tiny-dnn, check out [MNIST classification](https://github.com/tiny-dnn/tiny-dnn/tree/master/examples/mnist) where you can see the end-to-end example.
You will find tiny-dnn's API in [How-to](../how_tos/How-Tos.md).
