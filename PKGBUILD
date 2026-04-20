# Maintainer: Your Name <your.email@example.com>
pkgname=popdict
pkgver=1.0.0
pkgrel=1
pkgdesc="Pop-up dictionary application using Qt6"
arch=('x86_64')
license=('MIT')
depends=('qt6-base')
makedepends=('cmake' 'extra-cmake-modules' 'qt6-tools')
source=("git+https://github.com/leonbcode/PopDict.git")
sha256sums=('SKIP')  # skip checksum for git sources

build() {
    cd "$srcdir/PopDict"  # CMake root inside cloned repo
    mkdir -p build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr
    make
}

package() {
    cd "$srcdir/PopDict/build"
    make DESTDIR="$pkgdir" install
}