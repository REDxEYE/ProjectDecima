//
// Created by i.getsman on 05.08.2020.
//

#include "archive_array.h"
#include "archive.h"

int main() {
    Decima::ArchiveArray archive_array(R"(F:\SteamLibrary\steamapps\common\Death Stranding\data)");

    archive_array.read_prefetch_file();

    return 0;
}