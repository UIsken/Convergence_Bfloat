#include "setup.hpp"

Setup::Setup(size_t i_m, size_t i_n, size_t i_k) {
  l_m = i_m;
  l_n = i_n;
  l_k = i_k;

  l_lda = i_m;
  l_ldb = i_k;
  l_ldc = i_m;

  l_a.resize(l_m * l_k, 0);
  l_b.resize(l_k * l_n, 0);

  l_c_ref_fp32.resize(l_m * l_n, 0);
  l_c_bf_3.resize(l_m * l_n, 0);
  l_c_bf_6.resize(l_m * l_n, 0);
  l_c_bf_9.resize(l_m * l_n, 0);
}

Setup::~Setup() {}

void Setup::random_expo(float lambda) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random floats
  std::exponential_distribution<float> dist{lambda};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };
  generate(l_a.begin(), l_a.end(), gen);
  generate(l_b.begin(), l_b.end(), gen);
}

void Setup::random_uniform(float i_min, float i_max) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random floats
  std::uniform_real_distribution<float> dist{i_min, i_max};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };
  generate(l_a.begin(), l_a.end(), gen);
  generate(l_b.begin(), l_b.end(), gen);
}

void Setup::random_normal(float i_mean, float i_var) {
  // First create an instance of an engine.
  std::random_device rnd_device;
  // Specify the engine and distribution.
  std::mt19937 mersenne_engine{rnd_device()};  // Generates random floats
  std::normal_distribution<float> dist{i_mean, i_var};

  auto gen = [&dist, &mersenne_engine]() { return dist(mersenne_engine); };
  generate(l_a.begin(), l_a.end(), gen);
  generate(l_b.begin(), l_b.end(), gen);
}

void Setup::GEMM() {
  gemmd_ref(l_a.data(), l_b.data(), l_c_ref_fp64.data(), l_m, l_n, l_k, l_lda,
            l_ldb, l_ldc);
  gemms_ref(l_a.data(), l_b.data(), l_c_ref_fp32.data(), l_m, l_n, l_k, l_lda,
            l_ldb, l_ldc);
  gemm_bfloat(l_a.data(), l_b.data(), l_c_bf_3.data(), l_m, l_n, l_k, l_lda,
              l_ldb, l_ldc, 3);
  gemm_bfloat(l_a.data(), l_b.data(), l_c_bf_6.data(), l_m, l_n, l_k, l_lda,
              l_ldb, l_ldc, 6);
  gemm_bfloat(l_a.data(), l_b.data(), l_c_bf_9.data(), l_m, l_n, l_k, l_lda,
              l_ldb, l_ldc, 9);
}

void Setup::XSMM() {
  MMxsmm_svanilla(l_a.data(), l_b.data(), l_c_ref_fp32.data(), l_m, l_n, l_k,
                  l_lda, l_ldb, l_ldc);
}
