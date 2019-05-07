#include "include/bitcoin_provider.hpp"
#include "include/bitcoinexplorer.hpp"
#include <vector>
#include <boost/shared_ptr.hpp>
#include "libraries/singularity/include/singularity.hpp"

using namespace std;
using namespace singularity;

vector<shared_ptr<relation_t> > bitcoin_provider_t::get_block()
{
    vector<shared_ptr<relation_t> > block;
    
    string file_name;
    uint32_t height;
    
    bitcoin_explorer be;
    
    for (uint32_t i=end_block; i >= start_block; i--) {
        height++;
        string file = bitcoin_data_dir + "b" + to_string(i) + ".json";
        cout << file << endl;
        auto extracted_transactions = be.extract_transactions( file, height);
        
        block.insert(block.end(), extracted_transactions.begin(), extracted_transactions.end());
    }
    
    return block;
};

