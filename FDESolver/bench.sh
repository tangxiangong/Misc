cd ./fdesolver_jl && julia bench.jl -O3 && cd ..

cd ./fdesolver_rs && cargo bench && cd .. 
