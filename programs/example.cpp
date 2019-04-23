#include "../include/providers.hpp"
#include "../libraries/singularity/include/singularity.hpp"

using namespace singularity;
using namespace std;

int main (int argc, char *argv[]) 
{ 
    stress_test_provider_t provider("A-", 10000, 100000, 1000000);
    
    parameters_t p;
    
    auto calc = rank_calculator_factory::create_calculator_for_transfer(p);
    
    auto block = provider.get_block();
    
    calc->add_block(block);
    
    auto result = calc->calculate();
}
