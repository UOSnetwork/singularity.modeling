#include "../include/providers.hpp"
#include "../libraries/singularity/include/singularity.hpp"
#include <fstream>
#include "../include/exporter.hpp"

using namespace singularity;
using namespace std;

int main (int argc, char *argv[]) 
{ 
    string output_filename;
    
    if (argc < 2) {
        cout << "Usage: example <output file>" << endl;
    } else {
        output_filename = argv[1];
    }
    
    stress_test_provider_t provider("A-", 10000, 100000, 1000000);
    
    parameters_t p;
    
    p.account_amount_threshold = 0;
    p.transaction_amount_threshold = 0;
    
    auto calc = rank_calculator_factory::create_calculator_for_transfer(p);
    
    auto block = provider.get_block();
    
    calc->add_block(block);
    
    auto result = calc->calculate();
    
    ofstream fs (output_filename);
    
    exporter xp;
    
    xp.export_rank(fs, result[node_type::ACCOUNT]);
}
