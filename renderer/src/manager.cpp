#include "manager.hpp"
#include "core/log.hpp"

namespace wen {

void Manager::initializeEngine() {
    g_log = new Log("wen");
    WEN_INFO("Initialize Engine!");
}

void Manager::destroyEngine() {
    WEN_INFO("Destroy Engine!");
    g_log = nullptr;
    delete g_log;
}

} // namespace wen