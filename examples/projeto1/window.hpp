#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  // onCreate chamado uma vez so quando a janela e inciaida
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  //variando de acordo com o tamanho da tela
  void onResize(glm::ivec2 const &size) override;
  // onDestroy chamado somente quando a janela e desligada
  //liberar recursos da OpenGl apos o termino do programa
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};

  //recursos alocados pelo Open_Gl para criar figuras matematicas
  GLuint m_VAO{};
  GLuint m_VBOVertices{};
  GLuint m_program{};

  std::default_random_engine m_randomEngine;
  std::array<glm::vec2, 3> const m_points{{{0, 1}, {-1, -1}, {1, -1}}};
  glm::vec2 m_P{};

  void setupModel();
};

#endif