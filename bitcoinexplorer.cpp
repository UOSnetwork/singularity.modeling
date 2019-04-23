
#include "include/bitcoinexplorer.hpp"
#include <bitcoin/bitcoin/formats/base_16.hpp>
#include <string.h>
#include <iostream>
#include <fstream>
#include "libraries/singularity/include/singularity.hpp"

using namespace std;
using namespace singularity;
namespace pt = boost::property_tree;

std::string bitcoin_explorer::convert_script_sig_to_address (std::string script_sig)
{
    string::size_type pos = script_sig.find_last_of(" ");
    
    if (pos != string::npos) {
        string public_key = script_sig.substr(pos + 1);
        
//         cout << "PK: " << public_key << endl;
        string address = this->convert_public_key_to_address(public_key);
        
        return address;
    }
    
    return "";
}

std::vector<std::shared_ptr<singularity::relation_t> > bitcoin_explorer::extract_transactions(std::string filename, uint32_t block_height)
{
    uint32_t used_count = 0;
    uint32_t ignored_count = 0;
    
    double used_value = 0;
    double ignored_value = 0;
    
    std::vector<std::shared_ptr<relation_t> > result;
    
    ifstream file(filename);
    
    pt::ptree block_json;
    
    pt::read_json(file, block_json);
    
    
    for (auto transaction_json: block_json.get_child("tx")) {
        string txid = transaction_json.second.get<string>("txid");
//         cout << "TXID: " << txid << endl;
        
        auto vin_collection_json = transaction_json.second.get_child("vin");
        
        string from = "";
        
        std::set<std::string> from_addresses;
        
        for (auto vin_json: vin_collection_json) {
            auto script_sig = vin_json.second.get_optional<string>("scriptSig.asm");
                /* 
                 * Если scriptSig отсутствует, то это первая транзакция блока, содержащая вознаграждение витнесу.
                 * Оставляем from пустым
                 */
            if (script_sig) {
//                     cout << "scriptSig: " << script_sig.get() << endl;
                string local_from = convert_script_sig_to_address(script_sig.get());
                from_addresses.insert(local_from);
//                     cout << "local_from: " << local_from << endl;
            }
                
        }
        
        if (from_addresses.size() != 1) {
            /* 
             * Пока не учитываем транзакции, в которых больше одного исходящего адреса, 
             * поскольку это невозможно сделать без механизма доступа 
             * к произвольному предыдущему блоку 
             */
            from = "";
        } else {
            auto from_it = from_addresses.cbegin();
        
            from = *from_it;
//         cout << "from: " << from << endl;
        }
        
        
        for (auto vout_json: transaction_json.second.get_child("vout")) {
            auto value = vout_json.second.get<double>("value");
            
//             cout << "value: " << value << endl;

            auto type = vout_json.second.get_optional<string>("scriptPubKey.type");

            if (!type || type.get() != "pubkeyhash") {
                /* Погка учитываем только транзакции типа PUBKEYHASH, для учета других типов требуется знать поля vin из последующих блоков */
                continue;
            }
            
            auto addresses = vout_json.second.get_child_optional("scriptPubKey.addresses");
            
            string to = "";
            
            if (addresses && addresses.get().size() == 1) {
                /* 
                 * Для транзакций типа PUBKEYHASH всегда один адрес;
                 * Нужно проверить, что поле addresses присутствует, если нет - адрес вычислять из хэша публичного ключа,  
                 * взятого из scriptPubKey
                 */
                for (auto address: addresses.get()) {
                    to = address.second.get_value<string>();
//                     cout << "to: " << to << endl;
                }
            } else {
                to = "";
//                 continue;
            }
            
//             cout << "Adding a transaction" << endl;
            
            if (!from.empty() && !to.empty() && !(from == to) && value > 1.5 ) {
                
                auto transaction = std::make_shared<transaction_t>(
                    100000000 * value, // Пересчитываем в сатоши 
                    0, // Комиссия учитывается отдельной coinbase транзакцией 
                    from, 
                    to, 
                    time(nullptr), // Время нигде при расчете не учитывается, ставим произвольное 
                    0, 0, // Балансы эккаунтов не можем вычислить, не зная всю предыдущую историю блоков
                    block_height
                );
                
                result.push_back(transaction);
                
                used_count++;
                used_value += value;
            } else {
                ignored_count++;
                ignored_value += value;
            }
        }
    }
    
    cout << "Used count: " << used_count << endl;
    cout << "Used value: " << used_value << endl;
    cout << "Ignored count: " << ignored_count << endl;
    cout << "Ignored value: " << ignored_value << endl;
    
    return result;
}


std::string bitcoin_explorer::convert_public_key_to_address(std::string public_key, address_type_t address_type)
{
    bc::data_chunk public_key_bin;
    
    bc::decode_base16(public_key_bin, public_key);
    
    auto public_key_hash = bc::bitcoin_short_hash(public_key_bin);
     
    bc::one_byte addr_prefix;
    
    if (address_type == PUBKEYHASH) {
        addr_prefix =  { { 0x00 } }; //Testnet 0x6f
    } else {
        addr_prefix =  { { 0x05 } };
    }
     
    bc::data_chunk prefix_pubkey_checksum(bc::to_chunk(addr_prefix));
    bc::extend_data(prefix_pubkey_checksum, public_key_hash);
    bc::append_checksum(prefix_pubkey_checksum);

    return bc::encode_base58(prefix_pubkey_checksum);
}

std::string bitcoin_explorer::convert_public_key_hash_to_address(std::string public_key_hash, address_type_t address_type)
{
    bc::one_byte addr_prefix;
    
    if (address_type == PUBKEYHASH) {
        addr_prefix =  { { 0x00 } }; //Testnet 0x6f
    } else {
        addr_prefix =  { { 0x05 } };
    }
    bc::data_chunk public_key_hash_bin;
    bc::decode_base16(public_key_hash_bin, public_key_hash);
    
    bc::data_chunk prefix_pubkey_checksum(bc::to_chunk(addr_prefix));
    bc::extend_data(prefix_pubkey_checksum, public_key_hash_bin);
    bc::append_checksum(prefix_pubkey_checksum);

    return bc::encode_base58(prefix_pubkey_checksum);
}


void bitcoin_explorer::test()
{
}
