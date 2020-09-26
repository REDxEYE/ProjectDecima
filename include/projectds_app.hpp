#pragma once

#include "app.hpp"

#include <imgui.h>
#include <imgui_memory_editor.h>

#include "decima/archive/archive_manager.hpp"
#include "decima/archive/archive_tree.hpp"

class ProjectDS : public App {
public:
    enum class Popup {
        None,
        About,
        AppendExportByName,
        AppendExportByHash,
        Shortcuts
    };

    struct ShortcutInfo {
        std::string_view name;
        std::string_view description;
        std::size_t key;
        ImGuiKeyModFlags mods;
        std::function<void()> callback;
    };

public:
    ProjectDS(const std::pair<uint32_t, uint32_t>& windowSize, const std::string& title,
        bool imgui_multi_viewport = false);

public:
    bool m_multi_viewport;
    Popup current_popup = Popup::None;
    std::vector<ShortcutInfo> shortcuts;

    Decima::ArchiveManager archive_array;
    std::vector<const char*> file_names;
    FileTree root_tree;
    SelectionInfo selection_info;
    ImGuiTextFilter filter;
    MemoryEditor file_viewer;
    bool root_tree_constructing { false };

    void init_user() override;

    void init_imgui();

    void init_filetype_handlers();

protected:
    void update_user(double ts) override;
    void input_user() override;

    void draw_dockspace();
    void draw_filepreview();
    void draw_tree();
    void draw_export();

    void begin_frame_user() override;

    void end_frame_user() override;
};
