#ifndef INCLUDE_CMD_CMD_HPP
#define INCLUDE_CMD_CMD_HPP

#include <fstream>
#include <string>
#include <vector>


class Cmd final {
public:
    using Args = std::vector<std::string>;

public:
    Cmd(int argc, char **argv);

    void execute();


private:
    void parse       ()                    const;
    void render_parse(std::ifstream & ifs) const;
    void proc_parse  (std::ifstream & ifs) const;

private:
    Args args_;
};


#endif // INCLUDE_CMD_CMD_HPP
