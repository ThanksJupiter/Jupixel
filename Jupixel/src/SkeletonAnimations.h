#pragma once

struct Spritesheet;
struct Sprite;

void load_skeleton_sprites();

Spritesheet* get_idle_sheet();
Spritesheet* get_walk_sheet();