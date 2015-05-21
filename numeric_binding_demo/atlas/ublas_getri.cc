
// inverting A
// using getrf() & getri() 
//#include <cstddef>
#include <iostream>
#include <complex>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/blas/level3.hpp>
#include <boost/numeric/bindings/lapack/computational/getri.hpp>
#include <boost/numeric/bindings/lapack/computational/getrf.hpp>
#include <boost/numeric/bindings/ublas/matrix.hpp>
#include <boost/numeric/bindings/std/vector.hpp>
#include "utils.h"

namespace ublas = boost::numeric::ublas;
namespace blas = boost::numeric::bindings::blas;
namespace lapack = boost::numeric::bindings::lapack;

using std::size_t; 
using std::cout;
using std::endl; 

typedef std::complex<double> cmpx; 
typedef ublas::matrix<double, ublas::column_major> m_t;
typedef ublas::matrix<cmpx, ublas::column_major> cm_t;

int main() {

  cout << endl; 
  cout << "real matrix:" << endl << endl; 

  size_t n = 5; 
  m_t a (n, n);
  init_symm (a); 
  cout << "A: " << endl << a << endl;

  m_t aa (a);  // copy of a, for later use

  std::vector<int> ipiv (n);   // pivot vector 
  lapack::getrf (a, ipiv);  // no lu_factor() alias for getrf() available
  lapack::getri (a, ipiv);  // no lu_invert() alias for getrf() available

  m_t i1 (n, n), i2 (n, n);  
  blas::gemm (1.0, a, aa, 0.0, i1);   // i1 should be (almost) identity matrix
  blas::gemm (1.0, aa, a, 0.0, i2);   // i2 should be (almost) identity matrix

  //  print_m (i1, "I = A^(-1) * A");  
  cout << "I = A^(-1) * A: " << endl << i1 << endl;
  cout << "I = A * A^(-1): " << endl << i2 << endl;
  
  //////////////////////////////////////////////////////

  cout << "complex matrix:" << endl << endl; 
  cm_t ca (3, 3); 

  ca (0, 0) = cmpx (3, 0);
  ca (0, 1) = cmpx (4, 2);
  ca (0, 2) = cmpx (-7, 5);
  ca (1, 0) = cmpx (4, -2);
  ca (1, 1) = cmpx (-5, 0);
  ca (1, 2) = cmpx (0, -3);
  ca (2, 0) = cmpx (-7, -5);
  ca (2, 1) = cmpx (0, 3);
  ca (2, 2) = cmpx (2, 0);
  cout << "ca: " << ca << endl;

  cm_t caa (ca); 
  
  std::vector<int> ipiv2 (3); 
  
  int ierr = lapack::getrf (ca, ipiv2);
  if (ierr == 0) {
    lapack::getri (ca, ipiv2); 
    cm_t ii (3, 3); 
    blas::gemm (1.0, ca, caa, 0.0, ii);
    cout << "I = CA^(-1) * CA: " << endl << ii << endl;

    blas::gemm (1.0, caa, ca, 0.0, ii);
    cout << "I = CA * CA^(-1): " << endl << ii << endl;

  }
  else
    cout << "matrix is singular" << endl; 
  
  cout << endl; 

}

