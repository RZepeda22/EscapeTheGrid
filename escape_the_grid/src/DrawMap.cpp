#include <SFML/Graphics.hpp>
#include "Config.hpp"
#include <vector> 
//#include <chrono>
//#include <cmath>
//#include <iostream>
#include "DrawMap.hpp"




void draw_map(sf::RenderWindow &i_window, const std::pair<int, int> &playerPos,
              const std::vector<std::pair<int, int>> &highlightedCells,
              float pulseIntensity) {

  // DEFINIR EL ÁREA FIJA DEL MAPA EN PÍXELES (el rectángulo grande)
  const float MAP_AREA_X = 30;        // Posición X del área del mapa 50
  const float MAP_AREA_Y = 75.6;        // Posición Y del área del mapa 80
  const float MAP_AREA_WIDTH = 853.3;   // Ancho del área del mapa 750
  const float MAP_AREA_HEIGHT = 568.8;  // Alto del área del mapa 550 568.8

      // Modificar valores
    //GridConfig::updateGridSize(15, 15);  // Cambiar a 20x15
    //GridConfig::updateCellSize(50.0f);   // Cambiar tamaño de celda

  // Configuración de la grid (puedes cambiar estos valores)
  const int GRID_COLUMNS = GridConfig::COLUMNS;  // Número de columnas que quieres
  const int GRID_ROWS = GridConfig::ROWS;
  
    // CALCULAR EL TAMAÑO AUTOMÁTICO DE LAS CELDAS
  // El tamaño se ajusta automáticamente para que toda la grid quepa en el área
  float cellWidth = MAP_AREA_WIDTH / (GRID_COLUMNS / 2.0f + 1.0f);
  float cellHeight = MAP_AREA_HEIGHT / GRID_ROWS;
  
  // Usar el menor de los dos para mantener proporciones
  float CELL_SIZE = std::min(cellWidth, cellHeight);
  GridConfig::updateCellSize(CELL_SIZE);
  
    // CALCULAR EL TAMAÑO REAL QUE OCUPARÁ LA GRID CON ESTE CELL_SIZE
  //float actualGridWidth = GRID_COLUMNS * CELL_SIZE * 0.5f;

    // Calcular las dimensiones reales que ocupará la grid
  float actualGridWidth = (GRID_COLUMNS / 2.0f + 1.0f) * CELL_SIZE;
  float actualGridHeight = GRID_ROWS * CELL_SIZE;

    // CENTRAR LA GRID EN EL ÁREA DISPONIBLE
  float ORIGIN_X = MAP_AREA_X + (MAP_AREA_WIDTH - actualGridWidth) / 2.0f;
  float ORIGIN_Y = MAP_AREA_Y + (MAP_AREA_HEIGHT - actualGridHeight) / 2.0f;
  GridConfig::updateOrigins(ORIGIN_X, ORIGIN_Y);

  // DIBUJAR EL BORDE DEL ÁREA DEL MAPA
  sf::RectangleShape mapBorder;
  mapBorder.setSize(sf::Vector2f(MAP_AREA_WIDTH, MAP_AREA_HEIGHT));
  mapBorder.setPosition(sf::Vector2f(MAP_AREA_X, MAP_AREA_Y));
  mapBorder.setFillColor(sf::Color(1, 4, 9)); // Fondo claro 240, 240, 240
  mapBorder.setOutlineThickness(3);
  mapBorder.setOutlineColor(sf::Color::White);
  i_window.draw(mapBorder);

  // Crear el mapa directamente como std::array
  //std::array<std::array<Cell, GRID_ROWS>, GRID_COLUMNS> gameMap;
  
  // CAMBIO PRINCIPAL: Usar vector en lugar de array
  std::vector<std::vector<GameConfig::Cell>> gameMap = GameConfig::gameMap;

  //std::vector<std::vector<GameConfig::Cell>> gameMap(GRID_COLUMNS, std::vector<GameConfig::Cell>(GRID_ROWS, GameConfig::Cell::Empty));

  // Inicializar todas las celdas como Empty
  /*for (unsigned short a = 0; a < GRID_COLUMNS; a++) {
    for (unsigned short b = 0; b < GRID_ROWS; b++) {
      gameMap[a][b] = Cell::Empty;
    }
  }*/

  //**************************************************** */

  for (unsigned short a = 0; a < GRID_COLUMNS; a++) {
    for (unsigned short b = 0; b < GRID_ROWS; b++) {

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
      float x = static_cast<float>(ORIGIN_X +
                                   (a / 2.0f) * CELL_SIZE);
      float y =
          static_cast<float>(ORIGIN_Y + (b)*CELL_SIZE);

            // Verificar que no se salga del área del mapa
      /*if (x < MAP_AREA_X || x + CELL_SIZE > MAP_AREA_X + MAP_AREA_WIDTH ||
          y < MAP_AREA_Y || y + CELL_SIZE > MAP_AREA_Y + MAP_AREA_HEIGHT) {
        continue; // Saltar si se sale del área
      }*/

      // Definir los 3 vértices del triángulo
      sf::Vertex vertex1, vertex2, vertex3;
      sf::Vector2f pos1, pos2, pos3; // Guardar posiciones para los bordes

      if ((a + b) % 2 == 0) {
        // Triángulo apuntando hacia arriba
        pos1 = sf::Vector2f(x + CELL_SIZE / 2.0f, y);
        pos2 = sf::Vector2f(x, y + CELL_SIZE);
        pos3 =
            sf::Vector2f(x + CELL_SIZE, y + CELL_SIZE);

        vertex1.position = pos1;
        vertex2.position = pos2;
        vertex3.position = pos3;

        // vertex1.color = sf::Color(29, 155, 240);
        // vertex2.color = sf::Color(29, 155, 240);
        // vertex3.color = sf::Color(29, 155, 240);

      } else {
        // Triángulo apuntando hacia abajo
        pos1 = sf::Vector2f(x, y);
        pos2 = sf::Vector2f(x + CELL_SIZE, y);
        pos3 = sf::Vector2f(x + CELL_SIZE / 2.0f,
                            y + CELL_SIZE);

        vertex1.position = pos1;
        vertex2.position = pos2;
        vertex3.position = pos3;

        // vertex1.color = sf::Color(100, 200, 100);  // Color diferente para
        // variedad vertex2.color = sf::Color(100, 200, 100); vertex3.color =
        // sf::Color(100, 200, 100);
      }

      GameConfig::Cell cell_type = gameMap[b][a];
      

      switch (cell_type) {
      case GameConfig::Cell::Empty: {
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
        cellColor = sf::Color(0, 255, 0); // Rojo para el jugador
      } /*else if (GameConfig::START_POSITION.first == a && GameConfig::START_POSITION.second == b) {
        // Posición de inicio
        cellColor = sf::Color::Red;
      } else if (GameConfig::FINISH_POSITION.first == a && GameConfig::FINISH_POSITION.second == b) {
        // Posición de fin
        cellColor = sf::Color::Green;
      }*/
      /*if (cell_type == GameConfig::Cell::Start) {
        // Posición del jugador - MÁXIMA PRIORIDAD
        cellColor = sf::Color(255, 255, 0); // Rojo para el jugador
      } else*/ if (cell_type == GameConfig::Cell::Start) {
        // Posición de inicio
        cellColor = sf::Color::Red;
      } else if (cell_type == GameConfig::Cell::Finish) {
        // Posición de fin
        cellColor = sf::Color::Green;
      } else if (cell_type == GameConfig::Cell::Empty) {
        // Celda vacía - color azul claro
        cellColor = sf::Color(29, 155, 240);
      } else if (cell_type == GameConfig::Cell::Finish) {
        // Meta - color verde
        cellColor = sf::Color(0, 255, 0);
      }


      else if (cell_type == GameConfig::Cell::Wall) {
        // Pared - color gris oscuro
        cellColor = sf::Color(100, 100, 100);
      } else if (cell_type == GameConfig::Cell::Item) {
        // Item - color amarillo
        cellColor = sf::Color(255, 215, 0);
      } else if (cell_type == GameConfig::Cell::Visited) {
        // Celda visitada - color azul claro
        cellColor = sf::Color(173, 216, 230);
      } else if (cell_type == GameConfig::Cell::Path) {
        // Camino - color naranja
        cellColor = sf::Color(255, 165, 0);
      } else if (cell_type == GameConfig::Cell::Invalid) {
        // Celda inválida - color rojo oscuro
        cellColor = sf::Color(139, 0, 0);
      }
            // Aplicar colores especiales en el orden correcto de prioridad
      if (playerPos.first == static_cast<int>(a) &&
          playerPos.second == static_cast<int>(b)) {
        // Posición del jugador - MÁXIMA PRIORIDAD
        cellColor = sf::Color(0, 255, 0); // Rojo para el jugador
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
