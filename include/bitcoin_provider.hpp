#ifndef BITCOIN_PROVIDER_HPP
#define BITCOIN_PROVIDER_HPP

#ifdef USE_BITCOIN

#include "providers.hpp"

class bitcoin_provider_t: public provider_t
{
public:
    bitcoin_provider_t(std::string bitcoin_data_dir, uint64_t start_block, uint64_t end_block, uint64_t height)
        :bitcoin_data_dir(bitcoin_data_dir) {}
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block();
private:
    std::string bitcoin_data_dir;
    uint64_t start_block; 
    uint64_t end_block; 
    uint64_t height;
};

#endif /* USE_BITCOIN */

#endif /* BITCOIN_PROVIDER_HPP */


