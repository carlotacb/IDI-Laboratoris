#configuration
CONFIG +=  shared qpa no_mocdepend release qt_no_framework
host_build {
    QT_ARCH = x86_64
    QT_TARGET_ARCH = x86_64
} else {
    QT_ARCH = x86_64
    QMAKE_DEFAULT_LIBDIRS = /usr/lib64/gcc/x86_64-suse-linux/4.8 /usr/lib64 /lib64 /usr/x86_64-suse-linux/lib /lib /usr/lib
    QMAKE_DEFAULT_INCDIRS = /usr/include/c++/4.8 /usr/include/c++/4.8/x86_64-suse-linux /usr/include/c++/4.8/backward /usr/lib64/gcc/x86_64-suse-linux/4.8/include /usr/local/include /usr/lib64/gcc/x86_64-suse-linux/4.8/include-fixed /usr/x86_64-suse-linux/include /usr/include
}
QT_CONFIG +=  minimal-config small-config medium-config large-config full-config gtk2 gtkstyle fontconfig libudev evdev xlib xrender xcb-plugin xcb-render xcb-glx xcb-xlib xcb-sm accessibility-atspi-bridge linuxfb c++11 accessibility egl egl_x11 eglfs opengl shared qpa reduce_exports reduce_relocations clock-gettime clock-monotonic posix_fallocate mremap getaddrinfo ipv6ifname getifaddrs inotify eventfd system-jpeg system-png png system-freetype system-harfbuzz system-zlib mtdev nis cups iconv glib dbus dbus-linked openssl-linked xcb xinput2 alsa pulseaudio icu concurrent audio-backend release

#versioning
QT_VERSION = 5.4.2
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 4
QT_PATCH_VERSION = 2

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 

QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 8
QT_GCC_PATCH_VERSION = 0
