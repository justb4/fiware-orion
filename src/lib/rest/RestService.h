#ifndef REST_SERVICE_H
#define REST_SERVICE_H

/*
*
* Copyright 2013 Telefonica Investigacion y Desarrollo, S.A.U
*
* This file is part of Orion Context Broker.
*
* Orion Context Broker is free software: you can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License as
* published by the Free Software Foundation, either version 3 of the
* License, or (at your option) any later version.
*
* Orion Context Broker is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero
* General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with Orion Context Broker. If not, see http://www.gnu.org/licenses/.
*
* For those usages not covered by this license please contact with
* iot_support at tid dot es
*
* Author: Ken Zangelin
*/
#include <string>
#include <vector>

#include "rest/ConnectionInfo.h"
#include "ngsi/ParseData.h"
#include "ngsi/Request.h"
#include "xmlParse/xmlRequest.h"
#include "jsonParse/jsonRequest.h"
#include "jsonParseV2/jsonRequestTreat.h"



/* ****************************************************************************
*
* RestServiceHandler - 
*/
typedef std::string (*RestServiceHandler)(ConnectionInfo* ciP, int compononts, std::vector<std::string>& compV);



/* ****************************************************************************
*
* RestService - 
*/
typedef std::string (*RestTreat)(ConnectionInfo* ciP, int components, std::vector<std::string>& compV, ParseData* reqDataP);
typedef struct RestService
{
  std::string   verb;
  RequestType   request;
  int           components;
  std::string   compV[10];
  std::string   payloadWord;
  RestTreat     treat;
} RestService;



/* ****************************************************************************
*
* restService - 
*/
extern std::string restService(ConnectionInfo* ciP, RestService* serviceV);



/* ****************************************************************************
*
* payloadParse - 
*/
extern std::string payloadParse
(
  ConnectionInfo*            ciP,
  ParseData*                 parseDataP,
  RestService*               service,
  XmlRequest**               reqPP,
  JsonRequest**              jsonPP,
  JsonDelayedRelease*        jsonReleaseP,
  std::vector<std::string>&  compV
);

#endif
