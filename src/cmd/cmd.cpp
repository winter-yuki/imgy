#include "include/cmd/cmd.hpp"

#include <stdexcept>

// TODO(rename cli)


Cmd::Cmd(int argc, char **argv)
{
    for (size_t i = 1; i < argc; ++i) {
        args_.emplace_back(argv[i]);
    }
    parse();
}


void Cmd::execute()
{
    // TODO()
}


void Cmd::parse() const
{
    if (args_.size() < 2) {
        throw std::runtime_error("no arguments"); // TODO (help msg)
    }

    const size_t I_QUERY_TYPE = 0;
    const size_t I_FILENAME   = 1;

    const std::string render  = "rnd";
    const std::string process = "proc";

    if (args_[I_QUERY_TYPE] == render) {
        std::ifstream ifs(args_[I_FILENAME]);
        render_parse(ifs);
    } else if (args_[I_QUERY_TYPE] == process) {
        std::ifstream ifs(args_[I_FILENAME]);
        proc_parse(ifs);
    } else {
        throw std::runtime_error("wrong working mode");
    }
}


void Cmd::render_parse(std::ifstream & ifs) const
{
    // TODO()
}


void Cmd::proc_parse(std::ifstream & ifs) const
{
    // TODO(xml)
}

























