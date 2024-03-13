struct bitmap {
  uint8_t *map;
  uint16_t size;
}

struct bitmap *
newBitmap(uint16_t size) {
  struct bitmap *bm = (struct bitmap *)malloc(sizeof(struct bitmap));
  bm->map = (uint8_t *)calloc(size);
  bm->size = size;
  return bm;
};

// Flip bit from 0 to 1
bool flipBit(struct bitmap *bm, uint16_t index) {
  if (bm == NULL || index > bm->size) return false;
  bm[index / 16] |= (1 << (index % 16));
  return true;
}

bool checkBit(struct bitmap *bm, uint16_t index) {
  if (bm == NULL || index > bm->size) return false;
  return bm[index / 16] &= ~(1 << (index % 16));
}

// Uses code from https://stackoverflow.com/questions/1225998/what-is-a-bitmap-in-c