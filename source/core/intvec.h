#pragma once

class FSerializer;

struct vec2_16_t
{
    int16_t x, y;
};

struct vec2_t
{
    int32_t X, Y;

    vec2_t() = default;
    vec2_t(const vec2_t&) = default;
    vec2_t(int x, int y) : X(x), Y(y) {}
    vec2_t operator+(const vec2_t& other) const { return { X + other.X, Y + other.Y }; }
    vec2_t operator-(const vec2_t& other) const { return { X - other.X, Y - other.Y }; }
    vec2_t& operator+=(const vec2_t& other) { X += other.X; Y += other.Y; return *this; };
    vec2_t& operator-=(const vec2_t& other) { X -= other.X; Y -= other.Y; return *this; };
	vec2_t& operator/= (int other) { X /= other; Y /= other; return *this; }
    bool operator == (const vec2_t& other) const { return X == other.X && Y == other.Y; };
};
inline vec2_t operator/ (const vec2_t& vec, int other) { return { vec.X / other, vec.Y / other }; }

struct vec3_t
{
    union
    {
        struct
        {
            int32_t X, y, z;
        };
        vec2_t  vec2;
    };

    vec3_t() = default;
    vec3_t(const vec3_t&) = default;
    vec3_t(int x, int y_, int z_) : X(x), y(y_), z(z_) {}
    vec3_t operator+(const vec3_t& other) const { return { X + other.X, y + other.y, z + other.z }; }
    vec3_t operator-(const vec3_t& other) const { return { X - other.X, y - other.y, z - other.z }; }
    vec3_t& operator+=(const vec3_t& other) { X += other.X; y += other.y; z += other.z; return *this; };
    vec3_t& operator-=(const vec3_t& other) { X -= other.X; y -= other.y; z += other.z; return *this; };

};




#if 0
struct FVector2
{
    float x, y;
};

struct vec3_16_t
{
    union 
	{
        struct 
		{ 
			int16_t x, y, z; 
		};
        vec2_16_t vec2;
    };
};
#endif

FSerializer& Serialize(FSerializer& arc, const char* key, vec2_t& c, vec2_t* def);
FSerializer& Serialize(FSerializer& arc, const char* key, vec3_t& c, vec3_t* def);
