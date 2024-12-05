#!/bin/zsh

cd cpp && clang++ -g0 -Ofast -O3 -std=c++20 code.cpp -o main  && ./main && cd ..

cd rust && cargo run --release && cd ..

cd julia && julia -O3 code.jl && cd ..

cd python && python code.py && cd ..