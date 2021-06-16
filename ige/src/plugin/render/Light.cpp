#include "ige/plugin/RenderPlugin.hpp"
#include <glm/vec3.hpp>

using glm::vec3;
using ige::plugin::render::Light;
using ige::plugin::render::LightType;

Light Light::point(float intensity, vec3 color)
{
    Light light;
    light.type = LightType::POINT;
    light.color = color;
    light.intensity = intensity;

    return light;
}

Light Light::directional(float intensity, vec3 color)
{
    Light light;
    light.type = LightType::DIRECTIONAL;
    light.color = color;
    light.intensity = intensity;

    return light;
}
