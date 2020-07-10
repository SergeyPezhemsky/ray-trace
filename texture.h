#pragma once
#include "LiteMath.h"

using namespace HydraLiteMath;

class Texture {
public:
    virtual float3 value(float u, float v) const = 0;
};

class ImageTexture : public Texture {
public:
    explicit ImageTexture(unsigned char* pixels, int A, int B) : data(pixels), nx(A), ny(B) {};
    virtual ~ImageTexture() = default;

    virtual float3 value(float u, float v) const;
private:
    unsigned char* data;
    int nx, ny;
};

class ConstantTexture : public Texture {
public:
	ConstantTexture(float3 c) : color(c) {}
    virtual ~ConstantTexture() = default;

	virtual float3 value(float u, float v) const {
		return color;
	}

private:
	float3 color;
};