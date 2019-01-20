#include "include/cmd/cmd.hpp"


//void parse        (int argc, char *argv[]);
//void parse_render (int argc, char *argv[]);
//void parse_process(int argc, char *argv[]);


//void parse(int argc, char *argv[])
//{
//    const size_t min_args = 4;
//    if (argc < min_args) {
//        throw std::runtime_error("no arguments");
//    }

//    const std::string render  = "rnd";
//    const std::string process = "proc";

//    std::string mode = argv[1];

//    if (mode == render) {
//        parse_render(argc, argv);
//    } else if (mode == process) {
//        parse_process(argc, argv);
//    } else {
//        throw std::runtime_error("wrong working mode");
//    }
//}


//void parse_render (int argc, char *argv[])
//{
//    const size_t n_args_need = 4;
//    if (argc != 4) {
//        throw std::runtime_error("Wrong number of arguments in render mode.\n"
//                                 "Need: mode conf_file rez_file");
//    }

//    std::string conf = argv[2];
//    std::string out  = argv[3];
//    // TODO(parse_render)
//}


//void parse_process(int argc, char *argv[])
//{
//    // TODO(process parse)
//}
