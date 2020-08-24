#pragma once


#include "decima/file_types/pod/pod.hpp"
#include "decima/file_types/pod/string.hpp"
#include "decima/file_types/pod/guid.hpp"

namespace Decima {
    class Reference;
    class ArchiveArray;
    class CoreFile;
    class CoreEntry;

    extern std::vector<Reference*> global_refs;
    extern std::vector<std::pair<Reference*, CoreFile*>> local_refs;



    enum class ReferenceLoadMode : std::uint8_t {
        NotPresent = 0x00,
        Embedded = 0x01,
        ImmediateCoreFile = 0x02,
        CoreFile = 0x03,
        WorkOnly = 0x05
    };

    std::ostream& operator<<(std::ostream&, ReferenceLoadMode);

    class Reference : public POD {
    public:
        std::weak_ptr<CoreEntry> ref;

        void parse(Source& stream);

        void draw();

        inline static void add_ref(Reference* ref, CoreFile* core_file) {
            if (ref->mode() == ReferenceLoadMode::Embedded)local_refs.emplace_back(ref, core_file);
            else if (ref->mode() == ReferenceLoadMode::CoreFile ||
                     ref->mode() == ReferenceLoadMode::ImmediateCoreFile) {
                global_refs.emplace_back(ref);
            }
        }

        static void resolve_local_refs();

        static void resolve_global_refs(ArchiveArray& arch_array);

        inline ReferenceLoadMode mode() const noexcept { return m_mode; }

        inline GUID guid() const noexcept { return m_guid; }

        inline StringHashed file() const noexcept { return m_file; }

    private:
        ReferenceLoadMode m_mode;
        GUID m_guid;
        StringHashed m_file;

        //temporary stuff TODO:refactor this


    };
}