#define ShortName "Pictus"
#define LongName "Pictus Image Viewer"

[Setup]
ArchitecturesAllowed=x86 x64
ArchitecturesInstallIn64BitMode=x64
AppName=Pictus
AppMutex=Local\Pictus Viewer
AppPublisherURL=http://www.poppeman.se/
AppSupportURL=http://www.poppeman.se/pictus
AppUpdatesURL=http://www.poppeman.se/pictus
AppVerName=Pictus
DefaultDirName={pf}\Pictus
MinVersion=0.0,5.01
UninstallDisplayIcon={app}\Pictus.exe
VersionInfoDescription=Pictus Setup
OutputBaseFilename=PictInst
OutputDir=../../dist/
DisableProgramGroupPage=yes
VersionInfoVersion=1.4.0.0
DisableDirPage=auto

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"; LicenseFile: "license_en.txt"
Name: sv; MessagesFile: "compiler:Languages\Swedish.isl"; LicenseFile: "license_sv.txt"

[CustomMessages]
en.ExplorerIntegration=Show thumbnails in Explorer
sv.ExplorerIntegration=Visa tummnaglar i Utforskaren

[Components]
Name: "integration"; Description: "{cm:ExplorerIntegration}"; Types: full custom

[Files]
Source: "..\x64\release\Pictus.exe"; DestDir: "{app}"; Flags: replacesameversion; Check: Is64BitInstallMode
Source: "..\release\Pictus.exe"; DestDir: "{app}"; Flags: replacesameversion; Check: not Is64BitInstallMode
Source: "..\x64\release\PictThumbs.dll"; DestDir: "{app}"; Flags: replacesameversion regserver; Check: Is64BitInstallMode; Components: integration
Source: "..\release\PictThumbs.dll"; DestDir: "{app}"; Flags: replacesameversion regserver; Check: not Is64BitInstallMode; Components: integration
Source: "..\..\Readme.txt"; DestDir: "{app}"; Flags: replacesameversion

[Icons]
Name: "{commonprograms}\Pictus"; Filename: "{app}\Pictus.exe"

[Tasks]
Name: "assoc_bmp"; Description: "{cm:AssocFileExtension,{#ShortName},.bmp}"
Name: "assoc_gif"; Description: "{cm:AssocFileExtension,{#ShortName},.gif}"
Name: "assoc_jpg"; Description: "{cm:AssocFileExtension,{#ShortName},.jpg}"
Name: "assoc_pcx"; Description: "{cm:AssocFileExtension,{#ShortName},.pcx}"
Name: "assoc_png"; Description: "{cm:AssocFileExtension,{#ShortName},.png}"
Name: "assoc_psd"; Description: "{cm:AssocFileExtension,{#ShortName},.psd}"; Flags: unchecked
Name: "assoc_psp"; Description: "{cm:AssocFileExtension,{#ShortName},.psp}"; Flags: unchecked
Name: "assoc_tga"; Description: "{cm:AssocFileExtension,{#ShortName},.tga}"
Name: "assoc_tif"; Description: "{cm:AssocFileExtension,{#ShortName},.tiff}"
Name: "assoc_wbmp"; Description: "{cm:AssocFileExtension,{#ShortName},.wbmp}"
Name: "assoc_xyz"; Description: "{cm:AssocFileExtension,{#ShortName},.xyz}"


[Registry]
Root: HKLM; Subkey: "SOFTWARE\Poppeman"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities"; ValueType: string; ValueName: "ApplicationDescription"; ValueData: "Pictus is a fast, lightweight image viewer."
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities"; ValueType: string; ValueName: "ApplicationName"; ValueData: "{#LongName}"
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".bmp"; ValueData: "Pictus.BMP"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".gif"; ValueData: "Pictus.GIF"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".jfif"; ValueData: "Pictus.JPG"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".jpe"; ValueData: "Pictus.JPG"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".jpeg"; ValueData: "Pictus.JPG"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".jpg"; ValueData: "Pictus.JPG"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".pcx"; ValueData: "Pictus.PCX"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".png"; ValueData: "Pictus.PNG"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".psd"; ValueData: "Pictus.PSD"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".psp"; ValueData: "Pictus.PSP"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".pspimage"; ValueData: "Pictus.PSP"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".tga"; ValueData: "Pictus.TGA"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".tif"; ValueData: "Pictus.TIF"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".tiff"; ValueData: "Pictus.TIF"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".wbm"; ValueData: "Pictus.WBMP"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".wbmp"; ValueData: "Pictus.WBMP"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".xif"; ValueData: "Pictus.TIF"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\Poppeman\Pictus\Capabilities\FileAssociations"; ValueType: string; ValueName: ".xyz"; ValueData: "Pictus.XYZ"; Flags: uninsdeletekey
Root: HKLM; Subkey: "SOFTWARE\RegisteredApplications"; ValueType: string; ValueName: "Pictus Image Viewer"; ValueData: "SOFTWARE\Poppeman\Pictus\Capabilities"; Flags: uninsdeletevalue

Root: HKCR; Subkey: "Pictus.BMP"; ValueType: string; ValueData: "Bitmap image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.BMP\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.GIF"; ValueType: string; ValueData: "GIF image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.GIF\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.JPG"; ValueType: string; ValueData: "Jpeg image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.JPG\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PCX"; ValueType: string; ValueData: "PCX image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PCX\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PNG"; ValueType: string; ValueData: "PNG image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PNG\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PSD"; ValueType: string; ValueData: "Photoshop image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PSD\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PSP"; ValueType: string; ValueData: "Paint Shop Pro image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.PSP\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.TGA"; ValueType: string; ValueData: "Targa TGA image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.TGA\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.TIF"; ValueType: string; ValueData: "TIFF image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.TIF\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.WBMP"; ValueType: string; ValueData: "Wireless bitmap image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.WBMP\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.XYZ"; ValueType: string; ValueData: "XYZ image"; Flags: uninsdeletekey
Root: HKCR; Subkey: "Pictus.XYZ\shell\open\command"; ValueType: string; ValueData: """{app}\Pictus.exe"" ""%1"""; Flags: uninsdeletekey

Root: HKCR; Subkey: ".bmp"; ValueType: String; ValueData: "Pictus.BMP"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_bmp
Root: HKCR; Subkey: ".gif"; ValueType: String; ValueData: "Pictus.GIF"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_gif
Root: HKCR; Subkey: ".jfif"; ValueType: String; ValueData: "Pictus.JPG"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_jpg
Root: HKCR; Subkey: ".jpe"; ValueType: String; ValueData: "Pictus.JPG"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_jpg
Root: HKCR; Subkey: ".jpeg"; ValueType: String; ValueData: "Pictus.JPG"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_jpg
Root: HKCR; Subkey: ".jpg"; ValueType: String; ValueData: "Pictus.JPG"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_jpg
Root: HKCR; Subkey: ".pcx"; ValueType: String; ValueData: "Pictus.PCX"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_pcx
Root: HKCR; Subkey: ".png"; ValueType: String; ValueData: "Pictus.PNG"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_png
Root: HKCR; Subkey: ".psd"; ValueType: String; ValueData: "Pictus.PSD"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_psd
Root: HKCR; Subkey: ".psp"; ValueType: String; ValueData: "Pictus.PSP"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_psp
Root: HKCR; Subkey: ".pspimage"; ValueType: String; ValueData: "Pictus.PSP"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_psp
Root: HKCR; Subkey: ".tga"; ValueType: String; ValueData: "Pictus.TGA"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_tga
Root: HKCR; Subkey: ".tif"; ValueType: String; ValueData: "Pictus.TIF"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_tif
Root: HKCR; Subkey: ".tiff"; ValueType: String; ValueData: "Pictus.TIF"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_tif
Root: HKCR; Subkey: ".wbm"; ValueType: String; ValueData: "Pictus.WBMP"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_wbmp
Root: HKCR; Subkey: ".wbmp"; ValueType: String; ValueData: "Pictus.WBMP"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_wbmp
Root: HKCR; Subkey: ".xif"; ValueType: String; ValueData: "Pictus.TIF"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_tif
Root: HKCR; Subkey: ".xyz"; ValueType: String; ValueData: "Pictus.XYZ"; Flags: uninsdeletevalue uninsdeletekeyifempty; Tasks: assoc_xyz

