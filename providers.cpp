
#include "include/providers.hpp"
#include "include/bitcoinexplorer.hpp"
#include "include/importer.hpp"

using namespace std;
using namespace singularity;

vector<shared_ptr<relation_t> > testnet_provider_t::get_block()
{
    importer im;
    return im.import_relations(relations_filename);
};

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

vector<shared_ptr<relation_t> > stress_test_provider_t::get_block()
{
    vector<shared_ptr<relation_t> > transactions;
    
    time_t now = time(nullptr);
    
    for (uint32_t i=0; i<num_transactions; i++) {
        uint32_t source_id;
        uint32_t target_id;
        do {
            source_id = num_accounts * drand48();
            target_id = num_accounts * drand48();
        } while (source_id == target_id);
        
        string source = get_prefix() + to_string(source_id);
        string target = get_prefix() + to_string(target_id);
        money_t amount = max_amount * drand48();
        transactions.push_back(make_shared<transaction_t>(amount, 0, source, target, now, 0, 0, 0));
    }
    
    return transactions;
}

vector<shared_ptr<relation_t> > cycle_attack_provider_t::get_block()
{
    vector<shared_ptr<relation_t> > transactions;
    
    time_t now = time(nullptr);
    
    for (uint32_t i=0; i<num_accounts; i++) {
        uint32_t j = (i+1) % num_accounts;
        string source = get_prefix() + to_string(i);
        string target = get_prefix() + to_string(j);
        transactions.push_back(make_shared<transaction_t>(max_amount, 0, source, target, now, 0, 0, 0));
    }
    
    return transactions;
}

vector<shared_ptr<relation_t> > concentrate_attack_provider_t::get_block()
{
    vector<shared_ptr<relation_t> > transactions;
    
    time_t now = time(nullptr);
    
    for (uint32_t i=1; i<num_accounts; i++) {
        string source = get_prefix() + to_string(i);
        string target = get_prefix() + "0";
        transactions.push_back(make_shared<transaction_t>(max_amount, 0, source, target, now, 0, 0, 0));
    }
    
    return transactions;
}

map<string, money_t> stack_import_from_file_provider_t::get_stack()
{
    map<string, money_t> result;
    
    return result;
}

