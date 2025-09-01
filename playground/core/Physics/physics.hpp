#pragma once
#include "gp/core/types.hpp"


    // Stateless helpers
    void apply_input(gp::Kinematics& kin,
                     const gp::MovementParams& move,
                     gp::Flags& flags,
                     const gp::Controls& input);

    void integrate(gp::Transform& tf, gp::Kinematics& kin, float dt, float gravity_px_s2);

    // Simple ground collision (top edge at ground_y)
    void clamp_to_ground(gp::Transform& tf, gp::Kinematics& kin, gp::Flags& flags, float ground_y);
