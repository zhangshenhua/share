"C:\Program Files\MRE SDK v1.0\tools\DllPackage.exe" "C:\Documents and Settings\coanor\Desktop\AnserCygnoides\AnserCygnoides.vcproj"
if %errorlevel% == 0 ( 
 echo postbuild OK.
 exit 0
)else (
  echo postbuild error
  echo error code: %errorlevel%
  exit 1
)
