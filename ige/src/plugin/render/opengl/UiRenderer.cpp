#include "UiRenderer.hpp"
#include "Program.hpp"
#include "Shader.hpp"
#include "TextureCache.hpp"
#include "VertexArray.hpp"
#include "WeakPtrMap.hpp"
#include "blobs/shaders/gl3/ui-img-fs.glsl.h"
#include "blobs/shaders/gl3/ui-img-vs.glsl.h"
#include "blobs/shaders/gl3/ui-rect-fs.glsl.h"
#include "blobs/shaders/gl3/ui-rect-vs.glsl.h"
#include "glad/gl.h"
#include "ige/asset/Texture.hpp"
#include "ige/core/App.hpp"
#include "ige/ecs/System.hpp"
#include "ige/ecs/World.hpp"
#include "ige/plugin/RenderPlugin.hpp"
#include "ige/plugin/TransformPlugin.hpp"
#include "ige/plugin/WindowPlugin.hpp"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <iostream>

using glm::vec2;
using glm::vec4;
using ige::asset::Texture;
using ige::core::App;
using ige::ecs::System;
using ige::ecs::World;
using ige::plugin::render::ImageRenderer;
using ige::plugin::render::RectRenderer;
using ige::plugin::transform::RectTransform;
using ige::plugin::window::WindowInfo;

struct UiRenderCache {
    gl::VertexArray quad_vao;
    gl::Program rect_program;
    gl::Program image_program;
    WeakPtrMap<Texture, TextureCache> textures;

    // called on the first frame
    UiRenderCache()
    {
        vec2 quad[4] = {
            { 0.0f, 0.0f },
            { 1.0f, 0.0f },
            { 0.0f, 1.0f },
            { 1.0f, 1.0f },
        };

        quad_vao.attrib(0, quad);

        {
            gl::Shader vs {
                gl::Shader::ShaderType::VERTEX,
                BLOBS_SHADERS_GL3_UI_RECT_VS_GLSL,
            };

            gl::Shader fs {
                gl::Shader::ShaderType::FRAGMENT,
                BLOBS_SHADERS_GL3_UI_RECT_FS_GLSL,
            };

            rect_program.link(vs, fs);
        }

        {
            gl::Shader vs {
                gl::Shader::ShaderType::VERTEX,
                BLOBS_SHADERS_GL3_UI_IMG_VS_GLSL,
            };

            gl::Shader fs {
                gl::Shader::ShaderType::FRAGMENT,
                BLOBS_SHADERS_GL3_UI_IMG_FS_GLSL,
            };

            image_program.link(vs, fs);
        }
    }

    gl::Texture& operator[](Texture::Handle tex)
    {
        auto it = textures.find(tex);

        if (it != textures.end()) {
            return it->second.gl_texture;
        } else {
            // FIXME: textures are never destroyed
            return textures.emplace(tex, *tex).first->second.gl_texture;
        }
    }
};

namespace systems {

static void render_ui(World& wld)
{
    vec2 winsize(0.0f);

    if (auto wininfo = wld.get<WindowInfo>()) {
        winsize.x = static_cast<float>(wininfo->width);
        winsize.y = static_cast<float>(wininfo->height);
    } else {
        std::cerr << "[WARN] No window info available!" << std::endl;
        return;
    }

    auto& cache = wld.get_or_emplace<UiRenderCache>();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cache.quad_vao.bind();

    for (auto [ent, rect, xform] : wld.query<RectRenderer, RectTransform>()) {
        cache.rect_program.use();
        cache.rect_program.uniform("u_FillColor", rect.fill);
        cache.rect_program.uniform(
            "u_Bounds",
            vec4 {
                xform.abs_bounds_min() / winsize * 2.0f - 1.0f,
                xform.abs_bounds_max() / winsize * 2.0f - 1.0f,
            });

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    for (auto [ent, img, xform] : wld.query<ImageRenderer, RectTransform>()) {
        if (img.texture == nullptr) {
            continue;
        }

        cache.image_program.use();
        cache.image_program.uniform("u_Texture", cache[img.texture]);
        cache.image_program.uniform("u_Tint", img.tint);
        cache.image_program.uniform(
            "u_Bounds",
            vec4 {
                xform.abs_bounds_min() / winsize * 2.0f - 1.0f,
                xform.abs_bounds_max() / winsize * 2.0f - 1.0f,
            });

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}

static void clear_cache(World& wld)
{
    wld.remove<UiRenderCache>();
}

}

void UiRenderer::plug(App::Builder& builder) const
{
    builder.add_system(System::from(systems::render_ui));
    builder.add_system(System::from(systems::clear_cache));
}