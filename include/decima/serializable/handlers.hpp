#pragma once

#include <unordered_map>
#include <functional>
#include <memory>

namespace Decima {
    class CoreObject;

    template <class Base, typename... Args>
    using Handler = std::function<std::shared_ptr<Base>(Args&&...)>;

    std::invoke_result_t<Handler<CoreObject>> get_type_handler(std::uint64_t hash);

    std::string get_type_name(std::uint64_t hash);
}