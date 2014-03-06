# Copyright (c) 2012 NDM Systems, Inc. http://www.ndmsystems.com/
# This software is freely distributable, see COPYING for details.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.

TARGET=pam_ndm.so

HEADERS=$(wildcard *.h)
OBJECTS=$(patsubst %.c,%.o,$(wildcard *.c))

ifneq ($(OPENWRT_BUILD),1)
CFLAGS+=\
	-g3 -pipe -fPIC -std=c99 \
	-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 \
	-D_POSIX_C_SOURCE=199309L -D_XOPEN_SOURCE=600 \
	-ffunction-sections -fdata-sections -Wempty-body \
	-Wall -Winit-self -Wswitch-enum -Wundef -Wunsafe-loop-optimizations \
	-Waddress -Wmissing-field-initializers -Wnormalized=nfkc \
	-Wredundant-decls -Wvla -Wstack-protector -ftabstop=4 -Wshadow \
	-Wpointer-arith -Wtype-limits -Wclobbered
LDFLAGS= -shared -lndm
endif

all: $(TARGET)

$(TARGET): $(OBJECTS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	rm -fv *.o *~ $(TARGET)

distclean: clean
