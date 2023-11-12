Projeto 2 - Computação Gráfica

Participantes:

Nome: Fernando Gabriel Chacon Fernandes Teruel do Prado RA:11201811700

Nome: Tiago Cornetta Campos RA:11201922123

[LINK DO PROJETO](https://gabriel-chacon.github.io/Projeto2Computa-oGrafica/)

**DESCRIÇÃO DO PROJETO**

O projeto a seguir foi desenvolvido com base no projeto "lookat". O principal propósito consiste em aplicar os conceitos aprendidos até o momento para criar uma representação tridimensional de uma cidade. Nessa composição, os elementos visuais centrais são as casas, representadas por cubos, que não apenas compõem o tecido urbano, mas também incorporam dinamismo ao alternar entre tamanhos variados, expandindo e contraindo de maneira contínua. Além disso, uma muralha é formada por esses cubos, contribuindo para a estética e estrutura visual da cidade. Um cubo flutuante, rotacionando em torno de seu próprio eixo, e um cubo vertical, oscilando entre expansões e contrações, adicionam elementos visuais dinâmicos à composição geral.

Este projeto é considerado um prelúdio para a Atividade 3, onde a intenção é criar uma cidade-labirinto. Nesse contexto, a cidade-labirinto proposta terá como desafio encontrar o cubo colorido e vibrante, promovendo uma experiência interativa e lúdica para os usuários. Ao utilizar os conceitos fundamentais e aprimorados neste projeto, buscamos criar uma experiência envolvente e desafiadora na exploração da cidade-labirinto tridimensional.

**COMO FOI IMPLEMENTADO:**

No método paint() da classe Ground, ocorreram alterações significativas para modificar a aparência visual do chão. Ao invés do padrão estático de tabuleiro de xadrez, agora as cores dos quadrados são dinamicamente calculadas com base na posição no plano xz. A lógica de cálculo utiliza a posição de cada quadrado para determinar cores alternadas de vermelho e verde, criando assim uma grade com uma aparência mais dinâmica e colorida. Essa abordagem adiciona uma camada visual interessante, tornando o chão mais atrativo e variado. Além disso, é importante observar que as cores amarelo e preto também são formadas. Isso ocorre quando as condições para o vermelho e o verde são satisfeitas ao mesmo tempo, resultando na formação da cor amarela. Por outro lado, quando ambas as condições não são satisfeitas, o quadrado assume a cor preta. Isso contribui para a riqueza visual do tabuleiro, introduzindo uma gama adicional de cores e padrões.

```C++
  // Calculate colors based on the position
  auto const red{std::abs(x) % 2 == 0 ? 1.0f : 0.1f};
  auto const green{std::abs(z) % 2 == 0 ? 0.9f : 0.1f};
  abcg::glUniform4f(m_colorLoc, red, green, 0.0f, 1.0f);
```

A maioria das alterações foi feita na classe Window, onde são criadas algumas varáveis para dar certa dinamicidade aos objetos. Definimos as escalas, cores e velocidade dos principais obejetos presentes na cena. Essas variáveis serão alteradas em tempo de execução no método onUpdate().

```C++
  //Inicializadores Random para casasQuadrados
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> floatDistribution(0.0, 1.0);
  
  float colorV0 = floatDistribution(gen);
  float colorV1 = floatDistribution(gen);
  float colorV2 = floatDistribution(gen);
  float colorV3 = floatDistribution(gen);
  
  //Para casa colorida
  std::uniform_real_distribution<float> floatDistribution2(0.0, 10.0);
  
  
  //Para aumentar e diminuir a escala do bloco colorido
  float scaleObjetoColorido = 1.0f;  // Escala inicial
  float scaleSpeed = 0.2f;  // Velocidade de crescimento e diminuição
  
  //Para aumentar os restos dos blocos
  float scaleMuro = 0.5f;
  float scaleSpeedMuro = 0.3f;
  float scaleSpeedRotacao = 0.3f;
  
  //Para rotacionar o cubo
  float rotationAngle = glm::radians(0.0f); // Ângulo de rotação (sentido horário)
```

No método OnCreate() carregamos um modelo. Esse modelo representa um cubo e esse cubo servirá para simular algumas casas.

```C++
  // Load model
  loadModelFromFile(assetsPath + "box.obj");
```

O método onPaint() é acionado a cada quadro e desempenha o papel essencial de renderizar a cena, que foi concebida para simular uma cidade tridimensional. Nessa representação urbana, os cubos são empregados para simbolizar casas, e esses mesmos cubos são utilizados para compor uma muralha. Uma característica dinâmica notável é atribuída às casas, que alternam constantemente entre tamanhos reduzidos e expandidos, introduzindo movimento e vitalidade à cena. Adicionalmente, é gerado um cubo flutuante que realiza uma rotação em torno de seu eixo, acrescentando um elemento visual dinâmico à composição geral. Complementando a diversidade visual, um cubo vertical oscila entre expansões e contrações, simultaneamente alterando suas cores diversas vezes a cada segundo.

```C++
void Window::onPaint() {
  float colorColorida1 = floatDistribution2(gen);
  float colorColorida2 = floatDistribution2(gen);
  float colorColorida3 = floatDistribution2(gen);
  float colorColorida4 = floatDistribution2(gen);

  
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);

  // Parte da esquerda do muralha
  glm::mat4 model{1.0f};
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
  model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
  
  // Parte do meio do muralha
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));
  model = glm::scale(model, glm::vec3(1.5f, 1.0f, 0.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  // Parte da direita muralha
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(0.5f, 1.5f, 0.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  //Muro da esquerda
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -2.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(0.5f, 1.5f, 1.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);

  //Muro da direita
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(0.5, 1.5f, 1.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);


  //Casa colorida
  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
  model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
  model = glm::scale(model, glm::vec3(0.5f, scaleObjetoColorido, 0.5f));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, colorColorida1,colorColorida2,colorColorida3,colorColorida4);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);


  
  //Casa da esquerda
  float aux = 0.0;
  for(int i = -4 ; i < 7; i++){
  
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(-2.0f, 0.0f, - 1.0f + i));
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
      model = glm::scale(model, glm::vec3(scaleMuro, 1.0f, 0.5f));
      abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(m_colorLocation, colorV0 + aux , colorV1*i, colorV2 -aux +i, colorV3+aux);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
      aux +=0.1;
    }

  
  
  //Casa da esquerda + 2
  
  for(int i = -4 ; i < 7; i++){
  
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(-4.0f, 0.0f, - 1.0f + i));
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
      model = glm::scale(model, glm::vec3(scaleMuro, 1.0f, 0.5f));
      abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(m_colorLocation, colorV0 + aux , colorV1*i, colorV2 -aux +i, colorV3+aux);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
      aux +=0.1;
    }



  //CasaDireita
  aux =0.0;
  for(int i = -4 ; i < 7; i++){
  
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(2.0f, 0.0f, - 1.0f + i));
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
      model = glm::scale(model, glm::vec3(scaleMuro, 1.0f, 0.5f));
      abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(m_colorLocation, colorV0 + aux , colorV1, colorV2 +aux, colorV3+aux);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
      aux +=0.2;
    }
  
  //CasaDireita +2 
  aux =0.0;
  for(int i = -4 ; i < 7; i++){
  
      model = glm::mat4(1.0);
      model = glm::translate(model, glm::vec3(4.0f, 0.0f, - 1.0f + i));
      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0, 1, 0));
      model = glm::scale(model, glm::vec3(scaleMuro, 1.0f, 0.5f));
      abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
      abcg::glUniform4f(m_colorLocation, colorV0 + aux , colorV1, colorV2 +aux, colorV3+aux);
      abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                       nullptr);
      aux +=0.2;
    }
  
   // Parte do meio do muralha

  model = glm::mat4(1.0);
  model = glm::translate(model, glm::vec3(0.0f, 1.0f, 3.0f));
  model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
  model = glm::rotate(model, rotationAngle, glm::vec3(0, 1, 0));
  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
  abcg::glUniform4f(m_colorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
  abcg::glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
  
  
  abcg::glBindVertexArray(0);

  // Draw ground
  m_ground.paint();

  abcg::glUseProgram(0);
}
```

O método **onUpdate()** desempenha um papel crucial no ciclo de vida da aplicação, sendo chamado a cada quadro renderizado. Sua principal função é atualizar dinamicamente o estado da cena e dos objetos com base no tempo decorrido entre quadros, também conhecido como intervalo de tempo ou Δt. A variável 'deltaTime' armazena esse valor, indicando quanto tempo se passou desde o último quadro renderizado.

Dentro do método onUpdate() várias operações dinâmicas são executadas para criar efeitos visuais animados. Algumas dessas operações incluem:

1. **Atualização da Escala**:

- scaleObjetoColorido e scaleMuro representam escalas de objetos específicos na cena, como uma casa colorida e as casas que compõem a muralha, respectivamente.
- scaleSpeed e scaleSpeedMuro controlam as velocidades de crescimento e diminuição desses objetos, proporcionando variação visual.
- scaleSpeedRotacao contrala a velocidade de rotação do objeto flutuante.
  
2. **Rotação de Objetos**:

- rotationAngle é a variável que controla o ângulo de rotação do obejeto flutuante em torno de seu próprio eixo. Essa rotação adiciona um elemento dinâmico e visualmente atraente à cena.
 
3. **Restrições e Limites**:

-Existem verificações para garantir que as escalas e os ângulos estejam dentro de limites específicos, evitando comportamentos indesejados ou extremos.

4. **Atualização da Câmera**:

- As operações m_camera.dolly, m_camera.truck, e m_camera.pan atualizam a posição da câmera com base nas entradas do usuário, proporcionando uma experiência de visualização interativa.
- 
```C++
void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Atualize a escala
  scaleObjetoColorido += scaleSpeed * deltaTime;
  scaleMuro += scaleSpeedMuro * deltaTime;

  // Tamanho objeto colorido
  if (scaleObjetoColorido >= 1.5f || scaleObjetoColorido <= 0.5f) {
    scaleSpeed = -scaleSpeed;
  }
  //Tamanho cada quadrado
  if (scaleMuro >= 1.0f || scaleMuro <= 0.5f) {
    scaleSpeedMuro = -scaleSpeedMuro;
  }

  // Rotação do objeto no ar
  rotationAngle += scaleSpeedRotacao * deltaTime;
  if (rotationAngle >= glm::radians(360.0f)) {
      rotationAngle -= glm::radians(360.0f);
  }

  // Restrinja a escala dentro de limites
  scaleObjetoColorido = std::max(0.5f, std::min(2.5f, scaleObjetoColorido));
  scaleMuro = std::max(0.5f, std::min(1.0f, scaleMuro));

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}
```
