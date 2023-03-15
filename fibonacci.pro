/* Fibonacci numbers
 *
 * This function calculates F(n), that is the nth number
 * in the Fibonacci sequence.
 */
fib : function(n: const uint) -> uint {
    t1     = 1:uint
    t2     = 0:uint
    result = 0:uint

    for (i = 0:uint; i < n; i += 1:uint) {
        t2      = result
        result  = t1
        t1      = t1 + t2
    }

    return result
}

main : function() -> int {
    // println(fib(9))
    return 0
}
