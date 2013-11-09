/* Include within a function. */
union Color { // とりあえずここに仮置き
	ArgbColor rgbaAsOneValue;
#if defined(__BIG_ENDIAN__) || defined(WORDS_BIGENDIAN)
	struct {std::uint8_t a, r, g, b;} rgbaAsStruct;
#else
	struct {std::uint8_t b, g, r, a;} rgbaAsStruct;
#endif
};
static_assert(sizeof (Color) == 4, "union Color is not 32-bit long");
