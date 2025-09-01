#include "gp/physics/physics.hpp"


    void apply_input(Kinematics& kin, const MovementParams& move, Flags& flags, const Controls& in) {
        float dir = (in.right ? 1.f : 0.f) - (in.left ? 1.f : 0.f);
        kin.vx = dir * move.move_speed;
        if (in.jump && flags.grounded) { kin.vy = -move.jump_force; flags.grounded = false; }
    }

    void integrate(Transform& tf, Kinematics& kin, float dt, float gravity) {
        kin.ay  = gravity;
        kin.vy += kin.ay * dt;
        tf.x   += kin.vx * dt;
        tf.y   += kin.vy * dt;
    }

    void clamp_to_ground(Transform& tf, Kinematics& kin, Flags& flags, float ground_y) {
        if (tf.y + tf.h > ground_y) { tf.y = ground_y - tf.h; kin.vy = 0.f; flags.grounded = true; }
    }
