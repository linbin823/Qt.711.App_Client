/* soaprdliteProxy.cpp
   Generated by gSOAP 2.8.14 from rdlite.h

Copyright(C) 2000-2013, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under ONE of the following licenses:
GPL or Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#include "soaprdliteProxy.h"

rdliteProxy::rdliteProxy()
{	rdliteProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

rdliteProxy::rdliteProxy(const struct soap &_soap) : soap(_soap)
{ }

rdliteProxy::rdliteProxy(const char *url)
{	rdliteProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = url;
}

rdliteProxy::rdliteProxy(soap_mode iomode)
{	rdliteProxy_init(iomode, iomode);
}

rdliteProxy::rdliteProxy(const char *url, soap_mode iomode)
{	rdliteProxy_init(iomode, iomode);
	soap_endpoint = url;
}

rdliteProxy::rdliteProxy(soap_mode imode, soap_mode omode)
{	rdliteProxy_init(imode, omode);
}

rdliteProxy::~rdliteProxy()
{ }

void rdliteProxy::rdliteProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns2", "urn:rdLite", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap_set_namespaces(this, namespaces);
}

void rdliteProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void rdliteProxy::reset()
{	destroy();
	soap_done(this);
	soap_init(this);
	rdliteProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void rdliteProxy::soap_noheader()
{	this->header = NULL;
}

const SOAP_ENV__Header *rdliteProxy::soap_header()
{	return this->header;
}

const SOAP_ENV__Fault *rdliteProxy::soap_fault()
{	return this->fault;
}

const char *rdliteProxy::soap_fault_string()
{	return *soap_faultstring(this);
}

const char *rdliteProxy::soap_fault_detail()
{	return *soap_faultdetail(this);
}

int rdliteProxy::soap_close_socket()
{	return soap_closesock(this);
}

int rdliteProxy::soap_force_close_socket()
{	return soap_force_closesock(this);
}

void rdliteProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void rdliteProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *rdliteProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int rdliteProxy::CreateSubscription(const char *endpoint, const char *soap_action, unsigned char stationId, int &result)
{	struct soap *soap = this;
	struct ns2__CreateSubscription soap_tmp_ns2__CreateSubscription;
	struct ns2__CreateSubscriptionResponse *soap_tmp_ns2__CreateSubscriptionResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__CreateSubscription.stationId = stationId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__CreateSubscription(soap, &soap_tmp_ns2__CreateSubscription);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__CreateSubscription(soap, &soap_tmp_ns2__CreateSubscription, "ns2:CreateSubscription", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__CreateSubscription(soap, &soap_tmp_ns2__CreateSubscription, "ns2:CreateSubscription", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__CreateSubscriptionResponse = soap_get_ns2__CreateSubscriptionResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__CreateSubscriptionResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::AddSubscriptionItem(const char *endpoint, const char *soap_action, unsigned char stationId, int subId, ns2__TagIdArray *tagId, int &result)
{	struct soap *soap = this;
	struct ns2__AddSubscriptionItem soap_tmp_ns2__AddSubscriptionItem;
	struct ns2__AddSubscriptionItemResponse *soap_tmp_ns2__AddSubscriptionItemResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__AddSubscriptionItem.stationId = stationId;
	soap_tmp_ns2__AddSubscriptionItem.subId = subId;
	soap_tmp_ns2__AddSubscriptionItem.tagId = tagId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__AddSubscriptionItem(soap, &soap_tmp_ns2__AddSubscriptionItem);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__AddSubscriptionItem(soap, &soap_tmp_ns2__AddSubscriptionItem, "ns2:AddSubscriptionItem", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__AddSubscriptionItem(soap, &soap_tmp_ns2__AddSubscriptionItem, "ns2:AddSubscriptionItem", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__AddSubscriptionItemResponse = soap_get_ns2__AddSubscriptionItemResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__AddSubscriptionItemResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::RefreshSubscription(const char *endpoint, const char *soap_action, unsigned char stationId, int subId, struct ns2__RefreshSubscriptionResponse &_param_1)
{	struct soap *soap = this;
	struct ns2__RefreshSubscription soap_tmp_ns2__RefreshSubscription;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__RefreshSubscription.stationId = stationId;
	soap_tmp_ns2__RefreshSubscription.subId = subId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__RefreshSubscription(soap, &soap_tmp_ns2__RefreshSubscription);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__RefreshSubscription(soap, &soap_tmp_ns2__RefreshSubscription, "ns2:RefreshSubscription", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__RefreshSubscription(soap, &soap_tmp_ns2__RefreshSubscription, "ns2:RefreshSubscription", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&_param_1)
		return soap_closesock(soap);
	soap_default_ns2__RefreshSubscriptionResponse(soap, &_param_1);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns2__RefreshSubscriptionResponse(soap, &_param_1, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int rdliteProxy::RemoveSubscription(const char *endpoint, const char *soap_action, unsigned char stationId, int subId, int &result)
{	struct soap *soap = this;
	struct ns2__RemoveSubscription soap_tmp_ns2__RemoveSubscription;
	struct ns2__RemoveSubscriptionResponse *soap_tmp_ns2__RemoveSubscriptionResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__RemoveSubscription.stationId = stationId;
	soap_tmp_ns2__RemoveSubscription.subId = subId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__RemoveSubscription(soap, &soap_tmp_ns2__RemoveSubscription);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__RemoveSubscription(soap, &soap_tmp_ns2__RemoveSubscription, "ns2:RemoveSubscription", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__RemoveSubscription(soap, &soap_tmp_ns2__RemoveSubscription, "ns2:RemoveSubscription", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__RemoveSubscriptionResponse = soap_get_ns2__RemoveSubscriptionResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__RemoveSubscriptionResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::Read(const char *endpoint, const char *soap_action, unsigned char stationId, ns2__TagIdArray *tagId, struct ns2__ReadResponse &_param_2)
{	struct soap *soap = this;
	struct ns2__Read soap_tmp_ns2__Read;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__Read.stationId = stationId;
	soap_tmp_ns2__Read.tagId = tagId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__Read(soap, &soap_tmp_ns2__Read);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__Read(soap, &soap_tmp_ns2__Read, "ns2:Read", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__Read(soap, &soap_tmp_ns2__Read, "ns2:Read", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&_param_2)
		return soap_closesock(soap);
	soap_default_ns2__ReadResponse(soap, &_param_2);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns2__ReadResponse(soap, &_param_2, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int rdliteProxy::Query(const char *endpoint, const char *soap_action, unsigned char stationId, ns2__TagIdArray *tagId, unsigned char valueType, LONG64 beginTime, LONG64 endTime, unsigned short count, struct ns2__QueryResponse &_param_3)
{	struct soap *soap = this;
	struct ns2__Query soap_tmp_ns2__Query;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__Query.stationId = stationId;
	soap_tmp_ns2__Query.tagId = tagId;
	soap_tmp_ns2__Query.valueType = valueType;
	soap_tmp_ns2__Query.beginTime = beginTime;
	soap_tmp_ns2__Query.endTime = endTime;
	soap_tmp_ns2__Query.count = count;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__Query(soap, &soap_tmp_ns2__Query);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__Query(soap, &soap_tmp_ns2__Query, "ns2:Query", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__Query(soap, &soap_tmp_ns2__Query, "ns2:Query", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&_param_3)
		return soap_closesock(soap);
	soap_default_ns2__QueryResponse(soap, &_param_3);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns2__QueryResponse(soap, &_param_3, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int rdliteProxy::SetTag(const char *endpoint, const char *soap_action, unsigned char stationId, unsigned int id, unsigned short metaId, int &result)
{	struct soap *soap = this;
	struct ns2__SetTag soap_tmp_ns2__SetTag;
	struct ns2__SetTagResponse *soap_tmp_ns2__SetTagResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__SetTag.stationId = stationId;
	soap_tmp_ns2__SetTag.id = id;
	soap_tmp_ns2__SetTag.metaId = metaId;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__SetTag(soap, &soap_tmp_ns2__SetTag);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__SetTag(soap, &soap_tmp_ns2__SetTag, "ns2:SetTag", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__SetTag(soap, &soap_tmp_ns2__SetTag, "ns2:SetTag", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__SetTagResponse = soap_get_ns2__SetTagResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__SetTagResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::EnableTag(const char *endpoint, const char *soap_action, unsigned char stationId, unsigned int id, bool enable, int &result)
{	struct soap *soap = this;
	struct ns2__EnableTag soap_tmp_ns2__EnableTag;
	struct ns2__EnableTagResponse *soap_tmp_ns2__EnableTagResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__EnableTag.stationId = stationId;
	soap_tmp_ns2__EnableTag.id = id;
	soap_tmp_ns2__EnableTag.enable = enable;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__EnableTag(soap, &soap_tmp_ns2__EnableTag);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__EnableTag(soap, &soap_tmp_ns2__EnableTag, "ns2:EnableTag", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__EnableTag(soap, &soap_tmp_ns2__EnableTag, "ns2:EnableTag", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__EnableTagResponse = soap_get_ns2__EnableTagResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__EnableTagResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::SetMeta(const char *endpoint, const char *soap_action, unsigned char stationId, unsigned short id, int index, std::string name, int dataLen, bool isValue, bool isAlarm, int &result)
{	struct soap *soap = this;
	struct ns2__SetMeta soap_tmp_ns2__SetMeta;
	struct ns2__SetMetaResponse *soap_tmp_ns2__SetMetaResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__SetMeta.stationId = stationId;
	soap_tmp_ns2__SetMeta.id = id;
	soap_tmp_ns2__SetMeta.index = index;
	soap_tmp_ns2__SetMeta.name = name;
	soap_tmp_ns2__SetMeta.dataLen = dataLen;
	soap_tmp_ns2__SetMeta.isValue = isValue;
	soap_tmp_ns2__SetMeta.isAlarm = isAlarm;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__SetMeta(soap, &soap_tmp_ns2__SetMeta);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__SetMeta(soap, &soap_tmp_ns2__SetMeta, "ns2:SetMeta", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__SetMeta(soap, &soap_tmp_ns2__SetMeta, "ns2:SetMeta", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__SetMetaResponse = soap_get_ns2__SetMetaResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__SetMetaResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::RemvoeMeta(const char *endpoint, const char *soap_action, unsigned char stationId, unsigned short id, int &result)
{	struct soap *soap = this;
	struct ns2__RemvoeMeta soap_tmp_ns2__RemvoeMeta;
	struct ns2__RemvoeMetaResponse *soap_tmp_ns2__RemvoeMetaResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__RemvoeMeta.stationId = stationId;
	soap_tmp_ns2__RemvoeMeta.id = id;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__RemvoeMeta(soap, &soap_tmp_ns2__RemvoeMeta);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__RemvoeMeta(soap, &soap_tmp_ns2__RemvoeMeta, "ns2:RemvoeMeta", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__RemvoeMeta(soap, &soap_tmp_ns2__RemvoeMeta, "ns2:RemvoeMeta", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__RemvoeMetaResponse = soap_get_ns2__RemvoeMetaResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__RemvoeMetaResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::SetStation(const char *endpoint, const char *soap_action, unsigned char id, std::string name, bool isScada, std::string recvHost, unsigned short recvPort, std::string syncHost, unsigned short syncPort, int &result)
{	struct soap *soap = this;
	struct ns2__SetStation soap_tmp_ns2__SetStation;
	struct ns2__SetStationResponse *soap_tmp_ns2__SetStationResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__SetStation.id = id;
	soap_tmp_ns2__SetStation.name = name;
	soap_tmp_ns2__SetStation.isScada = isScada;
	soap_tmp_ns2__SetStation.recvHost = recvHost;
	soap_tmp_ns2__SetStation.recvPort = recvPort;
	soap_tmp_ns2__SetStation.syncHost = syncHost;
	soap_tmp_ns2__SetStation.syncPort = syncPort;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__SetStation(soap, &soap_tmp_ns2__SetStation);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__SetStation(soap, &soap_tmp_ns2__SetStation, "ns2:SetStation", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__SetStation(soap, &soap_tmp_ns2__SetStation, "ns2:SetStation", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__SetStationResponse = soap_get_ns2__SetStationResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__SetStationResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::EnableStation(const char *endpoint, const char *soap_action, unsigned char id, bool enabled, int &result)
{	struct soap *soap = this;
	struct ns2__EnableStation soap_tmp_ns2__EnableStation;
	struct ns2__EnableStationResponse *soap_tmp_ns2__EnableStationResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__EnableStation.id = id;
	soap_tmp_ns2__EnableStation.enabled = enabled;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__EnableStation(soap, &soap_tmp_ns2__EnableStation);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__EnableStation(soap, &soap_tmp_ns2__EnableStation, "ns2:EnableStation", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__EnableStation(soap, &soap_tmp_ns2__EnableStation, "ns2:EnableStation", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__EnableStationResponse = soap_get_ns2__EnableStationResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__EnableStationResponse->result;
	return soap_closesock(soap);
}

int rdliteProxy::RemoveStation(const char *endpoint, const char *soap_action, unsigned char id, int &result)
{	struct soap *soap = this;
	struct ns2__RemoveStation soap_tmp_ns2__RemoveStation;
	struct ns2__RemoveStationResponse *soap_tmp_ns2__RemoveStationResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://tempuri/rdservice";
	if (soap_action == NULL)
		soap_action = "";
	soap->encodingStyle = "http://schemas.xmlsoap.org/soap/encoding/";
	soap_tmp_ns2__RemoveStation.id = id;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns2__RemoveStation(soap, &soap_tmp_ns2__RemoveStation);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns2__RemoveStation(soap, &soap_tmp_ns2__RemoveStation, "ns2:RemoveStation", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns2__RemoveStation(soap, &soap_tmp_ns2__RemoveStation, "ns2:RemoveStation", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&result)
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns2__RemoveStationResponse = soap_get_ns2__RemoveStationResponse(soap, NULL, "", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns2__RemoveStationResponse->result;
	return soap_closesock(soap);
}
/* End of client proxy code */
