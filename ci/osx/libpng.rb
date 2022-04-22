# BSD 2-Clause License
#
# Copyright (c) 2009-present, Homebrew contributors
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Derived from https://github.com/Homebrew/homebrew-core/blob/c3005baf36a2f8677073a8acb1d71bf8de4e0d77/Formula/libpng.rb
class Libpng < Formula
  desc "Library for manipulating PNG images"
  homepage "http://www.libpng.org/pub/png/libpng.html"
  url "https://downloads.sourceforge.net/project/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz"
  mirror "https://sourceforge.mirrorservice.org/l/li/libpng/libpng16/1.6.37/libpng-1.6.37.tar.xz"
  sha256 "505e70834d35383537b6491e7ae8641f1a4bed1876dbfe361201fc80868d88ca"
  license "libpng-2.0"

  livecheck do
    url :stable
    regex(%r{url=.*?/libpng[._-]v?(\d+(?:\.\d+)+)\.t}i)
  end

  bottle do
    sha256 cellar: :any,                 arm64_monterey: "40b9dd222c45fb7e2ae3d5c702a4529aedf8c9848a5b6420cb951e72d3ad3919"
    sha256 cellar: :any,                 arm64_big_sur:  "766a7136ee626b411fb63da0c7e5bc1e848afb6e224622f25ea305b2d1a4a0f1"
    sha256 cellar: :any,                 monterey:       "7209cfe63b2e8fdbd9615221d78201bfac44405f5206f7b08867bcd0c6046757"
    sha256 cellar: :any,                 big_sur:        "a8f1c35f9f004c4f7878c30027e35a9fb9551782df963f88deebd3dc29d94d51"
    sha256 cellar: :any,                 catalina:       "c8e74da602c21f978cd7ee3d489979b4fc6681e71f678a1d99012943ee3a909f"
    sha256 cellar: :any,                 mojave:         "53bbd14cc27c86c16605e256e7646a1b5656c253abca084958c5d80a2961cb01"
    sha256 cellar: :any,                 high_sierra:    "bbdd94bdd5954bc50c096391486e67265dce5631efb913dcffe4469806a242b6"
    sha256 cellar: :any,                 sierra:         "e66797079a9a8134f91bd36b58054c6c32f6a9cd161c1bd19f0192319edb80aa"
    sha256 cellar: :any_skip_relocation, x86_64_linux:   "aeb238f8b62e3e8923a032caf88152e287a4435ab4afd663fa98b4a57495d116"
  end

  head do
    url "https://github.com/glennrp/libpng.git"

    depends_on "autoconf" => :build
    depends_on "automake" => :build
    depends_on "libtool" => :build
  end

  uses_from_macos "zlib"

  def install
    args = []
    args << "MACOSX_DEPLOYMENT_TARGET=X_MACOSX_DEPLOYMENT_TARGET"
    args << "CFLAGS= -mmacosx-version-min=X_MACOSX_DEPLOYMENT_TARGET -fno-stack-check "

    system "./configure", "--disable-dependency-tracking",
                          "--disable-silent-rules",
                          "--prefix=#{prefix}", *args
    system "make"
    system "make", "test"
    system "make", "install"
  end

  test do
    (testpath/"test.c").write <<~EOS
      #include <png.h>
      int main()
      {
        png_structp png_ptr;
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return 0;
      }
    EOS
    system ENV.cc, "test.c", "-I#{include}", "-L#{lib}", "-lpng", "-o", "test"
    system "./test"
  end
end
