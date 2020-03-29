#pragma once

struct Spritesheet;
struct Sprite;

void load_skeleton_sprites();

Spritesheet* get_anim(int index);
Spritesheet* get_attack_anim(int index);
int get_attack_anim_count();

Spritesheet* get_idle_sheet();
Spritesheet* get_walk_sheet();
Spritesheet* get_punch_sheet();