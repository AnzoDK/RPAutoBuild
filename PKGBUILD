# Maintainer: AnzoDK <anton@rosenoern-productions.dk>
pkgname="rpautobuild-git"
provides=('rpautobuild')
depends=('rpcommon>=0.1.11')
# conflicts=('rpcommon-dev')
pkgver()
{
  cd "$pkgname"
  git describe --long --tags | sed 's/\([^-]*-g\)/r\1/;s/-/./g'
}
pkgrel=1
arch=('any')
pkgdesc="Yet Another C++ Auto Build System"
url="https://github.com/AnzoDK/RPAutoBuild"
makedepends=('git')
source=("git+https://github.com/AnzoDK/RPCommonLib.git#branch=master")
license=('BSD2')
md5sums=('SKIP')
package()
{
   which rpauto
   if [ $? -ne 0 ]
   then
    echo 'No previous version of RPAuto - using make instead'
    make OS=linux
   else
    rpauto ./ --local
   fi
   mkdir -p "$pkgdir/usr/bin/"
   install -D ./rpauto.out "$pkgdir/usr/bin/"
   #install -D ./LICENSE "${pkgdir}"/usr/share/licenses/$pkgname/LICENSE   
}

