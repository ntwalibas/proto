// Fibonacci numbers using iteration
iterative_fibonacci : function(n: const uint) -> uint {
    t1: uint = 0
    t2: uint = 1
    result: uint = t1 + t2

    i: uint = 2
    while (i < n) {
        t1 = t2
        t2 = result
        result = t1 + t2
        i += 1
    }

    return result
}

// Entry point
main : function() -> uint {
    // We will worry about printing the results later
    iterative_fibonacci(9)
    return 0
}
