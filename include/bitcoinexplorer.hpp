
#ifndef BITCOINEXPLORER_HPP
#define BITCOINEXPLORER_HPP

#include "../libraries/singularity/include/singularity.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <bitcoin/bitcoin.hpp>

enum address_type_t {PUBKEYHASH, SCRIPTHASH};

class bitcoin_explorer
{
public:
    
    std::string convert_script_sig_to_address (std::string script_sig);
    
    std::vector<std::shared_ptr<singularity::relation_t> > extract_transactions(std::string filename, uint32_t block_height);
    
    std::string convert_public_key_to_address(std::string public_key, address_type_t address_type = address_type_t::PUBKEYHASH);

    std::string convert_public_key_hash_to_address(std::string public_key, address_type_t address_type = address_type_t::PUBKEYHASH);
    
    void test();
};

#endif
