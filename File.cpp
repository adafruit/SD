/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics
     Multi-file support and some extras added by Limor Fried

 */

#include <SD.h>

/* for debugging file open/close leaks
   uint8_t nfilecount=0;
*/

File::File(SdFile f, char *n) {
  // oh man you are kidding me, new() doesnt exist? Ok we do it by hand!
  _file = (SdFile *)malloc(sizeof(SdFile)); 
  if (_file) {
    memcpy(_file, &f, sizeof(SdFile));
    
    strncpy(_name, n, 12);
    _name[12] = 0;
    _c = -1;
    
    /* for debugging file open/close leaks
       nfilecount++;
       Serial.print("Created \"");
       Serial.print(n);
       Serial.print("\": ");
       Serial.println(nfilecount, DEC);
    */
  }
}

File::File(void) {
  _c = -1;
  _file = 0;
  _name[0] = 0;
  //Serial.print("Created empty file object");
}

File::~File(void) {
  /*if (_file) 
    delete _file;
  */

  //  Serial.print("Deleted file object");
}

char *File::name(void) {
  return _name;
};

boolean File::isDirectory(void) {
  return (_file && _file->isDir());
}

void File::write(uint8_t val) {
  if (_file)
    _file->write(val);
}

void File::write(const char *str) {
  if (_file)
    _file->write(str);
}

void File::write(const uint8_t *buf, size_t size) {
  if (_file) 
    _file->write(buf, size);
}

int File::peek() {
  if (!_file) 
    return 0;

  if (_c != -1) 
    return _c;
  _c = _file->read();
  return _c;
}

boolean File::seekSet(uint32_t pos) {
  return (_file && _file->seekSet(pos));
}

uint32_t File::size(void) {
  if (_file)
    return _file->fileSize();
  return 0;
}


int File::read() {
  if (!_file)
    return 0;

  if (_c != -1) {
    int tmp = _c;
    _c = -1;
    return tmp;
  }
  return _file->read();
}

int File::read(void *buf, uint16_t nbyte) {
  if (!_file)
    return 0;

  if ((_c != -1) && (nbyte > 0)) {
    ((char *)buf)[0] = SD.c;
    _c = -1;
    return _file->read(((char *)buf)+1, nbyte-1);
  }
  return _file->read(buf, nbyte);
}

int File::available() {
  if (_c != -1) 
    return 1;
  if (! _file)
    return 0;
  _c = _file->read();
  return _c != -1;
}

void File::flush() {
  if (_file)
    _file->sync();
}

void File::close() {
  if (_file) {
    _file->close();
    free(_file); 
    _file = 0;

    /* for debugging file open/close leaks
    nfilecount--;
    Serial.print("Deleted ");
    Serial.println(nfilecount, DEC);
    */
  }
}

File::operator bool() {
  return (_file && _file->isOpen());
}
