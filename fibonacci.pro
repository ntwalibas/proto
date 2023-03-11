/* Fibonacci numbers
 *
 * This function calculates F(n), that is the nth number
 * in the Fibonacci sequence.
 */
fib : function(n: const uint) -> uint {
    t1:     uint = 0
    t2:     uint = 1
    result: uint = t1 + t2

    for (i: uint = 2; i < n; i += 1) {
        t1 = t2
        t2 = result
        result = t1 + t2
    }

    return result
}

main : function() -> uint {
    // println(fib(9))
    return 0
}
