#define BOOST_TEST_MODULE BITCOIN_EXPLORER

#include <boost/test/included/unit_test.hpp>
#include "../include/bitcoinexplorer.hpp"

using namespace singularity;
using namespace boost;
using namespace std;

BOOST_AUTO_TEST_SUITE( address_convertation )

BOOST_AUTO_TEST_CASE( address_convertation_1 )
{
    
    bitcoin_explorer be;
    
    string pk_hash = "631f853224bd8772566ef1cfc4b6b632f2b6b0d4";
    
    string addr0 = "3Aj8abxbwxu5qBP9ENCHWaFeBABj1nR27R";
    
    string addr1 = be.convert_public_key_hash_to_address(pk_hash, SCRIPTHASH);
    
    
    BOOST_CHECK_EQUAL(addr0, addr1);
}

BOOST_AUTO_TEST_CASE( address_convertation_2 )
{
    bitcoin_explorer be;
    
    string pk_hash = "89f80555b0a7ce96162ba72d0c678758f89097f8";
    
    string addr0 = "1DaWh572VuaziAyqMTzyWJFyJkSF1JhgdF";
    
    string addr1 = be.convert_public_key_hash_to_address(pk_hash, PUBKEYHASH);
    
    BOOST_CHECK_EQUAL(addr0, addr1);
}

BOOST_AUTO_TEST_CASE( address_convertation_3 )
{
    
    bitcoin_explorer be;
    
    string pk_hash = "0250863ad64a87ae8a2fe83c1af1a8403cb53f53e486d8511dad8a04887e5b2352";
    
    string addr0 = "1PMycacnJaSqwwJqjawXBErnLsZ7RkXUAs";
    
    string addr1 = be.convert_public_key_to_address(pk_hash, PUBKEYHASH);
    
    
    BOOST_CHECK_EQUAL(addr0, addr1);
}


BOOST_AUTO_TEST_SUITE_END()
