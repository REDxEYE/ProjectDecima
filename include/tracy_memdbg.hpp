//
// Created by MED45 on 04.09.2020.
//

#ifndef PROJECTDS_TRACY_MEMDBG_HPP
#define PROJECTDS_TRACY_MEMDBG_HPP

#ifdef TRACY_ENABLE

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    #ifdef max
        #undef max
    #endif

    #include "Tracy.hpp"
    #include <cstdint>

void* operator new(std::size_t count);
void operator delete(void* ptr) noexcept;



#else
    #include "Tracy.hpp"
#endif

#endif //PROJECTDS_TRACY_MEMDBG_HPP
