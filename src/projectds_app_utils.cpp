//
// Created by i.getsman on 07.08.2020.
//

#include "projectds_app.hpp"

#include "decima/file_types.hpp"


void ProjectDS::parse_core_file() {
    imemstream stream(selection_info.file.storage);

    while (stream.tellg() < selection_info.file.storage.size()) {
        uint64_t magic = Decima::CoreFile::peek_header(stream);

        switch (magic) {
            case (Decima::DeathStranding_FileMagics::Localization): {
                Decima::Localization localization;
                localization.parse(stream);
                parsed_files.push_back(std::make_shared<Decima::Localization>(localization));
            }
            default:{
                Decima::Dummy dummy;
                dummy.parse(stream);
                parsed_files.push_back(std::make_shared<Decima::Dummy>(dummy));
            }
        }

    }
}