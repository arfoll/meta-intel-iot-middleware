SUMMARY = "Low Level Skeleton Library for Communication on Intel platforms"
SECTION = "libs"
AUTHOR = "Brendan Le Foll, Tom Ingleby"

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://COPYING;md5=66493d54e65bfc12c7983ff2e884f37f"

S = "${WORKDIR}/git"
SRC_URI = "git://github.com/intel-iot-devkit/mraa.git;protocol=git \
           file://imraa.conf \
           file://ConfigurableFirmataCurieImu.ino.bin \
           file://d043faf687eda18d9acda09e1bc028c931a68417.patch \
          "

inherit distutils-base pkgconfig python-dir cmake

EXTRA_OECMAKE_append = " -DINSTALLTOOLS:BOOL=ON -DIMRAA=ON -DFIRMATA=ON -DPYTHON2_PACKAGES_PATH:FILEPATH=../${PYTHON_SITEPACKAGES_DIR} -DBASE_LIB_INSTALL_DIR=${base_libdir} -DCMAKE_SKIP_RPATH=ON"

# Prepend mraa-utils to make sure bindir ends up in there
PACKAGES =+ "${PN}-utils ${PN}-imraa python-mraa python-mraa-dbg node-mraa"

DEPENDS += " json-c udev python nodejs swig-native "

### Docs ###

FILES_${PN}-doc += "${datadir}/mraa/examples/"

### Utils ###

FILES_${PN}-utils = "${bindir}/mraa-gpio \
                     ${bindir}/mraa-i2c \
                    "

### Bindings ###

EXTRA_OECMAKE_append = " -DBUILDSWIGPYTHON=ON -DBUILDSWIGNODE=ON "

### Debug ###

FILES_${PN}-dbg = "${bindir}/.debug/ \
                   /usr/src/debug/ \
                   /usr/lib64/.debug \
                   "

### Python ###

# python-mraa package containing Python bindings
FILES_python-${PN} = "${libdir}/python2.7/site-packages/mraa.py \
                      ${libdir}/python2.7/site-packages/_mraa.so \
                      ${datadir}/mraa/examples/python/ \
                      ${prefix}/src/debug/${BPN}/${PV}-${PR}/build/src/python/ \
                     "
RDEPENDS_python-${PN} += "python"

FILES_python-${PN}-dbg = "${libdir}/python2.7/site-packages/.debug/ \
                         "

### Node ###

# node-mraa package containing Nodejs bindings
FILES_node-${PN} = "${prefix}/lib/node_modules/ \
                    ${datadir}/mraa/examples/javascript/ \
                   "
RDEPENDS_node-${PN} += "nodejs"

### Imraa ###

FILES_${PN}-imraa = "${bindir}/imraa"
RDEPENDS_${PN}-imraa = "udev"

do_install_append() {
  mkdir -p ${D}${systemd_unitdir}/system/
  install -m 0644 ${S}/imraa/imraa.service ${D}${systemd_unitdir}/system/
  install -m 0644 ${WORKDIR}/ConfigurableFirmataCurieImu.ino.bin ${D}${datadir}/${PN}/firmata101.ino.bin
  install -d ${D}${sysconfdir}
  install -m 0644 ${WORKDIR}/imraa.conf ${D}${sysconfdir}/
}

FILES_${PN}-imraa_append += " ${systemd_unitdir}/system/imraa.service \
                        ${datadir}/${PN}/firmata101.ino.bin \
                        ${sysconfdir}/imraa.conf \
                      "
SYSTEMD_SERVICE_${PN}-imraa = "imraa.service"

DEPCHAIN_POST_remove = "-dev"
