/**************************************************************************
 Copyright (c) 2014, Sergey Kulishenko <serkul(at)ukr(dot)net>

 Permission to use, copy, modify, and/or distribute this software for any
 purpose with or without fee is hereby granted, provided that the above
 copyright notice and this permission notice appear in all copies.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**************************************************************************/

#pragma once

#include <string>

namespace bc39 {

using std::string;

/**
 * Simple monochrome bitmap implementation.
 */
class Bitmap
{
    friend class Generator;

  public:
    /**
     * @return Image in XPM format or NULL if data is invalid
     */
    const char *toXpm() const { return m_data.empty() ? NULL : m_data.c_str(); }


  private:
    Bitmap() {}

    bool resize(int width, int height);

    /**
     * Draws a black rectangle at given position, its height == m_height
     * @param x rectangle's left position
     * @param width
     */
    void drawBar(int x, int width);

    string m_data;
    int m_width, m_height, m_offset;
};

/**
 * @brief The Code 39 generator (refer to
 * http://en.wikipedia.org/wiki/Code_39 for detailed description).
 */
class Generator
{
  public:
    enum Result {
      kSuccess = 0,
      kInvalidCharacter,
      kOutOfMemory
    };

    /**
     * Ratio between wide and narrow bars' width, must be in range [2, 3]
     */
    static double wideToNarrowRatio() { return s_w2nr; }
    static void setWideToNarrowRatio(double ratio);

    Generator()
      : m_height(100),
        m_narrowWidth(3),
        m_wideWidth(m_narrowWidth * wideToNarrowRatio()) {}

    int height() const { return m_height; }
    void setHeight(int height)
    {
      m_height = height < kMinHeight ? kMinHeight : height;
    }

    int narrowWidth() const { return m_narrowWidth; }
    void setNarrowWidth(int narrowWidth)
    {
      m_narrowWidth = narrowWidth < kMinNarrowWidth
                      ? kMinNarrowWidth : narrowWidth;
      m_wideWidth = m_narrowWidth * wideToNarrowRatio();
    }

    const Bitmap &bitmap() const { return m_bitmap; }

    /**
     * Generates a barcode for the given text. Generated barcode can be verified
     * using any online tool (e.g. http://www.onlinebarcodereader.com/).
     */
    int generate(const std::string &text);

    /**
     * @overload
     * Equivalent to subsequent calls of:
     * @code
     * setHeight(height);
     * setNarrowWidth(narrowWidth);
     * generate(text)
     * @endcode
     * @return 0 on success, see Result enum for all possible values
     */
    int generate(const string &text, //!< text to encode
                 int height,         //!< bitmap height, pixels
                 int narrowWidth     //!< width of narrow bars, pixels
                 );

  private:
    static const int kMinHeight = 1;
    static const int kMinNarrowWidth = 1;

    static double s_w2nr;

    /**
     * Converts input in Code39 code in Format1 representation
     * (refer to http://en.wikipedia.org/wiki/Code_39#Encoding)
     */
    static const char *encode(char ch);

    bool prepareBitmap(const string &text);
    bool drawChar(char ch);

    int m_height;
    int m_narrowWidth, m_wideWidth;
    int m_penPos;
    Bitmap m_bitmap;
};

} //- namespace bc39
