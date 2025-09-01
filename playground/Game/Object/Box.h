#pragma once
#include <SDL3/SDL.h>           // single include guards against mixing SDL2/SDL3

struct Transform;

class Box {
public:
    explicit Box(unsigned int rgba) : rgba_(rgba) {}

    void render(SDL_Renderer* r, const Transform& t) const {
        const SDL_FRect rect{ t.x, t.y, t.w, t.h };

        SDL_SetRenderDrawColor(r,
            Uint8((rgba_ >> 24) & 0xFF),
            Uint8((rgba_ >> 16) & 0xFF),
            Uint8((rgba_ >>  8) & 0xFF),
            Uint8( rgba_        & 0xFF));

        // If you ever link against SDL2 by mistake, this line will fail to compile,
        // which is what we want (fail fast).
        SDL_RenderFillRect(r, &rect);
    }
private:
    unsigned int rgba_;
};
