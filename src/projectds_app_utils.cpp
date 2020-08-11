//
// Created by i.getsman on 07.08.2020.
//

#include "projectds_app.hpp"

#include "decima/file_types.hpp"

template <class Base, typename... Args>
using Constructor = std::function<std::unique_ptr<Base>(Args&&...)>;

template <class T, typename... Args>
static std::unique_ptr<Decima::CoreFile> construct(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

void ProjectDS::parse_core_file() {
    parsed_files.clear();
    Decima::Source stream(selection_info.file.storage, 1024);

    static const std::map<std::uint64_t, Constructor<Decima::CoreFile>> types = {
        { Decima::DeathStranding_FileMagics::Translation, construct<Decima::Translation> },
        { Decima::DeathStranding_FileMagics::Texture, construct<Decima::Texture> }
    };

    static const auto get_handler = [](std::uint64_t hash) noexcept {
        const auto handler = types.find(hash);
        return handler != types.end() ? handler->second() : construct<Decima::Dummy>();
    };

    while (!stream.eof()) {
        const auto offset = stream.tell();
        const auto magic = Decima::CoreFile::peek_header(stream);

        auto handler = get_handler(magic);
        handler->parse(archive_array, stream);
        handler->offset = offset;

        parsed_files.push_back(std::move(handler));
    }
}