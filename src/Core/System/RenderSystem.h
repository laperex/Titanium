#pragma once

#include "System.h"
#include <unordered_map>

namespace ti {
	namespace System {
		class RenderSystem: public System {
			UniformBuffer* uniformbuffer;

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;

			std::unordered_map<std::size_t, VertexArray*> vertexarray_registry;
			std::unordered_map<std::string, Shader*> shader_registry;
			std::unordered_map<std::string, ti::Component::Material> material_registry;

			Shader* shader;

			std::vector<uint32_t> indices;
			uint32_t indexcount;
			std::vector<Vertex> vertices;
			uint32_t vertexcount;
		
		public:
			RenderSystem() {}

			void Init() override {
				uniformbuffer = UniformBuffer_Create();
				uniformbuffer->Allocate(sizeof(glm::mat4) * 3);
				uniformbuffer->BindRange(0, sizeof(glm::mat4) * 3);

				auto* shader = Shader_Create("material", "D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\color.fs");
				shader->BindUniformBlock("ProjectionMatrix", 0);
				RegisterShader(shader);

				RegisterVertexArray<Vertex>(
					{
						{ position, 0, 3, GL_FLOAT },
						{ uv0, 1, 3, GL_FLOAT },
						{ normal, 2, 3, GL_FLOAT },
					}
				);
			}

			template <typename T>
			void RegisterVertexArray(std::vector<VertexArrayAttribDescriptor> vertexarraydesc) {
				assert(vertexarray_registry.find(typeid(T).hash_code()) == vertexarray_registry.end());

				vertexarray_registry[typeid(T).hash_code()] = VertexArray_Create(vertexarraydesc);
			}

			void RegisterShader(Shader* shader) {
				assert(shader_registry.find(shader->name) == shader_registry.end());

				shader_registry[shader->name] = shader;
			}

			void SetShader(Shader* shader) {
				this->shader = shader;
			}

			void SetShader(std::string name) {
				SetShader(GetShader(name));
			}

			Shader* GetShader(std::string name) {
				return shader_registry[name];
			}

			template <typename T>
			void SetMeshVertexArray(ti::Component::Mesh& mesh, std::vector<T> vertices) {
				if (mesh.vertexarray != nullptr) return;
				
				assert(vertexarray_registry.find(typeid(T).hash_code()) != vertexarray_registry.end());

				mesh.vertexarray = vertexarray_registry[typeid(T).hash_code()];

				if (mesh.vertexbuffer == nullptr)
					mesh.vertexbuffer = VertexBuffer_Create();
				if (mesh.indexbuffer == nullptr)
					mesh.indexbuffer = IndexBuffer_Create(mesh.vertexarray);
			}
			
			void RegisterMaterial(ti::Component::Material material) {
				// assert(material_registry.find(material.name) == material_registry.end());

				material_registry[material.name] = material;
			}

			ti::Component::Material& GetMaterial(std::string name) {
				assert(material_registry.find(name) != material_registry.end());

				return material_registry[name];
			}

			void SetMaterial(std::string name) {
				SetMaterial(GetMaterial(name));
			}

			void SetMaterial(ti::Component::Material& material) {
				// SetShader("material");

				shader->SetUniformVec3("material.ambient", &material.ambient[0]);
				shader->SetUniformVec3("material.diffuse", &material.diffuse[0]);
				shader->SetUniformVec3("material.specular", &material.specular[0]);
				shader->SetUniformf("material.shininess", material.shininess);

				// if (material.ambient_map != nullptr) {
				// 	glActiveTexture(GL_TEXTURE0);
				// 	material.ambient_map->BindUnit(0);
				// 	shader->SetUniformi("material.ambient_map", 0);
				// }

				// if (material.diffuse_map != nullptr) {
				// 	glActiveTexture(GL_TEXTURE1);
				// 	material.diffuse_map->BindUnit(1);
				// 	shader->SetUniformi("material.diffuse_map", 1);
				// }

				// if (material.specular_map != nullptr) {
				// 	glActiveTexture(GL_TEXTURE2);
				// 	material.specular_map->BindUnit(2);
				// 	shader->SetUniformi("material.specular_map", 2);
				// }
			}

			template <typename T>
			void TransferVertices(const std::vector<T>& vertices, VertexBuffer* vertexbuffer) {
				if (vertices.size())
					vertexbuffer->AddDataStatic((void*)vertices.data(), vertices.size()*sizeof(T));
			}

			void TransferIndices(std::vector<uint32_t>& indices, IndexBuffer* indexbuffer) {
				if (indices.size())
					indexbuffer->AddData(indices.data(), indices.size()*sizeof(uint32_t));
			}

			void TransferMesh(ti::Component::Mesh& mesh) {
				TransferVertices(mesh.vertices, mesh.vertexbuffer);
				TransferIndices(mesh.indices, mesh.indexbuffer);

				mesh.vertexcount = mesh.vertices.size();
				mesh.indexcount = mesh.indices.size();
				mesh.changed = false;
			}

			void SetModel(const glm::mat4& model) {
				if (this->model == model) return;
				this->model = model;
				uniformbuffer->AddDataDynamic(&this->model[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 0);
			}

			void SetView(const glm::mat4& view) {
				if (this->view == view) return;
				this->view = view;
				uniformbuffer->AddDataDynamic(&this->view[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 1);
			}

			void SetProjection(const glm::mat4& projection) {
				if (this->projection == projection) return;
				this->projection = projection;
				uniformbuffer->AddDataDynamic(&this->projection[0][0], sizeof(glm::mat4), sizeof(glm::mat4) * 2);
			}

			void RenderMesh(ti::Component::Mesh& mesh) {
				if (mesh.vertexarray == nullptr)
					SetMeshVertexArray(mesh, mesh.vertices);

				SetMaterial(mesh.material);

				if (mesh.changed) {
					TransferMesh(mesh);
				}
				
				mesh.vertexarray->Bind();
				mesh.vertexarray->BindVertexBuffer(mesh.vertexbuffer, mesh.vertexarray->stride);

				if (mesh.indexcount) {
					mesh.vertexarray->BindIndexBuffer(mesh.indexbuffer);
					
					if (mesh.primitive == ti::Primitive::TRIANGLE) glDrawElements(GL_TRIANGLES, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::TRIANGLE_FAN) glDrawElements(GL_TRIANGLE_FAN, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawElements(GL_TRIANGLE_STRIP, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::LINE) glDrawElements(GL_LINES, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::LINE_STRIP) glDrawElements(GL_LINE_STRIP, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
					if (mesh.primitive == ti::Primitive::POINT) glDrawElements(GL_POINTS, mesh.indexcount, GL_UNSIGNED_INT, nullptr);
				} else {
					if (mesh.primitive == ti::Primitive::TRIANGLE) glDrawArrays(GL_TRIANGLES, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::TRIANGLE_FAN) glDrawArrays(GL_TRIANGLE_FAN, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::TRIANGLE_STRIP) glDrawArrays(GL_TRIANGLE_STRIP, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::LINE) glDrawArrays(GL_LINES, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::LINE_STRIP) glDrawArrays(GL_LINE_STRIP, 0, mesh.vertexcount);
					if (mesh.primitive == ti::Primitive::POINT) glDrawArrays(GL_POINTS, 0, mesh.vertexcount);
				}
			}

			void Update(double dt) override {
				using namespace ti::Component;

				for (auto& entity: registry->View<Properties, Transform, ti::Component::Camera>()) {
					auto& camera = registry->Get<ti::Component::Camera>(entity);

					if (camera.enable) {
						SetView(camera.view);
						SetProjection(camera.projection);
					}
				}

				for (auto& entity: registry->View<Properties, Transform, Mesh>()) {
					auto& mesh = registry->Get<Mesh>(entity);
					auto& transform = registry->Get<Transform>(entity);

					SetShader("material");
					SetModel(transform.GetModel());
					SetMaterial(mesh.material);
					RenderMesh(mesh);
				}

				for (auto& entity: registry->View<Properties, Transform, Model>()) {
					auto& model = registry->Get<Model>(entity);
					auto& transform = registry->Get<Transform>(entity);

					SetShader("material");
					SetModel(transform.GetModel());
					shader->Bind();

					for (auto& mesh: model.meshes) {
						RenderMesh(mesh);
					}
				}

				// for (auto& entity: registry->View<Properties, Transform, Sprite>()) {
				// 	auto& sprite = registry->Get<Sprite>(entity);
				// 	auto& transform = registry->Get<Transform>(entity);

				// 	SetModel(transform.GetModel());
				// 	SetShader("sprite");
				// }

				// for (auto& entity: registry->View<Properties, Transform, Texture>()) {
				// 	auto& sprite = registry->Get<Sprite>(entity);
				// 	auto& transform = registry->Get<Transform>(entity);

				// 	SetModel(transform.GetModel());
				// 	SetShader("texture");
				// }
			}
		};
	}
}