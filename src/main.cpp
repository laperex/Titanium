// #include "define.h"

// #include "OpenGL/OpenGL.h"
// #include "Camera.h"

// #include "Math/math.h"

// struct Vertex {
// 	Vec3 pos;
// 	Vec3 color;
// 	Vec3 normal;
// };

// int x = 0, y = 0, w = 1, h = 1;
// int nx = -50, ny = -50, nw = 100, nh = 100;

// std::vector<Vertex> vertices = {
// 	{ Vec3(x, y, 0), Vec3(1, 1, 0), Vec3() },
// 	{ Vec3(x + w, y, 0), Vec3(1, 1, 0), Vec3() },
// 	{ Vec3(x + w, y + h, 0), Vec3(1, 1, 0), Vec3() },
// 	{ Vec3(x + w, y + h, 0), Vec3(1, 1, 0), Vec3() },
// 	{ Vec3(x, y + h, 0), Vec3(1, 1, 0), Vec3() },
// 	{ Vec3(x, y, 0), Vec3(1, 1, 0), Vec3() },
	
// 	// plane
// 	{ Vec3(nx, -1, ny), Vec3(0.5, 0.5, 0.5), Vec3() },
// 	{ Vec3(nx + nw, -1, ny), Vec3(0.5, 0.5, 0.5), Vec3() },
// 	{ Vec3(nx + nw, -1, ny + nh), Vec3(0.5, 0.5, 0.5), Vec3() },
// 	{ Vec3(nx + nw, -1, ny + nh), Vec3(0.5, 0.5, 0.5), Vec3() },
// 	{ Vec3(nx, -1, ny + nh), Vec3(0.5, 0.5, 0.5), Vec3() },
// 	{ Vec3(nx, -1, ny), Vec3(0.5, 0.5, 0.5), Vec3() },
// };

// void RecalculateNormals(std::vector<Vertex> &vertices) {
// 	for (int i = 0; i < 4; i++) {
// 		auto A = vertices[i*3 + 0].pos;
// 		auto B = vertices[i*3 + 1].pos;
// 		auto C = vertices[i*3 + 2].pos;

// 		auto normal = (B - A).cross(C - A).normalize();

// 		vertices[i*3 + 0].normal = normal;
// 		vertices[i*3 + 1].normal = normal;
// 		vertices[i*3 + 2].normal = normal;
// 	}
// }

// int main(int ArgCount, char **Args) {
// 	SDL_Init(SDL_INIT_VIDEO);
// 	OpenGL_CreateContext();

// 	SDL_Window* window = SDL_CreateWindow("2.5D Engine", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
// 	assert(window);
// 	SDL_GLContext context = SDL_GL_CreateContext(window);

// 	gladLoadGLLoader(SDL_GL_GetProcAddress);

// 	RecalculateNormals(vertices);

// 	auto* vertexarray = VertexArray_Create({ {0, 3, GL_FLOAT}, {1, 3, GL_FLOAT}, {2, 3, GL_FLOAT} });
// 	auto* vertexbuffer = VertexBuffer_Create();
// 	auto* shader = Shader_Create("D:\\C++\\2.5D Engine\\src\\Shaders\\default.vs", "D:\\C++\\2.5D Engine\\src\\Shaders\\ray.fs");
// 	auto* uniformbuffer = UniformBuffer_Create();

// 	vertexbuffer->AddDataStatic(vertices.data(), vertices.size()*sizeof(Vertex));

// 	glm::mat4 proj = glm::ortho<float>(0, WIDTH, HEIGHT, 0, -100, 100);
// 	glm::mat4 view = glm::mat4(1.0f);
// 	glm::mat4 model = glm::mat4(1.0f);

// 	shader->BindUniformBlock("ProjectionMatrix", 0);

// 	uniformbuffer->BindRange(0, 3 * sizeof(glm::mat4));
// 	uniformbuffer->Allocate(3 * sizeof(glm::mat4));

// 	auto* camera = new Camera("cam0", PERSPECTIVE);

// 	uniformbuffer->AddDataDynamic(&camera->projection[0][0], sizeof(glm::mat4), 2 * sizeof(glm::mat4));
// 	uniformbuffer->AddDataDynamic(&camera->model[0][0], sizeof(glm::mat4), 0 * sizeof(glm::mat4));

// 	// TODO: Add Stencil buffer Support
// 	// TODO: Add Framebuffer Support
// 	// TODO: Add Editor Camera
// 	// TODO: Mesh Import
// 	// TODO: Material Import

// 	IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO &io = ImGui::GetIO();
// 	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
// 	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
// 	io.ConfigDockingWithShift = true;
	
//     ImGui_ImplSDL2_InitForOpenGL(window, (void*)context);
//     ImGui_ImplOpenGL3_Init("#version 400");
	
//     ImGui::StyleColorsDark();
	
// 	unsigned int fbo;
// 	glGenFramebuffers(1, &fbo);

// 	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 	unsigned int texture;
// 	glGenTextures(1, &texture);
// 	glBindTexture(GL_TEXTURE_2D, texture);
	
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

// 	glBindTexture(GL_TEXTURE_2D, 0);

// 	unsigned int depth_stencil;
// 	glGenTextures(1, &depth_stencil);
// 	glBindTexture(GL_TEXTURE_2D, depth_stencil);
	
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

// 	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil, 0);

// 	glBindTexture(GL_TEXTURE_2D, 0);

// 	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);

// 	double dt = 0;
// 	bool quit = false;
// 	while (!quit) {
// 		SDL_Event event;
// 		while (SDL_PollEvent(&event)) {
// 			if (event.type == SDL_QUIT) {
// 				quit = true;
// 			}
// 		}

// 		const auto& start_time = std::chrono::high_resolution_clock::now();
		
// 		// int display_w, display_h;
//         // glfwGetFramebufferSize(GLFW_GetWindow(), &display_w, &display_h);
//         glViewport(0, 0, WIDTH, HEIGHT);
		
// 		glClearColor(0, 0, 0, 0);
// 		glClear(GL_COLOR_BUFFER_BIT);
		
// 		ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplSDL2_NewFrame();
//         ImGui::NewFrame();
		
// 		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

// 		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		
// 		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
// 		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

//         ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//         ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
// 		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

// 		ImGui::SetNextWindowPos(ImVec2(0, 0));
// 		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		
// 		static bool p_open;
// 		ImGui::Begin("DockSpace Demo", &p_open, window_flags);

// 		ImGui::PopStyleVar();
// 		ImGui::PopStyleVar(2);

// 		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
// 			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
// 			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
// 		}

// 		ImGui::End();

// 		static bool b = true;
// 		ImGui::ShowDemoWindow(&b);

// 		ImGui::Begin("Game");
// 			const auto& size = ImGui::GetWindowSize();
// 			const auto& pos = ImGui::GetWindowPos();
// 			{
// 				glBindFramebuffer(GL_FRAMEBUFFER, fbo);

// 				glViewport(0, 0, WIDTH, HEIGHT);

// 				glClearColor(0.2, 0.2, 0.2, 0.2);
// 				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 				glEnable(GL_DEPTH_TEST);
				
// 				glEnable(GL_LINE_SMOOTH);

// 				glEnable(GL_BLEND);
// 				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 				glDepthFunc(GL_LESS);
// 				glDepthMask(GL_TRUE);

// 				shader->Bind();
// 				vertexarray->Bind();
// 				vertexarray->BindVertexBuffer(vertexbuffer, vertexarray->stride);
// 				camera->Update(dt);
				
// 				// camera->projection = glm::perspective(glm::radians(45.0f), (float)size.x/size.y, 0.001f, 1000.0f);
// 				// uniformbuffer->AddDataDynamic(&camera->projection[0][0], sizeof(glm::mat4), 2 * sizeof(glm::mat4));
// 				uniformbuffer->AddDataDynamic(&camera->view[0][0], sizeof(glm::mat4), 1 * sizeof(glm::mat4));

// 				shader->SetUniformVec3("material.ambient", &Vec3(1.0f, 0.5f, 0.31f)[0]);
// 				shader->SetUniformVec3("material.diffuse", &Vec3(1.0f, 0.5f, 0.31f)[0]);
// 				shader->SetUniformVec3("material.specular", &Vec3(0.5f, 0.5f, 0.5f)[0]);
// 				shader->SetUniformf("material.shininess", 32.0f);

// 				shader->SetUniformVec3("dir_light.ambient", &Vec3(0.2f, 0.2f, 0.2f)[0]);
// 				shader->SetUniformVec3("dir_light.diffuse", &Vec3(0.5f, 0.5f, 0.5f)[0]);
// 				shader->SetUniformVec3("dir_light.specular", &Vec3(1.0f, 1.0f, 1.0f)[0]);

// 				shader->SetUniformVec3("dir_light.direction", &Vec3(0.0f, 5.0f, 0.0f)[0]);

// 				shader->SetUniformVec3("view_pos", &camera->Position[0]);
// 				shader->SetUniformVec3("view_dir", &camera->Front[0]);

// 				glDrawArrays(GL_TRIANGLES, 0, vertices.size());

// 				glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 			}
// 			// ImGui::Image((void*)fbo, size, ImVec2(0, 1), ImVec2(1, 0));
// 			ImGui::GetWindowDrawList()->AddImage((void*)fbo, pos, ImVec2(pos.x + size.x, pos.y + size.y), ImVec2(0, 1), ImVec2(1, 0));
//         ImGui::End();

// 		ImGui::Render();
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

// 		SDL_GL_SwapWindow(window);

// 		const auto& end_time = std::chrono::high_resolution_clock::now();
// 		dt = std::chrono::duration<double, std::ratio<1, 60>>(end_time - start_time).count();
// 	}

// 	ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplSDL2_Shutdown();
//     ImGui::DestroyContext();

// 	return 0;
// }

#include "Engine.h"

int main(int ArgCount, char **Args) {
	Mainloop();

	return 0;
}