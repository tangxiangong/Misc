# C 与 Julia 的交互

## Julia call C
```
.
├── CMakeLists.txt
├── README.md
├── include
│   └── test.h
├── julia
│   └── test.jl
├── lib
│   └── libtest.dylib
└── src
    └── test.c
```
### 简介
头文件 [`test.h`](./include/test.h) 声明了实现复合梯形求积公式的函数 `double trapezoid(double (*)(double), double, double, double)` , 对应的源文件为 [`test.c`](./src/test.c), 生成的动态链接库 [`libtest`](./lib/). 在 [`test.jl`](./julia/test.jl) 定义了一个同名函数 `trapezoid(func::Function, a::Cdouble, b::Cdouble, dx::Cdouble)` 来包装该 C 函数 (`Cdouble` 是 Julia 数值类型 `Float64` 的别名, 对应于 C 的双精度浮点型 `double`) . [`CMakeLists.txt`](./CMakeLists.txt) 为这个 C 项目的 CMAKE 配置文件.

- `cd build && cmake .. && make` 构建生成动态链接库.
- 在 Julia 中加载动态链接库 `dlopen("./lib/libtest."*dlext)` .
- 将 Julia 函数 `trapezoid` 中的第一个参数 `func(::Cdouble)::Cdouble` 转换成 C 可调用的函数指针 `cfunc = @cfunction(func, Cdouble, (Cdouble,))`, 第一个 `Cdouble` 表示返回类型, 元组 `(Cdouble,)` 表示该函数的参数类型.
- 调用 C 函数计算结果 `result = @ccall trapezoid(cfunc::Ptr{Cvoid}, a::Cdouble, b::Cdouble, dx::Cdouble)::Cdouble`, `cfunc::Ptr{Cvoid}` 表示 `cfunc` 为一函数指针.