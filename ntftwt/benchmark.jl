# benchmark.jl
println("benchmark.jl julia program")

function ntftwt_main()::Int64
    println("Julia benchmark")
    println("----------------")
    
    N = 10_000_000
    
    # Warm-up
    x = 0.0
    for i in 1:1000
        x += sin(i) * cos(i)
    end
    
    # Actual benchmark
    println("Running $N iterations...")
    
    start = time_ns()
    
    x = 0.0
    for i in 1:N
        x += sin(i) * cos(i)
    end
    
    elapsed = (time_ns() - start) / 1e9
    
    println("Result:   ", x)
    println("Time:     ", round(elapsed, digits=6), " seconds")
    println("Iterations: ", N)
    println("Rate:     ", round(N / elapsed / 1e6, digits=2), " million/sec")
    return 0;
end