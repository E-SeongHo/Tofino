#pragma once

#include "Object.h"
namespace Tofino
{
    class Geometry : public Object
    {
    public:
        Geometry(const std::string name = "Untitled", const bool isHittable = true) : Object(name, isHittable) {}

        // Loads geometry on object's mesh
        virtual void LoadGeometry(const float scale = 1.0f) = 0;
    };

    class Triangle : public Geometry
    {
    public:
        Triangle(const std::string name = "Untitled", const bool isHittable = true) : Geometry(name, isHittable) {}

        void LoadGeometry(const float scale = 1.0f) override;
    };

    class Square : public Geometry
    {
    public:
        Square(const std::string name = "Untitled", const bool isHittable = true) : Geometry(name, isHittable) {}

        void LoadGeometry(const float scale = 1.0f) override;
    };

    class Cube : public Geometry
    {
    public:
        Cube(const std::string name = "Untitled", const bool isHittable = true) : Geometry(name, isHittable) {}

        void LoadGeometry(const float scale = 1.0f) override;
    };

    class Sphere : public Geometry
    {
    public:
        Sphere(const std::string name = "Untitled", const bool isHittable = true) : Geometry(name, isHittable) {}

        void LoadGeometry(const float scale = 1.0f) override;
    };

}