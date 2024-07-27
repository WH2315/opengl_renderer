#pragma once

#include <imgui.h>

namespace wen {

class Imgui final {
public:
    Imgui();
    ~Imgui();

    void init();
    void begin();
    void end();
    void shutdown();
};

} // namespace wen