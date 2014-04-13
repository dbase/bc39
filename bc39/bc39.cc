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

#include "bc39.h"

#include <sstream>

namespace bc39 {

bool Bitmap::resize(int width, int height)
{
  m_width = width;
  m_height = height;
  m_data.clear();

  if (m_width > 0 && m_height > 0) {
    std::stringstream stream;

    //- header
    stream << "/* XPM */\n";
    stream << "static char *[] = {\n";
    stream << "\"" << m_width << " " << m_height << " 2 1\",\n";
    stream << "\"* c #000000\",\n";
    stream << "\". c #ffffff\",\n";

    //- data
    m_offset = stream.str().size();        //- offset of image data within XPM
    string whiteRaw(m_width, '.');
    for (int i = 0; i < m_height - 1; i++)
      stream << '"' << whiteRaw << "\",\n";
    stream << '"' << whiteRaw << "\"\n";   //- last raw without trailing ','

    //- footer
    stream << "}\n";

    m_data = stream.str();
  }

  return !m_data.empty();
}

void Bitmap::drawBar(int x, int width)
{
  //- validate parameters
  if (x < 0)
    x = 0;
  if (x >= m_width)
    x = m_width - 1;
  if (x + width > m_width)
    width = m_width - x;

  //- draw it
  for (int y = 0; y < m_height; ++y)
    drawLine(x, y, width);
}

void Bitmap::drawLine(int x, int y, int length)
{
  int begin = offset(x, y);
  int end = begin + length;
  for (int i = begin; i < end; ++i)
    m_data[i] = '*';
}

double Generator::s_w2nr = 3.0f;

void Generator::setWideToNarrowRatio(double ratio)
{
  if (ratio < 2.0)
    ratio = 2.0;
  if (ratio > 3.0)
    ratio = 3.0;
  s_w2nr = ratio;
}

int Generator::generate(const string &text, int height, int narrowWidth)
{
  if (height < 1 || narrowWidth < 1)
    return kBadInput;

  m_narrowWidth = narrowWidth;
  m_wideWidth = m_narrowWidth * wideToNarrowRatio();

  if (!prepareBitmap(text, height))
    return kOutOfMemory;

  m_penPos = m_narrowWidth;     //- margin
  drawChar('*');                //- start delimiter
  m_penPos += m_narrowWidth;    //- gap
  for (unsigned i = 0; i < text.size(); i++) {
    if (!drawChar(text[i]))
      return kInvalidCharacter;
    m_penPos += m_narrowWidth;  //- gap
  }
  drawChar('*');                //- stop delimiter

  return kSuccess;
}

const char *Generator::encode(char ch)
{
  switch (ch) {
  case '0':
    return "NnNwWnWnN";
  case '1':
    return "WnNwNnNnW";
  case '2':
    return "NnWwNnNnW";
  case '3':
    return "WnWwNnNnN";
  case '4':
    return "NnNwWnNnW";
  case '5':
    return "WnNwWnNnN";
  case '6':
    return "NnWwWnNnN";
  case '7':
    return "NnNwNnWnW";
  case '8':
    return "WnNwNnWnN";
  case '9':
    return "NnWwNnWnN";
  case 'A':
    return "WnNnNwNnW";
  case 'B':
    return "NnWnNwNnW";
  case 'C':
    return "WnWnNwNnN";
  case 'D':
    return "NnNnWwNnW";
  case 'E':
    return "WnNnWwNnN";
  case 'F':
    return "NnWnWwNnN";
  case 'G':
    return "NnNnNwWnW";
  case 'H':
    return "WnNnNwWnN";
  case 'I':
    return "NnWnNwWnN";
  case 'J':
    return "NnNnWwWnN";
  case 'K':
    return "WnNnNnNwW";
  case 'L':
    return "NnWnNnNwW";
  case 'M':
    return "WnWnNnNwN";
  case 'N':
    return "NnNnWnNwW";
  case 'O':
    return "WnNnWnNwN";
  case 'P':
    return "NnWnWnNwN";
  case 'Q':
    return "NnNnNnWwW";
  case 'R':
    return "WnNnNnWwN";
  case 'S':
    return "NnWnNnWwN";
  case 'T':
    return "NnNnWnWwN";
  case 'U':
    return "WwNnNnNnW";
  case 'V':
    return "NwWnNnNnW";
  case 'W':
    return "WwWnNnNnN";
  case 'X':
    return "NwNnWnNnW";
  case 'Y':
    return "WwNnWnNnN";
  case 'Z':
    return "NwWnWnNnN";
  case '-':
    return "NwNnNnWnW";
  case '.':
    return "WwNnNnWnN";
  case ' ':
    return "NwWnNnWnN";
  case '$':
    return "NwNwNwNnN";
  case '/':
    return "NwNwNnNwN";
  case '+':
    return "NwNnNwNwN";
  case '%':
    return "NnNwNwNwN";
  case '*':
    return "NwNnWnWnN";
  }
  return NULL;
}

bool Generator::prepareBitmap(const std::string &text, int height)
{
  const int kNumNarrowsInChar = 6;
  const int kNumWidesInChar   = 3;

  int numChars = text.size() + 2;       //- + two stop chars
  int numGaps = numChars - 1;

  int numNarrows = numChars * kNumNarrowsInChar + numGaps;
  int numWides = numChars * kNumWidesInChar;

  int width = numNarrows * m_narrowWidth +
              numWides * m_wideWidth +
              numGaps * m_narrowWidth +
              2 * m_narrowWidth;        //- margin

  return m_bitmap.resize(width, height);
}

bool Generator::drawChar(char ch)
{
  const char *code = encode(toupper(ch));
  if (code == NULL)
    return false;

  for (int i = 0; i <= 9; i++) {
    switch (code[i]) {
    case 'W':
      m_bitmap.drawBar(m_penPos, m_wideWidth);
      break;
    case 'N':
      m_bitmap.drawBar(m_penPos, m_narrowWidth);
      break;
    }
    m_penPos += code[i] == 'W' || code[i] == 'w' ? m_wideWidth : m_narrowWidth;
  }
  return true;
}

} //- namespace bc39
