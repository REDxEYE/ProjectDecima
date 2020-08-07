//
// Created by i.getsman on 07.08.2020.
//

#include "projectds_app.hpp"

#include "decima/file_types.hpp"


void ProjectDS::parse_core_file() {
    parsed_files.clear();
    imemstream stream(selection_info.file.storage);

    while (stream.tellg() < selection_info.file.storage.size()) {
        uint64_t magic = Decima::CoreFile::peek_header(stream);

        switch (magic) {
            case (Decima::DeathStranding_FileMagics::Localization): {
                Decima::Localization localization;
                localization.parse(stream);
                parsed_files.push_back(std::make_shared<Decima::Localization>(localization));
                break;
            }
            case (Decima::DeathStranding_FileMagics::Texture): {
                Decima::Texture texture;
                texture.parse(stream);
                parsed_files.push_back(std::make_shared<Decima::Texture>(texture));
                break;
            }
            default:{
                Decima::Dummy dummy;
                dummy.parse(stream);
                parsed_files.push_back(std::make_shared<Decima::Dummy>(dummy));
                break;
            }
        }

    }
}