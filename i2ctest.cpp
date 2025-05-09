#include <iostream>
#include "serial/headers/i2c.hpp"
#include <chrono>

union float_to_bytes {
  float f;
  uint8_t b[4];
};

float_to_bytes fdata[3];
float_to_bytes output[3];


std::chrono::high_resolution_clock::time_point start;
std::chrono::high_resolution_clock::time_point end;
std::chrono::nanoseconds elapsed_ns, m_w, m_r;

bool testI2C() {

  if (arduino.open()) {
    std::cout << "I2C opened successfully.\n";
  } else {
    std::cerr << "Failed to open I2C.\n";
    return 1;
  }

  for (int i = 0; i < 50000; i++) {
    fdata[1].f = i * 10.673f;
    fdata[2].f = i * 0.123f;
    fdata[0].f = i * 0.456f;
    
    start = std::chrono::high_resolution_clock::now();
    arduino.write(reinterpret_cast<uint8_t*>(fdata), 12);
    end = std::chrono::high_resolution_clock::now();
    elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    m_w += elapsed_ns;
    if (i!=0) m_w /= 2;
    std::cout << "W - Iteration " << i << ": " << elapsed_ns.count() << '\n';

    start = std::chrono::high_resolution_clock::now();
    arduino.read(reinterpret_cast<uint8_t*>(output), 12);
    end = std::chrono::high_resolution_clock::now();
    elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    m_r += elapsed_ns;
    if (i!=0) m_r /= 2;
    std::cout << "R - Iteration "<< i << ": " << elapsed_ns.count() << '\n';

    if (output[0].f != fdata[0].f || 
        output[1].f != fdata[1].f || 
        output[2].f != fdata[2].f) {
      std::cout << "Data mismatch at iteration " << i;
      return false;
    }
  }

  arduino.close();
  std::cout << "\nI2C closed.\n";
  std::cout << "Average write time: " << m_w.count() << " ns\n";
  std::cout << "Average read time: " << m_r.count() << " ns\n";
  std::cout << "Average total time: " << (m_w + m_r).count() << " ns\n";

  return true;
}