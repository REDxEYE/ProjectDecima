#ifndef OPENMC_LAYER_HPP
#define OPENMC_LAYER_HPP

#include <memory>
#include <utility>

namespace omc {
    class application;
    class camera;
    class opengl_backend;

    class layer {
    public:
        omc::application* app = nullptr;

        explicit layer(omc::application* app);

        explicit layer() = default;

        virtual void on_attach() {};

        virtual void on_detach() {};

        virtual void on_update(double ts) {};

        omc::opengl_backend* opengl();
    };

    class camera_layer : public layer {
        omc::camera* camera;
    public:
        explicit camera_layer(omc::application* app);
        void on_update(double ts) override;
    };
}

#endif //OPENMC_LAYER_HPP
