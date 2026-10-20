#pragma once

namespace TigerEngine {
    using KeyCode = int;

    // These match GLFW key codes exactly for direct mapping
    namespace Key {
        constexpr KeyCode Space  = 32;
        constexpr KeyCode W      = 87;
        constexpr KeyCode A      = 65;
        constexpr KeyCode S      = 83;
        constexpr KeyCode D      = 68;
        constexpr KeyCode Escape = 256;
        constexpr KeyCode Up     = 265;
        constexpr KeyCode Down   = 264;
        constexpr KeyCode Left   = 263;
        constexpr KeyCode Right  = 262;
    }
}