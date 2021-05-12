#include "FlacFrameStorage.h"
#include <nltools/logging/Log.h>

struct Bitstream
{
  Bitstream(std::vector<uint8_t> &d)
      : data(d)
  {
  }

  void seek(uint64_t toBit)
  {
    pos = toBit;
  }

  uint64_t read(uint32_t numBits)
  {
    uint64_t ret = 0;
    for(uint32_t i = 0; i < numBits; i++)
      ret |= readBit() << (numBits - i - 1);

    return ret;
  }

  void patch(uint32_t numBits, uint64_t num)
  {
    for(uint32_t i = 0; i < numBits; i++)
      patchBit((num >> (numBits - i - 1)) & 0x01);
  }

  uint64_t readBit()
  {
    auto byte = pos / 8;
    auto bit = 7 - (pos % 8);
    pos++;
    return (data[byte] >> bit) & 0x01;
  }

  void patchBit(uint32_t i)
  {
    auto byte = pos / 8;
    auto bit = 7 - (pos % 8);
    pos++;
    uint8_t mask = ~(1 << bit);
    data[byte] &= mask;
    data[byte] |= i << bit;
  }

  std::vector<uint8_t> &data;
  uint32_t pos = 0;
};

FlacFrameStorage::FlacFrameStorage(uint64_t maxMemUsage)
    : m_maxMemUsage(maxMemUsage)
{
}

void FlacFrameStorage::push(std::unique_ptr<FlacEncoder::Frame> frame, bool isHeader)
{
  std::unique_lock<std::mutex> l(m_mutex);

  if(isHeader)
  {
    m_header.push_back(std::move(frame));
    return;
  }

  m_streams.erase(std::remove_if(m_streams.begin(), m_streams.end(), [](auto &s) { return s.unique(); }),
                  m_streams.end());

  m_memUsage += frame->getMemUsage();
  m_frames.push_back(std::move(frame));

  while(m_memUsage > m_maxMemUsage)
  {
    auto invalid = m_frames.begin();

    for(auto &s : m_streams)
    {
      if(s->it == invalid)
        s->it = std::next(invalid);

      if(s->end == invalid)
        s->end = std::next(invalid);
    }

    m_memUsage -= (*invalid)->getMemUsage();
    m_frames.pop_front();
  }
}

FlacFrameStorage::StreamHandle FlacFrameStorage::startStream(FrameId begin, FrameId end)
{
  std::unique_lock<std::mutex> l(m_mutex);

  struct Cmp
  {
    bool operator()(const std::unique_ptr<FlacEncoder::Frame> &frame, FrameId id) const
    {
      return frame->id < id;
    }

    bool operator()(FrameId id, const std::unique_ptr<FlacEncoder::Frame> &frame) const
    {
      return id < frame->id;
    }
  };

  auto beginIt = std::lower_bound(m_frames.begin(), m_frames.end(), begin, Cmp {});
  auto endIt = std::upper_bound(m_frames.begin(), m_frames.end(), end, Cmp {});

  if(end <= begin)
    endIt = beginIt;

  auto stream = std::make_shared<Stream>(this, beginIt, endIt);
  m_streams.push_back(stream);
  return m_streams.back();
}

nlohmann::json FlacFrameStorage::generateInfo()
{
  std::unique_lock<std::mutex> l(m_mutex);

  if(m_frames.empty())
    return {};

  nlohmann::json memUsage = { { "current", m_memUsage }, { "max", m_maxMemUsage } };

  return { { "memUsage", memUsage },
           { "first", m_frames.front()->generateInfo() },
           { "last", m_frames.back()->generateInfo() } };
}

const FlacFrameStorage::Frames &FlacFrameStorage::getFrames() const
{
  return m_frames;
}

const std::vector<std::unique_ptr<FlacEncoder::Frame> > &FlacFrameStorage::getHeaders() const
{
  std::unique_lock<std::mutex> l(m_mutex);
  return m_header;
}

nlohmann::json FlacFrameStorage::reset()
{
  std::unique_lock<std::mutex> l(m_mutex);

  m_streams.erase(std::remove_if(m_streams.begin(), m_streams.end(), [](auto &s) { return s.unique(); }),
                  m_streams.end());

  m_memUsage = 0;
  m_frames.clear();

  for(auto &s : m_streams)
  {
    s->it = m_frames.end();
    s->end = m_frames.end();
  }

  return {};
}

FlacFrameStorage::Stream::Stream(FlacFrameStorage *s, Frames::const_iterator begin, Frames::const_iterator end)
    : storage(s)
    , it(begin)
    , end(end)
{
}

bool FlacFrameStorage::Stream::eos() const
{
  std::unique_lock<std::mutex> l(storage->m_mutex);
  return storage->m_frames.end() == it || it == end;
}

bool FlacFrameStorage::Stream::next(std::function<void(const FlacEncoder::Frame &, bool last)> cb)
{
  std::unique_lock<std::mutex> l(storage->m_mutex);

  if(it == end)
    return false;

  auto frame = *(it++)->get();
  cb(frame, it == end);
  return true;
}

std::vector<std::unique_ptr<FlacEncoder::Frame> > FlacFrameStorage::Stream::getHeaders() const
{
  std::vector<std::unique_ptr<FlacEncoder::Frame> > tgt;

  for(auto &s : storage->getHeaders())
    tgt.push_back(std::make_unique<FlacEncoder::Frame>(*s));

  auto dist = std::distance(it, end);
  auto numSamples = dist * FlacEncoder::flacFrameSize;

  assert(tgt[0]->buffer.size() == 4);   // exepected to be "fLaC";
  assert(tgt[1]->buffer.size() >= 22);  // exepected to have enough space for STREAMINFO;

  Bitstream bits(tgt[1]->buffer);

  // see https://xiph.org/flac/format.html#metadata_block_streaminfo
  // METADATA_BLOCK_HEADER
  bits.seek(140);
  bits.patch(36, numSamples);

#if DEV_PC
  Bitstream test(tgt[1]->buffer);
  assert(test.read(1) == 0);
  assert(test.read(7) == 0);
  test.read(24);

  // METADATA_BLOCK_STREAMINFO
  test.read(16);
  test.read(16);
  test.read(24);
  test.read(24);
  assert(test.read(20) == 48000);
  assert(test.read(3) == 1);
  assert(test.read(5) == 23);
  assert(test.read(36) == numSamples);
#endif

  return tgt;
}

bool FlacFrameStorage::Stream::getFirstAndLast(
    std::function<void(const FlacEncoder::Frame &, const FlacEncoder::Frame &)> cb)
{
  std::unique_lock<std::mutex> l(storage->m_mutex);
  if(it != end)
  {
    cb(*(it)->get(), *(std::prev(end))->get());
    return true;
  }
  return false;
}
