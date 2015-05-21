
MPI/ illustrates combination of boost::mpi, booost::ublas numeric binding and boost::serialization

to compile:
mpicxx x.cpp -I $HMS_BOOST_INC -L$HMS_BOOST_LIB -lboost_serialization -lboost_mpi -L$HMS_OPENBLAS_LIB -lopenblas 

