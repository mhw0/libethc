#include "eth-rpc.h"
#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ethc/abi.h>
#include <ethc/unit.h>

#include <ethc/hex.h>
#include <ethc/ecdsa.h>
#include <ethc/rlp.h>
#include <ethc/keccak256.h>

#include <gmp.h>

#define ok(ethcop) assert(ethcop >= 0)
using namespace std;
using json = nlohmann::json;
string UniswapFactoryAddress = "0x5C69bEe701ef814a2B6a3EDD4B1652CB9cc5aA6f";
string UniswapexchangeAddress = "0x7a250d5630B4cF539739dF2C5dAcb4c659F2488D";

using namespace std;

class rpc_uniswap
{
public:
    string balanceOf(string addressToCheckBalance, string ExchangeAddress=UniswapexchangeAddress)
	{
		char *a = strdup(addressToCheckBalance.c_str());
		// abi encode
		struct eth_abi abi;
        string function= "balanceOf(address)";
		char *fn =  strdup(function.c_str()), *hex;
		size_t hexlen;
		// abi encoding
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_address(&abi,&a);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
	
		json response = eth_call(hex, ExchangeAddress);/*sending rpc call*/
		string result; 
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		mpz_t r1;
		mpz_init(r1);
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_mpint(&abi,r1);
		eth_abi_free(&abi1);

		char *temp;
		gmp_asprintf(&temp, "%Zd", r1);
		string t1=temp;
        free(temp);

		string decodedRes(t1);
		return decodedRes;
	}
	string * getReserves(string addressToCheckBalance)
	{
		static string fin[2]{"x","x"};
		char *a = strdup(addressToCheckBalance.c_str());
		// abi encode
		struct eth_abi abi;
        string function= "getReserves()";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		// abi encoding
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
	
		json response = eth_call(hex, addressToCheckBalance);/*sending rpc call*/
		string result; 
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		mpz_t r1,r2,r3;
		mpz_init(r1);
		mpz_init(r2);
		mpz_init(r3);

		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_mpint(&abi1,r1);
		eth_abi_mpint(&abi1,r2);
		eth_abi_mpint(&abi1,r3);
		eth_abi_free(&abi1);

		char *temp0, *temp1;
		gmp_asprintf(&temp0, "%Zd", r1);
		gmp_asprintf(&temp1, "%Zd", r2);
		string t1=temp0;
		string t2=temp1;
        free(temp0);
		free(temp1);
        string dec0(t1);
		string dec1(t2);
		//decodedRes+=" "+decode0;
		fin[0]=dec0;
		fin[1]=dec1;
		//cout<<"fins are "<<fin[0]<<" "<<fin[1]<<endl;
		return fin;
	}
	string Symbol(string addressToCheckSymb)
	{
		// abi encode
		struct eth_abi abi;
        string function="symbol()";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		// abi encoding
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		// cout<<"address passed to rpc "<<addressToCheckSymb<<endl;
		//  rpc call
		json response = eth_call(hex, addressToCheckSymb);
		string result; /*sending rpc call*/
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		if (result == "0x")
			result += "000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		uint8_t *add;
		size_t a; // result.length();
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_bytes(&abi1, &add, &a);
		eth_abi_free(&abi1);

		char *final = (char *)add;

		string decodedres;
		for (int i = 0; i < a; i++)
		{
			decodedres += final[i];
		}
		//cout << "decoded symbol is " << decodedres << endl;
		return decodedres;
	}

	
	string swapExactETHForTokens()
	{
		/*ABI encoding*/
		struct eth_abi abi;
        string function= "swapExactETHForTokens(uint256,address[],address,uint256)";
		char *fn =  strdup(function.c_str()), *hex;
		string sweth = "0xC02aaA39b223FE8D0A0e5C4F27eAD9083C756Cc2";
		string susdt = "0xdAC17F958D2ee523a2206206994597C13D831ec7";
		string sto_address = "0xa9d0a7dC416f586491f2fb596731598F937617b5";

        char *weth=strdup(sweth.c_str());
        char *usdt=strdup(susdt.c_str());
        char *to_address=strdup(sto_address.c_str());

		uint64_t path_len = 2;
		size_t hexlen;
		uint64_t amount_out_min = 0, deadline = 346744838;
		//////////////////
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_uint64(&abi, &amount_out_min);
		eth_abi_array(&abi, &path_len); // open an array
		eth_abi_address(&abi, &weth);
		eth_abi_address(&abi, &usdt);
		eth_abi_array_end(&abi); // close the array
		eth_abi_address(&abi, &to_address);
		eth_abi_uint64(&abi, &deadline);
		eth_abi_call_end(&abi);

		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		cout << hex << endl;
		/*ABi encoding*/

		/*copying ABI result with prefix to abi_result*/
		string testlen = hex;
		int leng = testlen.length();
		char prefix[leng + 3] = "0x";
		strcat(prefix, hex);
		char *abi_result = prefix;
		/*copying ABI result with prefix to abi_result*/

		/*generating raw transaction*/
		struct eth_rlp rlp0, rlp1;
		struct eth_ecdsa_signature sign;

		uint8_t privkey[] = {0x80, 0x3e, 0x53, 0x83, 0x51, 0x93, 0xdd, 0x6e,
							 0xad, 0x61, 0xa2, 0x5a, 0x74, 0x5f, 0xa6, 0x35,
							 0x86, 0xb4, 0xb2, 0xdd, 0x7b, 0x6a, 0xbe, 0x4a,
							 0x06, 0xfa, 0x7f, 0x33, 0xc1, 0x1a, 0xed, 0x81}; // real private key of the account 0xa9d0a7dC416f586491f2fb596731598F937617b5
	

		uint8_t nonce = 0x00, zero = 0x00, keccak[32], *rlp0bytes, *r, *s;
		char *gasprice = "0x031948ba4b", *gaslimit = "0x068f2e", *value = "0x00";
		char *toaddr = "0xa9d0a7dC416f586491f2fb596731598F937617b5", *txn;
		uint16_t chainid = 0x1;
		uint8_t v;
		size_t rlp0len, rlp1len, siglen = 32;

		ok(eth_rlp_init(&rlp0, ETH_RLP_ENCODE));
		ok(eth_rlp_array(&rlp0));
		ok(eth_rlp_uint8(&rlp0, &nonce));
		ok(eth_rlp_hex(&rlp0, &gasprice, NULL));
		ok(eth_rlp_hex(&rlp0, &gaslimit, NULL));
		ok(eth_rlp_address(&rlp0, &toaddr));
		ok(eth_rlp_uint8(&rlp0, &zero));
		ok(eth_rlp_hex(&rlp0, &abi_result, NULL));
		ok(eth_rlp_uint16(&rlp0, &chainid));
		ok(eth_rlp_uint8(&rlp0, &zero)); //   0x,
		ok(eth_rlp_uint8(&rlp0, &zero)); //   0x,
		ok(eth_rlp_array_end(&rlp0));	 // ]

		ok(eth_rlp_to_bytes(&rlp0bytes, &rlp0len, &rlp0));
		ok(eth_rlp_free(&rlp0));

		// compute the keccak hash of the encoded rlp elements
		ok(eth_keccak256(keccak, rlp0bytes, rlp0len));
		free(rlp0bytes);

		// sign the transaction
		ok(eth_ecdsa_sign(&sign, privkey, keccak));

		// calculate v
		v = sign.recid + chainid * 2 + 35;
		r = sign.r;
		s = sign.s;

		ok(eth_rlp_init(&rlp1, ETH_RLP_ENCODE));
		ok(eth_rlp_array(&rlp1));
		ok(eth_rlp_uint8(&rlp1, &nonce));
		ok(eth_rlp_hex(&rlp1, &gasprice, NULL));
		ok(eth_rlp_hex(&rlp1, &gaslimit, NULL));
		ok(eth_rlp_address(&rlp1, &toaddr));
		ok(eth_rlp_uint8(&rlp1, &zero));
		ok(eth_rlp_hex(&rlp1, &abi_result, NULL)); // replaced
		ok(eth_rlp_uint8(&rlp1, &v));
		ok(eth_rlp_bytes(&rlp1, &r, &siglen));
		ok(eth_rlp_bytes(&rlp1, &s, &siglen));
		ok(eth_rlp_array_end(&rlp1));

		// FIX3: this actually returns the output length
		ok(eth_rlp_to_hex(&txn, &rlp1) > 0);
		ok(eth_rlp_free(&rlp1));
		json response = eth_sendRawTransaction(txn);
		string result;
		if (response.contains("result"))
			result = response["result"];
		else
			result = response.dump();
		return result;
	}
	string quote(uint64_t amountA, uint64_t reserveA, uint64_t reserveB, string exchangeAddress = UniswapexchangeAddress) // default is uniswap v2 address
	{
		struct eth_abi abi;
        string function="quote(uint256,uint256,uint256)";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_uint64(&abi, &amountA);
		eth_abi_uint64(&abi, &reserveA);
		eth_abi_uint64(&abi, &reserveB);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		json response = eth_call(hex, exchangeAddress); /*end of abi encoding*/

		string result = response.dump(); /*sending rpc call*/

		if (response.contains("result"))
			result = response["result"];
		else
			result = response.dump();
		return result;
	}
	string getAmountOut(string amountA, string reserveIn, string reserveOut, string exchangeAddress = UniswapexchangeAddress) // default is uniswap v2 address
	{
		
		char *a = strdup(reserveIn.c_str());
		char *b = strdup(reserveOut.c_str());
		char *c = strdup(amountA.c_str());

        mpz_t reA,reB,amt;
		mpz_init_set_str(reA,a,0);
		mpz_init_set_str(reB,b,0);
		mpz_init_set_str(amt,c,0);

		struct eth_abi abi;
        string function="getAmountOut(uint256,uint256,uint256)";
		char *fn =strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_mpint(&abi,amt);
		eth_abi_mpint(&abi,reA);
		eth_abi_mpint(&abi,reB);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		json response = eth_call(hex, exchangeAddress); /*end of abi encoding*/

		string result = response.dump(); /*sending rpc call*/
       // cout << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());
		// decoding
		struct eth_abi abi1;
		mpz_t r1;
		mpz_init(r1);

		uint64_t arrSize;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_mpint(&abi1, r1);
		eth_abi_free(&abi1);
		//gmp_printf("%Zd %s\n", r1, "test");

		char *temp;
		gmp_asprintf(&temp, "%Zd", r1);
		string t3=temp;
        free(temp);

		string decodedRes(t3);
		return decodedRes;
	}
	string getAmountsOut(string amountIn, string pathA, string pathB, string exchangeAddress = UniswapexchangeAddress)
	{
		char *a, *b, *c;
		a = strdup(pathA.c_str());
		b = strdup(pathB.c_str());
		c = strdup(amountIn.c_str());
        mpz_t amt;
		mpz_init_set_str(amt,c,0);
		/*cout<<"amt is ";
		gmp_printf("%Zd\n", amt);
        cout<<endl;*/
		struct eth_abi abi;
		string function="getAmountsOut(uint256,address[])";
		char *fn = strdup(function.c_str()), *hex;
		uint64_t arraySize = 2;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_mpint(&abi,amt);
		eth_abi_array(&abi, &arraySize);
		eth_abi_address(&abi, &a);
		eth_abi_address(&abi, &b);
		eth_abi_array_end(&abi);
		eth_abi_call_end(&abi);

		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		/*end of abi encoding*/
        //cout<<"ABI is "<<hex<<endl;
		json response = eth_call(hex, exchangeAddress); /*sending rpc call*/

		string result;
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());
		// decoding
		struct eth_abi abi1;
		mpz_t r1, r2;
		mpz_init(r1);
		mpz_init(r2);

		uint64_t arrSize = 2;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_array(&abi1, &arrSize);
		eth_abi_mpint(&abi1, r1);
		eth_abi_mpint(&abi1, r2);
		eth_abi_array_end(&abi1);
		eth_abi_free(&abi1);
		//gmp_printf("%Zd %s\n", r1, "test");

		char *temp;
		gmp_asprintf(&temp, "%Zd", r2);
		string t3=temp;
        free(temp);

		string decodedRes(t3);
		return decodedRes;
	}
	string Decimals(string addressToCheckDecimal)
	{
		// abi encode
		struct eth_abi abi;
        string function="decimals()";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		// abi encoding
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));
		// cout<<"address passed to rpc "<<addressToCheckSymb<<endl;
		//  rpc call
		json response = eth_call(hex, addressToCheckDecimal);
		string result; /*sending rpc call*/
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		if (result == "0x")
			result += "000000000000000000000000000000000000000000000000000000000000002000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		mpz_t add;
		mpz_init(add);

		size_t a; // result.length();
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_mpint(&abi1,add);
		eth_abi_free(&abi1);
		char *temp;
		gmp_asprintf(&temp, "%Zd", add);
		string t3=temp;
		
		string decoded(temp);
		//cout << "decoded symbol is " << decodedres << endl;
		return decoded;
	}
	
};

class rpc_uniswap_factory
{
	public:
	string allPairs(uint64_t index, string contractAddress = UniswapFactoryAddress) // returns 'n'th index of pair list in uniswap v2
	{
		// abi encode
		struct eth_abi abi;
        string function= "allPairs(uint256)";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_uint64(&abi, &index);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));

		// rpc call
		json response = eth_call(hex, contractAddress);
		string result = response.dump(); /*sending rpc call*/
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		// cout<<response.dump()<<endl;
		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		char *add;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_address(&abi1, &add);
		eth_abi_free(&abi1);
		string temp = add;
		string decodedResult = "0x";
		decodedResult += temp;
		return decodedResult;
	}
	string getPair(string address0, string address1, string contractAddress = UniswapFactoryAddress) // returns 'n'th index of pair list in uniswap v2
	{
		char *a=strdup(address0.c_str());
		char *b=strdup(address1.c_str());
		// abi encode
		struct eth_abi abi;
        string function= "getPair(address,address)";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_address(&abi,&a);
		eth_abi_address(&abi,&b);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));

		// rpc call
		json response = eth_call(hex, contractAddress);
		string result = response.dump(); /*sending rpc call*/
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			//exit(-1);
		}

		// cout<<response.dump()<<endl;
		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		char *add;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_address(&abi1, &add);
		eth_abi_free(&abi1);
		string temp = add;
		string decodedResult = "0x";
		decodedResult += temp;
		return decodedResult;
	}
	 
	string allPairsLength(string factoryAddress = UniswapFactoryAddress)
	{
		json response = eth_call("574f2ba3", factoryAddress); // 0902f1ac is encoded abi form of allPairsLength()

		string result;

		if (response.contains("result"))
			result = response["result"];
		else
			result = response.dump();
		return result;
	}

	string Token0(string addressToCheckSymb)
	{
		// abi encode
		struct eth_abi abi;
        string function="token0()";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));

		// rpc call
		json response = eth_call(hex, addressToCheckSymb);
		string result; /*sending rpc call*/
		// cout<<"dump "<<response.dump()<<endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		char *add;
		size_t a = 10;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_address(&abi1, &add);
		eth_abi_free(&abi1);
		string temp = add;
		string decodedRes = "0x";
		decodedRes += temp;
		return decodedRes;
	}
	string Token1(string addressToCheckSymb)
	{
		// abi encode
		struct eth_abi abi;
        string function="token1()";
		char *fn = strdup(function.c_str()), *hex;
		size_t hexlen;
		eth_abi_init(&abi, ETH_ABI_ENCODE);
		eth_abi_call(&abi, &fn, NULL);
		eth_abi_call_end(&abi);
		ok(eth_abi_to_hex(&abi, &hex, &hexlen));
		ok(eth_abi_free(&abi));

		// rpc call
		json response = eth_call(hex, addressToCheckSymb);
		string result; /*sending rpc call*/
		//cout << "dump " << response.dump() << endl;
		if (response.contains("result"))
			result = response["result"];
		else
		{
			cout << response.dump() << endl;
			exit(-1);
		}

		/*copying ABI result to data*/
		char *data = strdup(result.c_str());

		// decoding
		struct eth_abi abi1;
		char *add;
		size_t a = 10;
		eth_abi_from_hex(&abi1, data, -1);
		eth_abi_address(&abi1, &add);
		eth_abi_free(&abi1);
		string temp = add;
		string decodedRes = "0x";
		decodedRes += temp;
		return decodedRes;
	}

};