#include <video.h>

// Variables estaticas
static uint8_t * defaultVideoPos = (uint8_t*)0xB8000;

static unsigned int currentVideoPosOffset = START_LEFT;
static unsigned int currentVideoPosLeftOffset = START_LEFT;
static unsigned int currentVideoPosRightOffset = START_RIGHT;

uint8_t * defaultVideo() {
	return defaultVideoPos;
}

unsigned int * currentVideoOffset() {
	return &currentVideoPosOffset;
}

unsigned int * currentVideoLeftOffset() {
	return &currentVideoPosLeftOffset;
}

unsigned int * currentVideoRightOffset() {
	return &currentVideoPosRightOffset;
}