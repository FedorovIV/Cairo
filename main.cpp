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

    const std::string hull_color = "#f3f6fb";
    const std::string card_color = "#ffffff";
    const std::string soft_color = "#e1e7f5";
    const std::string accent_color = "#3c6df0";
    const std::string accent_soft = "#c7d4ff";
    const std::string ok_color = "#3cc36b";
    const std::string warn_color = "#f5a524";
    const std::string danger_color = "#ef5350";

    Element hull(nullptr, 960, 720);
    hull.set_position(20, 30);
    hull.set_color(hull_color);
    hull.set_border_radius(12);
    renderer.render(hull);

    // Header band
    Element header(&hull, 920, 70);
    header.set_position(40, 50);
    header.set_color(card_color);
    header.set_border_radius(10);
    renderer.render(header);

    Element title(&header, 260, 26);
    title.set_position(60, 70);
    title.set_color(card_color);
    title.set_text("Flight Deck // ORION-3");
    renderer.render(title);

    Element sector(&header, 320, 20);
    sector.set_position(60, 98);
    sector.set_color(card_color);
    sector.set_text("Sector: Perseus | Trajectory: Stable orbit | Crew: 5");
    renderer.render(sector);

    // Thruster cycle lights (driven by active_lamp)
    Element thruster_label(&header, 180, 20);
    thruster_label.set_position(720, 70);
    thruster_label.set_color(card_color);
    thruster_label.set_text("Thruster cycle");
    renderer.render(thruster_label);

    const std::string thruster_on = "#68d76d";
    const std::string thruster_off = "#cbd2e4";
    Element thruster1(&header, 26, 26);
    thruster1.set_position(720, 94);
    thruster1.set_color(active_lamp == 0 ? thruster_on : thruster_off);
    thruster1.set_border_radius(100);
    renderer.render(thruster1);

    Element thruster2(&header, 26, 26);
    thruster2.set_position(756, 94);
    thruster2.set_color(active_lamp == 1 ? thruster_on : thruster_off);
    thruster2.set_border_radius(100);
    renderer.render(thruster2);

    Element thruster3(&header, 26, 26);
    thruster3.set_position(792, 94);
    thruster3.set_color(active_lamp == 2 ? thruster_on : thruster_off);
    thruster3.set_border_radius(100);
    renderer.render(thruster3);

    // Left column: status panels
    auto make_status_panel = [&](double x, double y, const std::string &name, const std::string &value, double fill, const std::string &fill_color)
    {
        Element panel(&hull, 420, 110);
        panel.set_position(x, y);
        panel.set_color(card_color);
        panel.set_border_radius(10);
        renderer.render(panel);

        Element label(&panel, 200, 20);
        label.set_position(x + 20, y + 16);
        label.set_color(card_color);
        label.set_text(name);
        renderer.render(label);

        Element reading(&panel, 200, 18);
        reading.set_position(x + 20, y + 44);
        reading.set_color(card_color);
        reading.set_text(value);
        renderer.render(reading);

        Element bar_bg(&panel, 360, 14);
        bar_bg.set_position(x + 20, y + 74);
        bar_bg.set_color(soft_color);
        bar_bg.set_border_radius(10);
        renderer.render(bar_bg);

        Element bar_fill(&panel, 360 * fill, 14);
        bar_fill.set_position(x + 20, y + 74);
        bar_fill.set_color(fill_color);
        bar_fill.set_border_radius(10);
        renderer.render(bar_fill);
    };

    make_status_panel(40, 150, "Reactor Core", "Output: 78% | Temp: 523K", 0.78, accent_color);
    make_status_panel(40, 280, "Life Support", "O2 stable | Pressure nominal", 0.64, ok_color);
    make_status_panel(40, 410, "Shield Matrix", "Charge cycling", 0.42, warn_color);

    // Right column: navigation grid & telemetry
    Element nav_panel(&hull, 440, 320);
    nav_panel.set_position(520, 150);
    nav_panel.set_color(card_color);
    nav_panel.set_border_radius(10);
    renderer.render(nav_panel);

    Element nav_label(&nav_panel, 200, 20);
    nav_label.set_position(540, 170);
    nav_label.set_color(card_color);
    nav_label.set_text("Navigation Grid");
    renderer.render(nav_label);

    // Simple sensor pings
    Element sensor1(&nav_panel, 12, 12);
    sensor1.set_position(720, 240);
    sensor1.set_color(accent_color);
    sensor1.set_border_radius(100);
    renderer.render(sensor1);

    Element sensor2(&nav_panel, 18, 18);
    sensor2.set_position(640, 320);
    sensor2.set_color(ok_color);
    sensor2.set_border_radius(100);
    renderer.render(sensor2);

    Element sensor3(&nav_panel, 10, 10);
    sensor3.set_position(820, 270);
    sensor3.set_color(danger_color);
    sensor3.set_border_radius(100);
    renderer.render(sensor3);

    // Telemetry bars
    auto draw_bar = [&](double x, double y, double h, const std::string &color)
    {
        Element bar(&nav_panel, 24, h);
        bar.set_position(x, y + (160 - h));
        bar.set_color(color);
        bar.set_border_radius(6);
        renderer.render(bar);
    };
    draw_bar(560, 330, 120, accent_soft);
    draw_bar(594, 330, 160, accent_color);
    draw_bar(628, 330, 100, ok_color);
    draw_bar(662, 330, 70, warn_color);
    draw_bar(696, 330, 140, accent_color);

    // Communications console
    Element comms(&hull, 420, 120);
    comms.set_position(40, 540);
    comms.set_color(card_color);
    comms.set_border_radius(10);
    renderer.render(comms);

    Element comms_label(&comms, 200, 20);
    comms_label.set_position(60, 560);
    comms_label.set_color(card_color);
    comms_label.set_text("Comms Channel");
    renderer.render(comms_label);

    Element comms_text(&comms, 360, 20);
    comms_text.set_position(60, 590);
    comms_text.set_color(card_color);
    comms_text.set_text("Last message: \"Standby burn in T-120s\"");
    renderer.render(comms_text);

    // Action buttons
    Button btn1("Docking");
    btn1.set_position(520, 500);
    renderer.render(btn1);

    Button btn2("Pulse");
    btn2.set_position(680, 500);
    renderer.render(btn2);

    Button btn3("Shields");
    btn3.set_position(840, 500);
    renderer.render(btn3);

    // Live input echo panel
    Element input_panel(&hull, 440, 120);
    input_panel.set_position(520, 580);
    input_panel.set_color(card_color);
    input_panel.set_border_radius(10);
    renderer.render(input_panel);

    Element input_label(&input_panel, 200, 20);
    input_label.set_position(540, 600);
    input_label.set_color(card_color);
    input_label.set_text("Command Line");
    renderer.render(input_label);

    Element input_text_el(&input_panel, 400, 20);
    input_text_el.set_position(540, 630);
    input_text_el.set_color(card_color);
    input_text_el.set_text(input_text.empty() ? "Awaiting input..." : input_text);
    renderer.render(input_text_el);

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
