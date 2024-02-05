;; Development environment when using `guix shell'
;; see https://guix.gnu.org/cookbook/en/html_node/Software-Development.html

(use-modules (guix)
             (guix build-system cmake)
             ((guix licenses) #:prefix license:)
             (gnu packages qt)
             (gnu packages cpp))

(package
 (name "MagGUI")
 (version "0")
 (source #f)
 (build-system cmake-build-system)
 (native-inputs
  (list ccls))
 (propagated-inputs
  (list qtbase))
 (synopsis "Logging GUI for magnetometers")
 (description
  "Logging GUI for magnetometers")
 (home-page "https://gitlab.com/berkowski/maggui/")
 (license license:lgpl3+))
