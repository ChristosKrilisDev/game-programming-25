//
// Created by krili on 25/08/2025.
//

#pragma once
#include <cstdint>

    struct Transform {
        float x{0}, y{0}, w{0}, h{0};
        static Transform XYWH(float x, float y, float w, float h) { return {x,y,w,h}; }
    };

    struct Kinematics {
        float vx{0}, vy{0};
        float ax{0}, ay{0};
        float mass{1.0f};
    };

    struct MovementParams {
        float move_speed{250.0f};  // px/s
        float jump_force{420.0f};  // px/s impulse
    };

    struct Flags {
        bool grounded{false};
        bool is_ai{false};
    };

    struct Controls {
        bool left{false}, right{false}, jump{false};
    };

    using RGBA = std::uint32_t; // 0xRRGGBBAA

