/* Fibonacci numbers
 *
 * This program calculates F(n), that is the n-th number
 * in the Fibonacci sequence.
 *
 * The first function uses iteration and is quite fast.
 *
 * The second function uses recursion and is slow.
 * Algorithmically, it is slow because it runs in O(2^n).
 * But a Python implementation would be faster than this one.
 * This is due to frequently needing to maintain function arguments
 * in the function's scope with subsequent calls.
 * That can of course be improved upon but it is not important right now.
 */
 iter_fib : function(n: const uint) -> uint {
    t1          = 1:uint
    t2          = 0:uint
    result      = 0:uint

    for (i      = 0:uint; i < n; i += 1:uint) {
        t2      = result
        result  = t1
        t1      = t1 + t2
    }

    return result
}

rec_fib : function(n: const uint) -> uint {
    return n < 2:uint ? n else rec_fib(n - 1:uint) + rec_fib(n - 2:uint)
}

main : function() -> int {
    // Since our unsigned int is 64 bits max,
    // our limit is Fib of 93.
    println(iter_fib(93:uint))

    // Recursion will be slower as n increases
    println(rec_fib(25:uint))
    return 0
}
