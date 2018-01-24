#pragma once

template<typename T, gsize size>
  class RingBuffer
  {
    public:
      RingBuffer() :
          m_buffer(size)
      {
      }

      void push(const T* data, size_t count)
      {
        auto filled = m_writeHead - m_readHead;
        auto space = size - filled;
        g_assert(count <= space);

        if(count)
        {
          auto idx = m_writeHead & mask;
          auto space = size - idx;
          auto for_now = std::min(space, count);
          std::copy(data, data + for_now, m_buffer.data() + idx);
          m_writeHead += for_now;
          push(data + for_now, count - for_now);
        }
      }

      size_t pull(T* data, size_t count)
      {
        count = std::min<uint64_t>(count, m_writeHead - m_readHead);

        if(count)
        {
          auto idx = m_readHead & mask;
          auto space = size - idx;
          auto for_now = std::min(space, count);
          std::copy(m_buffer.data() + idx, m_buffer.data() + idx + for_now, data);
          m_readHead += for_now;
          return for_now + pull(data + for_now, count - for_now);
        }
        return 0;
      }

      void unpull(size_t count)
      {
        m_readHead -= count;
      }

      void flush()
      {
        m_readHead = m_writeHead = 0;
      }

    private:
      static constexpr size_t mask = (size - 1);
      std::vector<T> m_buffer;
      uint64_t m_writeHead = 0;
      uint64_t m_readHead = 0;
  };

