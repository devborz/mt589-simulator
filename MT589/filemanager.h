#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <emulator.hpp>
#include <string>
#include <json.hpp>

using nlohmann::json;

namespace fm {

    struct programm_data {
        MK589 mk;
        int start_row;
        int start_col;
    };

    void write_to_file(const std::string& filename, const json& data);

    std::string read_from_file(const std::string& filename);

    programm_data get_data(const std::string& filename);

    void save(const std::string& filename, MK589& mk, int startCol, int startRow, MT::Mode mode);

}

#endif // FILEMANAGER_H
