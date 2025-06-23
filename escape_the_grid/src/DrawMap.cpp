#include <SFML/Graphics.hpp>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include "DrawMap.hpp"
#include "Config.hpp"

enum Cell { Empty, Invalid, Path, Visited, Wall };

void draw_map(sf::RenderWindow &i_window, const std::pair<int, int> &playerPos,
              const std::vector<std::pair<int, int>> &highlightedCells,
              float pulseIntensity) {
  // Crear el mapa directamente como std::array
  std::array<std::array<Cell, GridConfig::ROWS>, GridConfig::COLUMNS> gameMap;

  // Inicializar todas las celdas como Empty
  for (unsigned short a = 0; a < GridConfig::COLUMNS; a++) {
    for (unsigned short b = 0; b < GridConfig::ROWS; b++) {
      gameMap[a][b] = Cell::Empty;
    }
  }

  //**************************************************** */

  for (unsigned short a = 0; a < GridConfig::COLUMNS; a++) {
    for (unsigned short b = 0; b < GridConfig::ROWS; b++) {

      // Dibujar grid cuadrículada - solo para una guia
      /*float x_rec = static_cast<float>(ORIGIN_X + a * CELL_SIZE);
      float y_rec = static_cast<float>(ORIGIN_Y + b * CELL_SIZE);

      // Crear un rectángulo solo con borde
      sf::RectangleShape cellFrame;
      cellFrame.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
      cellFrame.setPosition(sf::Vector2f(x_rec, y_rec));
      cellFrame.setFillColor(sf::Color::Transparent);  // Sin relleno
      cellFrame.setOutlineThickness(1.0f);             // Grosor del borde
      cellFrame.setOutlineColor(sf::Color::Black);     // Color del borde

      // Dibujar el marco
      i_window.draw(cellFrame);*/

      // draw_triangle(a, b, 29, 155, 240, i_window);
      sf::VertexArray triangles(
          sf::PrimitiveType::Triangles); // SFML 3.0: PrimitiveType::
      sf::VertexArray borders(sf::PrimitiveType::Lines); // Para los bordes

      // Dibujar la grid de triangulos
      float x = static_cast<float>(GridConfig::ORIGIN_X +
                                   (a / 2.0f) * GridConfig::CELL_SIZE);
      float y =
          static_cast<float>(GridConfig::ORIGIN_Y + (b)*GridConfig::CELL_SIZE);

      // Definir los 3 vértices del triángulo
      sf::Vertex vertex1, vertex2, vertex3;
      sf::Vector2f pos1, pos2, pos3; // Guardar posiciones para los bordes

      if ((a + b) % 2 == 0) {
        // Triángulo apuntando hacia arriba
        pos1 = sf::Vector2f(x + GridConfig::CELL_SIZE / 2.0f, y);
        pos2 = sf::Vector2f(x, y + GridConfig::CELL_SIZE);
        pos3 =
            sf::Vector2f(x + GridConfig::CELL_SIZE, y + GridConfig::CELL_SIZE);

        vertex1.position = pos1;
        vertex2.position = pos2;
        vertex3.position = pos3;

        // vertex1.color = sf::Color(29, 155, 240);
        // vertex2.color = sf::Color(29, 155, 240);
        // vertex3.color = sf::Color(29, 155, 240);

      } else {
        // Triángulo apuntando hacia abajo
        pos1 = sf::Vector2f(x, y);
        pos2 = sf::Vector2f(x + GridConfig::CELL_SIZE, y);
        pos3 = sf::Vector2f(x + GridConfig::CELL_SIZE / 2.0f,
                            y + GridConfig::CELL_SIZE);

        vertex1.position = pos1;
        vertex2.position = pos2;
        vertex3.position = pos3;

        // vertex1.color = sf::Color(100, 200, 100);  // Color diferente para
        // variedad vertex2.color = sf::Color(100, 200, 100); vertex3.color =
        // sf::Color(100, 200, 100);
      }

      Cell cell_type = gameMap[a][b];

      switch (cell_type) {
      case Cell::Empty: {
        vertex1.color = sf::Color(29, 155, 240);
        vertex2.color = sf::Color(29, 155, 240);
        vertex3.color = sf::Color(29, 155, 240);
        break;
      }
      }

      // Determinar el color basado en el tipo de celda y efectos especiales
      sf::Color cellColor = sf::Color(29, 155, 240); // Color por defecto

      // Verificar si es una celda resaltada (adyacente al jugador)
      bool isHighlighted = false;
      for (const auto &highlighted : highlightedCells) {
        if (highlighted.first == static_cast<int>(a) &&
            highlighted.second == static_cast<int>(b)) {
          isHighlighted = true;
          break;
        }
      }

      // Aplicar colores especiales en el orden correcto de prioridad
      if (playerPos.first == static_cast<int>(a) &&
          playerPos.second == static_cast<int>(b)) {
        // Posición del jugador - MÁXIMA PRIORIDAD
        cellColor = sf::Color(255, 255, 0); // Rojo para el jugador
      } else if (GameConfig::START_POSITION.first == a && GameConfig::START_POSITION.second == b) {
        // Posición de inicio
        cellColor = sf::Color::Red;
      } else if (GameConfig::FINISH_POSITION.first == a && GameConfig::FINISH_POSITION.second == b) {
        // Posición de fin
        cellColor = sf::Color::Green;
      }

      if (isHighlighted) {
        // Celda adyacente - aplicar efecto de parpadeo
        float intensity = 0.4f + (pulseIntensity * 0.6f); // Entre 0.4 y 1.0

        int cellIndex = -1;
        for (size_t i = 0; i < highlightedCells.size(); i++) {
          if (highlightedCells[i].first == static_cast<int>(a) &&
              highlightedCells[i].second == static_cast<int>(b)) {
            cellIndex = static_cast<int>(i);
            break;
          }
        }

        // Asignar color base diferente según el índice de la celda
        sf::Color baseColor;
        switch (cellIndex) {
        case 0:
          baseColor = sf::Color(255, 0, 0, 170);
          break; // Rojo suave
        case 1:
          baseColor = sf::Color(0, 255, 0, 170);
          break; // Verde suave
        case 2:
          baseColor = sf::Color(0, 0, 255, 170);
          break; // Azul suave
        }

        cellColor =
            sf::Color(static_cast<uint8_t>(baseColor.r * intensity), // Rojo
                      static_cast<uint8_t>(baseColor.g * intensity), // Verde
                      static_cast<uint8_t>(baseColor.b * intensity), // Azul
                      static_cast<uint8_t>(baseColor.a * intensity)  // Alpha
            );
      }

      // Darle color al triangulo a, b actual
      vertex1.color = cellColor;
      vertex2.color = cellColor;
      vertex3.color = cellColor;

      // Agregar triángulo
      triangles.append(vertex1);
      triangles.append(vertex2);
      triangles.append(vertex3);

      // Agregar bordes (líneas entre cada par de vértices)
      sf::Color borderColor = sf::Color::Black;

      // Línea 1: vertex1 -> vertex2
      borders.append(sf::Vertex{pos1, borderColor});
      borders.append(sf::Vertex{pos2, borderColor});

      // Línea 2: vertex2 -> vertex3
      borders.append(sf::Vertex{pos2, borderColor});
      borders.append(sf::Vertex{pos3, borderColor});

      // Línea 3: vertex3 -> vertex1
      borders.append(sf::Vertex{pos3, borderColor});
      borders.append(sf::Vertex{pos1, borderColor});

      // Dibujar triángulos primero, luego bordes
      i_window.draw(triangles);
      i_window.draw(borders);
    }
  }
}
