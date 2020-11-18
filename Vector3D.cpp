/**
Copyright 2020 Andrey Kudryavtsev (andrewkoudr@hotmail.com)

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted, provided
that the above copyright notice appears in all copies and that both the
copyright notice and this permission notice appear in supporting
documentation, and that the same name not be used in advertising or
publicity pertaining to distribution of the software without specific,
written prior permission. 
We make no representations about the suitability of this software for any 
purpose. It is provided "as is" without any expressed or implied warranty.
*/

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "Vector.h"

/**
  Some tests for 3D vectors, without (TVector<T>) and with SIMD (Vector4).

  NDEBUG removed in Release from  C/C++ > Preprocessor > Preprocessor Definitions
  to enable assert().
*/

int main()
{
  using namespace std;
  using namespace std::chrono;
  high_resolution_clock::time_point t1,t2,t3,t4;
  #define NUM_VECTORS 1000000

  {
    #define T float
    //#define NO_AUTOVECTORISATION
    cout << "  Test 1 : Simple ariphmetic of 3D vectors" << endl;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<T> dist(1.0, 10.0);
                              // init vectors
    vector<TVector<T>> v1(NUM_VECTORS);
    vector<TVector<T>> v2(NUM_VECTORS);
    vector<Vector4> v3(NUM_VECTORS);
    vector<Vector4> v4(NUM_VECTORS);
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      for (size_t j = 0; j < 3; j++)
      {
        v1[i].XYZ[j] = v3[i].XYZ[j] = dist(mt);
        v2[i].XYZ[j] = v4[i].XYZ[j] = dist(mt);
      }
    }
                              // without SIMD
    t1 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v1.size(); i++)
    {
      v1[i] = v1[i] + v2[i] + (v2[i] - v1[i]) * static_cast<T>(0.333); 
    }
    t2 = high_resolution_clock::now();

                              // with SIMD
    t3 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v3.size(); i++)
    {
      v3[i] = v3[i] + v4[i]+ (v4[i] - v3[i]) * static_cast<float>(0.333); 
    }
    t4 = high_resolution_clock::now();

                              // test for correctness : v1 must be same as v3
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      T dx = std::abs(v1[i].X - v3[i].X);
      T dy = std::abs(v1[i].Y - v3[i].Y);
      T dz = std::abs(v1[i].Z - v3[i].Z);
      T dw = std::abs(v1[i].W - v3[i].W);

      assert(dx < TOLERANCE(T) && dy < TOLERANCE(T) && dz < TOLERANCE(T) && dw < TOLERANCE(T));
    }

    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);

    cout << "time without SIMD " << time_span1.count() << ", with SIMD " << time_span2.count() << 
      " sec, speedup " << time_span1.count() / time_span2.count() << endl;
    
    #undef NO_AUTOVECTORISATION
    #undef T
  }

  {
    #define T float
    //#define NO_AUTOVECTORISATION
    cout << "  Test 2 : Ariphmetic, length and normalisation of 3D vectors" << endl;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<T> dist(1.0, 10.0);
                              // init vectors
    vector<TVector<T>> v1(NUM_VECTORS);
    vector<TVector<T>> v2(NUM_VECTORS);
    vector<Vector4> v3(NUM_VECTORS);
    vector<Vector4> v4(NUM_VECTORS);
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      for (size_t j = 0; j < 3; j++)
      {
        v1[i].XYZ[j] = v3[i].XYZ[j] = dist(mt);
        v2[i].XYZ[j] = v4[i].XYZ[j] = dist(mt);
      }
    }
                              // without SIMD
    t1 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v1.size(); i++)
    {
      v1[i] = 6 * (v1[i] + v2[i]) / (!(v1[i] + v2[i])) + (+(v2[i] - v1[i])) * static_cast<T>(0.333);
    }
    t2 = high_resolution_clock::now();

                              // with SIMD
    t3 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v3.size(); i++)
    {
      v3[i] = 6 * (v3[i] + v4[i]) / (!(v3[i] + v4[i])) + (+(v4[i] - v3[i])) * static_cast<float>(0.333);  
    }
    t4 = high_resolution_clock::now();

                              // test for correctness : v1 must be same as v3
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      T dx = std::abs(v1[i].X - v3[i].X);
      T dy = std::abs(v1[i].Y - v3[i].Y);
      T dz = std::abs(v1[i].Z - v3[i].Z);
      T dw = std::abs(v1[i].W - v3[i].W);

      assert(dx < TOLERANCE(T) && dy < TOLERANCE(T) && dz < TOLERANCE(T) && dw < TOLERANCE(T));
    }

    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);

    cout << "time without SIMD " << time_span1.count() << ", with SIMD " << time_span2.count() << 
      " sec, speedup " << time_span1.count() / time_span2.count() << endl;

    #undef NO_AUTOVECTORISATION
    #undef T
  }

  {
    #define T float
    //#define NO_AUTOVECTORISATION
    cout << "  Test 3 : Ariphmetic, dot and cross products of 3D vectors" << endl;

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<T> dist(1.0, 10.0);
                              // init vectors
    vector<TVector<T>> v1(NUM_VECTORS);
    vector<TVector<T>> v2(NUM_VECTORS);
    vector<Vector4> v3(NUM_VECTORS);
    vector<Vector4> v4(NUM_VECTORS);
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      for (size_t j = 0; j < 3; j++)
      {
        v1[i].XYZ[j] = v3[i].XYZ[j] = dist(mt);
        v2[i].XYZ[j] = v4[i].XYZ[j] = dist(mt);
      }
    }
                              // without SIMD
    t1 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v1.size(); i++)
    {
      v1[i] = ((v2[i] + v1[i]) ^ (v2[i] - v1[i])) * (v2[i] * v1[i]);
    }
    t2 = high_resolution_clock::now();

                              // with SIMD
    t3 = high_resolution_clock::now();
    #ifdef NO_AUTOVECTORISATION
      #pragma loop(no_vector)
    #endif
    for (size_t i = 0; i < v3.size(); i++)
    {
      v3[i] = ((v4[i] + v3[i]) ^ (v4[i] - v3[i])) * (v4[i] * v3[i]);
    }
    t4 = high_resolution_clock::now();

                              // test for correctness : v1 must be same as v3
    for (size_t i = 0; i < NUM_VECTORS; i++)
    {
      T dx = std::abs(v1[i].X - v3[i].X);
      T dy = std::abs(v1[i].Y - v3[i].Y);
      T dz = std::abs(v1[i].Z - v3[i].Z);
      T dw = std::abs(v1[i].W - v3[i].W);

      assert(dx < TOLERANCE(T) && dy < TOLERANCE(T) && dz < TOLERANCE(T) && dw < TOLERANCE(T));
    }

    duration<double> time_span1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> time_span2 = duration_cast<duration<double>>(t4 - t3);

    cout << "time without SIMD " << time_span1.count() << ", with SIMD " << time_span2.count() << 
      " sec, speedup " << time_span1.count() / time_span2.count() << endl;

    #undef NO_AUTOVECTORISATION
    #undef T
  }

  #undef NUM_VECTORS

  cout << endl << "Press [ENTER]" << endl;

  getchar();

}
