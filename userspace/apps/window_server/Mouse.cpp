#include "Mouse.hpp"

#include <libc/syscalls.hpp>

#include <libsys/Log.hpp>

#include <wisterialib/posix/shared.hpp>


void Mouse::update_position()
{
    m_prev_x = m_x;
    m_prev_y = m_y;

    static MousePacket mouse_buffer[32];
    int bytes = read(m_fd, mouse_buffer, sizeof(mouse_buffer));
    for (size_t packet_index = 0 ; packet_index < bytes / sizeof(MousePacket); packet_index++) {
        m_x += mouse_buffer[packet_index].x_move;
        m_y -= mouse_buffer[packet_index].y_move;
    }
}