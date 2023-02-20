# Proto

Proto (for prototype and primitive) is a single-file programming language that I will be using
as I play with different parts of a compiler and programming languages concepts.

It is also serves as a tool to help me learn and update my C++ knowledge.

I intend to use it later to build something more fully featured upon it.


## Testing

> Make sure bazel is installed. Instructions can be found at https://bazel.build/install/bazelisk.

All tests are under the `tests` folder. Each subfolder contains tests for a particular module of the compiler.

To run all tests:

```shell
bazel test --test_output=all //tests/...
```

To run tests for a specific module of the compiler, e.g. the parser:

```shell
bazel test --test_output=all //tests/parser:parser_test
```

## Building

To generate the main binary for the compiler:

```shell
bazel build //src:main
```

And to run the compiler:

```shell
bazel-bin/sr/main program.pro
```

Where `program.pro` is the file containing your Proto code.
