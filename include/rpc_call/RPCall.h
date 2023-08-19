#include <iostream>
#include <curl/curl.h>
#include <unordered_map>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

string baseUrl;
static string gs_strLastResponse;

/* concatenate the query parameters */
string joinQueryParameters(const unordered_map<string, string> &parameters)
{
	string queryString = "{";
	for (auto it = parameters.cbegin(); it != parameters.cend(); ++it)
	{
		if (it == parameters.cbegin())
		{
			queryString += '\"' + it->first + '\"' + ':' + '\"' + it->second + '\"';
		}
		else if (it->first == "params")
		{
			queryString += ",\"" + it->first + '\"' + ':' + '[' + it->second + ']';
		}
		else if (it->first == "id")
		{
			queryString += ",\"" + it->first + '\"' + ':' + it->second;
		}
		else
		{
			queryString += ",\"" + it->first + '\"' + ':' + '\"' + it->second + '\"';
		}
	}
	queryString += "}";
	return queryString;
}

/*Sending the HTTP Request */
void executeHTTPRequest(CURL *curl)
{
	CURLcode res;
	gs_strLastResponse = "";
	/* Perform the request, res will get the return code */
	res = curl_easy_perform(curl);
	/* Check for errors */
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
}

/*Write the server response to string "gs_strLastResponse"*/
size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
	gs_strLastResponse += (const char *)contents;
	gs_strLastResponse += '\n';
	return size * nmemb;
}

void sendSignedRequest(CURL *curl, string httpMethod, unordered_map<string, string> &parameters)
{
	string url = baseUrl;
	string queryString = "";
	if (!parameters.empty())
	{
		string res = joinQueryParameters(parameters);
		queryString += res;
	}
	else
	{
		cout << "error!! Empty parameter Detected" << endl;
		exit(-1);
	}

	curl_easy_setopt(curl, CURLOPT_URL, (baseUrl).c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, queryString.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	parameters.clear();
	executeHTTPRequest(curl);
}

json RPC(string protocol, unordered_map<string, string> parameters, string url, string path)
{
	CURL *curl;

	curl_global_init(CURL_GLOBAL_ALL);

	/* get a curl handle */
	curl = curl_easy_init();

	/* Adding API key to header */
	struct curl_slist *chunk = NULL;
	chunk = curl_slist_append(chunk, ("Content-Type: application/json"));
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

	baseUrl = url + path;
	sendSignedRequest(curl, protocol, parameters);
	if (!json::accept(gs_strLastResponse)) // prevents wierd nlohmann parse error due to bad json format caused by the api
	{
		string temp;
		for (int i = 0; i < gs_strLastResponse.length(); i++)
		{
			if (!isspace(gs_strLastResponse[i]))
				temp += gs_strLastResponse[i];
		}
		gs_strLastResponse = temp;
	}

	json JsonForm = json::parse(gs_strLastResponse);
	/* always cleanup */
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return JsonForm;
}
