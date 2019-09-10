#include <iostream>
#include <cstdlib>
#include <memory>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "vec2.hpp"

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

const int START_WINDOW_WIDTH = 800;
const int START_WINDOW_HEIGHT = 600;

struct Player {
    float y = 200.f;
    float added = 0.f;
    float size = 60.f;
    int points = 0;
    float margin = 70.f;
    float width = 10.f;
    float speed = 25.f;
};

struct Ball {
    Vec2 pos;
    Vec2 added;
    float size = 10.f;
    float speed = 10.f;
};

struct Line {
    Vec2 s;
    Vec2 e;
};

struct GameState {
    Player player1;
    Player player2;
    Ball ball;
    Line left_wall;
    Line right_wall;
    Line top_wall;
    Line bot_wall;
    TTF_Font *font;
};

bool line_line_intersection(const Line &line1, const Line &line2, Vec2 *result) {
    float x1 = line1.s.x, x2 = line1.e.x, x3 = line2.s.x, x4 = line2.e.x;
    float y1 = line1.s.y, y2 = line1.e.y, y3 = line2.s.y, y4 = line2.e.y;
    float x12 = x1 - x2, y12 = y1 - y2;
    float x34 = x3 - x4, y34 = y3 - y4;

    float det = x12 * y34 - y12 * x34;
    
    if (abs(det) < 0.001) return false;

    float a = x1 * y2 - y1 * x2;
    float b = x3 * y4 - y3 * x4;

    float x = (a * x34 - b * x12) / det;
    float y = (a * y34 - b * y12) / det;

    Vec2 point = {x, y};
    Vec2 line1_dir = line1.e - line1.s;
    Vec2 line2_dir = line2.e - line2.s;
    float line1_dot = dot(line1_dir, line1_dir);
    float line2_dot = dot(line2_dir, line2_dir);

    float dot_test = dot(line1_dir, point - line1.s);
    bool on_line1 = (dot_test >= 0 && dot_test <= line1_dot);

    dot_test = dot(line2_dir, point - line2.s);
    bool on_line2 = (dot_test >= 0 && dot_test <= line2_dot);

    if (on_line1 && on_line2) {
        if (result != nullptr)  {
            result->x = x;
            result->y = y;
        }
        return true;
    }

    return false;
}

struct Input {
    bool up_held = false;
    bool down_held = false;
    bool up_pressed = false;
    bool down_pressed = false;
};

bool up(const Input &input) {
    return input.up_pressed || input.up_held;
}

bool down(const Input &input) {
    return input.down_pressed || input.down_held;
}

bool check_events(Input &input1, Input &input2) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT: {
            return false;
        } break;

        case SDL_KEYDOWN: {
            if (e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                case SDLK_w:
                    input1.up_held = true; input1.up_pressed = true;
                    break;
                case SDLK_s:
                    input1.down_held = true; input1.down_pressed = true;
                    break;
                case SDLK_UP:
                    input2.up_held = true; input2.up_pressed = true;
                    break;
                case SDLK_DOWN:
                    input2.down_held = true; input2.down_pressed = true;
                    break;
                }
            }
        } break;

        case SDL_KEYUP: {
            switch (e.key.keysym.sym) {
            case SDLK_w:
                input1.up_held = false;
                break;
            case SDLK_s:
                input1.down_held = false;
                break;
            case SDLK_UP:
                input2.up_held = false;
                break;
            case SDLK_DOWN:
                input2.down_held = false;
                break;
            }
        } break;
        }
    }

    return true;
}

void reset_pressed(Input &input1, Input &input2) {
    input1.up_pressed = false;
    input1.down_pressed = false;
    input2.up_pressed = false;
    input2.down_pressed = false;
}

void throw_ball(GameState *gs) {
    gs->ball.pos = {START_WINDOW_WIDTH * 0.5f, START_WINDOW_HEIGHT * 0.5f};
    gs->ball.added.x = ((rand() % 2) == 0) ? 1 : -1;
    gs->ball.added.y = ((float)rand() / RAND_MAX) - 0.5f;
    gs->ball.speed = 10.f;
    gs->ball.added = normalized(gs->ball.added) * gs->ball.speed;
}

void update(float t, const Input &input1, const Input &input2, GameState *gs) {
    Player &p1 = gs->player1;
    Player &p2 = gs->player2;

    // ball
    Vec2 new_ball_pos = gs->ball.pos + gs->ball.added;
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, gs->top_wall, nullptr)) {
        gs->ball.added.y *= -1;
    }
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, gs->bot_wall, nullptr)) {
        gs->ball.added.y *= -1;
    }
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, gs->left_wall, nullptr)) {
        p2.points++;
        throw_ball(gs);
    }
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, gs->right_wall, nullptr)) {
        p1.points++;
        throw_ball(gs);
    }

    float bs = gs->ball.size;
    Vec2 p1_s = {p1.margin + p1.width, p1.y - bs};
    Vec2 p1_e = {p1.margin + p1.width, p1.y + p1.size + bs};
    Vec2 p2_s = {START_WINDOW_WIDTH - p2.margin - p2.width, p2.y - bs};
    Vec2 p2_e = {START_WINDOW_WIDTH - p2.margin - p2.width, p2.y + p2.size + bs};

    Vec2 intersection_p;
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, {p1_s, p1_e}, &intersection_p)) {
        gs->ball.added.x *= -1;
        gs->ball.speed += 1.f;

        if (intersection_p.y < p1.y + bs || intersection_p.y > p1.y + p1.size - bs)
            gs->ball.added.y += p1.added * 0.5f;
        
        gs->ball.added = normalized(gs->ball.added) * gs->ball.speed;
    }
    if (line_line_intersection({gs->ball.pos, new_ball_pos}, {p2_s, p2_e}, &intersection_p)) {
        gs->ball.added.x *= -1;
        gs->ball.speed += 1.f;

        if (intersection_p.y < p2.y + bs || intersection_p.y > p2.y + p2.size - bs)
            gs->ball.added.y += p2.added * 0.5f;

        gs->ball.added = normalized(gs->ball.added) * gs->ball.speed;
    }

    gs->ball.pos += gs->ball.added;

    // players
    p1.added = 0.f;
    p2.added = 0.f;

    if (up(input1)) p1.added -= p1.speed;
    if (down(input1)) p1.added += p1.speed;
    if (up(input2)) p2.added -= p2.speed;
    if (down(input2)) p2.added += p2.speed;

    float new_p1_y = p1.y + p1.added;
    if (new_p1_y < 0) p1.added -= new_p1_y;
    if (new_p1_y + p1.size > START_WINDOW_HEIGHT) p1.added -= (new_p1_y + p1.size - START_WINDOW_HEIGHT);

    float new_p2_y = p2.y + p2.added;
    if (new_p2_y < 0) p2.added -= new_p2_y;
    if (new_p2_y + p2.size > START_WINDOW_HEIGHT) p2.added -= (new_p2_y + p2.size - START_WINDOW_HEIGHT);

    p1.y += p1.added;
    p2.y += p2.added;
}

void render(SDL_Renderer *renderer, float interpolation, GameState *gs) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

    float player1_draw_y = gs->player1.y - gs->player1.added + (interpolation * gs->player1.added);
    SDL_Rect draw_rect;
    draw_rect.w = gs->player1.width;
    draw_rect.h = gs->player1.size;
    draw_rect.x = gs->player1.margin;
    draw_rect.y = player1_draw_y;
    SDL_RenderFillRect(renderer, &draw_rect);

    float player2_draw_y = gs->player2.y - gs->player2.added + (interpolation * gs->player2.added);
    draw_rect.x = START_WINDOW_WIDTH - gs->player2.margin - gs->player2.width;
    draw_rect.y = player2_draw_y;
    draw_rect.w = gs->player2.width;
    draw_rect.h = gs->player2.size;
    SDL_RenderFillRect(renderer, &draw_rect);

    Vec2 ball_draw_pos = gs->ball.pos - gs->ball.added + (interpolation * gs->ball.added);
    draw_rect.x = ball_draw_pos.x - gs->ball.size * 0.5f;
    draw_rect.y = ball_draw_pos.y - gs->ball.size * 0.5f;
    draw_rect.w = gs->ball.size;
    draw_rect.h = gs->ball.size;
    SDL_RenderFillRect(renderer, &draw_rect);

    SDL_Surface *p1_score_sur = TTF_RenderText_Solid(gs->font, std::to_string(gs->player1.points).c_str(), {0xff, 0xff, 0xff, 0xff});
    SDL_Texture *p1_score_tex = SDL_CreateTextureFromSurface(renderer, p1_score_sur);
    SDL_QueryTexture(p1_score_tex, nullptr, nullptr, &draw_rect.w, &draw_rect.h);
    float text_margin = 16;
    draw_rect.x = text_margin;
    draw_rect.y = 0;
    SDL_RenderCopy(renderer, p1_score_tex, nullptr, &draw_rect);
    SDL_DestroyTexture(p1_score_tex);
    SDL_FreeSurface(p1_score_sur);

    SDL_Surface *p2_score_sur = TTF_RenderText_Solid(gs->font, std::to_string(gs->player2.points).c_str(), {0xff, 0xff, 0xff, 0xff});
    SDL_Texture *p2_score_tex = SDL_CreateTextureFromSurface(renderer, p2_score_sur);
    SDL_QueryTexture(p2_score_tex, nullptr, nullptr, &draw_rect.w, &draw_rect.h);
    draw_rect.x = START_WINDOW_WIDTH - text_margin - draw_rect.w;
    SDL_RenderCopy(renderer, p2_score_tex, nullptr, &draw_rect);
    SDL_DestroyTexture(p2_score_tex);
    SDL_FreeSurface(p2_score_sur);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, START_WINDOW_WIDTH, START_WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 2;
    }

    std::unique_ptr<GameState> game_state(new GameState);
    float bs = game_state->ball.size * 0.5f;
    game_state->left_wall.s = {bs, bs};
    game_state->left_wall.e = {bs, START_WINDOW_HEIGHT - bs};
    game_state->right_wall.s = {START_WINDOW_WIDTH - bs, bs};
    game_state->right_wall.e = {START_WINDOW_WIDTH - bs, START_WINDOW_HEIGHT - bs};
    game_state->top_wall.s = {bs, bs};
    game_state->top_wall.e = {START_WINDOW_WIDTH - bs, bs};
    game_state->bot_wall.s = {bs, START_WINDOW_HEIGHT - bs};
    game_state->bot_wall.e = {START_WINDOW_WIDTH - bs, START_WINDOW_HEIGHT - bs};
    game_state->ball.pos = {START_WINDOW_WIDTH * 0.5f, START_WINDOW_HEIGHT * 0.5f};
    game_state->font = TTF_OpenFont("Pixeled.ttf", 30);
    srand(time(nullptr));
    throw_ball(game_state.get());

    Input input_player1;
    Input input_player2;

    const uint32 MS_PER_UPDATE = 33;
    float ms_elapsing = 0.0f;
    float running_sec = 0.0f;
    bool running = true;
    uint32 current;
    uint32 previous = SDL_GetTicks();

    while (running) {
        current = SDL_GetTicks();
        uint32 ms_elapsed = current - previous;
        previous = current;
        ms_elapsing += ms_elapsed;
        running_sec += ms_elapsed / 1000.f;

        running = check_events(input_player1, input_player2);

        if (ms_elapsing >= MS_PER_UPDATE) {
            update(ms_elapsing / 1000.f, input_player1, input_player2, game_state.get());
            reset_pressed(input_player1, input_player2);
            ms_elapsing -= MS_PER_UPDATE;
        }

        render(renderer, ms_elapsing / MS_PER_UPDATE, game_state.get());
    }

    TTF_CloseFont(game_state->font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}