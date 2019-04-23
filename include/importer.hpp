
#ifndef IMPORTER_HPP
#define IMPORTER_HPP

#include <fstream>
#include "../libraries/singularity/include/singularity.hpp"

class importer 
{
public:
    std::vector<std::shared_ptr<singularity::relation_t> > import_transactions(std::string filename)
    {
        std::ifstream file(filename);

        std::vector<std::shared_ptr<singularity::relation_t> > transactions;
    
        time_t now = time(nullptr); 
    
        while (file.good()) {
            std::string line;
    
            getline(file, line);
        
            std::vector<std::string> parsed_line;
        
            std::istringstream sline(line);
        
            while (sline.good()) {
                std::string value;
                getline(sline, value, '\t');
                parsed_line.push_back(value);
            }
        
            if (parsed_line.size() == 4) {
//                 std::cout << parsed_line[0] << "; "  << parsed_line[1] << "; " << parsed_line[2] << "; " << parsed_line[3] << "; " << std::endl;
                std::shared_ptr<singularity::transaction_t> t = std::make_shared<singularity::transaction_t> (stol(parsed_line[2]), 0, parsed_line[0], parsed_line[1], now, 0, 0, stol(parsed_line[3]));
                transactions.push_back(t);
            }
        }
        file.close();
        
        return transactions;
    };

    std::vector<std::shared_ptr<singularity::relation_t> > import_relations(std::string filename)
    {
        std::ifstream file(filename);

        std::vector<std::shared_ptr<singularity::relation_t> > transactions;
    
        time_t now = time(nullptr); 
    
        while (file.good()) {
            std::string line;
    
            getline(file, line);
        
            std::vector<std::string> parsed_line;
        
            std::istringstream sline(line);
        
            while (sline.good()) {
                std::string value;
                getline(sline, value, ';');
                parsed_line.push_back(value);
            }
        
            if (parsed_line.size() == 10) {
//                 std::cout << parsed_line[0] << "; "  << parsed_line[1] << "; " << parsed_line[2] << "; " << parsed_line[3] << "; " << std::endl;
                std::shared_ptr<singularity::custom_relation_t> t = std::make_shared<singularity::custom_relation_t> (
                    parsed_line[4], // name
                    parsed_line[2], // source
                    parsed_line[3], // target
                    parsed_line[8] == "ACCOUNT" ? singularity::node_type::ACCOUNT : singularity::node_type::CONTENT,
                    parsed_line[9] == "ACCOUNT" ? singularity::node_type::ACCOUNT : singularity::node_type::CONTENT,
                    0,
                    stol(parsed_line[6]),
                    stol(parsed_line[7]),
                    false
                );
//             std::string source, 
//             std::string target, 
//             node_type source_type, 
//             node_type target_type, 
//             uint64_t height, 
//             int64_t weight,
//             int64_t reverse_weight,
//             bool decayable
                    
                    // stol(parsed_line[2]), 0, parsed_line[0], parsed_line[1], now, 0, 0, stol(parsed_line[3])
                transactions.push_back(t);
            }
        }
        file.close();
        
        return transactions;
    };
    
    std::vector<std::shared_ptr<singularity::relation_t> > rebuild_relations(std::vector<std::shared_ptr<singularity::relation_t> > relations)
    {
        using namespace singularity;
        using namespace std;
        
        std::vector<std::shared_ptr<singularity::relation_t> > rebuilded_relations;
        
        for(auto r: relations) {
            
            std::shared_ptr<singularity::relation_t> new_r;
            
            if (r->get_source() == "" || r->get_target() == "") {
                continue;
            }
            
            if (r->get_name() == "OWNERSHIP") {
                new_r = make_shared<ownwership_t>(r->get_source(), r->get_target(), r->get_height());
            }
            else if (r->get_name() == "UPVOTE") {
                 new_r = make_shared<upvote_t>(r->get_source(), r->get_target(), r->get_height());
            }
            else if (r->get_name() == "DOWNVOTE") {
//                  new_r = make_shared<downvote_t>(r->get_source(), r->get_target(), r->get_height());
            } else {
                cout << "CAUTION: unexpected relation " << r->get_name() << endl;
//                 new_r = r;
            }
            if (new_r) {
                rebuilded_relations.push_back(new_r);
            }
            
        }

        return rebuilded_relations;
    };
    
    
};

#endif

