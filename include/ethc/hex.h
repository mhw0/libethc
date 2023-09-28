#ifndef ETHC_HEX_H
#define ETHC_HEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ethc-common.h"
#include <stddef.h>
#include <stdint.h>

/*!
 * @brief Checks whether the given string is hex or not
 *
 * @param[in] str Target string to check
 * @param[in] len Length of the input string (`-1` if the string is NUL-terminated)
 * @return `1` when the input is valid hex, `-1` otherwise
 */
ETHC_EXPORT int eth_is_hex(const char *str, int len);

/*!
 * @brief Left pads the hexadecimal input
 *
 * @param[in] dest String to write the result to
 * @param[in] str Original hexadecimal value
 * @param[in] len Length of the input string (`-1` if the string is NUL-terminated)
 * @param[in] width Total width of the output
 * @return `1` on success, `-1` otherwise
 */
ETHC_EXPORT int eth_hex_pad_left(char *dest, const char *str, int len, size_t width);

/*!
 * @brief Right pads the hexadecimal input
 *
 * @param[in] dest String to write the result to
 * @param[in] str Original hexadecimal value
 * @param[in] len Length of the input string (`-1` if the string is NUL-terminated)
 * @param[in] width Total width of the output
 * @return `1` on success, `-1` otherwise
 */
ETHC_EXPORT int eth_hex_pad_right(char *dest, const char *str, int len, size_t width);

/*!
 * @brief Converts bytes to hexadecimal string
 *
 * @param[in] dest String to write the result to
 * @param[in] bytes Bytes to read the data from
 * @param[in] len Length of the input bytes
 * @return `1` on success, `-1` otherwise
 */
ETHC_EXPORT int eth_hex_from_bytes(char **dest, const uint8_t *bytes, size_t len);

/*!
 * @brief Converts hexadecimal string to bytes
 *
 * @param[in] dest Bytes to write the data to
 * @param[in] hex Hexadecimal string to read the data from
 * @param[in] hlen Length of hexadecimal input string
 * @return `1` on success, `-1` otherwise
 */
ETHC_EXPORT int eth_hex_to_bytes(uint8_t **dest, const char *hex, int hlen);

#ifdef __cplusplus
}
#endif

#endif
