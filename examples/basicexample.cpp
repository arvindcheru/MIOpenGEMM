#include "basicfind.hpp"


template <typename TFloat>
void basicexample(){
  /* define the GEMM problem */
  bool isColMajor = true;




// ./driver/MLOpenDriver conv  -W 341 -H 79 -c 32 -n 8 -k 32 -y 5 -x 10 -p 0 -q 0 -u 2 -v 2/
// tC:0 tA:1 tB:0 colMaj:1 m:1600 n:32 k:6308 lda:6308 ldb:6308 ldc:1600 a_offset:0 b_offset:0 c_offset:0


  bool tC = false;
  bool tA = true;
  bool tB = false;

  
  unsigned m = 33;//1600; //3000;    
  unsigned n = 32;//400;
  unsigned k = 63080;//500;

  unsigned lda = (tA == isColMajor ? k : m ) + 0;//3;
  unsigned ldb = (tB == isColMajor ? n : k ) + 0;//5;
  unsigned ldc = (tC == isColMajor ? n : m ) + 0;//11;

  unsigned a_offset = 0;//100;
  unsigned b_offset = 0;//200;
  unsigned c_offset = 0;//300;

  /* These must be double, irrespective of the float type of the matrices */
  double alpha = 1.0;
  double beta = .2;//0.3;
  /* floattype should be 
   * 'f' for single-precision, 32-bit floats and 
   * 'd' for double-precision, 64-bit doubles */
  char floattype = (sizeof(TFloat) == 4) ? 'f' : 'd';
  /* define how long to search for, in seconds. No kernels will be compiled after this allotted time. */
  float allotted_time = 55.01;
  /* print output to terminal (true) or complete silence to terminal (false) */
  bool verbose = true;
  /* print output to logfile (non-empty string) or not (empty string) */
  /* MUST BE SET BY USER */
  std::string logfile("/home/james/tinygemm/examples/findlog.txt");
  /* enforce that the kernel is deterministic, or not. Note that 
   * for small problems, non-deterministic kernels are significantly (2x) faster */
  bool enforce_deterministic = false;
  unsigned n_postfind_runs = 5;//4;
  
  
  basicfind<TFloat>(isColMajor, tA, tB, tC, m, n, k, lda, ldb, ldc, a_offset, b_offset, c_offset, alpha, beta, floattype, allotted_time, verbose, logfile, enforce_deterministic, n_postfind_runs);
  
  
}

int main(){
  basicexample<float>(); /* or example<double> for dgemm example */
  return 0;
}


