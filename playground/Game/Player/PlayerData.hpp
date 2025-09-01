//
// Created by krili on 25/08/2025.
//

#pragma once
#include <SDL3/SDL.h>
#include <types.hpp>

struct MovementParams {
    float movement_speed{250.0f};
    float jump_force{420.0f};
};

struct Flags {
    bool grounded{false};
    bool is_ai{false};
};

struct Controls {
    bool left{false};
    bool right{false};
    bool jump{false};
};

struct PlayerData {
    Transform transform;
    Kinematics kinematics;
    struct MovementParams movement_params;
    struct Flags flags;
    struct Controls controls;
    int color{static_cast<int>(0xff'ff'ff'ff)};
};


inline void applyInput(PlayerData& player_data, const Controls& controls, float dt) {
    float desired = 0.0f;
    if (controls.left) {
        desired -= 1.0f;
    }
    if (controls.right) {
        desired += 1.0f;
    }

    player_data.kinematics.vx = desired * player_data.movement_params.movement_speed;

    if (controls.jump && player_data.flags.grounded) {
        player_data.kinematics.vy = -player_data.movement_params.jump_force;
        player_data.flags.grounded = false;
    }
}

inline void simulatePhysics(PlayerData& player_data, float dt, float gravity, float ground_y) {
    player_data.kinematics.ay = gravity;
    player_data.kinematics.vy += player_data.kinematics.ay * dt;
    player_data.transform.x += player_data.kinematics.vx * dt;
    player_data.transform.y += player_data.kinematics.vy * dt;

    if (player_data.transform.y + player_data.transform.h > ground_y) {
        player_data.transform.y   = ground_y - player_data.transform.h;
        player_data.kinematics.vy = 0.f;
        player_data.flags.grounded = true;
    }
    else {
        player_data.flags.grounded = false;
    }
}

inline SDL_FRect toSDLRect(const PlayerData& player_data) {
    return SDL_FRect{
        player_data.transform.x,
        player_data.transform.y,
        player_data.transform.w,
        player_data.transform.h
    };
}