# 3DVector
3D vector with and without SIMD
-------------------------------

  These are two basic classes for 3D vectors with operators which make
possible use of simple operations on vectors like v3 = v1 + v2 etc.
  This is a VC 2019 console test project with no Windows specifics, easily converted into Linux.

  The first class is a traditional C++ 11/14 without SIMD acceleration; the second is
based on basic SSE2 Intel intrinsics 
(see https://software.intel.com/sites/landingpage/IntrinsicsGuide/). The intrinsics
together with the optimiser may produce very good acceleration of the code
(e.g. https://github.com/AndreyKoudr/ThreadedGaussElimination),
but it is a little bit tricky.

  To make a fast code, you must follow the two principles
  
  (1) <B>more calculations</B> (in registers)
  
  (2) <B>less memory access</B> because it is much slower

It means that such constructions <B>without temporary variables</B> like these

    v3.data = _mm_sub_ps(
        _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 0, 2, 1)),
          _mm_shuffle_ps(v2.data, v2.data, _MM_SHUFFLE(3, 1, 0, 2))),
        _mm_mul_ps(_mm_shuffle_ps(data, data, _MM_SHUFFLE(3, 1, 0, 2)),
          _mm_shuffle_ps(v2.data, v2.data, _MM_SHUFFLE(3, 0, 2, 1))));
          
or 

    v3[i] = 6 * (v3[i] + v4[i]) / (!(v3[i] + v4[i])) + (+(v4[i] - v3[i])) * static_cast<float>(0.333); 

are preferable. Therefore, for just adding two vector arrays, acceleration is very modest if any (many register loads and saves, too little calculaton).
  
  The first template class <B>TVector<T></B> works with both float-s and double-s; the second
  SIMD-base class <B>Vector4</B> is written for 4-byte floats only.

  The sets of operations are identical for both classes.

  The fourth <B>homogeneous</B> coordinate is set to zero everywhere but normally it should be 
initialised to 1.0. It is used in matrix coordinate transforms like V[4] = M[4x4] v[4]. For example,
it is necessary to formulate a transform matrix for translation.

  4-component vector template, no SIMD
  ------------------------------------
  
  The fourth component is a homogeneous coordinate.
  It does not make much sense to make a template for a N-component (e.g. 3) 
vector as currently there are such things as SIMD enabling to make operations
on all 4 components at once.
 
  Operations
  ----------
  "float" below is 4 or 8 byte float

  float = V[AxisX]  - get vector coordinate
  
  float = !V        - get vector length       
  
  v3 = v1 ^ v2      - cross product      
  
  v2 = +v1          - normalisation
  
  float = v1 * v2   - dot product, W is ignored
  
  (v1 > v2)         - vectors co-directed?    
  
  bool(v1==v2)      - vectors equal?          
  
  v2 = -v1          - change sign of components    
  
  v3 = v1 + v2      - addition
  
  v1 += v2;         - addition
  
  v3 = v1 - v2      - subtraction
  
  v1 -= v2;         - subtraction
  
  v3 = v1 * float   - multiply by scalar
  
  v3 = float * v    - multiply by scalar
  
  v1 *= float       - multiply by scalar
  
  v3 = v1 / float   - divide by scalar
  
  v1 /= float       - divide by scalar
  
  4-component vector on 4-byte floats driven by SIMD
  --------------------------------------------------
    
  The fourth component is a homogeneous coordinate.
 
  Operations
  ----------
  "float" below is a 4-byte float

  float = V[AxisX]  - get vector coordinate
  
  float = !V        - get vector length   
  
  v3 = v1 ^ v2      - cross product      
  
  v2 = +v1          - normalisation
  
  float = v1 * v2   - dot product, W is ignored
  
  (v1 > v2)         - vectors co-directed?    
  
  bool(v1==v2)      - vectors equal?          
  
  v2 = -v1          - change sign of components     
  
  v3 = v1 + v2      - addition
  
  v1 += v2;         - addition
  
  v3 = v1 - v2      - subtraction
  
  v1 -= v2;         - subtraction
  
  v3 = v1 * float   - multiply by scalar
  
  v3 = float * v    - multiply by scalar
  
  v1 *= float       - multiply by scalar
  
  v3 = v1 / float   - divide by scalar
  
  v1 /= float       - divide by scalar
  
  Tests (in console VS 2019 project)
  ----------------------------------
  
  Test 1 : Simple ariphmetic of 3D vectors
    
time without SIMD 0.004114, with SIMD 0.0031171 sec, speedup 1.31982

  Test 2 : Ariphmetic, length and normalisation of 3D vectors
  
time without SIMD 0.0100695, with SIMD 0.0052064 sec, speedup 1.93406

  Test 3 : Ariphmetic, dot and cross products of 3D vectors
  
time without SIMD 0.0041863, with SIMD 0.0028711 sec, speedup 1.45808

  
  


