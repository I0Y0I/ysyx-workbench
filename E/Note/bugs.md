## _GNU_SOURCE重定义

通过`make -n`检查编译项：

```bash
gcc -std=gnu11 -O2 -MMD -Wall -Werror -I/home/ioyoi/Documents/ysyx/am-kernels/tests/am-tests/include -I/home/ioyoi/Documents/ysyx/abstract-machine/am/include/ -I/home/ioyoi/Documents/ysyx/abstract-machine/klib/include/ -D__ISA__=\"native\" -D__ISA_NATIVE__ -D__ARCH__=native -D__ARCH_NATIVE -D__PLATFORM__= -D__PLATFORM_ -DARCH_H=\"arch/native.h\" -fno-asynchronous-unwind-tables -fno-builtin -fno-stack-protector -Wno-main -U_FORTIFY_SOURCE -fvisibility=hidden -fpie -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -c -o /home/ioyoi/Documents/ysyx/am-kernels/tests/am-tests/build/native/src/tests/audio.o /home/ioyoi/Documents/ysyx/am-kernels/tests/am-tests/src/tests/audio.c
```

注意到其中定义了`-D_GNU_SOURCE=1`，这个CFLAGS来自`sdl2-config --cflags`，有可能是由于pacman上的sdl2是基于sdl3的兼容版本。

因此对所有报错文件的头部进行了更改，将

```c
#define _GNU_SOURCE
```

修改为

```c
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif /* ifndef _GNU_SOURCE */
```

## 运行am-tests时无窗口显示

开始时我用默认的wayland sdl驱动，发现窗口不会显示，加入`WAYLAND_DEBUG=1`查看日志，发现运行时会强制设定窗口大小，所以我认为是Hyprland的窗口规则限制了窗口的创建，导致无法正常创建缓冲区

```
[1854599.358] {Default Queue} -> wl_surface#40.attach(nil, 0, 0)
```

为了解决这个问题，我在hypr配置中加入了对id含有`elf`的窗口强制使用浮动模式创建

```
windowrulev2 = float, class:^(.*\.elf)$
```

随后在重启系统的第一次运行中，窗口能够被正常创建，但关闭窗口后无法再次创建窗口。

现在我还不具备解决这个bug的能力，我尝试了回退到x11驱动，窗口可以正常显示，即在系统变量中添加

```fish
set -gx SDL_VIDEODRIVER x11
```

不过使用xwayland的话，会有一些系统快捷键无法被捕获，比如super、被fcitx占用的Caplock、F4等等。



