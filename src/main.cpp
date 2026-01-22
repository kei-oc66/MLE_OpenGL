#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>
namespace fs = std::filesystem;

#include "AsimpModel.hpp"
#include "Mesh.hpp"

const unsigned int width = 800;
const unsigned int height = 800;

float xpos{0.0f};
float ypos{0.0f};
float zpos{0.0f};

// Vertices coordinates
Vertex vertices[] =
    { //               COORDINATES           /            COLORS          /
      //               NORMALS         /       TEXTURE COORDINATES    //
        Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
        Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
        Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
               glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}};

// Indices for vertices order
GLuint indices[] = {0, 1, 2, 0, 2, 3};

Vertex lightVertices[] = { //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

GLuint lightIndices[] = {0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
                         2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

Vertex boxVertices[] = { //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
    Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
    Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};

GLuint boxIndices[] = {0, 1, 2, 0, 2, 3, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2,
                       2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

bool spawnBox = false;

int main() {
  // Initialize GLFW
  glfwInit();

  // Tell GLFW what version of OpenGL we are using
  // In this case we are using OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Tell GLFW we are using the CORE profile
  // So that means we only have the modern functions
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
  GLFWwindow *window = glfwCreateWindow(width, height, "Yo", NULL, NULL);
  // Error check if the window fails to create
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  // Introduce the window into the current context
  glfwMakeContextCurrent(window);

  // Load GLAD so it configures OpenGL
  gladLoadGL();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  // Initialize ImGui backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  // Specify the viewport of OpenGL in the Window
  // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
  glViewport(0, 0, width, height);

  std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
  std::string texPath = "/build/resources/textures/";

  // Texture data
  Texture textures[]{Texture((parentDir + texPath + "planks.png").c_str(),
                             "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
                     Texture((parentDir + texPath + "planksSpec.png").c_str(),
                             "specular", 1, GL_RED, GL_UNSIGNED_BYTE)};

  // Generates Shader object using shaders default.vert and default.frag
  Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
  // Store mesh data in vectors for the mesh
  std::vector<Vertex> verts(vertices,
                            vertices + sizeof(vertices) / sizeof(Vertex));
  std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
  std::vector<Texture> tex(textures,
                           textures + sizeof(textures) / sizeof(Texture));
  // Create floor mesh
  Mesh floor(verts, ind, tex);

  // Shader for light cube
  Shader lightShader("shaders/light.vert", "shaders/light.frag");
  // Store mesh data in vectors for the mesh
  std::vector<Vertex> lightVerts(
      lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
  std::vector<GLuint> lightInd(
      lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
  // Create light mesh
  Mesh light(lightVerts, lightInd, tex);

  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.5f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  glm::vec3 objectPos = glm::vec3(xpos, ypos, zpos);
  glm::mat4 objectModel = glm::mat4(1.0f);
  objectModel = glm::translate(objectModel, objectPos);

  Shader boxShader("shaders/light.vert", "shaders/light.frag");

  std::vector<Vertex> boxVerts(boxVertices, boxVertices + sizeof(boxVertices) /
                                                              sizeof(Vertex));
  std::vector<GLuint> boxInd(boxIndices,
                             boxIndices + sizeof(boxIndices) / sizeof(GLuint));

  Mesh box(boxVerts, boxInd, tex);
  glm::vec4 boxColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
  glm::vec3 boxPos = glm::vec3(-0.5f, 0.5f, 0.0f);
  glm::mat4 boxModel = glm::mat4(1.0f);
  boxModel = glm::translate(boxModel, boxPos);

  lightShader.Activate();
  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x,
              lightColor.y, lightColor.z, lightColor.w);
  shaderProgram.Activate();
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1,
                     GL_FALSE, glm::value_ptr(objectModel));
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"),
              lightColor.x, lightColor.y, lightColor.z, lightColor.w);
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x,
              lightPos.y, lightPos.z);

  boxShader.Activate();
  glUniformMatrix4fv(glGetUniformLocation(boxShader.ID, "model"), 1, GL_FALSE,
                     glm::value_ptr(boxModel));
  glUniform4f(glGetUniformLocation(boxShader.ID, "lightColor"), boxColor.x,
              boxColor.y, boxColor.z, boxColor.w);
  glUniform3f(glGetUniformLocation(boxShader.ID, "lightPos"), boxPos.x,
              boxPos.y, boxPos.z);

  Model model1("resources/models/Untitled.obj");

  // Enables the Depth Buffer
  glEnable(GL_DEPTH_TEST);

  // Creates camera object
  Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

  // Main while loop
  while (!glfwWindowShouldClose(window)) {
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Handles camera inputs
    camera.Inputs(window);
    // Updates and exports the camera matrix to the Vertex Shader
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    // Draws different meshes
    floor.Draw(shaderProgram, camera);
    model1.Draw(shaderProgram, camera);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(300, 200));
    ImGui::Begin("Color");
    ImGui::Text("king!");
    if (ImGui::Button("Free money")) {
      spawnBox = true;
    }

    if (ImGui::Button("delete")) {
      spawnBox = false;
    }

    ImGui::SliderFloat("change x pos", &xpos, 0.0f, 1.0f);
    ImGui::End();

    if (spawnBox) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);
    // Take care of all GLFW events
    glfwPollEvents();
  }

  // Delete all the objects we've created
  shaderProgram.Delete();
  lightShader.Delete();
  boxShader.Delete();
  // Delete window before ending the program
  glfwDestroyWindow(window);
  // Terminate GLFW before ending the program
  glfwTerminate();
  return 0;
}
