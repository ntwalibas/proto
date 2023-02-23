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


#ifndef PROTO_INFERENCE_ERROR_H
#define PROTO_INFERENCE_ERROR_H

#include <exception>
#include <string>

#include "common/token.h"


class InferenceError : public std::runtime_error
{
    public:
        InferenceError(
            Token& token,
            std::string const& primary_message,
            std::string const& secondary_message,
            bool fatal
        ) : std::runtime_error(primary_message),
            token(token),
            secondary_message(secondary_message),
            fatal(fatal)
        {}
        
        /**
         * Returns the token associated with this error.
         */
        Token& getToken()
        {
            return token;
        }
        
        /**
         * Returns the primary error message.
         */
        char const * getPrimaryMessage() const
        {
            return what();
        }
        
        /**
         * Returns the secondary error message.
         */
        char const * getSecondaryMessage() const
        {
            return secondary_message.c_str();
        }

        /**
         * Returns true if we can't recover from this error.
         */
        bool isFatal() const
        {
            return fatal;
        }

    private:
        Token token;                    /* Where the error happened. */
        std::string secondary_message;  /* Error secondary message. */
        bool fatal;                     /* Is this an error we can recover from. */
};

#endif
