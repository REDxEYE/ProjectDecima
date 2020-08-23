//
// Created by MED45 on 06.08.2020.
//

#ifndef PROJECTDS_DUMMY_H
#define PROJECTDS_DUMMY_H

#include "core.h"

namespace Decima {
    class Dummy : public Decima::CoreEntry {
    public:
        void parse(ArchiveArray& archives, Source& stream) override;
    };
}

#endif //PROJECTDS_DUMMY_H
