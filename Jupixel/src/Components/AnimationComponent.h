#pragma once

struct Spritesheet;

struct AnimationComponent
{
	Spritesheet* current_anim = nullptr;
	bool isFlipped = false;
	int currentSpriteIndex = 0;
	float currentSpriteTime = 0.0f;
	float nextSpriteDelay = 0.1f;
};
