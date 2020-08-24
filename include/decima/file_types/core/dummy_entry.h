//
// Created by MED45 on 06.08.2020.
//

#ifndef PROJECTDS_DUMMY_ENTRY_H
#define PROJECTDS_DUMMY_ENTRY_H

#include "entry.h"

namespace Decima {
    class Dummy : public Decima::CoreEntry {
    public:
        void parse(ArchiveArray& archives, Source& stream, CoreFile* core_file) override;
    };
}

#endif //PROJECTDS_DUMMY_ENTRY_H
