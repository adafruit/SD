/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics

 */

#include <SD.h>

void File::write(uint8_t val) {
  SD.file.write(val);
}

void File::write(const char *str) {
  SD.file.write(str);
}

void File::write(const uint8_t *buf, size_t size) {
  SD.file.write(buf, size);
}

int File::peek() {
  if (SD.c != -1) return SD.c;
  SD.c = SD.file.read();
  return SD.c;
}

boolean File::seekSet(uint32_t pos) {
  return SD.file.seekSet(pos);
}

uint32_t File::size(void) {
  return SD.file.fileSize();
}


int File::read() {
  if (SD.c != -1) {
    int tmp = SD.c;
    SD.c = -1;
    return tmp;
  }
  return SD.file.read();
}

int File::read(void *buf, uint16_t nbyte) {
  if ((SD.c != -1) && (nbyte > 0)) {
    ((char *)buf)[0] = SD.c;
    SD.c = -1;
    return SD.file.read(((char *)buf)+1, nbyte-1);
  }
  return SD.file.read(buf, nbyte);
}

int File::available() {
  if (SD.c != -1) return 1;
  SD.c = SD.file.read();
  return SD.c != -1;
}

void File::flush() {
  SD.file.sync();
}

void File::close() {
  SD.file.close();
}

File::operator bool() {
  return SD.file.isOpen();
}
