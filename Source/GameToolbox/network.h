#pragma once
#include "network/HttpRequest.h"
#include <optional>
#include <string>

namespace ax
{
	namespace network
	{
		typedef std::function<void(HttpClient* client, HttpResponse* response)> ccHttpRequestCallback;
	}
}

namespace GameToolbox
{
	std::optional<std::string> getResponse(ax::network::HttpResponse* response);
	void executeHttpRequest(const std::string& url, const std::string& postData, ax::network::HttpRequest::Type type, const ax::network::ccHttpRequestCallback& callback);
};

