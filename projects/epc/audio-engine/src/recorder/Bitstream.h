#pragma once

#include <vector>
#include <stdint.h>

struct Bitstream
{
  Bitstream(std::vector<uint8_t> &d);

  void seek(uint64_t toBit);
  uint64_t read(uint32_t numBits);
  void patch(uint32_t numBits, uint64_t num);

 private:
  uint64_t readBit();
  void patchBit(uint32_t i);

  std::vector<uint8_t> &data;
  uint32_t pos = 0;
}; 