#include "Engine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <FrustumCull.h>

#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <random>

#include "Renderer.h"
#include "Vertex.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "CameraController.h"

namespace timothy
{

	//void Engine::window_size_callback(GLFWwindow* window, int width, int height)
	//{
	//	GLCall(glViewport(0, 0, width, height));
	//	camera.ProjectionMatrix = glm::perspective(45.0f, (float)(width) / (float)(height), 1.0f, 500.0f);
	//}

	//bool centerCursor = false;

	Engine::Engine() : m_offsets(std::make_unique<std::vector<glm::vec3>>())
	{
		m_camera = std::make_unique<timothy::Camera>(glm::perspective(45.0f, (float)(DEFAULT_WINDOW_WIDTH) / (float)(DEFAULT_WINDOW_HEIGHT), 1.0f, 500.0f), glm::vec3(0.0f, -10.0f, -45.0f), glm::vec3(0.0f));
	}

	Engine::~Engine()
	{
	}

	int Engine::Run() {
		GLFWwindow* window;

		if (!glfwInit())
		{
			const char* error;
			glfwGetError(&error);
			std::cout << "GLFW Initialization Error: " << error << std::endl;
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "Grass Renderer", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return -1;
		}

		//glfwSetFramebufferSizeCallback(window, window_size_callback);

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "[ERROR]: Glew Initialization has failed." << std::endl;
		}


		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
			0, 1, 4,
		};

		unsigned int groundIndices[] = {
		0, 1, 2,
		2, 3, 0,
		};




		IndexBuffer IBO(&indices[0], 9);

		timothy::Vertex vertices[] = { glm::vec3(-0.25f,3.0f,0.0f),glm::vec3(0.25f,3.0f,0.0f), glm::vec3(0.3f,0.0f,0.0f),glm::vec3(-0.3f,0.0f,0.0f), glm::vec3(-0.1f,5.0f,0.2f)};
		VertexBufferLayout layout;
		layout.Push<float>(3);

		VertexBuffer VBO(vertices, sizeof(timothy::Vertex) * 5);

		VertexArray VAO;

		VAO.AddBuffer(VBO, layout);
		std::vector<glm::vec3> off;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distr(0, 1000);
		for (int i = 0; i < INSTANCES_PER_CHUNK; ++i)
		{
			float randomNumberX;
			float randomNumberY;
			randomNumberX = distr(gen) * 0.001f;
			randomNumberY = distr(gen) * 0.001f;
			off.push_back(glm::vec3(randomNumberX * 100.0f, 0.0f, randomNumberY * 100.0f));
		};

		// -- Chunk Generation --
		float chunkHeight = 100.0f;
		float chunkWidth = 100.0f;
		float chunkDepth = 100.0f;
		std::vector<glm::vec3> chunkOffsets;
		
		for (int x = -10; x < 10; x++)
		{
			for (int z = -10; z < 10; z++)
			{
				chunkOffsets.push_back(glm::vec3(x * chunkWidth, 0, z * chunkDepth));
				m_offsets->insert(m_offsets->end(), off.begin(), off.end());
			}
		}
		// -- Chunk Generation --

		VertexBuffer bladeOffsetBuffer(nullptr, sizeof(glm::vec3) * INSTANCES_PER_CHUNK * chunkOffsets.size(), GL_DYNAMIC_DRAW);
		bladeOffsetBuffer.SetData(0, sizeof(glm::vec3) * m_offsets->size(), m_offsets->data());


		VertexArray groundVAO;

		VertexBuffer chunkOffsetBuffer(nullptr, sizeof(glm::vec3) * chunkOffsets.size(), GL_DYNAMIC_DRAW);

		IndexBuffer groundIBO(&groundIndices[0],6);

		timothy::Vertex groundVertices[] = {
			glm::vec3(0.0f,0.0f,0.0f),
			glm::vec3(100.0f,0.0f,0.0f),
			glm::vec3(100.0f,0.0f,100.0f),
			glm::vec3(0.0f,0.0f,100.0f),
		};

		VertexBuffer groundVBO(groundVertices, sizeof(timothy::Vertex) * 4);
		VertexBufferLayout groundLayout;
		groundLayout.Push<float>(3);
		groundVAO.AddBuffer(groundVBO,groundLayout);


		VertexBufferLayout offsetLayout;
		offsetLayout.Push<float>(3);
		VertexBufferLayout chunkOffsetLayout;
		chunkOffsetLayout.Push<float>(3);

		groundVAO.AddBuffer(chunkOffsetBuffer, chunkOffsetLayout);
		GLCall(glVertexAttribDivisor(1, 1));

		VAO.AddBuffer(bladeOffsetBuffer,offsetLayout);


		VAO.AddBuffer(chunkOffsetBuffer, chunkOffsetLayout);
		GLCall(glVertexAttribDivisor(1, 1));
		GLCall(glVertexAttribDivisor(2, INSTANCES_PER_CHUNK));

		Shader groundShader("res/ground.shader");
		groundShader.Bind();

		m_shader = std::make_unique<Shader>("res/grass.shader");
		m_shader->Bind();
		float rotations[16];
		for (int i = 0; i < 16; i++) 
		{
			rotations[i] = (float)(rand() % 360);
		}
		m_shader->SetUniform1fv("u_rotations", 16, rotations);
		m_shader->SetUniform3fv("u_wind_direction", glm::vec3(1.0f, 0.0f, 0.0f));
		m_shader->SetUniform1f("u_y_scale", 1.4f);
		VAO.Unbind();
		m_shader->Unbind();
		VBO.Unbind();


		std::cout << glGetString(GL_VERSION) << std::endl;
		{
			GLCall(glEnable(GL_DEPTH_TEST));
			glEnable(GL_MULTISAMPLE);
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

			ImGui::CreateContext();
			ImGui_ImplGlfwGL3_Init(window, true);
			ImGui::StyleColorsDark();

			double deltaTime = 0;

			double previousTime = glfwGetTime();
			double fpsPTime = glfwGetTime();
			double fpsCTime = glfwGetTime();

			int fpsCount = 0;
			int fpsResult = 0;

			double lastMouseX, lastMouseY;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
			glfwSetCursorPos(window, lastMouseX, lastMouseY);

			m_renderer = std::make_unique<timothy::Renderer>();

			while (!glfwWindowShouldClose(window))
			{
				m_renderer->Clear();
				glfwPollEvents();
				ImGui_ImplGlfwGL3_NewFrame();
				fpsCTime = glfwGetTime();
				fpsCount = fpsCount + 1;
				if (fpsCTime - fpsPTime >= 1) {
					fpsPTime = fpsCTime;
					fpsResult = fpsCount;
					fpsCount = 0;
				}
				double currentTime = glfwGetTime();
				deltaTime = currentTime - previousTime;
				previousTime = currentTime;

				m_shader->Bind();
				glm::mat4 mvp = m_camera->ProjectionMatrix * m_camera->GetViewMatrix();
				Frustum m(mvp);

				m_shader->SetUniformMat4f("u_MVP", mvp);
				m_shader->SetUniform1f("u_time", glfwGetTime());
				m_shader->SetUniform3fv("cameraPos", glm::vec3(m_camera->Transform.Position.x * -1.0f , m_camera->Transform.Position.y * -1.0f, m_camera->Transform.Position.z * -1.0f));

				int chunksToDraw = 0;

				// -- Chunk Culling --
				chunkOffsetBuffer.Bind();
				glm::vec3 modPos = glm::vec3((int)(m_camera->Transform.Position.x / chunkWidth) * -chunkWidth, 0, (int)(m_camera->Transform.Position.z / chunkDepth) * -chunkDepth);
				for (unsigned int i = 0; i < chunkOffsets.size(); i++)
				{
					glm::vec3 newPos = chunkOffsets[i] + modPos;
					if(m.IsBoxVisible(newPos, glm::vec3(newPos.x + chunkWidth, newPos.y + chunkHeight, newPos.z + chunkDepth)))
					{
						chunkOffsetBuffer.SetData(chunksToDraw * sizeof(glm::vec3),sizeof(glm::vec3),(const void*)&newPos);

						chunksToDraw++;
					}
				}
				// -- Chunk Culling --
				groundShader.Bind();
				groundShader.SetUniformMat4f("u_MVP", mvp);
				groundShader.SetUniform3fv("cameraPos", glm::vec3(m_camera->Transform.Position.x * -1.0f, m_camera->Transform.Position.y * -1.0f, m_camera->Transform.Position.z * -1.0f));
				m_renderer->DrawInstanced(groundVAO, groundIBO, groundShader, chunksToDraw);

				m_renderer->DrawInstanced(VAO, IBO, *m_shader,chunksToDraw * INSTANCES_PER_CHUNK);

				timothy::CheckInputs(*m_camera,deltaTime, window, centerCursor);

				double mouseX, mouseY;
				glfwGetCursorPos(window, &mouseX, &mouseY);

				double mouseDeltaX = (mouseX - lastMouseX) * MOUSE_SENSITIVITY;
				double mouseDeltaY = (mouseY - lastMouseY) * MOUSE_SENSITIVITY;

				std::string deltaString = "Mouse Dleta X: " + std::to_string(mouseDeltaX) + " Y: " + std::to_string(mouseDeltaY);
				std::string cameraPositionString = "Camera Position X: " + std::to_string(m_camera->Transform.Position.x) + " Y: " + std::to_string(m_camera->Transform.Position.y) + " Z: " + std::to_string(m_camera->Transform.Position.z);

				std::string chunksToDrawString = "Chunks To Draw " + std::to_string(chunksToDraw);
				std::string totalChunksString = "Total Chunks " + std::to_string(chunkOffsets.size());

				if (centerCursor) {

					m_camera->Transform.Rotation.x = m_camera->Transform.Rotation.x - mouseDeltaY;

					if (m_camera->Transform.Rotation.x < -90) m_camera->Transform.Rotation.x = -90;
					if (m_camera->Transform.Rotation.x >  90) m_camera->Transform.Rotation.x =  90;

					m_camera->Transform.Rotation.y = m_camera->Transform.Rotation.y - mouseDeltaX;

					glfwSetCursorPos(window, lastMouseX, lastMouseY);
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				}
				else {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				}

				ImGui::Begin("Debug");
				ImGui::Text(deltaString.c_str());
				ImGui::Text(cameraPositionString.c_str());
				ImGui::Text(chunksToDrawString.c_str());
				ImGui::Text(totalChunksString.c_str());
				ImGui::Text(std::to_string(fpsResult).c_str());
				ImGui::End();
				std::string title = "Grass Renderer - FPS: " + std::to_string(fpsResult);
				glfwSetWindowTitle(window,title.c_str());
				ImGui::Render();
				ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(window);
			}
		}

		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
		return 0;
	}
}
