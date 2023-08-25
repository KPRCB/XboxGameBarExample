#pragma once
#include <string>

typedef struct Color {
	int8_t alpha, red, green, blue;
};
enum {
	_endFrame,
	rectangle,
	line,
	circle,
	_text
};

typedef struct float2 {
	float x, y;
};

typedef struct Rect {
	float height, width, x, y;
};

typedef struct _Rectangle {
	Rect rect;
	Color color;
};

typedef struct Line {
	float2 point1, point2;
	Color color;
};

typedef struct Circle {
	float2 point;
	float radius;
	Color color;
};

typedef struct TextData {
	uint32_t textLength;
	float2 point;
	Color color;
	float fontSize;
};