
#include <boost/mpi.hpp>
#include <iostream>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/serialization/serialization.hpp>

namespace mpi = boost::mpi;
namespace ublas = boost::numeric::ublas;
namespace mpi = boost::mpi;

typedef ublas::identity_matrix<int, ublas::column_major>  eye;
typedef ublas::matrix<int, ublas::column_major> m_t;

m_t sum(const m_t &lhs, const m_t &rhs)
{
  m_t temp = lhs + rhs;
  return temp;
}

int main()
{
  mpi::environment env;
  mpi::communicator world;
  std::size_t N = 4;
  eye ai (N);
  m_t a = ai * 2;
  std::cout << "on proc " << world.rank() << " a is: " << std::endl << a << std::endl;

  if (world.rank() == 0){
    m_t a_g (N, N);
    mpi::reduce(world, a, a_g, sum, 0);
    std::cout << "after reduction a_g is : " <<  std::endl << a_g << std::endl;
  } else {
    mpi::reduce(world, a, sum, 0);
  }
    
  return 0;
}
