#include <iostream>
#include <ctime>
using namespace std;


// Include header for uniform distribution between 0 and 1
#include  <boost/random/uniform_01.hpp>
using boost::uniform_01;

// Use Mersenne-Twister
//#define MT

#ifdef MT
  // Include Mersenne-Twister
  #include <boost/random/mersenne_twister.hpp>
  using boost::mt19937;
  
  //  function
  double boost_random() 
  {
    static mt19937 gen(time(0));
    static uniform_01<mt19937> dist(gen);   
    return dist();
  }
  
#else
  // Include Park-Miller Min-Standard
  #include <boost/random/linear_congruential.hpp>
  using boost::minstd_rand;
  
  // function
  double boost_random()
  {
    static minstd_rand gen(time(0));
    static uniform_01<minstd_rand> dist(gen); 
    return dist(); 
  }
#endif



// main
int main(int argc, char *argv[])
{
  int num_count;
  

  // Read input arguments
  if(argc < 2) 
  {
    num_count = 10; // Default: generate 10 numbers
  }
  else
  {
    num_count = atoi(argv[1]);
  }
  
  
  for(int i=0; i < num_count; i++)
  {
    cout << boost_random() << endl;
  }
  
  return 0;
}