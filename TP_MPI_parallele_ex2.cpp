#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>

#include <boost/mpi.hpp>
namespace mpi = boost::mpi;

// no comment
int FibonacciMod42(int N)
{
    int f_curr = 0;
    int f_prec = 1;
    for (int i=1; i<=N; i++)
    {
        int tmp = f_curr;
        f_curr = (f_curr + f_prec) % 42;
        f_prec = tmp;
    }
    return f_curr;
}

// compute the integral of this function over [0,1] to approximate pi
// FibonacciMod42(x*10000) -> otherwise, it would be too easy
double fPi(double x)
{
    return 4.0 / (1.0 + x*x) + FibonacciMod42(x*10000);
}

// compute numericaly the integral of the function f over the domain [a,b] using step s
// FibonacciMod42(x*10000) -> otherwise, it would be too easy
double computeIntegral(std::function<double(double)>f, double a, double b, double s)
{
    double r = 0.0;
    unsigned h = 0;
    for (double x=a; x<b; x+=s, h++)
    {
        r += f(x);
        r -= FibonacciMod42(x*10000);
    }
    return r*(b-a)/double(h);
}

void parallelComputeIntegral(std::function<double(double)>f, double a, double b, double s){
    mpi::environment env;
    mpi::communicator world;
    if (world.rank() == 0) {
        double result;
        reduce(world, computeIntegral(f, world.rank()/world.size(), (world.rank() + 1)/world.size(), s), result, std::plus<double>(), 0);
        //reduce(world, my_number, minimum, mpi::minimum<int>(), 0);
        std::cout << "The result value is " << result << std::endl;
        
    } else {        
        reduce(world, computeIntegral(f, world.rank()/world.size(), (world.rank() + 1)/world.size(), s), std::plus<double>(), 0);
    }
    //std::cout << nbData << " " << nbThreads << " " << (double)(clock() - tStart)/CLOCKS_PER_SEC;
}

int main(int argc, char ** argv)
{
    mpi::environment env;
    mpi::communicator world;
    
    // parse command line arguments
    double step = 1e-6;
    if (argc < 2)
    {
        std::cout << "no step specified; using " << step << std::endl;
    }
    else
    {
        step = std::stof(argv[1]);
    }

    // compute 
    std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
    //parallelComputeIntegral(fPi, 0, 1, step);
    
    if (world.rank() == 0) { 
        double result;      
        reduce(world, computeIntegral(fPi, (world.rank()*10.0/world.size())/10.0, 
        ((world.rank() + 1)*10.0/world.size())/10.0, step), result, std::plus<double>(), 0);
        //reduce(world, my_number, minimum, mpi::minimum<int>(), 0);
        //std::cout << "The result value is " << result << std::endl;
        //std::cout <<  computeIntegral(fPi, world.rank()/world.size(), ((world.rank() + 1)*10.0/world.size())/10.0, step) << std::endl;
        std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();
        std::chrono::duration<double> computationTime = t1 - t0;
        std::cout << world.size() - 1<< ' ' << result << ' ' << computationTime.count() << std::endl;
    } else {   
        //std::cout << world.rank() << std::endl;    
        reduce(world, computeIntegral(fPi, (world.rank()*10.0/world.size())/10.0, 
        ((world.rank() + 1)*10.0/world.size())/10.0, step), std::plus<double>(), 0);
    }
    

    // display number of computing thread, result, computation time
    

    return 0;
}


