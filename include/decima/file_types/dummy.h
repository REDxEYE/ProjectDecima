//
// Created by MED45 on 06.08.2020.
//

#ifndef PROJECTDS_DUMMY_H
#define PROJECTDS_DUMMY_H

#include "decima/file_types/core.h"

namespace Decima {
    class Dummy : public Decima::CoreFile {
    public:
        void parse(Source& stream) override;
    };
}

#endif //PROJECTDS_DUMMY_H
