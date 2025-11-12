#pragma once
#include <cstddef>
#include <utility>

class Element {
public:
    using coord_t = double;

    // Конструктор: parent может быть nullptr.
    // width/height по умолчанию 0.
    explicit Element(const Element* parent = nullptr,
                     coord_t width = 0.0,
                     coord_t height = 0.0) noexcept;

    // Доступ к геометрии
    coord_t x() const noexcept { return x_; }
    coord_t y() const noexcept { return y_; }
    coord_t width()  const noexcept { return width_; }
    coord_t height() const noexcept { return height_; }

    // Установка размеров и позиции (если понадобится вручную)
    void set_position(coord_t x, coord_t y) noexcept { x_ = x; y_ = y; }
    void set_size(coord_t w, coord_t h) noexcept { width_ = w; height_ = h; }

    // Родитель (невладеющая ссылка)
    const Element* parent() const noexcept { return parent_; }

private:
    const Element* parent_ = nullptr;  // невладеющий указатель
    coord_t x_ = 0.0;
    coord_t y_ = 0.0;
    coord_t width_  = 0.0;
    coord_t height_ = 0.0;
};
