// mpicxx 10_ublas_binding_reduce.cpp -I $HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi -L$HMS_OPENBLAS_LIB -lopenblas

// use the numeric binding for blas or atals to define reduction operator, using gemm BLAS level 3
#include <boost/mpi.hpp>
#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/serialization/serialization.hpp> // MPI communication 
#include <boost/numeric/bindings/blas/level3.hpp> //gemm method
#include <boost/numeric/bindings/ublas/matrix.hpp>

namespace ublas = boost::numeric::ublas;
namespace mpi = boost::mpi;
namespace blas = boost::numeric::bindings::blas;

typedef ublas::identity_matrix<double, ublas::column_major>  eye;
typedef ublas::matrix<double, ublas::column_major> m_t;

// m_t m_dot(const m_t &lhs, const m_t &rhs)
// {
//   m_t temp = prod(lhs, rhs);
//   return temp;
// } // this confirmed works

m_t m_dot(const m_t &lhs, const m_t &rhs)
{
  std::size_t n1 = lhs.size1();
  std::size_t n2 = rhs.size2();
  m_t temp (n1, n2); //specify size, otherwise BUG
  blas::gemm (1.0, lhs, rhs, 0.0, temp);
  return temp;
}

int main()
{
  mpi::environment env;
  mpi::communicator world;
  std::size_t N = 4;
  eye ai (N);
  m_t a = ai * 2.0;
  std::cout << "on proc " << world.rank() << " a is: " << std::endl << a << std::endl;

  if (world.rank() == 0){
    m_t a_g (N, N);
    mpi::reduce(world, a, a_g, m_dot, 0);
    std::cout << "after reduction a_g is : " <<  std::endl << a_g << std::endl;
  } else {
    mpi::reduce(world, a, m_dot, 0);
  }
    
  return 0;
}
