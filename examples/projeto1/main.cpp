#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setOpenGLSettings({.samples = 2, .doubleBuffering = false});
    //Local onde defini o nome da tela
    window.setWindowSettings({.width = 600,
                              .height = 600,
                              .showFullscreenButton = false,
                              .title = "Projeto1-Formas2D"});

    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}