/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#ifndef SHADER_H_
#define SHADER_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "utils/utils.h"

#include <gx2/draw.h>
#include <gx2/enum.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/sampler.h>
#include <gx2/shaders.h>
#include <gx2/surface.h>
#include <gx2/texture.h>
#include "common/gx2_ext.h"

class Shader
{
protected:
    Shader() {}
    virtual ~Shader() {}
public:
    static const u16 cuVertexAttrSize = sizeof(f32) * 3;
    static const u16 cuTexCoordAttrSize = sizeof(f32) * 2;
    static const u16 cuColorAttrSize = sizeof(u8) * 4;

    static void setLineWidth(const f32 & width) {
        GX2SetLineWidth(width);
    }

    static void draw(s32 primitive = GX2_PRIMITIVE_MODE_QUADS, u32 vtxCount = 4)
    {
        switch(primitive)
        {
            default:
            case GX2_PRIMITIVE_MODE_QUADS:
            {
                GX2DrawEx(GX2_PRIMITIVE_MODE_QUADS, vtxCount, 0, 1);
                break;
            }
            case GX2_PRIMITIVE_MODE_TRIANGLES:
            {
                GX2DrawEx(GX2_PRIMITIVE_MODE_TRIANGLES, vtxCount, 0, 1);
                break;
            }
            case GX2_PRIMITIVE_MODE_TRIANGLE_FAN:
            {
                GX2DrawEx(GX2_PRIMITIVE_MODE_TRIANGLE_FAN, vtxCount, 0, 1);
                break;
            }
            case GX2_PRIMITIVE_MODE_LINES:
            {
                GX2DrawEx(GX2_PRIMITIVE_MODE_LINES, vtxCount, 0, 1);
                break;
            }
            case GX2_PRIMITIVE_MODE_LINE_STRIP:
            {
                GX2DrawEx(GX2_PRIMITIVE_MODE_LINE_STRIP, vtxCount, 0, 1);
                break;
            }
            //! TODO: add other primitives later
        };
    }
};

#endif // SHADER_H_
