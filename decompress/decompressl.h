// decompressl.h

#ifndef UNCOMPRESS_H
#define UNCOMPRESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize the decompression module.
 * This function must be called before any other functions in this module.
 */
void uncompress_init();

/**
 * Decompress a compressed InnoDB page.
 *
 * @param compressed_page_data Pointer to the compressed page data (8KB).
 * @param decompressed_page_data Pointer to a buffer where decompressed data will be stored (16KB).
 * @return 0 on success, non-zero on failure.
 */
int decompress_page(const unsigned char *compressed_page_data, unsigned char *decompressed_page_data);

#ifdef __cplusplus
}
#endif

#endif // UNCOMPRESS_H
