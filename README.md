# libethc <img width="38" height="38" alt="optimism-ethereum-op-logo" src="https://github.com/user-attachments/assets/201e9eec-2d1e-4cb9-9e46-c2fa72c4f14b" align="right" /> <img width="38" height="38" alt="polygon-matic-logo" src="https://github.com/user-attachments/assets/e01171dc-aa84-486d-80aa-8ad9f772a7f7" align="right" /> <img width="38" height="38" alt="arbitrum-arb-logo" src="https://github.com/user-attachments/assets/a24c27a3-3d33-4ddd-bb64-2e9a661ce252" align="right" /> <img width="38" height="38" alt="ethereum-eth-logo" src="https://github.com/user-attachments/assets/cf6c3c48-9a47-4969-a45d-095f555937f6" align="right" />

[![ci-unix](https://github.com/mhw0/libethc/actions/workflows/ci-unix.yaml/badge.svg)](https://github.com/mhw0/libethc/actions/workflows/ci-unix.yaml)
[![ci-docs](https://github.com/mhw0/libethc/actions/workflows/ci-docs.yaml/badge.svg)](https://github.com/mhw0/libethc/actions/workflows/ci-docs.yaml)
[![GitHub release](https://img.shields.io/github/v/release/mhw0/libethc?include_prereleases)](https://GitHub.com/mhw0/libethc/releases/)

libethc is an open-source Ethereum library for C and C++. It comes with numerous utility functions that will help when interacting with EVM based networks.

## Motivation

Most Ethereum libraries are made for high-level languages like JavaScript, Python or others, but those aren’t a good fit for low-level environments. C and C++ are fast, lightweight, and widely used in embedded development, firmware, and performance-critical software. With libethc, it becomes possible to use blockchain features on devices that don’t have the resources for big runtimes or dynamic memory. This includes things like hardware wallets, IoT devices, and payment terminals — places where there’s a need to sign transactions or perform actions on Ethereum network, directly on the device. Right now, most tools simply aren’t built for that. libethc fills that gap and makes Ethereum integration possible in environments that have been left out.


## Features

| ABI                | RLP                | ACCOUNT                    | ADDRESS                   | HEXADECIMAL          | ECDSA                  | KECCAK256        | UNIT               | BLOOM FILTER           |
|--------------------|--------------------|----------------------------|---------------------------|----------------------|------------------------|------------------|--------------------| -----------------------|
| `eth_abi_bool `    | `eth_rlp_uint8`    | `eth_account_create`       | `eth_is_address`          | `eth_is_hex`         | `eth_ecdsa_pubkey_get` | `eth_keccak256`  | `eth_unit_convert` | `eth_bloom_from_bytes` |
| `eth_abi_uint8`    | `eth_rlp_uint16`   | `eth_account_from_privkey` | `eth_is_checksum_address` | `eth_hex_pad_left`   | `eth_ecdsa_sign`       | `eth_keccak256p` |                    | `eth_bloom_and`        |
| `eth_abi_uint16`   | `eth_rlp_uint32`   | `eth_account_address_get`  | `eth_to_checksum_address` | `eth_hex_pad_right`  |                        |                  |                    | `eth_bloom_or`         |
| `eth_abi_uint32`   | `eth_rlp_uint64`   | `eth_account_pubkey_get`   | `eth_hex_from_bytes`      |
| `eth_abi_uint64`   | `eth_rlp_uint`     | `eth_account_sign`         | `eth_hex_to_bytes`        |
| `eth_abi_int8`     | `eth_rlp_address`  |
| `eth_abi_int16`    | `eth_rlp_array`    |
| `eth_abi_int32`    | `eth_rlp_bytes`    |
| `eth_abi_int64`    | `eth_rlp_hex`      |
| `eth_abi_mpint`    | `eth_rlp_len`      |
| `eth_abi_bytes8`   | `eth_rlp_to_hex`   |
| `eth_abi_bytes16`  | `eth_rlp_to_bytes` |
| `eth_abi_bytes32`  | `eth_rlp_from_hex` |
| `eth_abi_bytes`    |
| `eth_abi_address`  |
| `eth_abi_string`   |
| `eth_abi_call`     |
| `eth_abi_array`    | 
| `eth_abi_tuple`    |
| `eth_abi_from_hex` |
| `eth_abi_to_hex`   |


## Documentation
Documentation can be found here: https://mhw0.github.io/libethc
