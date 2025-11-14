#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), buffer_() {}

void Writer::push( string data )
{
  if ( data.size() == 0 )
    return;
  if ( is_closed() ) {
    set_error();
    return;
  }
  for ( const char& c : data ) {
    if ( byte_buffered_ >= capacity_ )
      break; // stop if no space left
    buffer_.push_back( c );
    ++byte_buffered_;
    ++byte_pushed_;
  }
}

void Writer::close()
{
  closed_ = true;
}

bool Writer::is_closed() const
{
  return closed_;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - byte_buffered_;
}

uint64_t Writer::bytes_pushed() const
{
  return byte_pushed_;
}

string_view Reader::peek() const
{
  if ( byte_buffered_ == 0 )
    return {};
  return string_view( &buffer_.front(), buffer_.size() );
}

void Reader::pop( uint64_t len )
{
  if ( is_finished() || len > byte_buffered_ ) {
    set_error();
    return;
  }
  buffer_.erase( buffer_.begin(), buffer_.begin() + len );
  byte_popped_ += len;
  byte_buffered_ -= len;
}

bool Reader::is_finished() const
{
  return closed_ && ( byte_buffered_ == 0 );
}

uint64_t Reader::bytes_buffered() const
{
  return byte_buffered_;
}

uint64_t Reader::bytes_popped() const
{
  return byte_popped_;
}
