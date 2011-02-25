/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics

 */

#include <SD.h>


File::File(SdFile f, char *n) {
  _file = f;
  strncpy(_name, n, 12);
  _name[12] = 0;
  _c = -1;

  /*
  Serial.print("Created file object \"");
  Serial.print(n);
  Serial.println("\"");
  */
}

File::File(void) {
  _c = -1;
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
  return _file.isDir();
}

void File::write(uint8_t val) {
  _file.write(val);
}

void File::write(const char *str) {
  _file.write(str);
}

void File::write(const uint8_t *buf, size_t size) {
  _file.write(buf, size);
}

int File::peek() {
  if (_c != -1) return _c;
  _c = _file.read();
  return _c;
}

boolean File::seekSet(uint32_t pos) {
  return _file.seekSet(pos);
}

uint32_t File::size(void) {
  return _file.fileSize();
}


int File::read() {
  if (_c != -1) {
    int tmp = _c;
    _c = -1;
    return tmp;
  }
  return _file.read();
}

int File::read(void *buf, uint16_t nbyte) {
  if ((_c != -1) && (nbyte > 0)) {
    ((char *)buf)[0] = SD.c;
    _c = -1;
    return _file.read(((char *)buf)+1, nbyte-1);
  }
  return _file.read(buf, nbyte);
}

int File::available() {
  if (_c != -1) return 1;
  _c = _file.read();
  return _c != -1;
}

void File::flush() {
  _file.sync();
}

void File::close() {
  _file.close();
}

File::operator bool() {
  return _file.isOpen();
}
