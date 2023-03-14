/*  This file is part of the Proto programming language
 * 
 *  Copyright (c) 2023- Ntwali Bashige Toussaint
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#ifndef PROTO_CLEANER_WARNING_H
#define PROTO_CLEANER_WARNING_H

#include <stdexcept>
#include <string>

#include "common/token.h"


class CleanerWarning : public std::runtime_error
{
    public:
        CleanerWarning(
            Token& token,
            std::string const& primary_message,
            std::string const& secondary_message
        ) : std::runtime_error(primary_message),
            token(token),
            secondary_message(secondary_message)
        {}
        
        /**
         * Returns the token associated with this warning.
         */
        Token& getToken()
        {
            return token;
        }
        
        /**
         * Returns the primary warning message.
         */
        char const * getPrimaryMessage() const
        {
            return what();
        }
        
        /**
         * Returns the secondary warning message.
         */
        char const * getSecondaryMessage() const
        {
            return secondary_message.c_str();
        }

    private:
        Token token;                    /* Where the warning happened. */
        std::string secondary_message;  /* Warning secondary message. */
};

#endif
