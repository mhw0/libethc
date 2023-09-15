#include <iostream>
#include "eth-api.h"
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ethc/abi.h>

#include <ethc/hex.h>
#include <ethc/ecdsa.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>

#define ok(ethcop) assert(ethcop >= 0)
using namespace std;
using json = nlohmann::json;

// defaults
string eth_getBalance();
string eth_getCode();
string eth_getTransactionCount();
string eth_getStorageAt();
json eth_call(string data,string toAddress)// no need to check recipt. sends direct return of the method
{
    unordered_map<string, string> parameters;
    string url = "https://eth-mainnet.blastapi.io/";
    string path = "308913a1-a2b5-43e1-87f9-b2f02d7a66fa";
    string params = R"({"from":"0xa9d0a7dC416f586491f2fb596731598F937617b5","to":)";
    string dat0="\""+toAddress+"\"";
    string sss=R"(,"value":"0x0","data":)";//,"gas":"0x6C98","gasPrice":"0x28C7C9D83"
    params+=dat0+sss;
    string data1 = "\"0x" + data + "\"},\"latest\"";
    params += data1;
    string method = "eth_call";
    parameters.insert({{"id", "0"},
                       {"params", params},
                       {"method", method},
                       {"jsonrpc", "2.0"}

    });
    json response = RPC("POST", parameters, url, path);
    return response;
}

// gossip
string eth_blockNumber();
json eth_sendRawTransaction(string data)
{
    unordered_map<string, string> parameters;
    string url = "https://eth-mainnet.blastapi.io/";
    string path = "b871b6ef-cdc2-4c11-b791-9fcaf4da878d";
    string params = R"()";
    string data1 = "\"0x" + data + "\"";
    params += data1;
    string method = "eth_sendRawTransaction";
    parameters.insert({{"id", "1"},
                       {"params", params},
                       {"method", method},
                       {"jsonrpc", "2.0"}

    });
    json response = RPC("POST", parameters, url, path);
    return response;
}

// state method
string eth_getBalance();
string eth_getStorageAt();
string eth_getTransactionCount();
string eth_getCode();
string eth_call();
string eth_estimateGas();

// history methods
string eth_getBlockTransactionCountByHash();
string eth_getBlockTransactionCountByNumber();
string eth_getUncleCountByBlockHash();
string eth_getUncleCountByBlockNumber();
string eth_getBlockByHash();
string eth_getBlockByNumber();
string eth_getTransactionByHash();
string eth_getTransactionByBlockHashAndIndex();
string eth_getTransactionByBlockNumberAndIndex();
json eth_getTransactionReceipt(string data)
{
    unordered_map<string, string> parameters1;
    string url = "https://eth-mainnet.blastapi.io/";
    string path = "b871b6ef-cdc2-4c11-b791-9fcaf4da878d";
    string params = '\"' + data + '\"'; // raw string
    string method = "eth_getTransactionReceipt";
    json res;
    parameters1.insert({{"id", "0"},
                        {"params", params},
                        {"method", method},
                        {"jsonrpc", "2.0"}

    });
    res = RPC("POST", parameters1, url, path);
    return res;
}
string eth_getUncleByBlockHashAndIndex();
string eth_getUncleByBlockNumberAndIndex();
