// Benchmarked to be about 66% faster than std::log2 from <cmath>
static inline unsigned int fast_log2(const unsigned int x) {
    unsigned int y;
    asm ( "\tbsr %1, %0\n"
    : "=r"(y)
    : "r" (x)
    );
    return y;
}

int normalize(const int x) {
    return x ? static_cast<int>(fast_log2(x)) : 0;
}
