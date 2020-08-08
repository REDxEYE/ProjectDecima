//
// Created by i.getsman on 05.08.2020.
//

#include "decima/archive/archive_array.h"
#include "decima/archive/archive.h"

int main() {
    Decima::ArchiveArray archive_array(R"(F:\SteamLibrary\steamapps\common\Horizon Zero Dawn\Packed_DX12)");

    archive_array.read_prefetch_file();

    return 0;
}
