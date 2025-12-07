#include "crow.h"
#include <thread>
#include <mutex>
#include <vector>
#include <fstream>
#include <sstream>

// ==== твои классы ====
#include "element.h"
#include "cairo_renderer.h"

// =====================
// Глобальные WS-клиенты
// =====================
std::vector<crow::websocket::connection *> clients;
std::mutex clients_mutex;
std::string input_text = "";
int active_lamp = 1;

// ===========================
// Генерация кадра SVG
// ===========================
std::string generate_svg_frame()
{
    CairoRenderer renderer(1000, 800, OutputFormat::SVG_MEMORY);

    Element circle1(nullptr, 100, 100);
    circle1.set_position(20, 50);
    circle1.set_color(active_lamp == 0 ? "#ffdd00" : "#444444");
    circle1.set_border_radius(100);

    renderer.render(circle1);

    Element circle2(nullptr, 100, 100);
    circle2.set_position(150, 50);
    circle2.set_color(active_lamp == 1 ? "#ffdd00" : "#444444");
    circle2.set_border_radius(100);

    renderer.render(circle2);

    Element circle3(nullptr, 100, 100);
    circle3.set_position(280, 50);
    circle3.set_color(active_lamp == 2 ? "#ffdd00" : "#444444");
    circle3.set_border_radius(100);
    renderer.render(circle3);

    Element label(nullptr, 100, 30);
    label.set_position(20, 180);
    label.set_color("#ffffff");
    label.set_text("Users input:");
    renderer.render(label);

    Element text(nullptr, 100, 30);
    text.set_position(20, 220);
    text.set_color("#ffffff");
    text.set_text(input_text);
    renderer.render(text);

    renderer.finish();

    return renderer.get_svg();
}

int main()
{
    crow::SimpleApp app;

    // =======================================
    // Раздача статического файла index.html
    // =======================================
    CROW_ROUTE(app, "/")([]()
                         {
        std::ifstream file("./client/index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str(); });

    // =======================================
    // WebSocket
    // =======================================
    CROW_ROUTE(app, "/ws").websocket(&app).onopen([](crow::websocket::connection &conn)
                                                  {
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.push_back(&conn);
            }
            std::cout << "WS connected\n";

            // Отправляем стартовое SVG
            conn.send_text(generate_svg_frame()); })
        .onclose([](crow::websocket::connection &conn, const std::string &reason, uint16_t code)
                 {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.erase(std::remove(clients.begin(), clients.end(), &conn), clients.end());
            std::cout << "WS disconnected\n"; })
        .onmessage([](crow::websocket::connection &conn, const std::string &data, bool)
                   {
            std::cout << "Message: " << data << std::endl;
            conn.send_text("Echo: " + data); });

    // =======================================
    // Фоновая анимация — рассылаем SVG
    // =======================================
    std::thread([&]()
                {
        int active = 0;

        while (true)
        {
            active_lamp = active;
            std::string svg = generate_svg_frame();

            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                for (auto* c : clients)
                    c->send_text(svg);
            }

            active = (active + 1) % 3;   // переключаем лампочку
            std::this_thread::sleep_for(std::chrono::seconds(2));
        } })
        .detach();

    std::thread input_thread([&]()
                             {
    while (true)
    {
        std::string text;
        std::getline(std::cin, text);

        input_text = text;
        std::cout << "Input updated: " << input_text << std::endl;

        // Генерируем новый кадр
        std::string svg = generate_svg_frame();  // -1 → лампочки не меняем

        // Рассылаем всем клиентам
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            for (auto* c : clients)
                c->send_text(svg);
        }
    } });
    input_thread.detach();

    // =======================================
    // Запуск сервера
    // =======================================
    app.port(8080).multithreaded().run();
}
