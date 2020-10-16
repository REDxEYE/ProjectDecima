//
// Created by i.getsman on 06.08.2020.
//

#include "projectds_app.hpp"
#include <util/pfd.h>

int main() {
    /*
     * TODO: Initialize manager here (load settings, set up compressor)
     */
    Decima::ArchiveManager manager;

    while (true) {
        std::vector<std::string> compressor_path = pfd::open_file("Select compressor library file", "", {"oo2core_7_win64.dll", "oo2core_7_win64.dll"}).result();

        if (!compressor_path.empty()) {
            manager.compressor = std::make_unique<Decima::Compressor>(compressor_path[0]);
            break;
        }
    }

    ProjectDS app(std::move(manager), { 1700, 720 }, "Project Decima [" DECIMA_VERSION "]");
    app.init();
    app.run();
}
