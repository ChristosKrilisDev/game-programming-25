#include <iostream>
#include <ostream>
#include <SDL3/SDL.h>



// void readPlayerMovementInput(SDL_Keycode input) {
//
// }
//
// void readPlayerJumpInput(SDL_Keycode input) {
//
// }
//
//
// void playerMovement(SDL_FRect playerRect, int direction, float moSpeed) {
// 	playerRect.x += moSpeed * direction;
// }
//
// void playerJump(SDL_FRect playerRect, int jumpForce) {
// 	playerRect.y += jumpForce;
// }
//
// void applyGravity(SDL_FRect obj, int groundHeight) {
// 	if (obj.y > groundHeight) {
// 		obj.y = groundHeight;
// 	}
//
// 	obj.y -= obj.w * 2;
// }


int main(void)
{
	SDL_Log("hello sdl");
	
	float window_w = 1200;
	float window_h = 800;
	int target_framerate_ms = 1000 / 60;       // 16 milliseconds
	int target_framerate_ns = 1000000000 / 60; // 16666666 nanoseconds

	SDL_Window* window = SDL_CreateWindow("E00 - introduction", window_w, window_h, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	// increase the zoom to make debug text more legible
	// (ie, on the class projector, we will usually use 2)
	{
		float zoom = 1;
		window_w /= zoom;
		window_h /= zoom;
		SDL_SetRenderScale(renderer, zoom, zoom);
	}

	bool quit = false;

	SDL_Time walltime_frame_beg;
	SDL_Time walltime_work_end;
	SDL_Time walltime_frame_end = 0;
	SDL_Time time_elapsed_frame;
	SDL_Time time_elapsed_work;

	int delay_type = 0;

	float player_size = 40;
	SDL_FRect player_rect;
	player_rect.w = player_size;
	player_rect.h = player_size;
	player_rect.x = window_w / 2 - player_size / 2;
	player_rect.y = window_h / 2 - player_size / 2;


	float ground = 50;
	SDL_FRect ground_rect;
	ground_rect.w = ground;
	ground_rect.h = ground/2;
	ground_rect.x = window_w;
	ground_rect.y = window_h;


	bool btn_pressed_up = false;


	float player_speed = 10;
	float jumpForce = 5;
	int pX = 0;
	float pY = 0;
	bool isGrounded = true;

	SDL_GetCurrentTime(&walltime_frame_beg);
	while(!quit)
	{
		// input
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			int nums;
			switch(event.type)
			{
				case SDL_EVENT_QUIT:
					quit = true;
					break;
				case SDL_EVENT_KEY_DOWN:
					if(event.key.key >= SDLK_0 && event.key.key < SDLK_5) {
						delay_type = event.key.key - SDLK_0;
					}

					//find direction
					if (event.key.key == SDLK_A){
						pX = -1;
					}
					if (event.key.key == SDLK_D){
						pX = +1;
					}
					if (event.key.key == SDLK_SPACE) {
						if (isGrounded) {
							std::cout << "Jump" << std::endl;
							isGrounded = false;
							pY = -1;
						}
					}

					std::cout << pX << " - " << pY << std::endl;

					break;
			}
		}

		float gravity = 1;

		//add gravity?
		if (!isGrounded && pY < 1) {
			if (pY>0) {
		//		gravity = 3;
			}
			pY += 0.01;
			std::cout << "~~ " << pY << std::endl;
		}
		else {
			pY = 0;
			isGrounded = true;
		}

		//check boarders before moving
		if (abs(player_rect.x) < window_w/2 - 100) {
			//move player
		}

		player_rect.x += player_speed * pX;
		player_rect.y += jumpForce * pY * gravity;


		// clear screen
		// NOTE: `0x` prefix means we are expressing the number in hexadecimal (base 16)
		//       `0b` is another useful prefix, expresses the number in binary
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0x1C, 0x83, 0xFF, 0XFF);
		SDL_RenderFillRect(renderer, &player_rect);

		SDL_SetRenderDrawColor(renderer, 0x4C, 0x23, 0xFF, 0XFF);
		SDL_RenderFillRect(renderer, &ground_rect);

		SDL_GetCurrentTime(&walltime_work_end);
		time_elapsed_work = walltime_work_end - walltime_frame_beg;

		if(target_framerate_ns > time_elapsed_work)
		{
			switch(delay_type)
			{
				case 0:
				{
					// busy wait - very precise, but costly
					SDL_Time walltime_busywait = walltime_work_end;
					while(walltime_busywait - walltime_frame_beg < target_framerate_ns)
						SDL_GetCurrentTime(&walltime_busywait);
					break;
				}
				case 1:
				{
					// simple delay - too imprecise
					// NOTE: `SDL_Delay` gets milliseconds, but our timer gives us nanoseconds! We need to covert it manually
					SDL_Delay((target_framerate_ns - time_elapsed_work) / 1000000);
					break;
				}
				case 2:
				{
					// delay ns - also too imprecise
					SDL_DelayNS(target_framerate_ns - time_elapsed_work);
					break;
				}
				case 3:
				{
					// delay precise
					SDL_DelayPrecise(target_framerate_ns - time_elapsed_work);
					break;
				}
				case 4:
				{
					// custom delay - we use the sleeping delay with an arbitrary margin, then we busywait what's left
					SDL_DelayNS(target_framerate_ns - time_elapsed_work - 1000000);
					SDL_Time walltime_busywait = walltime_work_end;

					while(walltime_busywait - walltime_frame_beg < target_framerate_ns)
						SDL_GetCurrentTime(&walltime_busywait);
					break;
				}
			}
		}

		SDL_GetCurrentTime(&walltime_frame_end);
		time_elapsed_frame = walltime_frame_end - walltime_frame_beg;

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 10.0f, "elapsed (frame): %9.6f ms", (float)time_elapsed_frame/(float)1000000);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 20.0f, "elapsed(work   : %9.6f ms", (float)time_elapsed_work/(float)1000000);
		SDL_RenderDebugTextFormat(renderer, 10.0f, 30.0f, "delay type: %d (change with 0-4)", delay_type);


		// render
		SDL_RenderPresent(renderer);
		
		walltime_frame_beg = walltime_frame_end;
	}

	return 0;
};