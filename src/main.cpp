//
// Created by i.getsman on 06.08.2020.
//

#include "tracy_memdbg.hpp"
#include "Tracy.hpp"
#include "projectds_app.hpp"
int main() {
    ZoneScopedN("Main app")
    tracy::SetThreadName("Main");
    ProjectDS app({ 1700, 720 }, "Project Decima [" DECIMA_VERSION "]");
    app.init();
    app.run();
}
