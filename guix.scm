;; Development environment when using `guix shell'
;; see https://guix.gnu.org/cookbook/en/html_node/Software-Development.html

(use-modules (guix)
             (guix build-system cmake)
             ((guix licenses) #:prefix license:)
             (gnu packages linux)
             (gnu packages pkg-config)
             (gnu packages qt)
             (gnu packages cpp)
             (gnu packages llvm)
             (gnu packages cmake)
             (ice-9 match)
             (srfi srfi-1))

(define (qt-url component version)
  "Return a mirror URL for the Qt5 COMPONENT at VERSION."
  ;; We can't use a mirror:// scheme because these URLs are not exact copies:
  ;; the layout differs between them.
  (let ((x (match (version-major version)
             ("5" "-everywhere-opensource-src-")
             ;; Version 6 and later dropped 'opensource' from the archive
             ;; names.
             (_ "-everywhere-src-"))))
    (string-append "mirror://qt/qt/"
                   (version-major+minor version) "/" version
                   "/submodules/" component x version ".tar.xz")))

(package
 (name "MagGUI")
 (version "0")
 (source #f)
 (build-system cmake-build-system)
 (arguments
  (list
   #:cmake cmake))
 (native-inputs
  (list
   qttools
   clang-18))
 (propagated-inputs
  (list qtbase qtserialport))
 (synopsis "Logging GUI for magnetometers")
 (description
  "Logging GUI for magnetometers")
 (home-page "https://gitlab.com/berkowski/maggui/")
 (license license:lgpl3+))
