if(NOT EXISTS "D:/大學/政大 課程/大一下/物件導向程式設計_紀明德/final_project/raylib-dev-template/build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: D:/大學/政大 課程/大一下/物件導向程式設計_紀明德/final_project/raylib-dev-template/build/install_manifest.txt")
endif()

file(READ "D:/大學/政大 課程/大一下/物件導向程式設計_紀明德/final_project/raylib-dev-template/build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
  if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "C:/msys64/mingw64/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
    endif()
  else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
  endif()
endforeach()
