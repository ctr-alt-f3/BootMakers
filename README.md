# BootMakers
A native (IMAGE_SUBSYSTEM_NATIVE) Windows application that displays a line from Witkacy's "Szewcy" on the screen before the OS fully loads, advancing with every reboot. In that way you can read the entire book in only ~500 boots.


1. Place your compiled native executable in a location of your choice. 
2. Ensure your text file and state file are placed at the paths configured in source code.
3. Open the Registry Editor.
4. Navigate to:
   `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Session Manager`
5. Open the `SetupExecute` multi-string value (`REG_MULTI_SZ`).
6. Add the name of your compiled executable on a new line below the existing entries.
7. Reboot your computer to run the application.
