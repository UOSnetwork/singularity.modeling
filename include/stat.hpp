
#ifndef STAT_HPP
#define STAT_HPP

#include <string>
#include <map>
#include "../libraries/singularity/include/singularity.hpp"
#include "../libraries/singularity/include/gravity_index_calculator.hpp"

struct AccountStat
{
    std::string id;
    int32_t upvote_count;
    int32_t downvote_count;
    int32_t post_count;
    singularity::double_type rating;
};

struct PostStat
{
    std::string id;
    std::string owner_id;
    std::string parent_id;
    int32_t upvote_count;
    int32_t downvote_count;
    singularity::double_type rating;
};

class Stat
{
public:
    void add_block(std::vector<std::shared_ptr<singularity::relation_t> > relations)
    {
        using namespace singularity;
        for (auto r: relations) {
            if (r->get_source_type() == node_type::ACCOUNT) {
                auto it = account_stat_map.find(r->get_source());
                if (it == account_stat_map.end()) {
                    account_stat_map[r->get_source()] = AccountStat();
                }
            }
            if (r->get_target_type() == node_type::ACCOUNT) {
                auto it = account_stat_map.find(r->get_target());
                if (it == account_stat_map.end()) {
                    account_stat_map[r->get_target()] = AccountStat();
                }
            }
            if (r->get_source_type() == node_type::CONTENT) {
                auto it = post_stat_map.find(r->get_source());
                if (it == post_stat_map.end()) {
                    post_stat_map[r->get_source()] = PostStat();
                }
            }
            if (r->get_target_type() == node_type::CONTENT) {
                auto it = post_stat_map.find(r->get_target());
                if (it == post_stat_map.end()) {
                    post_stat_map[r->get_target()] = PostStat();
                }
            }
        }
        
        for (auto r: relations) {
            if (r->get_name() == "OWNERSHIP") {
                post_stat_map[r->get_target()].owner_id = r->get_source();
                account_stat_map[r->get_source()].post_count++;
            }
        }
        
        for (auto r: relations) {
            if (r->get_name() == "DOWNVOTE") {
                post_stat_map[r->get_target()].downvote_count++;
                account_stat_map[post_stat_map[r->get_target()].owner_id].downvote_count++;
            }
        }

        for (auto r: relations) {
            if (r->get_name() == "UPVOTE") {
                post_stat_map[r->get_target()].upvote_count++;
                account_stat_map[post_stat_map[r->get_target()].owner_id].upvote_count++;
            }
        }
    };
    
    void add_account_rating(const singularity::account_activity_index_map_t& rating_map)
    {
        using namespace singularity;
        
        auto scaled_rating_map = gravity_index_calculator::scale_activity_index_to_node_count(rating_map);
        
        for(auto im: scaled_rating_map) {
            account_stat_map[im.first].rating = im.second;
        }
    };
    
    void add_content_rating(const singularity::account_activity_index_map_t& rating_map)
    {
        using namespace singularity;
        
        auto scaled_rating_map = gravity_index_calculator::scale_activity_index_to_node_count(rating_map);
        
        for(auto im: scaled_rating_map) {
            post_stat_map[im.first].rating = im.second;
        }
    };
    
    void export_account_stat(std::ostream& output)
    {
        std::cout << "start" << "\n";
        for(auto stat: account_stat_map) {
            output << stat.first << "\t" << stat.second.rating.str(6)  << "\t" << stat.second.post_count  << "\t" << stat.second.upvote_count  << "\t" << stat.second.downvote_count << "\n";
        }
    };
    
    void export_post_stat(std::ostream& output)
    {
        std::cout << "start" << "\n";
        for(auto stat: post_stat_map) {
            output << stat.first  << "\t" << stat.second.owner_id << "\t" << stat.second.rating.str(6) << "\t" << stat.second.upvote_count  << "\t" << stat.second.downvote_count << "\n";
        }
    };
    
    std::map<std::string, AccountStat> account_stat_map;
    std::map<std::string, PostStat> post_stat_map;
};


#endif

