
#ifndef PROVIDERS_HPP
#define PROVIDERS_HPP

#include <string>
#include "../libraries/singularity/include/singularity.hpp"

class provider_t
{
public:
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block() = 0;
};

class stack_provider_t
{
public:
    virtual std::map<std::string, singularity::money_t> get_stack() = 0;
};

class prefix_aware_t
{
public:
    prefix_aware_t(std::string prefix):prefix(prefix) {}
    std::string get_prefix()
    {
        return prefix;
    }
private:
    std::string prefix;
};

class testnet_provider_t: public provider_t
{
public:
    testnet_provider_t(std::string relations_filename)
        :relations_filename(relations_filename) {}
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block();
private:
    std::string relations_filename;
};

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

class stress_test_provider_t: public provider_t, public prefix_aware_t
{
public:
    stress_test_provider_t(std::string prefix, uint32_t num_accounts, uint32_t num_transactions, uint32_t max_amount)
    :prefix_aware_t(prefix), num_accounts(num_accounts), num_transactions(num_transactions), max_amount(max_amount) {};
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block();
private:
    uint32_t num_accounts;
    uint32_t num_transactions;
    uint32_t max_amount;
};

class cycle_attack_provider_t: public provider_t, public prefix_aware_t
{
public:
    cycle_attack_provider_t(std::string prefix, uint32_t num_accounts, uint32_t num_transactions, uint32_t max_amount)
    :prefix_aware_t(prefix), num_accounts(num_accounts), num_transactions(num_transactions), max_amount(max_amount) {};
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block();
private:
    uint32_t num_accounts;
    uint32_t num_transactions;
    uint32_t max_amount;
};

class concentrate_attack_provider_t: public provider_t, public prefix_aware_t
{
public:
    concentrate_attack_provider_t(std::string prefix, uint32_t num_accounts, uint32_t num_transactions, uint32_t max_amount)
    :prefix_aware_t(prefix), num_accounts(num_accounts), num_transactions(num_transactions), max_amount(max_amount) {};
    virtual std::vector<std::shared_ptr<singularity::relation_t> > get_block();
private:
    uint32_t num_accounts;
    uint32_t num_transactions;
    uint32_t max_amount;
};

class stack_import_from_file_provider_t: public stack_provider_t
{
public:
    stack_import_from_file_provider_t(std::string file_name): file_name(file_name) {};
    virtual std::map<std::string, singularity::money_t> get_stack();
private:
    std::string file_name;
};

#endif
