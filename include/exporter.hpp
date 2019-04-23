
#ifndef EXPORTER_HPP
#define EXPORTER_HPP

#include <ostream>
#include "../libraries/singularity/include/singularity.hpp"

class exporter 
{
public:
    void export_transactions(std::ostream& output, std::vector<std::shared_ptr<singularity::relation_t> > relations)
    {
        for(auto relation: relations) {
            std::shared_ptr<singularity::transaction_t> t = std::dynamic_pointer_cast<singularity::transaction_t>(relation);
            if(t) {
                output << t->get_source() << "\t" << t->get_target() << "\t" << t->get_amount() << "\t" << t->get_height() << "\n";
            }
        }
    };

    void export_rank(std::ostream& output, std::shared_ptr<singularity::account_activity_index_map_t> ranks)
    {
        std::cout << "start" << "\n";
        for(auto rank: *ranks) {
//             std::cout << rank.first << "\t" << /* rank.second.str() */ "0" << "\n";
            output << rank.first << "\t" << rank.second.str() << "\n";
        }
    };
    
};

#endif
