/*
reduction on such formula:
A_final = A * B * C * D * E ...
A, B, C ... are distributed over nodes
use a binary tree to do the reduction in log2(N) steps
reduction is to root = 0

the sequence of multiply is important, otherwise result is wrong

defines:
pFlag: only ranks does nothing or recv updates the flag and participate in next step, those does a send will be idle in next step
step: global flag to compare to, updates each step
pSize: actual team size, initial is n_proc, updates to (pSize / 2) + (pSize % 2) in each step
pRank: actual rank index in each step, initial is proc, updates to pRank / 2 in each step   

*/
// module load boost openblas
// mpicxx x.cpp -I $HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi -L$HMS_OPENBLAS_LIB -lopenblas

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

int main()
{
  mpi::environment env;
  mpi::communicator world;

  std::size_t N = 2;
  m_t a(N,N);

  int value = world.rank();
  for (std::size_t j = 0; j < N; ++j)
    for (std::size_t i = 0; i < N; ++i) {
      a(i,j) = ++value;
    }
  
  std::cout << "on proc " << world.rank() << " a is: " << std::endl << a << std::endl;
  // using 3 nodes, find result shoud be:
  //109   169
  //160   248
  std::size_t pFlag = 1;
  std::size_t step = 1;
  std::size_t pSize = world.size();
  std::size_t pRank = world.rank();
  std::size_t intvl = 1; //send & recv rank intervel

  while (pSize > 1) {
    if ((pRank % 2) == 1 && pRank < pSize && pFlag == step) {//do send
      std::size_t dest = world.rank() - intvl;

      std::cout << "proc " << world.rank() << " does a isend to " << dest << " on step " << step << std::endl;

      mpi::request req;
      req = world.isend(dest, pFlag, a);//tag = pFlag 
      req.wait();
    } else if ((pRank % 2) == 0 && pRank < pSize -1 && pFlag == step) {//do recv and update flag
      std::size_t src = world.rank() + intvl;
      std::cout << "proc " << world.rank() << " does a irecv from " << src << " on step " << step << std::endl;

      m_t a_r(N,N);
      m_t a_t(N,N);
  
      mpi::request req;
      req = world.irecv(src, pFlag, a_r);
      req.wait();

      std::cout << "proc " << world.rank() << " in step " << step << " recv a_r = " << std::endl << a_r << std::endl;

      blas::gemm(1.0, a, a_r, 0.0, a_t);
      a = a_t; //a = a * a_r
      pFlag++;
    } else { //do nothing other than update flag
      std::cout << "proc " << world.rank() << " does nothing on step " << step << std::endl;
      pFlag++;
    }
    pSize = (pSize / 2) + (pSize % 2);
    pRank /= 2;
    intvl *= 2;
    step++;
  }

  if (world.rank() == 0) {
    std::cout << "final a is: " << std::endl << a << std::endl;
  }

  return 0;
}
