#pragma once

/**
 * @file fit_ellipsis.h
 * @brief Pure string truncation with ellipsis for fixed-width display.
 *
 * The device's default GFX font has no single-glyph ellipsis and filters
 * code points below 0x20, so the ellipsis is three ASCII dots "...".
 */

#include <cstddef>
#include <cstring>

namespace adversary {
namespace utils {

/**
 * @brief Copy @p src into @p out, truncating with a trailing "..." when it exceeds @p maxChars.
 *
 * A string that already fits (strlen(src) <= maxChars) is copied verbatim (never cropped).
 * Always writes a NUL-terminated result within @p outLen. Pure (no device deps), so it is
 * unit-testable off-hardware.
 *
 * @param src      Source string (NUL-terminated). nullptr is treated as empty.
 * @param maxChars Maximum characters the result may occupy (including the "..." when truncated).
 * @param out      Output buffer (NUL-terminated).
 * @param outLen   Size of @p out. Must be > 0.
 */
inline void fitEllipsis(const char* src, size_t maxChars, char* out, size_t outLen) {
    if (out == nullptr || outLen == 0) {
        return;
    }
    out[0] = '\0';
    if (src == nullptr || maxChars == 0) {
        return;
    }

    const size_t srcLen = strlen(src);
    if (srcLen <= maxChars) {
        // Fits: copy verbatim, respecting outLen.
        const size_t copyLen = (srcLen < outLen) ? srcLen : (outLen - 1);
        if (copyLen > 0) {
            memcpy(out, src, copyLen);
        }
        out[copyLen] = '\0';
        return;
    }

    // Needs truncation. Ellipsis is "..." (3 chars).
    constexpr size_t kEllipsisLen = 3;
    if (maxChars < kEllipsisLen) {
        // Degrade sanely: copy what fits of the source, still NUL-terminated, no overflow.
        const size_t copyLen = (maxChars < outLen) ? maxChars : (outLen - 1);
        if (copyLen > 0) {
            memcpy(out, src, copyLen);
        }
        out[copyLen] = '\0';
        return;
    }

    // Truncate to maxChars total including "...".
    size_t keep = maxChars - kEllipsisLen;
    size_t totalNeeded = keep + kEllipsisLen;
    if (totalNeeded >= outLen) {
        // Not enough room in the buffer for the full truncated result.
        // Prefer a valid NUL-terminated string that fits.
        const size_t room = outLen - 1;
        if (room <= kEllipsisLen) {
            const size_t copyLen = room;
            if (copyLen > 0) {
                memcpy(out, src, copyLen);
            }
            out[copyLen] = '\0';
            return;
        }
        keep = room - kEllipsisLen;
    }
    if (keep > 0) {
        memcpy(out, src, keep);
    }
    memcpy(out + keep, "...", kEllipsisLen);
    out[keep + kEllipsisLen] = '\0';
}

} // namespace utils
} // namespace adversary
