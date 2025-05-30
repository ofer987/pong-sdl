# Pong SDL

Pong written in [C](https://en.wikipedia.org/wiki/C_(programming_language)), but implemented in [SDL](https://en.wikipedia.org/wiki/Simple_DirectMedia_Layer).

![Screenshot](https://raw.githubusercontent.com/ofer987/pong-sdl/refs/heads/master/images/pong_sdl_screenshot.jpg)

## Installation Instructions

According to SDL, it is possible to install on Windows and macOS, https://github.com/libsdl-org/SDL/blob/main/INSTALL.md. But I have only built it for macOS.

1. Follow these instructions, https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-xcode.md
2. But I found that it is better to install SDL as a framework under `/Library/Frameworks/SDL3.framework`
3. Build and run in Xcode. (Personally, I use Xcode 16.2)

## Controls

- Left Player uses the keys **S** and **A** to move Down and Up, respectively.
- Right Player uses the keys **J** and **K** to move Down and Up respectively.
  - Alternatively, feel comfortable to use the arrow keys, **UP** and **DOWN**.

- Press the [any key](https://en.wikipedia.org/wiki/Any_key) to start the Game.
- **P** pauses and unpauses the games.
- **C** to continue after a player wins a round.
- **Q** to quit.

