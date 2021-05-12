#include "Bitstream.h"

Bitstream::Bitstream(std::vector<uint8_t> &d)
    : data(d)
{
}

void Bitstream::seek(uint64_t toBit)
{
  pos = toBit;
}

uint64_t Bitstream::read(uint32_t numBits)
{
  uint64_t ret = 0;
  for(uint32_t i = 0; i < numBits; i++)
    ret |= readBit() << (numBits - i - 1);

  return ret;
}

void Bitstream::patch(uint32_t numBits, uint64_t num)
{
  for(uint32_t i = 0; i < numBits; i++)
    patchBit((num >> (numBits - i - 1)) & 0x01);
}

uint64_t Bitstream::readBit()
{
  auto byte = pos / 8;
  auto bit = 7 - (pos % 8);
  pos++;
  return (data[byte] >> bit) & 0x01;
}

void Bitstream::patchBit(uint32_t i)
{
  auto byte = pos / 8;
  auto bit = 7 - (pos % 8);
  pos++;
  uint8_t mask = ~(1 << bit);
  data[byte] &= mask;
  data[byte] |= i << bit;
}
