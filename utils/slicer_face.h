/**************************************************************************/
/*  slicer_face.h                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef SLICER_FACE_H
#define SLICER_FACE_H

#include "core/math/color.h"
#include "core/math/face3.h"
#include "core/math/vector2.h"
#include "scene/resources/mesh.h"

/**
 * Godot's Face3 only keeps track of a mesh's vertices but we want to keep track
 * of things like UV and normal mappings
 */
struct SlicerFace : public Face3 {
	// Maybe it would be smarter to have these be pointers or PoolVectors,
	// seeing as most of these may often go unused. But for simplicity's
	// sake let's just do it the memory naive way and allocate what we
	// might need
	bool has_normals;
	Vector3 normal[3];

	bool has_tangents;
	Vector4 tangent[3];

	bool has_colors;
	Color color[3];

	bool has_bones;
	Vector4 bones[3];

	bool has_weights;
	Vector4 weights[3];

	// Documentation says that uvs can be either Vector2 or Vector3
	// but glancing through the visual server code it seems its just
	// handled as a Vector2 (which makes sense). For now this should
	// be fine
	bool has_uvs;
	Vector2 uv[3];

	bool has_uv2s;
	Vector2 uv2[3];

	/**
	 * Parse a mesh's surface into a vector of faces. This will preserve the mapping
	 * associated with each vertex and can handle both indexed and non indexed vertex
	 * arrays
	 */
	static Vector<SlicerFace> faces_from_surface(const Ref<Mesh> mesh, int surface_idx);

	/**
	 * Creates a new face while using barycentric weights to interpolate UV, normal, etc
	 * info on to the new points.
	 */
	SlicerFace sub_face(Vector3 a, Vector3 b, Vector3 c) const;

	/**
	 * Uses normal and UV information to generate tangents for each point in the face
	 */
	void compute_tangents();

	/**
	 * Calculates Barycentric coordinate weight values for the given point in respect to
	 * this face.
	 */
	Vector3 barycentric_weights(Vector3 point) const;

	void set_uvs(Vector2 a, Vector2 b, Vector2 c) {
		has_uvs = true;
		uv[0] = a;
		uv[1] = b;
		uv[2] = c;
	}

	void set_normals(Vector3 a, Vector3 b, Vector3 c) {
		has_normals = true;
		normal[0] = a;
		normal[1] = b;
		normal[2] = c;
	}

	void set_tangents(Vector4 a, Vector4 b, Vector4 c) {
		has_tangents = true;
		tangent[0] = a;
		tangent[1] = b;
		tangent[2] = c;
	}

	void set_colors(Color a, Color b, Color c) {
		has_colors = true;
		color[0] = a;
		color[1] = b;
		color[2] = c;
	}

	void set_bones(Vector4 a, Vector4 b, Vector4 c) {
		has_bones = true;
		bones[0] = a;
		bones[1] = b;
		bones[2] = c;
	}

	void set_weights(Vector4 a, Vector4 b, Vector4 c) {
		has_weights = true;
		weights[0] = a;
		weights[1] = b;
		weights[2] = c;
	}

	void set_uv2s(Vector2 a, Vector2 b, Vector2 c) {
		has_uv2s = true;
		uv2[0] = a;
		uv2[1] = b;
		uv2[2] = c;
	}

	bool operator==(const Face3 &other) const {
		return vertex[0] == other.vertex[0] && vertex[1] == other.vertex[1] && vertex[2] == other.vertex[2];
	}

	SlicerFace() {
		has_normals = false;
		has_tangents = false;
		has_uvs = false;
		has_uv2s = false;
		has_colors = false;
		has_bones = false;
		has_weights = false;
	}
	SlicerFace(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
		vertex[0] = a;
		vertex[1] = b;
		vertex[2] = c;

		has_normals = false;
		has_tangents = false;
		has_uvs = false;
		has_uv2s = false;
		has_colors = false;
		has_bones = false;
		has_weights = false;
	}
};

#endif // SLICER_FACE_H
