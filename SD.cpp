/*

 SD - a slightly more friendly wrapper for sdfatlib

 This library aims to expose a subset of SD card functionality
 in the form of a higher level "wrapper" object.

 License: GNU General Public License V3
          (Because sdfatlib is licensed with this.)

 (C) Copyright 2010 SparkFun Electronics


 This library provides four key benefits:

   * Including `SD.h` automatically creates a global
     `SD` object which can be interacted with in a similar
     manner to other standard global objects like `Serial` and `Ethernet`.

   * Boilerplate initialisation code is contained in one method named 
     `begin` and no further objects need to be created in order to access
     the SD card.

   * Calls to `open` can supply a full path name including parent 
     directories which simplifies interacting with files in subdirectories.

   * Utility methods are provided to determine whether a file exists
     and to create a directory heirarchy.


  Note however that not all functionality provided by the underlying
  sdfatlib library is exposed.

 */

/* Implementation of class used to create `SDCard` object. */

#include "SD.h"

boolean SDClass::begin(uint8_t csPin) {
  /*

    Performs the initialisation required by the sdfatlib library.

    Return true if initialization succeeds, false otherwise.

   */
  return card.init(SPI_HALF_SPEED, csPin) &&
         volume.init(card) &&
         root.openRoot(volume);
}

// this little helper is used to traverse paths
SdFile SDClass::getParentDir(char *filepath, int *index) {
  // get parent directory
  SdFile d1 = root; // start with the mostparent, root!
  SdFile d2;

  // we'll use the pointers to swap between the two objects
  SdFile *parent = &d1;
  SdFile *subdir = &d2;
  
  char *origpath = filepath;

  while (strchr(filepath, '/')) {

    // get rid of leading /'s
    if (filepath[0] == '/') {
      filepath++;
      continue;
    }
    
    if (! strchr(filepath, '/')) {
      // it was in the root directory, so leave now
      break;
    }

    // extract just the name of the next subdirectory
    uint8_t idx = strchr(filepath, '/') - filepath;
    char subdirname[12];
    strncpy(subdirname, filepath, idx);
    subdirname[idx] = 0;

    // close the subdir (we reuse them) if open
    subdir->close();
    if (! subdir->open(parent, subdirname, O_READ)) {
      // failed to open one of the subdirectories
      return SdFile();
    }
    // move forward to the next subdirectory
    filepath += idx;

    // we reuse the objects, close it.
    parent->close();

    // swap the pointers
    SdFile *t = parent;
    parent = subdir;
    subdir = t;
  }

  *index = (int)(filepath - origpath);
  // parent is now the parent diretory of the file!
  return *parent;
}

File SDClass::open(char *filepath, uint8_t mode) {
  /*

     Open the supplied file path for reading or writing.

     The file content can be accessed via the `file` property of
     the `SDClass` object--this property is currently
     a standard `SdFile` object from `sdfatlib`.

     Defaults to read only.

     If `write` is true, default action (when `append` is true) is to
     append data to the end of the file.

     If `append` is false then the file will be truncated first.

     If the file does not exist and it is opened for writing the file
     will be created.

     An attempt to open a file for reading that does not exist is an
     error.

   */

  int pathidx;

  // do the interative search
  SdFile parentdir = getParentDir(filepath, &pathidx);
  // no more subdirs!

  filepath += pathidx;

  if (! filepath[0]) {
    // it was the directory itself!
    return File(parentdir, "/");
  }

  // Open the file itself
  SdFile file;
  
  if (!parentdir.isOpen() || ! file.open(parentdir, filepath, mode)) {
    // failed to open the final file itself or one of the subdirs
    return File();
  }
  parentdir.close();
  return File(file, filepath);
}


//boolean SDClass::close() {
//  /*
//
//    Closes the file opened by the `open` method.
//
//   */
//  file.close();
//}


boolean SDClass::exists(char *filepath) {
  /*

     Returns true if the supplied file path exists.

   */

  File tester = open(filepath, O_RDONLY);
  if (tester) {
    tester.close();
    return true;
  }
  return false;
}


//boolean SDClass::exists(char *filepath, SdFile& parentDir) {
//  /*
//
//     Returns true if the supplied file path rooted at `parentDir`
//     exists.
//
//   */
//  return walkPath(filepath, parentDir, callback_pathExists);
//}


boolean SDClass::mkdir(char *filepath) {
  /*
  
    Makes a single directory or a heirarchy of directories.

    A rough equivalent to `mkdir -p`.
  
   */

  /*
  int pathidx;

  // do the interative search
  SdFile parentdir = getParentDir(filepath, &pathidx);
  // no more subdirs!

  filepath += pathidx;

  if (! filepath[0]) {
    // it was the directory itself!
    return File(parentdir, "/");
  }

  // Open the file itself
  SdFile file;
  
  if (!parentdir.isOpen() || ! file.open(parentdir, filepath, mode)) {
    // failed to open the final file itself or one of the subdirs
    return File();
  }
  parentdir.close();
  return File(file, filepath);
  // return walkPath(filepath, root, callback_makeDirPath);
  */
}

boolean SDClass::rmdir(char *filepath) {
  /*
  
    Makes a single directory or a heirarchy of directories.

    A rough equivalent to `mkdir -p`.
  
   */
  //return walkPath(filepath, root, callback_rmdir);
}

boolean SDClass::remove(char *filepath) {
  //return walkPath(filepath, root, callback_remove);
}


File File::openNextFile(uint8_t mode) {
  dir_t p;

  //Serial.print("\t\treading dir...");
  while (_file.readDir(&p) > 0) {

    // done if past last used entry
    if (p.name[0] == DIR_NAME_FREE) {
      //Serial.println("end");
      return File();
    }

    // skip deleted entry and entries for . and  ..
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') {
      //Serial.println("dots");
      continue;
    }

    // only list subdirectories and files
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) {
      //Serial.println("notafile");
      continue;
    }

    // print file name with possible blank fill
    SdFile f;
    char name[12];
    _file.dirName(p, name);
    //Serial.print("try to open file ");
    //Serial.println(name);

    if (f.open(_file, name, mode)) {
      //Serial.println("OK!");
      return File(f, name);    
    } else {
      //Serial.println("ugh");
      return File();
    }
  }

  //Serial.println("nothing");
  return File();
}

void File::rewindDirectory(void) {  
  if (isDirectory())
    _file.rewind();
}

SDClass SD;
