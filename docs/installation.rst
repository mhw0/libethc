Installation
============

Dependencies
-----------------------
libethc requires some external libraries to be installed on the host machine. They are:

* `gmp <https://gmplib.org>`_ >= ``6.2.1`` -  The GNU Multiple Precision Arithmetic Library.
* `secp256k1 <https://github.com/bitcoin-core/secp256k1>`_ >= ``0.3.1`` - Optimized C library for EC operations on curve secp256k1.

For computing keccak hashes, `The eXtended Keccak Code Package (XKCP) <https://github.com/XKCP/XKCP>`_ is used (statically linked).

Installed dependencies can be confirmed by running the following command (assuming that the host machine is unix):

.. code-block:: bash

  pkg-config --cflags --libs gmp libsecp256k1

If the both dependencies are installed properly, `pkg-config` should print a result similar to this:

.. code-block::

  -I/opt/homebrew/Cellar/gmp/6.2.1_1/include -I/opt/homebrew/include -L/opt/homebrew/Cellar/gmp/6.2.1_1/lib -L/opt/homebrew/lib -lgmp -lsecp256k1

**Dev dependencies:**

* `cmake <https://cmake.org>`_ >= ``?`` - Cross-platform build automation tool.

Building from source
--------------------
.. code-block:: bash

   git clone https://github.com/mhw0/libethc
   cd libethc
   mkdir build
   cd build
   cmake ..
   make
   sudo make install

The build process can be customized with these CMake definitions:

+------------------------------+------------+----------+--------------------------------+
| Definition name              | Type       | Default  | Description                    |
+==============================+============+==========+================================+
| ``ETHC_BUILD_TESTS``         | boolean    |  ``OFF`` | Build tests                    |
+------------------------------+------------+----------+--------------------------------+
| ``ETHC_BUILD_DOCS``          | boolean    |  ``OFF`` | Build docs                     |
+------------------------------+------------+----------+--------------------------------+
| ``ETHC_DISABLE_SHARED_LIBS`` | boolean    |  ``OFF`` | Disables building shared libs  |
+------------------------------+------------+----------+--------------------------------+
