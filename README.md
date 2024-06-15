# windowToggler
I was initially using Yakuake, a drop-down terminal emulator based on KDE. 
One of the features I particularly enjoyed was the ability to press **F12** to quickly show or hide the terminal.
This was a convenient feature as it allowed me to quickly access the terminal, run a command, 
and then hide it again, all without needing to switch windows or use the mouse. 
However, for reasons of my own, I decided to switch to Kitty terminal.
After switching to Kitty, I found that I missed the F12 show/hide feature from Yakuake.
As a KDE user, I was used to this feature and found it inconvenient to use Kitty without it.
So, I decided to implement a tool to do that.

This is a C program that interacts with the X11 window system to find, list, and toggle visibility of windows
based on their class name. It also has the ability to start a new process if a window of the specified class is not found.

## Features

- **Find Window:** The program can find a window by its class name in the X11 window system.
- **List Windows:** The program can list all windows along with their class names.
- **Toggle Window:** The program can toggle the visibility of a window (hide if it's visible, show if it's hidden).
- **Start Process:** If a window of the specified class is not found, the program can start a new process.

## Usage

The program is run from the command line with the following syntax:

```
windowToggler <window_class> [<path_to_process> [process_args...]]
```
- `<window_class>`: The class name of the window to find and toggle. Use "list" to list all windows.
- `<path_to_process>`: (Optional) The path to the process to start if a window of the specified class is not found.
- `process_args`: (Optional) Any arguments to pass to the process being started.

## Example

To toggle a window with the class name "kitty" and start Kitty if it's not already running, you would use:

```
./windowToggler kitty kitty --single-instance --detach
```

To toggle a Slack and start if it's not already running, you would use:

```
./windowToggler Slack slack
```

## KDE Plasma hotkeys like Yakuake
Steps to Create a Custom Global Shortcut Using Krunner
Open System Settings:
- Access System Settings from the application launcher or by searching for it.

Navigate to Shortcuts:
- Go to "Shortcuts" in the System Settings window.
Create a Custom Shortcut:
- Select "Custom Shortcuts" from the left sidebar.
- Click the "+" button at the bottom and choose "Global Shortcut" -> "Command/URL".
Configure the Shortcut:
- In the "Trigger" tab, set your desired hotkey by clicking on "None" and pressing the key combination you want to use.
- In the "Action" tab, set the Command/URL to the following:
- - ```~/path/to/windowToggler kitty kitty --single-instance --detach```

## Dependencies

This program uses the X11 library for interacting with the X11 window system. Make sure to install the X11 development package (`libx11-dev` on Debian-based systems) before compiling.

## Compilation

You can compile the program using gcc:

```
gcc -o -O2 windowToggler main.c -lX11
```

This will create an executable named `windowToggler`.

## Note

This program is intended for use on Unix-like operating systems with the X11 window system. It has not been tested on other platforms.

