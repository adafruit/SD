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

  // TODO: Allow for read&write? (Possibly not, as it requires seek.)


  // get parent directory
  SdFile parent;

  //fileOpenMode = mode;
  //walkPath(filepath, root, callback_openPath, this);

  Serial.print("Iterative directory search for ");
  Serial.println(filepath);

  if (strcmp(filepath, "/") == 0) {
    return File(root, "/");
  }
  

  
  while (strchr(filepath, '/')) {
    // get rid of leading /'s
    if (filepath[0] == '/') {
      filepath++;
      continue;
    }
    
    // some sort of subdir TODO
    
  }
  // no more subdirs!


  return File(SdFile(&root, *filepath, mode));

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
  //return walkPath(filepath, root, callback_pathExists);
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
  // return walkPath(filepath, root, callback_makeDirPath);
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


DirectoryEntry File::readNextDirectoryEntry(void) {
  dir_t p;

  while (_file.readDir(&p) > 0) {
    // done if past last used entry
    if (p.name[0] == DIR_NAME_FREE)
      return DirectoryEntry();

    // skip deleted entry and entries for . and  ..
    if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;

    // only list subdirectories and files
    if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;

    // print file name with possible blank fill
    return DirectoryEntry(p.name, p.fileSize, DIR_IS_SUBDIR(&p));
  }

  return DirectoryEntry();
}

void File::rewindDirectory(void) {  
  if (isDirectory())
    _file.rewind();
}

DirectoryEntry::DirectoryEntry(void) {
  _name[0] = 0;
  _filesize = 0;
  _exists = false;
  _isDirectory = false;
}

DirectoryEntry::DirectoryEntry(uint8_t *n, uint32_t s, bool isdir) {
  char *nameptr = _name;
  for (uint8_t i=0; i<11; i++) {
    if (n[i] == ' ')
      continue;
    if (i == 8) {
      nameptr[0] = '.';
      nameptr++;
    }
    nameptr[0] = n[i];
    nameptr++;
  }
  nameptr[0] = 0;
  _filesize = s;

  _exists = true;

  _isDirectory = isdir;
}

SDClass SD;
