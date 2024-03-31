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

(define-public qtserialport-6
  (package
    (name "qtserialport")
    (version "6.5.2")
    (source (origin
              (method url-fetch)
              (uri (qt-url name version))
              (sha256
               (base32
                "17nc5kmha6fy3vzkxfr2gxyzdsahs1x66d5lhcqk0szak8b58g06"))))
    (build-system cmake-build-system)
    ;; (arguments
    ;;  (list #:phases #~(modify-phases %standard-phases
    ;;                     (add-after 'install 'delete-installed-tests
    ;;                       (lambda _
    ;;                         (delete-file-recursively
    ;;                          (string-append #$output "/tests")))))))
    (native-inputs (list pkg-config))
    (inputs (list qtbase eudev))
    (home-page (package-home-page qtbase))
    (synopsis "Qt Serial Port module")
    (description "The Qt Serial Port module provides the library for
interacting with serial ports from within Qt.")
    (license (package-license qtbase))))

;; (define-public qtserialport-6
;;   (package
;;     (inherit qtsvg-5)
;;     (name "qtserialport")
;;     (version "5.15.10")
;;     (source (origin
;;               (method url-fetch)
;;               (uri (qt-url name version))
;;               (sha256
;;                (base32
;;                 "1zk8y20bqibpvsxi1mqg1ry37xq55yw3m0isij4j9pc58lm4wvzw"))))
;;     (native-inputs (list perl))
;;     (inputs (list qtbase-5 eudev))
;;     (arguments
;;      (substitute-keyword-arguments (package-arguments qtsvg-5)
;;        ((#:phases phases)
;;         #~(modify-phases #$phases
;;             (add-after 'unpack 'patch-dlopen-paths
;;               (lambda* (#:key inputs #:allow-other-keys)
;;                 (substitute* "src/serialport/qtudev_p.h"
;;                   ;; Use the absolute paths for dynamically loaded libs,
;;                   ;; otherwise the lib will be searched in LD_LIBRARY_PATH which
;;                   ;; typically is not set in guix.
;;                   (("setFileNameAndVersion\\(QStringLiteral\\(\"udev\")")
;;                    (format #f "setFileNameAndVersion(QStringLiteral(~s))"
;;                            (string-append #$(this-package-input "eudev")
;;                                           "/lib/libudev"))))))))))
;;     (synopsis "Qt Serial Port module")
;;     (description "The Qt Serial Port module provides the library for
;; interacting with serial ports from within Qt.")))

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
   clang-17))
 (propagated-inputs
  (list qtbase qtserialport-6))
 (synopsis "Logging GUI for magnetometers")
 (description
  "Logging GUI for magnetometers")
 (home-page "https://gitlab.com/berkowski/maggui/")
 (license license:lgpl3+))
