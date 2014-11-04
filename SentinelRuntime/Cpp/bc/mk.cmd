@rem
@rem build libhasp_cpp_windows_bcc.lib
@rem

@if exist *.obj del *.obj
@if exist libhasp_cpp_windows_bcc.lib del libhasp_cpp_windows_bcc.lib

bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspbase.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspdiag.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspfeature.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspfile.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\hasphandle.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\hasphasp.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspimpl.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspinfo.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\hasplegacy.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\hasplock.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspmain.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\haspmap.cpp 
@if errorlevel 1 pause
bcc32  /c /DWIN32 /DNDEBUG /D_CONSOLE /D_MBCS /x /W /I..\..\..\..\inc  ..\hasptime.cpp 
@if errorlevel 1 pause

tlib libhasp_cpp_windows_bcc.lib +haspbase.obj +haspdiag.obj +haspfeature.obj + haspfile.obj +hasphandle.obj +hasphasp.obj +haspimpl.obj +haspinfo.obj +hasplegacy.obj +hasplock.obj +haspmain.obj +haspmap.obj +hasptime.obj
