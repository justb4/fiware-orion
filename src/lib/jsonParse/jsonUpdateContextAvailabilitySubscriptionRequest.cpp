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
* fermin at tid dot es
*
* Author: Ken Zangelin
*/
#include <string>
#include <vector>

#include "logMsg/logMsg.h"
#include "logMsg/traceLevels.h"

#include "common/globals.h"
#include "ngsi/EntityId.h"
#include "ngsi9/SubscribeContextAvailabilityRequest.h"
#include "jsonParse/jsonNullTreat.h"
#include "jsonParse/JsonNode.h"
#include "jsonParse/jsonUpdateContextAvailabilitySubscriptionRequest.h"



/* ****************************************************************************
*
* entityId - 
*/
static std::string entityId(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("%s: %s", path.c_str(), value.c_str()));

  reqDataP->ucas.entityIdP = new EntityId();

  LM_T(LmtNew, ("New entityId at %p", reqDataP->ucas.entityIdP));
  reqDataP->ucas.entityIdP->id        = "not in use";
  reqDataP->ucas.entityIdP->type      = "not in use";
  reqDataP->ucas.entityIdP->isPattern = "false";

  reqDataP->ucas.res.entityIdVector.push_back(reqDataP->ucas.entityIdP);
  LM_T(LmtNew, ("After push_back"));

  return "OK";
}



/* ****************************************************************************
*
* entityIdId - 
*/
static std::string entityIdId(std::string path, std::string value, ParseData* reqDataP)
{
   reqDataP->ucas.entityIdP->id = value;
   LM_T(LmtParse, ("Set 'id' to '%s' for an entity", reqDataP->ucas.entityIdP->id.c_str()));

   return "OK";
}



/* ****************************************************************************
*
* entityIdType - 
*/
static std::string entityIdType(std::string path, std::string value, ParseData* reqDataP)
{
   reqDataP->ucas.entityIdP->type = value;
   LM_T(LmtParse, ("Set 'type' to '%s' for an entity", reqDataP->ucas.entityIdP->type.c_str()));

   return "OK";
}



/* ****************************************************************************
*
* entityIdIsPattern - 
*/
static std::string entityIdIsPattern(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got an entityId:isPattern: '%s'", value.c_str()));

  if (!isTrue(value) && !isFalse(value))
    LM_W(("bad 'isPattern' value: '%s'", value.c_str()));

  reqDataP->ucas.entityIdP->isPattern = value;

  return "OK";
}



/* ****************************************************************************
*
* attribute - 
*/
static std::string attribute(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got an attribute: '%s'", value.c_str()));

  reqDataP->ucas.res.attributeList.push_back(value);

  return "OK";
}



/* ****************************************************************************
*
* duration - 
*/
static std::string duration(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a duration: '%s'", value.c_str()));

  reqDataP->ucas.res.duration.set(value);

  return "OK";
}



/* ****************************************************************************
*
* restriction - 
*/
static std::string restriction(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a restriction"));

  ++reqDataP->ucas.restrictions;

  return "OK";
}



/* ****************************************************************************
*
* attributeExpression - 
*/
static std::string attributeExpression(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got an attributeExpression: '%s'", value.c_str()));

  reqDataP->ucas.res.restriction.attributeExpression.set(value);

  return "OK";
}



/* ****************************************************************************
*
* scope - 
*/
static std::string scope(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a scope"));

  reqDataP->ucas.scopeP = new Scope();
  reqDataP->ucas.res.restriction.scopeVector.push_back(reqDataP->ucas.scopeP);

  return "OK";
}



/* ****************************************************************************
*
* scopeType - 
*/
static std::string scopeType(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a scope type: '%s'", value.c_str()));

  reqDataP->ucas.scopeP->type = value;

  return "OK";
}



/* ****************************************************************************
*
* scopeValue - 
*/
static std::string scopeValue(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a scope value: '%s'", value.c_str()));

  reqDataP->ucas.scopeP->value = value;

  return "OK";
}



/* ****************************************************************************
*
* subscriptionId - 
*/
static std::string subscriptionId(std::string path, std::string value, ParseData* reqDataP)
{
  LM_T(LmtParse, ("Got a subscriptionId: '%s'", value.c_str()));
  reqDataP->ucas.res.subscriptionId.set(value);

  return "OK";
}



/* ****************************************************************************
*
* jsonUcasParseVector -
*/
JsonNode jsonUcasParseVector[] =
{
  { "/entities/entity",                    entityId             },
  { "/entities/entity/id",                 entityIdId           },
  { "/entities/entity/type",               entityIdType         },
  { "/entities/entity/isPattern",          entityIdIsPattern    },
  { "/attributes/attribute",               attribute            },
  { "/duration",                           duration             },
  { "/restriction",                        restriction          },
  { "/restriction/attributeExpression",    attributeExpression  },
  { "/restriction/scopes/scope",           scope,               },
  { "/restriction/scopes/scope/type",      scopeType            },
  { "/restriction/scopes/scope/value",     scopeValue           },
  { "/subscriptionId",                     subscriptionId       },
  { "LAST", NULL }
};



/* ****************************************************************************
*
* jsonUcasInit - 
*/
void jsonUcasInit(ParseData* reqDataP)
{
  jsonUcasRelease(reqDataP);

  reqDataP->errorString                = "";

  reqDataP->ucas.entityIdP             = NULL;
  reqDataP->ucas.restrictions          = 0;
  reqDataP->ucas.scopeP                = NULL;
}



/* ****************************************************************************
*
* jsonUcasRelease - 
*/
void jsonUcasRelease(ParseData* reqDataP)
{
  reqDataP->ucas.res.release();
}



/* ****************************************************************************
*
* jsonUcasCheck - 
*/
std::string jsonUcasCheck(ParseData* reqData, ConnectionInfo* ciP)
{
   std::string s;
   s = reqData->ucas.res.check(SubscribeContextAvailability, ciP->outFormat, "", reqData->errorString, 0);
   return s;
}



/* ****************************************************************************
*
* jsonUcasPresent - 
*/
void jsonUcasPresent(ParseData* reqDataP)
{
  printf("jsonUcasPresent\n");

  if (!lmTraceIsSet(LmtDump))
    return;

  reqDataP->ucas.res.present("");
}