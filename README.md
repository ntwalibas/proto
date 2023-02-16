# Proto

Proto is a very simple programming language that I intend to use to build other more fully-featured languages upon.
I kept it simple because for my side project in programming languages, I can just reuse the frontend provided here.


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
