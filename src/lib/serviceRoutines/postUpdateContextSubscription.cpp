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

#include "mongoBackend/mongoUpdateContextSubscription.h"
#include "ngsi/ParseData.h"
#include "ngsi10/UpdateContextSubscriptionResponse.h"
#include "rest/ConnectionInfo.h"
#include "rest/uriParamNames.h"
#include "serviceRoutines/postUpdateContextSubscription.h"



/* ****************************************************************************
*
* postUpdateContextSubscription - 
*/
std::string postUpdateContextSubscription
(
  ConnectionInfo*            ciP,
  int                        components,
  std::vector<std::string>&  compV,
  ParseData*                 parseDataP
)
{
  UpdateContextSubscriptionResponse  ucsr;
  std::string                        answer;

  ucsr.subscribeError.subscriptionId = parseDataP->ucsr.res.subscriptionId;

  Format  notifyFormat = stringToFormat(ciP->uriParam[URI_PARAM_NOTIFY_FORMAT]);
  ciP->httpStatusCode  = mongoUpdateContextSubscription(&parseDataP->ucsr.res,
                                                        &ucsr,
                                                        notifyFormat,
                                                        ciP->tenant,
                                                        ciP->httpHeaders.xauthToken,
                                                        ciP->servicePathV);

  answer = ucsr.render(UpdateContextSubscription, ciP->outFormat, "");

  return answer;
}
