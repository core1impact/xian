host-win = x86_64-w64-mingw32
mingw = $(host-win)-gcc

root-path = $(shell pwd)

mbedtls-path = $(root-path)/external/libs/mbedtls/mbedtls-2.16.3
curl-path = $(root-path)/external/libs/curl/curl-7.66.0
test-path = $(root-path)/external/libs/test

curl-config = CPPFLAGS='-I$(mbedtls-path)/include' \
	--disable-shared --enable-static --without-ssl --with-mbedtls \
	--disable-ftp --disable-file --disable-ldap --disable-ldaps --disable-rtsp --disable-proxy --disable-dict \
	--disable-telnet --disable-tftp --disable-pop3 --disable-imap --disable-smb --disable-smtp --disable-gopher \
	--disable-ares --disable-cookies --disable-crypto-auth --disable-ipv6 --disable-manual --disable-proxy \
	--disable-unix-sockets --disable-verbose --disable-versioned-symbols --enable-hidden-symbols --without-libidn2 \
	--without-librtmp --without-zlib

curl-mbedtls-flags = -I$(curl-path)/include -L$(curl-path)/lib/.libs -L$(mbedtls-path)/library -lcurl -lmbedtls -lmbedcrypto -lmbedx509 -lpthread

curl-mbedtls-prebuilt-flags = -I$(curl-path)/include -lcurl -lmbedtls -lmbedcrypto -lmbedx509 -lpthread

sources-common := main.c common/xor.c common/base64.c common/buffer.c common/strnstr.c modules/sysinfo.c external/base64/base64.c events/handlers/run_handler.c common/network/curl.c modules/exec.c transports/curl_transport.c transports/transport.c events/events.c common/md5.c external/md5/md5.c

sources-nix := modules/nix/sysinfo.c modules/nix/system.c events/handlers/nix/update_handler.c events/handlers/nix/exec_handler.c common/helpers/nix/file.c
sources-win := modules/win/sysinfo.c modules/win/system.c common/helpers/win/registry.c events/handlers/win/update_handler.c events/handlers/win/exec_handler.c common/helpers/win/file.c common/helpers/win/process.c

clean : curl-clean

# Mbed TLS

mbedtls-clean :
	cd $(mbedtls-path) && make clean

mbedtls-win32 :
	cd $(mbedtls-path) && make CC=i686-w64-mingw32-gcc CFLAGS=-O3 WINDOWS=1
	cp $(mbedtls-path)/library/*.a $(root-path)/build/lib/win32

mbedtls-win64 :
	cd $(mbedtls-path) && make CC=x86_64-w64-mingw32-gcc CFLAGS=-O3 WINDOWS=1
	cp $(mbedtls-path)/library/*.a $(root-path)/build/lib/win64

mbedtls-nix32 :
	cd $(mbedtls-path) && make CFLAGS='-m32 -O3'
	cp $(mbedtls-path)/library/*.a $(root-path)/build/lib/nix32

mbedtls-nix64 :
	cd $(mbedtls-path) && make CFLAGS='-m64 -O3'
	cp $(mbedtls-path)/library/*.a $(root-path)/build/lib/nix64

# Curl

curl-clean : mbedtls-clean
	cd $(curl-path) && make clean

curl-win32 : mbedtls-win32
	cd $(curl-path) && ./configure --host=i686-w64-mingw32 $(curl-config) LDFLAGS='-L$(root-path)/build/lib/win32' CFLAGS=-O3 && make
	cp $(curl-path)/lib/.libs/*.a $(root-path)/build/lib/win32

curl-win64 : mbedtls-win64
	cd $(curl-path) && ./configure --host=x86_64-w64-mingw32 $(curl-config) LDFLAGS='-L$(root-path)/build/lib/win64' CFLAGS=-O3 && make
	cp $(curl-path)/lib/.libs/*.a $(root-path)/build/lib/win64

curl-nix32 : mbedtls-nix32
	cd $(curl-path) && ./configure $(curl-config) LDFLAGS='-L$(root-path)/build/lib/nix32' CFLAGS='-m32 -O3' && make
	cp $(curl-path)/lib/.libs/*.a $(root-path)/build/lib/nix32

curl-nix64 : mbedtls-nix64
	cd $(curl-path) && ./configure $(curl-config) LDFLAGS='-L$(root-path)/build/lib/nix64' CFLAGS='-m64 -O3' && make
	cp $(curl-path)/lib/.libs/*.a $(root-path)/build/lib/nix64

# Win32

win32:
	i686-w64-mingw32-gcc $(sources-common) $(sources-win) -o $(root-path)/build/bin/win32.exe -m32 -static -DCURL_STATICLIB $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/win32 -mwindows -lws2_32 -liphlpapi -O3
	i686-w64-mingw32-strip $(root-path)/build/bin/win32.exe

win32-debug:
	i686-w64-mingw32-gcc $(sources-common) $(sources-win) -o $(root-path)/build/bin/win32-debug.exe -m32 -static -DCURL_STATICLIB $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/win32 -mwindows -lws2_32 -liphlpapi -O0 -g

# Win64

win64:
	x86_64-w64-mingw32-gcc $(sources-common) $(sources-win) -o $(root-path)/build/bin/win64.exe -m64 -static -DCURL_STATICLIB $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/win64 -mwindows -lws2_32 -liphlpapi -O3
	x86_64-w64-mingw32-strip $(root-path)/build/bin/win64.exe

win64-debug:
	x86_64-w64-mingw32-gcc $(sources-common) $(sources-win) -o $(root-path)/build/bin/win64-debug.exe -m64 -static -DCURL_STATICLIB $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/win64 -mwindows -lws2_32 -liphlpapi -O0 -g

# Nix32

nix32:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/nix32 -m32 -static $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix32 -O3
	strip $(root-path)/build/bin/nix32

nix32-debug:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/nix32-debug -m32 -static $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix32 -O0 -g

# Nix64

nix64:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/nix64 -m64 -static $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix64 -O3
	strip $(root-path)/build/bin/nix64

nix64-debug:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/nix64-debug -m64 -static $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix64 -O0 -g

# Osx64

osx64:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/osx64 -m64 $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix64 -O3
	strip $(root-path)/build/bin/osx64

osx64-debug:
	gcc $(sources-common) $(sources-nix) -o $(root-path)/build/bin/osx64-debug -m64 $(curl-mbedtls-prebuilt-flags) -L$(root-path)/build/lib/nix64 -O0 -g

download:
	i686-w64-mingw32-gcc downloader/downloader.c -o $(root-path)/build/bin/downloader.exe -mwindows -lurlmon
	i686-w64-mingw32-strip $(root-path)/build/bin/downloader.exe
