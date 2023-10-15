#include "window.hpp"
bool pointCirculo = false;
bool pointEspiral = false;
bool checkBoxValue = true;


void Window::onCreate() {
  auto const *vertexShader{R"gl(#version 300 es

    layout(location = 0) in vec2 inPosition;
    layout(location = 1) in vec4 inColor;

    uniform vec2 translation;
    uniform float scale;

    out vec4 fragColor;

    void main() {
      vec2 newPosition = inPosition * scale + translation;
      gl_Position = vec4(newPosition, 0, 1);
      fragColor = inColor;
    }
  )gl"};

  auto const *fragmentShader{R"gl(#version 300 es

    precision mediump float;

    in vec4 fragColor;

    out vec4 outColor;

    void main() { outColor = fragColor; }
  )gl"};

  m_program = abcg::createOpenGLProgram(
      {{.source = vertexShader, .stage = abcg::ShaderStage::Vertex},
       {.source = fragmentShader, .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glClear(GL_COLOR_BUFFER_BIT);

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());
}

void Window::onPaint() {
  if (m_timer.elapsed() < m_delay / 1000.0)
    return;
  m_timer.restart();

  if(pointEspiral == true){
    //Funções para desenho de uma circunferência que vai incrementando ou diminuindo
     m_P.x = m_radius * std ::cos(m_angle);
     m_P.y = m_radius * std :: sin(m_angle);
     m_angle += (2.0 * M_PI) / 2.0 * getDeltaTime();
     m_radius += m_direction* m_space * getDeltaTime();
       if (m_radius<0.0f)
        m_radius =0.0f;
      else if (m_radius>1.0f)
       m_radius =1.0f;
  }else if (pointCirculo == true){
     m_P.x = std ::cos(m_angle)* m_radius;
     m_P.y = std :: sin(m_angle)*m_radius;
     m_angle += (2 * M_PI) / 2 * getDeltaTime();
  }else {
    m_P.x = 0.0;
    m_P.y = 0.0;
  }

  // Create a regular polygon with number of sides in the range [3,20]
  std::uniform_int_distribution intDist(3, 20);
  auto const sides{intDist(m_randomEngine)};

  if(checkBoxValue)
    setupModel(sides);
  else
    setupModel(m_sides);


  int minSize = std::min(m_viewportSize.x, m_viewportSize.y);
  //int x = (m_viewportSize.x - minSize) / 2;
  //int y = (m_viewportSize.y - minSize) / 2;
  //abcg::glViewport(x, y, minSize, minSize);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  int maxSize = std::max(m_viewportSize.x, m_viewportSize.y);
  if (m_viewportSize.x>m_viewportSize.y){
    m_P.x = m_P.x * minSize/maxSize;
  }
  else if (m_viewportSize.y>m_viewportSize.x){
    m_P.y = m_P.y * minSize/maxSize;
  }
  
  
  //abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  //Pick a random xy position from (-1,-1) to (1,1)
  //std::uniform_real_distribution rd1(-1.0f, 1.0f);
  glm::vec2 const translation{m_P.x, m_P.y};
  auto const translationLocation{
      abcg::glGetUniformLocation(m_program, "translation")};
  abcg::glUniform2fv(translationLocation, 1, &translation.x);

  //glm::vec2 manualPosition(-1.0f, 1.0f);

  
  //std::uniform_real_distribution rd2(0.005f, 0.01f);
  //auto const scale{rd2(m_randomEngine)};
  auto const scale{m_scale*0.021f};
  auto const scaleLocation{abcg::glGetUniformLocation(m_program, "scale")};
  abcg::glUniform1f(scaleLocation, scale);

  // Render
  abcg::glBindVertexArray(m_VAO);
  abcg::glDrawArrays(GL_TRIANGLE_FAN, 0, sides + 2);
  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();
  bool isButtonStyleOnTop = false;
  {
    
    auto const widgetSize{ImVec2(220, 275)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    auto const windowFlags{ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, windowFlags);
    
    float previousRadius = m_radius;
    float previousScale = m_scale;


    // Adicione controles ImGui (Sliders)
    ImGui::PushItemWidth(140);
    ImGui::SliderInt("Delay", &m_delay, 0, 200, "%d ms");
    ImGui::SliderFloat("Radius",&m_radius, 0, 1.0f, "%.3f");
    ImGui::SliderFloat("Space",&m_space, 0.02f, 0.1f, "%.3f");
    ImGui::SliderFloat("Scale",&m_scale, 0.1f, 15.0f, "%.3f");
    ImGui::SliderInt("Sides",&m_sides, 3, 20, "%d");
    if (ImGui::Checkbox("random sides", &checkBoxValue)) {
    }
    ImGui::PopItemWidth();

    //Define os estilos dos botoes
    if (pointCirculo) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 1.0f, 1.0f)); // Azul claro
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.3f, 0.6f, 1.0f));
        isButtonStyleOnTop = true;
      }

    //Cria botao
    if (ImGui::Button("Circulo", ImVec2(-1, 30))){
      pointEspiral = false;
      pointCirculo = true;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if(isButtonStyleOnTop){
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      isButtonStyleOnTop=false;
    }
    

    if (pointEspiral) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 1.0f, 1.0f)); // Azul claro
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.3f, 0.6f, 1.0f));
        isButtonStyleOnTop = true;
    }

    if (ImGui::Button("Espiral", ImVec2(120, 30))){
      pointEspiral = true;
      pointCirculo = false;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if(isButtonStyleOnTop){
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      isButtonStyleOnTop=false;
    }

    ImGui::SameLine();

    if (pointEspiral && m_direction==1) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 1.0f, 1.0f)); // Azul claro
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.3f, 0.6f, 1.0f));
      isButtonStyleOnTop = true;
    }

    if (ImGui::Button("+", ImVec2(33, 30))) {
      m_direction = 1;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if(isButtonStyleOnTop){
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      isButtonStyleOnTop=false;
    }

    ImGui::SameLine();

    if (pointEspiral && m_direction==-1) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.6f, 1.0f, 1.0f)); // Azul claro
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.3f, 0.6f, 1.0f));
      isButtonStyleOnTop = true;
    }

    if (ImGui::Button("-", ImVec2(33, 30))) {
      m_direction = -1;
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if(isButtonStyleOnTop){
      ImGui::PopStyleColor();
      ImGui::PopStyleColor();
      isButtonStyleOnTop=false;
    }

    if (ImGui::Button("Clear window", ImVec2(-1, 30))) {
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    if(m_radius!=previousRadius || m_scale!=previousScale){
      abcg::glClear(GL_COLOR_BUFFER_BIT);
    }

    ImGui::End();
  }
}


void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::setupModel(int sides) {
  // Release previous resources, if any
  abcg::glDeleteBuffers(1, &m_VBOPositions);
  abcg::glDeleteBuffers(1, &m_VBOColors);
  abcg::glDeleteVertexArrays(1, &m_VAO);

  // Select random colors for the radial gradient
  std::uniform_real_distribution rd(0.0f, 1.0f);
  glm::vec3 const color1{rd(m_randomEngine), rd(m_randomEngine),
                         rd(m_randomEngine)};
  glm::vec3 const color2{rd(m_randomEngine), rd(m_randomEngine),
                         rd(m_randomEngine)};

  // Minimum number of sides is 3
  sides = std::max(3, sides);

  std::vector<glm::vec2> positions;
  std::vector<glm::vec3> colors;

  // Polygon center
  positions.emplace_back(0, 0);
  colors.push_back(color1);

  // Border vertices
  auto const step{M_PI * 2 / sides};
  for (auto const angle : iter::range(0.0, M_PI * 2, step)) {
    positions.emplace_back(std::cos(angle), std::sin(angle));
    colors.push_back(color2);
  }

  // Duplicate second vertex
  positions.push_back(positions.at(1));
  colors.push_back(color2);

  // Generate VBO of positions
  abcg::glGenBuffers(1, &m_VBOPositions);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2),
                     positions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate VBO of colors
  abcg::glGenBuffers(1, &m_VBOColors);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3),
                     colors.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  auto const colorAttribute{abcg::glGetAttribLocation(m_program, "inColor")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOPositions);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glEnableVertexAttribArray(colorAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBOColors);
  abcg::glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}