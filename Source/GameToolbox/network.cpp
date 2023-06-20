
#include "network.h"
#include <cctype>
#include "network/HttpResponse.h"
#include "network/HttpClient.h"

std::optional<std::string> GameToolbox::getResponse(ax::network::HttpResponse* response) {
	if (!response)
		return std::nullopt;

	auto buffer = response->getResponseData();
	std::string ret{buffer->begin(), buffer->end()};

	int code = response->getResponseCode();
	//GameToolbox::log("response code: {}", code);

	bool robtopError = ( ret.size() > 1 && ret[0] == '-' && std::isdigit(ret[1]) );
	if (code != 200 || robtopError)
	{
		//if (!ret.empty()) {
		//	//GameToolbox::log("recieved error: {}", ret);
		//}
		return std::nullopt;
	}

	return ret == "-1" ? std::nullopt : std::optional<std::string>{ret};
}



void GameToolbox::executeHttpRequest(const std::string& url, const std::string& postData, ax::network::HttpRequest::Type type, const ax::network::ccHttpRequestCallback& callback)
{
	ax::network::HttpRequest* request = new ax::network::HttpRequest();
	request->setUrl(url);
	request->setRequestType(type);
	request->setHeaders(std::vector<std::string>{"User-Agent: "});
	request->setRequestData(postData.c_str(), postData.length());
	request->setResponseCallback(callback);
	ax::network::HttpClient::getInstance()->send(request);
	request->release();
}


