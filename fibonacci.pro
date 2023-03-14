/* Fibonacci numbers
 *
 * This function calculates F(n), that is the nth number
 * in the Fibonacci sequence.
 */
fib : function(n: const uint) -> uint {
    t1:     uint = 1
    t2:     uint = 0
    result: uint = 0

    for (i: uint = 0; i < n; i += 1) {
        t2       = result
        result   = t1
        t1       = t1 + t2
    }

    return result
}

main : function() -> uint {
    // println(fib(9))
    return 0
}
