#include "window.hpp"
#include <algorithm>
#include <cmath>
bool pointCirculo = false;
bool pointEspiral = false;
float newPointX = 0.0f;     
float newPointY = 0.0f;
float newAngulo = 0.0f;
float newRadius = 0.0f;
float newTm = 1.0f;
float radius = 0.1f;
float angle = 0.0f;
float Tm = 1.0f;

void Window::onCreate() {

  auto const *vertexShader{R"gl(#version 300 es
    layout(location = 0) in vec2 inPosition;

    void main() { 
      gl_PointSize = 2.0;
      gl_Position = vec4(inPosition, 0, 1); 
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es
    precision mediump float;

    out vec4 outColor;

    void main() { outColor = vec4(1); }
  )gl"};

  // Create shader program
  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  // Clear window
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  std::array<GLfloat, 2> sizes{};
#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  abcg::glGetFloatv(GL_POINT_SIZE_RANGE, sizes.data());
#else
  abcg::glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes.data());
#endif
  fmt::print("Point size: {:.2f} (min), {:.2f} (max)\n", sizes.at(0),
             sizes.at(1));

  // Start pseudorandom number generator
  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  // Randomly pick a pair of coordinates in the range [-1; 1)
  std::uniform_real_distribution<float> realDistribution(-1.0f, 1.0f);
}



void Window::onPaint() {
  
  
  if(pointEspiral == true){
    //Funções para desenho de uma circunferência que vai incrementando ou diminuindo
     m_P.x = radius * std ::cos(angle);
     m_P.y = radius * std :: sin(angle);
     angle += (2.0 * M_PI) / 2.0 * getDeltaTime();
     radius += 0.02 * getDeltaTime();
  }else if (pointCirculo == true){
     m_P.x = std ::cos(angle)* Tm;
     m_P.y = std :: sin(angle)*Tm;
     angle += (2 * M_PI) / 2 * getDeltaTime();
  }else {
    m_P.x = 0.0;
    m_P.y = 0.0;
  }
  
  
  // Create OpenGL buffers for drawing the point at m_P
  setupModel();
  
  
 
  int minSize = std::min(m_viewportSize.x, m_viewportSize.y);
  int x = (m_viewportSize.x - minSize) / 2;
  int y = (m_viewportSize.y - minSize) / 2;

  abcg::glViewport(x, y, minSize, minSize);
 
   
  // Start using the shader programS
  abcg::glUseProgram(m_program);
  // Start using VAO
  abcg::glBindVertexArray(m_VAO);
  
  
  // Draw a single point
  abcg::glDrawArrays(GL_POINTS, 0, 1);

  // End using VAO
  abcg::glBindVertexArray(0);
  // End using the shader program
  abcg::glUseProgram(0);

  
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  // Release shader program, VBO and VAO
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel() {
  // Release previous VBO and VAO
  abcg::glDeleteBuffers(1, &m_VBOVertices);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Generate a new VBO and get the associated ID
  abcg::glGenBuffers(1, &m_VBOVertices);
  // Bind VBO in order to use it
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  // Upload data to VBO
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_P), &m_P, GL_STATIC_DRAW);
  // Unbinding the VBO is allowed (data can be released now)
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertices);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
    //Tamanho do Bloco
    ImGui::SetNextWindowSize(ImVec2(200, 270));
    ImGui::SetNextWindowPos(ImVec2(5, 90));
    ImGui::Begin("Selecionar Ponto Inicial", nullptr, ImGuiWindowFlags_NoDecoration);

    // Adicione controles ImGui para selecionar as coordenadas do ponto inicial
    ImGui::SliderFloat("Radius", &newRadius, -1.0f, 1.0f);
    ImGui::SliderFloat("Tamanho", &newTm, 0.0f, 1.0f);

    // Verifique se as coordenadas estão dentro do intervalo [-1, 1)
    newRadius = std::max(-1.0f, std::min(1.0f, newRadius));
    newTm = std::max(0.0f, std::min(1.0f, newTm));
    // Se o botão "Confirmar" for clicado, marque a variável de controle como verdadeira
    if (ImGui::Button("Confirmar", ImVec2(150, 30))) {
      // Atualize as coordenadas do ponto inicial com os novos valores
      radius = newRadius;
      Tm = newTm;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::Button("Circulo", ImVec2(150, 40))){
        pointEspiral = false;
        pointCirculo = true;
        abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::Button("Espiral", ImVec2(150, 50))){
        pointEspiral = true;
        pointCirculo = false;
        abcg::glClear(GL_COLOR_BUFFER_BIT);
    }
    if (ImGui::Button("Reset", ImVec2(150, 60))){
        newRadius = 0.0;
        newTm = 1.0;
        radius = 0.1f;
        Tm = 1.0f;
        abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    // Fim da janela ImGui
    ImGui::End();
  
  
  
}

