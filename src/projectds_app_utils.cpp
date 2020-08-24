//
// Created by i.getsman on 07.08.2020.
//

#include "projectds_app.hpp"

#include "decima/file_types.hpp"



void ProjectDS::parse_core_file() {
    parsed_files.clear();
    Decima::Source stream(selection_info.file.storage, 1024);




    while (!stream.eof()) {
        const auto offset = stream.tell();
        const auto magic = Decima::CoreEntry::peek_header(stream);

        auto handler = get_handler(magic);
        handler->parse(archive_array, stream);
        handler->offset = offset;

        parsed_files.push_back(std::move(handler));
    }
}