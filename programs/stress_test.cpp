#include "../include/providers.hpp"
#include "../libraries/singularity/include/singularity.hpp"
#include <fstream>
#include "../include/exporter.hpp"

using namespace singularity;
using namespace std;

int main (int argc, char *argv[]) 
{ 
    string output_filename;
    uint64_t accounts_quan, transactions_quan, threads_quan;
    
    if (argc < 5) {
        cout << "Usage: example <output file> <accounts quantity> <transactions quantity> <threads quantity>" << endl;
        return 0;
    } else {
        output_filename = argv[1];
        accounts_quan = stoi(argv[2]);
        transactions_quan = stoi(argv[3]);
        threads_quan = stoi(argv[4]);
    }
    
    stress_test_provider_t provider("A-", accounts_quan, transactions_quan, 1000000);
    
    parameters_t p;
    
    p.num_threads = threads_quan;
    
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
