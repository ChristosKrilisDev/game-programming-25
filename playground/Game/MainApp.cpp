#include <SDL3/SDL.h>
#include <vector>
#include "Player/PlayerData.hpp"
#include "Object/Box.h"


static inline void spawn_on_ground(PlayerData& p, float ground_y) {
    p.transform.y   = ground_y - p.transform.h; // top-of-ground minus player height
    p.kinematics.vy = 0.0f;
    p.flags.grounded = true;
}


int main() {
    SDL_Init(SDL_INIT_VIDEO);

    float window_w = 1200.f;
    float window_h = 800.f;

    SDL_Window*   window   = SDL_CreateWindow("Game Programming", window_w, window_h, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    // Optional zoom for projector readability
    {
        float zoom = 1.0f;
        SDL_SetRenderScale(renderer, zoom, zoom);
    }

    // --- World setup ---
    const float ground_h = 50.f;
    const float ground_y = window_h - ground_h;
    SDL_FRect ground_rect{ 0.f, ground_y, window_w, ground_h }; // simple ground at bottom



    // --- Players (multiple supported) ---
    std::vector<PlayerData> players;

    PlayerData p1{};
    p1.transform = Transform::XYWH(100.f, 0.f, 50.f, 70.f);
    spawn_on_ground(p1, ground_y);
    p1.flags.grounded = false;
    p1.color = 0x1C83FFFF; // RGBA
    players.push_back(p1);

    // Second player (different color/position) – optional
    PlayerData p2{};
    p2.transform = Transform::XYWH(300.f, 0.f, 50.f, 70.f);
    spawn_on_ground(p2, ground_y);
    p2.color = 0xFF8C00FF; // orange
    players.push_back(p2);

    // One Box renderer per *visual style* (or share one if color differs per player)
    // Here we keep one Box per player for simplicity.
    std::vector<Box> bodies;
    bodies.emplace_back(players[0].color);
    bodies.emplace_back(players[1].color);

    SDL_Log("p1: x=%.1f y=%.1f w=%.1f h=%.1f", p1.transform.x, p1.transform.y, p1.transform.w, p1.transform.h);


    // Timing
    const Uint64 target_framerate_ns = 1'000'000'000ull / 60;
    SDL_Time walltime_frame_beg;
    SDL_GetCurrentTime(&walltime_frame_beg);
    Uint64 last_ticks = SDL_GetTicks();

    bool quit = false;
    int delay_type = 3; // precise delay

    while (!quit) {
        // --- Input (for demo, drive player 0 with keyboard) ---
        Controls c{};
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) { quit = true; }
            if (e.type == SDL_EVENT_KEY_DOWN || e.type == SDL_EVENT_KEY_UP) {
                bool down = (e.type == SDL_EVENT_KEY_DOWN);
                switch (e.key.key) {
                    case SDLK_A:     c.left  = down; break;
                    case SDLK_D:     c.right = down; break;
                    case SDLK_SPACE: c.jump  = down; break;
                    default: break;
                }
            }
        }

        Uint64 now = SDL_GetTicks();
        float dt = float(now - last_ticks) / 1000.0f;   // seconds
        // (Optional) clamp dt to avoid big jumps when paused / breakpointed
        if (dt > 0.05f) dt = 0.05f;                     // 50 ms
        last_ticks = now;

        // --- Update ---
        // drive only player 0 from keyboard in this demo
        applyInput(players[0], c, dt);
        for (auto& p : players){
            simulatePhysics(p, dt, 980.f, ground_y);
        }


        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw ground
        SDL_SetRenderDrawColor(renderer, 0x4C, 0x23, 0xFF, 0xFF);
        SDL_RenderFillRect(renderer, &ground_rect);

        {   SDL_FRect yline{0.f, players[0].transform.y, window_w, 1.f};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &yline);
        }

        // draw players
        for (size_t i = 0; i < players.size(); ++i) {
            // keep Box color in sync if Player color changes
            bodies[i] = Box(players[i].color);
            bodies[i].render(renderer, players[i].transform);
        }

        SDL_RenderPresent(renderer);

        // --- Frame pacing (optional) ---
        SDL_Time work_end; SDL_GetCurrentTime(&work_end);
        SDL_Time work_ns = work_end - walltime_frame_beg;
        if (work_ns < target_framerate_ns) {
            if (delay_type == 3) SDL_DelayPrecise(target_framerate_ns - work_ns);
            else SDL_DelayNS(target_framerate_ns - work_ns);
        }
        SDL_GetCurrentTime(&walltime_frame_beg);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
