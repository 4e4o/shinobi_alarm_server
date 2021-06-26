#include "MotionReceiver.h"

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;
using TCamMap = MotionReceiver::TCamMap;

void process_program_options(const int argc, const char *const argv[],
                             std::string& s_ac_id,
                             TCamMap &id) {
    typedef std::vector<std::string> TShinobiIds;
    typedef std::vector<int> TCamIds;

    po::options_description desc("Usage");
    desc.add_options()
            ("s_ac_id", po::value<std::string>(), "description")
            ("s_id", po::value<TShinobiIds>()->multitoken(), "description")            
            ("c_id", po::value<TCamIds>()->multitoken(), "description");

    po::variables_map vm;
    TShinobiIds s_ids;
    TCamIds c_ids;

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (!vm["s_ac_id"].empty()) {
            s_ac_id = vm["s_ac_id"].as<std::string>();
        }

        if (!vm["s_id"].empty()) {
            s_ids = vm["s_id"].as<std::vector<std::string> >();
        }

        if (!vm["c_id"].empty()) {
            c_ids = vm["c_id"].as<std::vector<int> >();
        }

        if (s_ids.size() != c_ids.size())
            throw std::runtime_error("s_id size must be equal to c_id");

        for (size_t i = 0; i < s_ids.size(); ++i)
            id[c_ids[i]] = s_ids[i];
    } catch (po::error const& e) {
        std::cerr << e.what() << '\n';
        exit( EXIT_FAILURE );
    }
}

int main (int argc, char *argv[]) {
    using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;

    try {
        TCamMap ids;
        std::string s_ac_id;
        process_program_options(argc, argv, s_ac_id, ids);
        boost::asio::io_context io_context;
        MotionReceiver mr(io_context, s_ac_id, ids);
        mr.start();
        work_guard_type work_guard(io_context.get_executor());
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
