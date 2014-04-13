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
     * Draws a black horizontal line.
     * @param x starting x
     * @param y starting y
     * @param length line length
     */
    void drawLine(int x, int y, int length);

    /**
     * Draws a black rectangle at given position, its height == m_height
     * @param x rectangle's left position
     * @param width
     */
    void drawBar(int x, int width);

    /**
     * @brief Calculates the offset of given point in the XPM data
     */
    int offset(int x, int y) const
    {
      return
          m_offset +            //- skip the header
          (m_width + 4) * y +   //- leading '"', trailing '",\n'
          x + 1;                //- leading '"'
    }

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
      kBadInput,
      kInvalidCharacter,
      kOutOfMemory
    };

    /**
     * Ratio between wide and narrow bars' width, must be in range [2, 3]
     */
    static double wideToNarrowRatio() { return s_w2nr; }
    static void setWideToNarrowRatio(double ratio);

    const Bitmap &bitmap() const { return m_bitmap; }

    /**
     * Generetes a barcode for the given text. Generated barcode can be verified
     * using any online tool (e.g. http://www.onlinebarcodereader.com/).
     * @return 0 on success, see Result enum for all possible values
     */
    int generate(const string &text, //!< text to encode
                 int height,         //!< required bitmap height
                 int narrowWidth     //!< width of narrow bars
                 );

  private:
    static double s_w2nr;

    /**
     * Converts input in Code39 code in Format1 representation
     * (refer to http://en.wikipedia.org/wiki/Code_39#Encoding)
     */
    static const char *encode(char ch);

    bool prepareBitmap(const string &text, int height);
    bool drawChar(char ch);

    int m_narrowWidth, m_wideWidth, m_penPos;
    Bitmap m_bitmap;
};

} //- namespace bc39
