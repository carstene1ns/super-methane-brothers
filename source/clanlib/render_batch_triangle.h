/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#pragma once

class RenderBatchTriangle : public clan::RenderBatcher
{
public:
	RenderBatchTriangle(clan::Canvas& canvas);
	void draw_image(clan::Canvas &canvas, const clan::Rectf &src, const clan::Rectf &dest, float color, const clan::Texture2D &texture, float lighting);

public:
	const static int max_textures = 16;

private:
	struct SpriteVertex
	{
		clan::Vec4f position;
		clan::Vec2f texcoord;
		float color;
		int texindex;
		float lighting;
	};

	clan::VertexArrayBuffer get_vertex_buffer(clan::GraphicContext& gc, int& out_index);
	int set_batcher_active(clan::Canvas &canvas, const clan::Texture2D &texture, const clan::Colorf &constant_color = clan::Colorf::black);
	int set_batcher_active(clan::Canvas &canvas);
	int set_batcher_active(clan::Canvas &canvas, int num_vertices);
	void flush(clan::GraphicContext &gc) override;
	void matrix_changed(const clan::Mat4f &modelview, const clan::Mat4f &projection, clan::TextureImageYAxis image_yaxis, float pixel_ratio) override;

	inline clan::Vec4f to_position(float x, float y) const;

	clan::Mat4f modelview_projection_matrix;
	int position = 0;

	static const int num_vertex_buffers = 4;
	enum { vertex_buffer_size = 1024 * 1024 };
	char buffer[vertex_buffer_size];

	enum { max_vertices = vertex_buffer_size / sizeof(SpriteVertex) };
	SpriteVertex *vertices;

	clan::VertexArrayBuffer vertex_buffers[num_vertex_buffers];
	int current_vertex_buffer = 0;

	clan::PrimitivesArray prim_array[num_vertex_buffers];

	static const int max_number_of_texture_coords = 32;

	clan::Texture2D current_textures[max_number_of_texture_coords];
	int num_current_textures = 0;
	clan::Sizef tex_sizes[max_number_of_texture_coords];
	clan::Colorf constant_color;

	clan::ProgramObject m_Shader_DrawStandard;

};
