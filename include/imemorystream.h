#include <istream>
#include <streambuf>

class memoryreadbuf : public std::streambuf
{
public:
  memoryreadbuf(const char* buffer, size_t length)
   : _begin(buffer), _end(buffer + length), _current(buffer)
  {
  }
  memoryreadbuf(const memoryreadbuf&) = delete;
  memoryreadbuf(memoryreadbuf&&) = delete;
  memoryreadbuf& operator=(const memoryreadbuf &) = delete;
  memoryreadbuf& operator=(memoryreadbuf &&) = delete;

private:
  int_type underflow()
  {
    return  (_current == _end) ? traits_type::eof() :  traits_type::to_int_type(*_current);
  }

  int_type uflow()
  {
    if (_current == _end)
    {
      return traits_type::eof();
    }

    return traits_type::to_int_type(*_current++);
  }

  int_type pbackfail(int_type ch)
  {
    if (_current == _begin || (ch != traits_type::eof() && ch != _current[-1]))
    {
      return traits_type::eof();
    }

    return traits_type::to_int_type(*--_current);
  }

  std::streamsize showmanyc()
  {
    return _end - _current;
  }

  pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode = std::ios_base::in | std::ios_base::out)
  {
    switch (dir)
    {
    case std::ios::beg:
      break;
    case std::ios::cur:
      off += (_current - _begin);
      break;
    case std::ios::end:
      off += (_end - _begin);
      break;
    default:
      off = -1;
      break;
    }
    if (0 <= off && off <= (_end - _begin))
    {
      _current = _begin + off;
    }
    else
    {
      off = -1;
    }
    return std::streampos(off);
  }

  pos_type seekpos(pos_type pos, std::ios_base::openmode = std::ios_base::in | std::ios_base::out)
  {
    return seekoff(pos, std::ios::beg);
  }


  const char* const _begin;
  const char* const _end;
  const char* _current;
};

class imemorystream : public std::istream
{
public:
  imemorystream(const char* buffer, size_t length)
   : std::istream(new memoryreadbuf(buffer, length))
   , _buffer(buffer), _length(length)
  {

  }
protected:
  const char* _buffer;
  size_t _length;
};