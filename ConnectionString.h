/*!
 * \brief   The file contains connection string wrapper interface
 * \author  \verbatim
            Created by: Alexander Egorov
            \endverbatim
 * \date    \verbatim
            Creation date: 2010-04-11
            \endverbatim
 * Copyright 2008-2009 Alexander Egorov <egoroff@gmail.com> (http://www.egoroff.spb.ru)
*/

#pragma once

/**
* \brief Represents connection string wrapper class
*/
class ConnectionString {
 public:
        /**
        * \brief Creates new wrapper instance using raw string specified
        * @param pRawString Raw string to wrap up
        */
        explicit ConnectionString(__in const char* pRawString);

        /**
        * \brief Virtual descructor
        */
        virtual ~ConnectionString(void);

        /**
        * \brief Parses raw string
        */
        void Parse();

        /**
        * \brief Shows whether connection string contains charset parameter (backward compatibility only)
        * \return True in connection string contains charset parameter otherwise false
        */
        __checkReturn bool ContainsCharset() const;

        /**
        * \brief Gets charset parameter value (backward compatibility only)
        * \return charset parameter value
        */
        const std::string& GetCharset() const;

        /**
        * \brief Copies charset into buffer specified (backward compatibility only)
        * @param pDestination Destination buffer
        * @param destLength Destination buffer length
        */
        void CopyCharset(
            __out char* pDestination, std::string::size_type destLength) const;

        /**
        * \brief Gets charset parameter value length (backward compatibility only)
        * \return charset parameter value length
        */
        __checkReturn size_t CharsetLength() const;

        /**
        * \brief Gets pure OLE DB compatible string (without any additional parser's options)
        * \return OLE DB compatible string
        */
        const std::string& GetOleDBString() const;
 private:
    std::string charset_;
    std::string cnString_;
    std::string oleDBString_;
    bool containsCharset_;
    DISALLOW_COPY_AND_ASSIGN(ConnectionString);
};

