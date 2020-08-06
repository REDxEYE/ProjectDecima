//
// Created by i.getsman on 06.08.2020.
//

#ifndef PROJECTDS_LOCALIZATION_HPP
#define PROJECTDS_LOCALIZATION_HPP

#include "decima/file_types/core.h"

namespace Decima {
    class Localization : public CoreFile {
    public:
        std::string translations[25];


        void parse(std::vector<uint8_t>& buffer) override;

        void parse(std::istream& stream) override;
    };

}
#endif //PROJECTDS_LOCALIZATION_HPP
