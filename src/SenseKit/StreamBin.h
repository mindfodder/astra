#ifndef STREAMBIN_H
#define STREAMBIN_H

#include <exception>
#include <array>
#include <atomic>
#include <sensekit_types.h>

namespace sensekit {

    class StreamBin
    {
    public:
        StreamBin(size_t bufferSizeInBytes);
        ~StreamBin();

        //exposed to plugins
        sensekit_frame_t* get_backBuffer()
            {
                return m_buffers[m_backBufferTailIndex];
            }

        sensekit_frame_t* cycle_buffers();

        //TODO support reference counting for multi-client scenarios
        sensekit_frame_t* lock_front_buffer()
            {
                m_frontBufferLocked = true;
                return get_frontBuffer();
            }

        void unlock_front_buffer()
            {
                m_frontBufferLocked = false;
            }

        void inc_active() { m_activeCount++; }
        void dec_active()
            {
                m_activeCount--;
                if (m_activeCount < 0)
                {
                    throw std::exception();
                }
            }

        void inc_connected() { m_connectedCount++; }
        void dec_connected()
            {
                m_connectedCount--;
                if (m_connectedCount < 0)
                {
                    throw std::exception();
                }
            }

        bool is_active() { return m_activeCount > 0; }
        bool has_clients_connected() { return m_connectedCount > 0; }

    private:
        void allocate_buffers(size_t bufferLengthInBytes);
        sensekit_frame_t* get_frontBuffer();
        size_t inc_index(size_t index);

        std::atomic_bool m_frontBufferLocked;

        size_t m_frontBufferIndex{0};
        size_t m_backBufferHeadIndex{1};
        size_t m_backBufferTailIndex{1};

        const static size_t BUFFER_COUNT = 2;
        using FrameBufferArray = std::array<sensekit_frame_t*, BUFFER_COUNT>;

        FrameBufferArray m_buffers;
        int m_connectedCount{0};
        int m_activeCount{0};
    };
}

#endif /* STREAMBIN_H */