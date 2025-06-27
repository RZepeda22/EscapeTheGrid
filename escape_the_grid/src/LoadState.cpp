#include "LoadState.hpp"
#include "Game.hpp"
#include "EmbeddedResources.hpp"
#include <iostream>
#include <fstream>
#include "Config.hpp"
#include <vector>

constexpr int WINDOW_WIDTH = 1280;
constexpr int WINDOW_HEIGHT = 720;
std::vector<std::vector<GameConfig::Cell>> fileMap;

// API nativa de Windows
#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

LoadState::LoadState()  {
    if (!mFont.openFromMemory(embedded_font_data, embedded_font_data_size)) {
        std::cout << "Error cargando fuente en LoadState" << std::endl;
    }

}

void LoadState::handleEvent(const sf::Event& event, Game* game) {
        if (event.is<sf::Event::KeyPressed>()) {
        const sf::Event::KeyPressed* keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            game->changeState(GameState::MainMenu);
            return;
        }
    }

        if (event.is<sf::Event::MouseButtonPressed>()) {
        const sf::Event::MouseButtonPressed* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f clickPos(static_cast<float>(mouseEvent->position.x),
                                 static_cast<float>(mouseEvent->position.y));
            
            // LÓGICA DEL FILE CHOOSER DIRECTAMENTE AQUÍ
            if (mSelectMapButton.getGlobalBounds().contains(clickPos)) {
                
                // ===== FILE CHOOSER NATIVO DE WINDOWS =====
#ifdef _WIN32
                OPENFILENAMEA ofn;
                char szFile[260] = {0};
                
                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = GetActiveWindow();
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.lpstrFilter = "Archivos de Texto\0*.txt\0Todos los Archivos\0*.*\0";
                ofn.nFilterIndex = 1;
                ofn.lpstrTitle = "Seleccionar Mapa (.txt)";
                ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
                
                if (GetOpenFileNameA(&ofn) == TRUE) {
                    mSelectedMapPath = std::string(szFile);
                    std::cout << "Archivo seleccionado: " << mSelectedMapPath << std::endl;
                    
                    // ===== VALIDACIÓN DEL ARCHIVO DIRECTAMENTE AQUÍ =====
                    std::ifstream file(mSelectedMapPath);
                    if (file.is_open()) {
                        std::string line;
                        int lineCount = 0;
                        bool hasValidContent = false;
                        int appearS = 0;
                        int appearG = 0;
                        


                        while (std::getline(file, line) && lineCount < 50) {
                            if (line.empty()) continue;
                            fileMap.push_back(std::vector<GameConfig::Cell>()); // Añade una fila (vector) vacía
                            bool lineValid = true;
                            bool hasNumbers = false;
                            
                            /*for (char c : line) {
                                if (c >= '0' && c <= '9') {
                                    hasNumbers = true;
                                } else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                                    lineValid = false;
                                    break;
                                }
                            }*/

                            for (char c : line) {

                                if (c == 'S' && appearS < 1) {
                                    fileMap[lineCount].push_back(GameConfig::Cell::Start);
                                    appearS++;
                                } else if (c == 'G' && appearG < 1) {
                                    fileMap[lineCount].push_back(GameConfig::Cell::Finish);
                                    appearG++;
                                }
                                
                                //std::cout << c;
                                if (c == '.' || c == '#' || c == 'K' ||  c == ' ' || c == '\t' || c == '\n' || c == '\r') {
                                    //std::cout << "Caracter: " << lineCharacter << std::endl;
                                    switch (c) {
                                        case '.':
                                            fileMap[lineCount].push_back(GameConfig::Cell::Empty);
                                            break;
                                        case '#':
                                            fileMap[lineCount].push_back(GameConfig::Cell::Wall);
                                            break;
                                        case 'K':
                                            fileMap[lineCount].push_back(GameConfig::Cell::Item);
                                            break;
                                    }
                                    hasNumbers = true;
                                }else{
                                    hasNumbers = false;
                                }
                                
                                /*else if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                                    //lineValid = false;
                                    break;
                                }*/


                            }


                            if (hasNumbers ) {
                                hasValidContent = true;
                            }
                            
                            /*if (lineValid && hasNumbers) {
                                hasValidContent = true;
                            }*/
                            lineCount++;
                        }
                        /*std::cout << "CUENTA LAS FILAS" <<fileMap.size() << std::endl;
                        unsigned short numColumns = 0;
                        if (!fileMap.empty()) { // Verifica que haya al menos una fila
                            numColumns = fileMap[0].size();
                        }
                        std::cout << "CUENTA LAS COLUMNAS" <<numColumns << std::endl;*/
                        GameConfig::updateMap(fileMap); // Actualiza el mapa global

                        // Bucle exterior para las filas
                        if (!fileMap.empty()) { // Verifica que haya al menos una fila
                            for (size_t i = 0; i < fileMap.size(); ++i) {
                                // Bucle interior para las columnas en la fila actual
                                for (size_t j = 0; j < fileMap[0].size(); ++j) {
                                    //std::cout << fileMap[i][j] << "\t"; // ¡Salto de línea después de cada fila!
                                    if(fileMap[i][j] == GameConfig::Cell::Start){
                                        std::cout << i  << j << "\t"; 
                                        GameConfig::updateStart(j, i); // Actualiza la posición de inicio
                                    }

                                    if(fileMap[i][j] == GameConfig::Cell::Finish){
                                        GameConfig::updateStart(i, j); // Actualiza la posición de inicio
                                    }
                                }
                                std::cout << std::endl;
                                
                            }
                        }


                            // Otra forma más moderna y concisa usando bucles for basados en rango (C++11 en adelante)
                        /*for (const auto& fila : fileMap) { // Itera sobre cada 'fila' (que es un std::vector<int>)
                            for (const auto& valor : fila) { // Itera sobre cada 'valor' dentro de la 'fila'
                                std::cout << valor << "\t";
                            }
                            std::cout << std::endl; // Salto de línea después de cada fila
                        }*/

                        file.close();
                        mMapValid = hasValidContent && lineCount > 0;
                        

                        if (mMapValid) {
                            std::cout << "✓ Mapa válido! (" << lineCount << " líneas)" << std::endl;
                        } else {
                            std::cout << "✗ Formato de mapa inválido" << std::endl;
                        }
                    } else {
                        mMapValid = false;
                        std::cout << "✗ No se pudo abrir el archivo" << std::endl;
                    }
                } else {
                    std::cout << "Selección cancelada" << std::endl;
                }
#else
                std::cout << "File chooser no disponible en este sistema" << std::endl;
#endif
                return;
            }
            
            // Botón "Cargar Mapa"
            if (mMapValid && !mSelectedMapPath.empty() && mLoadMapButton.getGlobalBounds().contains(clickPos)) {
                        if (!fileMap.empty()) { // Verifica que haya al menos una fila
                            GridConfig::updateGridSize(fileMap[0].size() ,fileMap.size());
                        }else{
                            GridConfig::updateGridSize(50, 25);
                        }
                std::cout << "Cargando mapa: " << mSelectedMapPath << std::endl;
                // Aquí puedes guardar la ruta en alguna variable global o pasarla al juego
                game->changeState(GameState::Playing);
                return;
            }
            
            // Botón "Volver"
            if (mBackButton.getGlobalBounds().contains(clickPos)) {
                game->changeState(GameState::MainMenu);
                return;
            }
        }
    }

}

void LoadState::update(float deltaTime, Game* game) {
    // Animaciones del menú aquí
}

void LoadState::render(sf::RenderWindow& window, Game* game) {
        window.clear(sf::Color::Black);
    
    // Título
    /*sf::Text titleText(mFont, "Cargar Mapa - Ingresa la ruta del archivo", 28);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition({50, 100});
    window.draw(titleText);*/

        // Título principal
    sf::Text titleText(mFont, "Cargar Mapa Personalizado", 36);
    titleText.setFillColor(sf::Color::White);
    // SFML 3.0: usar .size.x en lugar de .width
    titleText.setPosition({WINDOW_WIDTH/2.0f - titleText.getGlobalBounds().size.x/2.0f, 60});
    window.draw(titleText);

    // Subtítulo
    sf::Text subtitleText(mFont, "Selecciona un archivo .txt con los datos del mapa", 18);
    subtitleText.setFillColor(sf::Color(180, 180, 180));
    // SFML 3.0: usar .size.x
    subtitleText.setPosition({WINDOW_WIDTH/2.0f - subtitleText.getGlobalBounds().size.x/2.0f, 120});
    window.draw(subtitleText);

    

    // CONFIGURAR Y DIBUJAR BOTONES DIRECTAMENTE AQUÍ
    const float buttonWidth = 400.0f;
    const float buttonHeight = 60.0f;
    const float centerX = WINDOW_WIDTH / 2.0f;

        // Botón "Seleccionar Archivo"
    mSelectMapButton.setSize({buttonWidth, buttonHeight});
    mSelectMapButton.setPosition({centerX - buttonWidth/2, 200});
    mSelectMapButton.setFillColor(sf::Color(70, 130, 180));
    mSelectMapButton.setOutlineThickness(3);
    mSelectMapButton.setOutlineColor(sf::Color::White);
    window.draw(mSelectMapButton);
    
    sf::Text selectText(mFont, L" Seleccionar Archivo (.txt)", 22);
    selectText.setFillColor(sf::Color::White);
    // SFML 3.0: usar .size.x
    selectText.setPosition({centerX - selectText.getGlobalBounds().size.x/2, 220});
    window.draw(selectText);
    
    // MOSTRAR INFORMACIÓN DEL ARCHIVO DIRECTAMENTE AQUÍ
    if (!mSelectedMapPath.empty()) {
        // Extraer nombre del archivo
        size_t lastSlash = mSelectedMapPath.find_last_of("/\\");
        std::string fileName = (lastSlash != std::string::npos) 
            ? mSelectedMapPath.substr(lastSlash + 1) 
            : mSelectedMapPath;
        
        // Caja de información
        sf::RectangleShape infoBox({600, 100});
        infoBox.setPosition({centerX - 300, 300});
        infoBox.setFillColor(sf::Color(40, 45, 60));
        infoBox.setOutlineThickness(2);
        infoBox.setOutlineColor(mMapValid ? sf::Color::Green : sf::Color::Red);
        window.draw(infoBox);
        
        sf::Text fileText(mFont, "Archivo: " + fileName, 18);
        fileText.setFillColor(sf::Color::White);
        // SFML 3.0: usar .size.x
        fileText.setPosition({centerX - fileText.getGlobalBounds().size.x/2, 320});
        window.draw(fileText);
        sf::String mapaValido = L" Mapa válido - Listo para cargar";
        sf::String mapaInvalido = L" Formato inválido";
        sf::String statusMsg = mMapValid ? mapaValido : mapaInvalido;
        sf::Text statusText(mFont, statusMsg, 16);
        statusText.setFillColor(mMapValid ? sf::Color::Green : sf::Color::Red);
        // SFML 3.0: usar .size.x
        statusText.setPosition({centerX - statusText.getGlobalBounds().size.x/2, 350});
        window.draw(statusText);
        
        // Ruta truncada
        std::string displayPath = mSelectedMapPath;
        if (displayPath.length() > 60) {
            displayPath = "..." + displayPath.substr(displayPath.length() - 57);
        }
        sf::Text pathText(mFont, displayPath, 12);
        pathText.setFillColor(sf::Color(120, 120, 120));
        // SFML 3.0: usar .size.x
        pathText.setPosition({centerX - pathText.getGlobalBounds().size.x/2, 375});
        window.draw(pathText);
    }
    
    // Botón "Cargar Mapa" (solo si es válido)
    if (mMapValid && !mSelectedMapPath.empty()) {
        mLoadMapButton.setSize({buttonWidth, buttonHeight});
        mLoadMapButton.setPosition({centerX - buttonWidth/2, 450});
        mLoadMapButton.setFillColor(sf::Color(34, 139, 34));
        mLoadMapButton.setOutlineThickness(3);
        mLoadMapButton.setOutlineColor(sf::Color::White);
        window.draw(mLoadMapButton);
        
        sf::Text loadText(mFont, L" Cargar Mapa y Comenzar", 24);
        loadText.setFillColor(sf::Color::White);
        // SFML 3.0: usar .size.x
        loadText.setPosition({centerX - loadText.getGlobalBounds().size.x/2, 470});
        window.draw(loadText);
    }
    
    // Botón "Volver"
    mBackButton.setSize({200, 50});
    mBackButton.setPosition({50, WINDOW_HEIGHT - 100});
    mBackButton.setFillColor(sf::Color(180, 70, 70));
    mBackButton.setOutlineThickness(2);
    mBackButton.setOutlineColor(sf::Color::White);
    window.draw(mBackButton);
    
    sf::Text backText(mFont, L"← Volver", 20);
    backText.setFillColor(sf::Color::White);
    backText.setPosition({70, WINDOW_HEIGHT - 85});
    window.draw(backText);
    
    // Ayuda
    sf::Text helpText(mFont, L"El archivo debe contener números separados por espacios", 14);
    helpText.setFillColor(sf::Color(100, 100, 100));
    // SFML 3.0: usar .size.x
    helpText.setPosition({centerX - helpText.getGlobalBounds().size.x/2, WINDOW_HEIGHT - 30});
    window.draw(helpText);
    

}

void LoadState::onEnter(Game* game) {
    mSelectedMapPath = "";
    mMapValid = false;
    std::cout << "Entrando a LoadState - File Chooser" << std::endl;
}