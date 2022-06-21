include(GNUInstallDirs)

ADD_CUSTOM_COMMAND(
  COMMENT "Installing en_US@nonlinear (needs root privileges)"
  OUTPUT .install-nl-locale
  VERBATIM
  COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/en_US@nonlinear /usr/share/i18n/locales/
  COMMAND echo "en_US@nonlinear.UTF-8 UTF-8" >> /usr/share/i18n/SUPPORTED
  COMMAND locale-gen en_US@nonlinear.UTF-8
)

install(FILES en_US@nonlinear DESTINATION ${CMAKE_INSTALL_PREFIX}/share/i18n/locales/)

ADD_CUSTOM_TARGET(install-nl-locale DEPENDS .install-nl-locale)