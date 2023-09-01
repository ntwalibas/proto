# Proto

Proto (for prototype and primitive) is a single-file programming language that I will be using
as I play with different parts of a compiler and programming languages concepts.

This repo contains the reference implementation using an interpreter.
As things progress, I will create different backends using bytecode,
a JIT compiler (that can either ouput bytecode or assembly code),
and of course just generate assembly for a particular machine.

That's the general plan thus far but initially, with the interpreter,
the goal is to first gain a better understanding of the choices
existing programming languages have made, why those choices were made,
and compare them.

## What a Proto program looks like

Here is a program that calculates the n-th Fibonnaci number.

```Rust
fib : function(n: const int) -> int {
    t1:     int = 1
    t2:     int = 0
    result: int = 0

    for (i: int = 0; i < n; i += 1) {
        t2      = result
        result  = t1
        t1      = t1 + t2
    }

    return result
}

main : function() -> int {
    println(fib(92))
    return 0
}
```

## Testing

> Make sure bazel is installed. Instructions can be found at https://bazel.build/install/bazelisk.

All tests are under the `tests` folder. Each subfolder contains tests for a particular module of the interpreter.

To run all tests:

```shell
bazel test --test_output=all //tests/...
```

To run tests for a specific module of the interpreter, e.g. the parser:

```shell
bazel test --test_output=all //tests/parser:parser_test
```

## Building

To generate the main binary for the interpreter:

```shell
bazel build //src:main
```

**Note for Windows:** after installing Bazel, remember to change the options in `.bazelrc` to use
Windows syntax of forward slashes and colons to pass options to the compiler.

## Running

In the file `fibonnaci.pro`, there is a sample program that calculates the n-th Fibonacci number.

It exercises almost all of the parts of the language already implemented.

To run the interpreter:

```shell
bazel-bin/sr/main program.pro
```

Where `program.pro` is the file containing your Proto code.
Or in this case replace `program.pro` with `fibonnaci.pro`.
