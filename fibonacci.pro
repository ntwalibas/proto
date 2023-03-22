/* Fibonacci numbers
 *
 * This program calculates F(n), that is the n-th number
 * in the Fibonacci sequence.
 *
 * The first function uses iteration and is quite fast.
 *
 * The second function uses recursion and is slow.
 * This is due to frequently needing to maintain function arguments
 * in the function's scope with subsequent calls.
 * That can of course be improved upon but it is not important right now.
 */
 iter_fib : function(n: const int) -> int {
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

rec_fib : function(n: const int) -> int {
    return n < 2 ? n else rec_fib(n - 1) + rec_fib(n - 2)
}

main : function() -> int {
    // Since our signed int is 64 bits max,
    // our limit is Fib of 92.
    println(iter_fib(92))

    // Recursion will be slower as n increases
    println(rec_fib(25))
    return 0
}
