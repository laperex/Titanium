#pragma once

#include <glm/glm.hpp>
#include <OpenGL.h>
#include "../Enumerations.h"

#define POSITION_ATTRIB_BIT ( 1 << VertexArrayAttrib::position )
#define NORMAL_ATTRIB_BIT ( 1 << VertexArrayAttrib::normal )
#define COLOR_ATTRIB_BIT ( 1 << VertexArrayAttrib::color )
#define UV0_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv0 )
#define UV1_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv1 )
#define UV2_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv2 )
#define UV3_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv3 )
#define UV4_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv4 )
#define UV5_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv5 )
#define UV6_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv6 )
#define UV7_ATTRIB_BIT ( 1 << VertexArrayAttrib::uv7 )

namespace ti {
	namespace Component {
		struct Mesh {
			std::vector<glm::vec3> positions;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec4> color;
			std::vector<glm::vec2> uv0;
			std::vector<glm::vec2> uv1;
			std::vector<glm::vec2> uv2;
			std::vector<glm::vec2> uv3;
			std::vector<glm::vec2> uv4;
			std::vector<glm::vec2> uv5;
			std::vector<glm::vec2> uv6;
			std::vector<glm::vec2> uv7;
			std::vector<uint32_t> indices;

			Primitive primitive = TRIANGLE;
			uint32_t flags = POSITION_ATTRIB_BIT | NORMAL_ATTRIB_BIT | UV0_ATTRIB_BIT;

			bool changed = true;

			Mesh() {}
			Mesh(const aiScene* ai_scene, aiMesh* ai_mesh) {
				for (int i = 0; i < ai_mesh->mNumVertices; i++) {
					if (ai_mesh->HasPositions())
						positions.push_back({ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z});

					if (ai_mesh->HasNormals())
						normals.push_back({ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z});
					
					// if (ai_mesh->mColors[0]) {
					// 	color.push_back({ai_mesh->mColors[0][i].r, ai_mesh->mColors[0][i].g, ai_mesh->mColors[0][i].b, ai_mesh->mColors[0][i].a});
					// }

					if (ai_mesh->mTextureCoords[0]) {
						uv0.push_back({ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y});
					}
				}

				for (int i = 0; i < ai_mesh->mNumFaces; i++) {
					aiFace face = ai_mesh->mFaces[i];

					for(unsigned int j = 0; j < face.mNumIndices; j++) {
						indices.push_back(face.mIndices[j]);
					}
				}

				// if (ai_mesh->mMaterialIndex >= 0) {
				// 	material = std::string(ai_scene->mMaterials[ai_mesh->mMaterialIndex]->GetName().C_Str());
				// }
			}
		};
	}
}