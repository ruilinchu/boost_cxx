//use mpi_scan to calculate final = A * B * C * D ...
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

typedef ublas::matrix<double, ublas::column_major> m_t;

BOOST_IS_BITWISE_SERIALIZABLE(m_t);//Homogeneous machines

m_t m_dot(const m_t &lhs, const m_t &rhs)
{
  std::size_t n1 = lhs.size1();
  std::size_t n2 = rhs.size2();
  m_t temp(n1,n2); //specify size, otherwise BUG
  blas::gemm(1.0, lhs, rhs, 0.0, temp);
  return temp;
}

int main()
{
  mpi::environment env;
  mpi::communicator world;

  std::size_t N = 2;
  m_t a(N,N);
  m_t a_r(N,N);

  int value = world.rank();
  for (std::size_t j = 0; j < N; ++j)
    for (std::size_t i = 0; i < N; ++i) {
      a(i,j) = ++value;
    }
  
  std::cout << "on proc " << world.rank() << " a is: " << std::endl << a << std::endl;
  // using 3 nodes, find result shoud be:
  //109   169
  //160   248

  mpi::scan(world, a, a_r, m_dot);

  std::cout << "on proc " << world.rank() << " a_r is: " << std::endl << a_r << std::endl;

  return 0;
}
