//
// Created by i.getsman on 06.08.2020.
//

#include "projectds_app.hpp"

int main() {
    Decima::ArchiveManager manager;

    ProjectDS app(std::move(manager), { 1700, 720 }, "Project Decima [" DECIMA_VERSION "]");
    app.init();
    app.run();
}
