ABI functions
=============

ABI/RLP functions slightly differ from other functions. For example, when the data needs to be encoded or decoded, only one function is used for both operations and the mode in which the ABI functions should work can be specified during the initialization process. Consider the following call and its representation in C function calls:

.. code::

  balanceOf(0x19CE609af17d094Bcf4143266D2F6dB35231E47c)

.. code:: c

  // ...
  char *func = "balanceOf(address)";
  char *addr = "0x19CE609af17d094Bcf4143266D2F6dB35231E47c";

  eth_abi_call(&abi, &func, NULL); // balanceOf(
    eth_abi_address(&abi, &addr);  //   0x19CE609af17d094Bcf4143266D2F6dB35231E47c
  eth_abi_call_end(&abi);          // )

If the call part is not needed, it can be omitted and it will still work.

**Nested data:**

If a function accepts a nested array (``myfunc(uint8[][])``), the encoding process may look like:

.. code:: c

  // ...
  char *func = "myfunc(uint8[][])";
  uint8_t a=0x83, b=0xac;

  eth_abi_call(&abi, &func, NULL);   // myfunc(
    eth_abi_array(&abi, NULL);       //   [
      eth_abi_array(&abi, NULL);     //     [
        eth_abi_uint8(&abi, &a);     //       0x83
      eth_abi_array_end(&abi, NULL); //     ],
      eth_abi_array(&abi, NULL);     //     [
        eth_abi_uint8(&abi, &a);     //       0xac
      eth_abi_array_end(&abi, NULL); //     ]
    eth_abi_array_end(&abi, NULL);   //   ]
  eth_abi_call_end(&abi);            // )

------------

.. doxygenfunction:: eth_abi_init
.. doxygenfunction:: eth_abi_free
.. doxygenfunction:: eth_abi_bool
.. doxygenfunction:: eth_abi_int8
.. doxygenfunction:: eth_abi_int16
.. doxygenfunction:: eth_abi_int32
.. doxygenfunction:: eth_abi_int64
.. doxygenfunction:: eth_abi_uint8
.. doxygenfunction:: eth_abi_uint16
.. doxygenfunction:: eth_abi_uint32
.. doxygenfunction:: eth_abi_uint64
.. doxygenfunction:: eth_abi_mpint
.. doxygenfunction:: eth_abi_address
.. doxygenfunction:: eth_abi_bytes8
.. doxygenfunction:: eth_abi_bytes16
.. doxygenfunction:: eth_abi_bytes32
.. doxygenfunction:: eth_abi_bytes
.. doxygenfunction:: eth_abi_to_hex
.. doxygenfunction:: eth_abi_from_hex
.. doxygenfunction:: eth_abi_call
.. doxygenfunction:: eth_abi_call_end
.. doxygenfunction:: eth_abi_array
.. doxygenfunction:: eth_abi_array_end
