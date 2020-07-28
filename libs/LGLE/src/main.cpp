#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>


#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui.h>

#include <utility>


#include <application/opengl_backend.h>
#include <application/application.hpp>

#include <input.h>


class imgui_layer : public omc::layer {

public:
    explicit imgui_layer(omc::application* app) : layer(app) {};

    void on_attach() override {
        layer::on_attach();
    }

    void on_detach() override {
        layer::on_detach();
    }

    void on_update(double ts) override {
        omc::opengl_backend::imgui_new_frame();
        {
            ImGui::Begin("DEBUG");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);

            ImGui::End();
        }
        omc::opengl_backend::imgui_render_frame();

    }

};

int main() {
    auto app = std::make_shared<omc::application>("Application", 1280, 720, true);

    app->push_layer(std::make_shared<imgui_layer>(app.get()));

    app->run();
}
