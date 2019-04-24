if (WITH_BITCOIN)
    add_executable(test_bitcoin_explorer "bitcoin_explorer.cpp" "../bitcoinexplorer.cpp")
    target_link_libraries(test_bitcoin_explorer singularity bitcoin boost_system icui18n icuuc)
    add_test(NAME test_bitcoin_explorer COMMAND test_bitcoin_explorer)
endif()

enable_testing ()

